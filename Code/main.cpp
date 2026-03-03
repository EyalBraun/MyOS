#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "../Headers/FileSystem.h" 
#include "../Headers/Commands.h"

using namespace std;

int main() {
    // 1. Initialize the Jump Table for commands
    build_commands();
    
    // 2. Initialize the Orbit Engine
    OrbitManager orbit; 
    string owner;
    
    cout << "------------------------------------------" << endl;
    cout << "   ORBIT OS v1.0 | Real-Time Subsystem    " << endl;
    cout << "------------------------------------------" << endl;
    
    cout << "Enter Operator Name: ";
    cin >> owner;
    cin.ignore(); // Clean the buffer for getline

    while (true) {
        // --- Prompt ---
        cout << "\033[1;36m" << owner << "@orbit\033[0m:";
        pwd(orbit.current);
        cout << "$ ";

        // --- Input Capture ---
        string input;
        if (!getline(cin, input)) break;
        if (input == "exit") break;
        if (input.empty()) continue;

        // --- Parsing Logic (Command vs Args) ---
        vector<string> args;
        string cmd = "";
        string word = "";
        bool found_cmd = false;

        for (int i = 0; i < input.size(); i++) {
            if (input[i] != ' ') {
                word += input[i];
            } else if (!word.empty()) {
                if (!found_cmd) {
                    cmd = word;
                    found_cmd = true;
                } else {
                    args.push_back(word);
                }
                word = "";
            }
        }
        
        // Handle the last word after the loop
        if (!word.empty()) {
            if (!found_cmd) cmd = word;
            else args.push_back(word);
        }

        // --- Execution ---
        if (!cmd.empty()) {
            // Pass orbit.current by reference to allow 'cd' to work
            compile_commands(cmd, orbit.current, args);
        }
    }

    cout << "Shutting down ORBIT OS..." << endl;
    return 0;
}
