#ifndef MINIGIT_HPP
#define MINIGIT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// Structure to store file version information
struct Blob {
    string filename;
    string hash;
};

// Structure to represent a commit node
struct Commit {
    string message;
    string timestamp;
    string hash;
    vector<Blob> blobs;
    Commit* prev = nullptr;
    Commit* next = nullptr;
};

class MiniGit {
private:
    Commit* head;  // First commit
    Commit* current;  // Latest commit in current branch
    vector<string> stagingArea;  // Files staged for commit
    unordered_map<string, Commit*> branches;  // Branches map
    string currentBranch;  // Name of active branch

    string getCurrentTime();  // Get current timestamp
    void saveBlob(const string& filename);
    Commit* findCommitByHash(const string& hash) const;  // Find commit by hash

public:
    MiniGit();
    void addFile(const string& filename);  // Add stage file
    void commit(const string& message);  // Commit staged files
    void log() const;  // Show commit history
    void createBranch(const string& branchName);  // Create a new branch
    void checkoutBranch(const string& branchName);  // Switch to branch
    void mergeBranch(const string& sourceBranch);  // Merge another branch
    void diff(const string& hash1, const string& hash2);  // Show differences between two commits
};

#endif
