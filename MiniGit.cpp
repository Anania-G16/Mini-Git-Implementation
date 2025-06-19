#include "MiniGit.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <ctime>


namespace fs = filesystem;

// A basic hash function to uniquely identify content
string simpleHash(const string& content) {
    hash<string> hasher;
    size_t hash = hasher(content);
    stringstream ss;
    ss << hex << hash;
    return ss.str();
}

// Constructor sets up initial folders and default branch
MiniGit::MiniGit() {
    if (fs::exists(".minigit") && fs::exists(".minigit/objects")) {
        initialized = true;
        head = nullptr;
        current = nullptr;
        currentBranch = "main";
        branches[currentBranch] = nullptr;
    }
}

void MiniGit::init() {
    if (fs::exists(".minigit") && fs::exists(".minigit/objects")) {
        cout << "Repository already initialized."<<endl;
        initialized = true;
        return;
    }
    fs::create_directory(".minigit");
    fs::create_directory(".minigit/objects");
    head = nullptr;
    current = nullptr;
    currentBranch = "main";
    branches[currentBranch] = nullptr;
    initialized = true;
    cout << "Initialized empty MiniGit repository in .minigit/"<<endl;
}

// Get the current timestamp for commits
string MiniGit::getCurrentTime() {
    time_t now = time(nullptr);
    stringstream ss;
    ss << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Save a file's contents into the .minigit/objects folder
void MiniGit::saveBlob(const string& filename) {
    ifstream inFile(filename);
    stringstream buffer;
    buffer << inFile.rdbuf();
    string content = buffer.str();
    string hash = simpleHash(content);
    ofstream outFile(".minigit/objects/" + hash);
    outFile << content;
}

// Add file to staging area (to be committed)
void MiniGit::addFile(const string& filename) {
    if (!initialized) { cout << "Repository not initialized. Use `init` first."<<endl; return; }

    if (!fs::exists(filename)) {
        cout << "The file does not exist."<<endl;
        return;
    }
    stagingArea.push_back(filename);
    cout << filename<< "Staged   for commit."<<endl;
}


// Commit all staged files
void MiniGit::commit(const string& message) {
    if (!initialized) { cout << "Repository not initialized. Use `init` first."<<endl; return; }

    if (stagingArea.empty()) {
        cout << "There are no files staged for commit."<<endl;
        return;
    }

    Commit* newCommit = new Commit;
    newCommit->message = message;
    newCommit->timestamp = getCurrentTime();

    stringstream fullContent;
    for (const auto& file : stagingArea) {
        ifstream inFile(file);
        stringstream buffer;
        buffer << inFile.rdbuf();
        string content = buffer.str();
        string hash = simpleHash(content);
        saveBlob(file);  // Save a copy in object store
        newCommit->blobs.push_back({file, hash});
        fullContent << file << hash;  // Unique hash for commit itself
    }

    newCommit->hash = simpleHash(fullContent.str());
    newCommit->prev = current;
    if (current) current->next = newCommit;
    current = newCommit;
    if (!head) head = current;

    branches[currentBranch] = current;
    stagingArea.clear();  // Clear staged files after commit
    cout << "Committed: " << message <<endl;
}

// Show history of commits
void MiniGit::log() const {
    if (!initialized) { cout << "Repository not initialized. Use `init` first."<<endl; return; }

    Commit* temp = current;
    while (temp) {
        cout << "Commit: " << temp->hash << endl<<"Message: " << temp->message
                  <<endl<< "Timestamp: " << temp->timestamp << endl<<"---"<<endl;
        temp = temp->prev;
    }
}

// Create a new branch pointing to current commit
void MiniGit::createBranch(const string& branchName) {
    branches[branchName] = current;
    cout << "Created branch: " << branchName << endl;
}

// Switch to another branch (only metadata-level here)
void MiniGit::checkoutBranch(const string& branchName) {
    if (!initialized) { cout << "Repository not initialized. Use `init` first."<<endl; return; }

    if (branches.find(branchName) == branches.end()) {
        cout << "branch not found."<<endl;
        return;
    }
    current = branches[branchName];
    currentBranch = branchName;
    cout << "Switched to branch: " << branchName << endl;
}

// Merge another branch into current one
void MiniGit::mergeBranch(const string& sourceBranch) {
    if (!initialized) { cout << "Repository not initialized. Use `init` first."<<endl; return; }

    if (branches.find(sourceBranch) == branches.end()) {
        cout << "Branch not found."<<endl;
        return;
    }

    Commit* source = branches[sourceBranch];
    Commit* target = current;
    unordered_map<string, string> mergedFiles;

    // Add current commit files
    for (const auto& blob : target->blobs) {
        mergedFiles[blob.filename] = blob.hash;
    }
    // Add/compare files from source branch
    for (const auto& blob : source->blobs) {
        if (mergedFiles.count(blob.filename) && mergedFiles[blob.filename] != blob.hash) {
            cout << "CONFLICT: both modified " << blob.filename << endl;
        }
        mergedFiles[blob.filename] = blob.hash;
    }

    // Restore merged files to working directory
    for (const auto& [file, hash] : mergedFiles) {
        ofstream outFile(file);
        ifstream inFile(".minigit/objects/" + hash);
        outFile << inFile.rdbuf();
    }

    cout << "Merged branch " << sourceBranch << " into " << currentBranch <<endl;
}

