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