#include "MiniGit.hpp"
#include <filesystem>

namespace fs = filesystem;

// Constructor sets up initial folders and default branch
MiniGit::MiniGit() {
    fs::create_directory(".minigit");
    fs::create_directory(".minigit/objects");
    head = nullptr;
    current = nullptr;
    currentBranch = "main";
    branches[currentBranch] = nullptr;
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