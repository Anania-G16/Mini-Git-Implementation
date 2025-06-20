#include "MiniGit.hpp"
#include <sstream>

int main() {
    MiniGit repository;
    string command;

    cout<<"Enter any of the commands below. All commands are listed with their respective formats."<<endl;
    cout<<"init"<<endl;
    cout<<"add <file>"<<endl;
    cout<<"commit -m <msg>"<<endl;
    cout<<"log "<<endl;
    cout<<"branch <name> "<<endl;
    cout<<"checkout <name>"<<endl;
    cout<<"merge <name>"<<endl;
    cout<<"exit"<<endl;

    while (true) {

        cout << "Enter a Command: "<<endl;
        getline(cin, command);

        if (command=="init") {
            repository.init();
        }
        else if (command.substr(0, 4) == "add ") {
            repository.addFile(command.substr(4));
        }
        else if (command.substr(0, 10) == "commit -m ") {
            repository.commit(command.substr(10));
        }
        else if (command == "log") {
            repository.log();
        }
        else if (command.substr(0, 7) == "branch ") {
            repository.createBranch(command.substr(7));
        }
        else if (command.substr(0, 9) == "checkout ") {
            repository.checkoutBranch(command.substr(9));
        }
        else if (command.substr(0, 6) == "merge ") {
            repository.mergeBranch(command.substr(6));
        }
        else if (command.substr(0, 5) == "diff ") {
            istringstream iss(command.substr(5));
            string h1, h2;
            iss >> h1 >> h2;
            repository.diff(h1, h2);
        }
        else if (command == "exit") {
            break;  // Exit the program
        }
        else {
            cout << "Unknown command, please select from specified commands only"<<endl;
        }
    }

    return 0;
}