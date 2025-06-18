#include "MiniGit.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

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
    fs::create_directory(".minigit");
    fs::create_directory(".minigit/objects");
    head = nullptr;
    current = nullptr;
    currentBranch = "main";
    branches[currentBranch] = nullptr;
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
    if (!fs::exists(filename)) {
        cout << "The file does not exist."<<endl;
        return;
    }
    stagingArea.push_back(filename);
    cout << filename<< "Staged   for commit."<<endl;
}

// Get the current timestamp for commits
string MiniGit::getCurrentTime() {
    time_t now = time(nullptr);
    stringstream ss;
    ss << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Commit all staged files
void MiniGit::commit(const string& message) {

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
