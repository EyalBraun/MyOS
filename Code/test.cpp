#include "../Headers/Commands.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

int main() {
    build_commands();
    auto root = make_unique<File>("/", true, false, false, nullptr, "");
    File* current = root.get();

    ifstream file("test.txt");
    if (!file.is_open()) {
        cout << "[FAIL] Fatal Error: Could not open test.txt!" << endl;
        return 1;
    }

    // Save the original cout buffer so we can restore it later
    streambuf* orig_cout = cout.rdbuf();
    stringstream capture;

    string line;
    int line_number = 0;
    bool passed = true;

    while (getline(file, line)) {
        line_number++;
        if (line.empty()) continue;

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        vector<string> args;
        string arg;
        while (ss >> arg) {
            args.push_back(arg);
        }

        // 1. Redirect standard output to our hidden 'capture' buffer
        cout.rdbuf(capture.rdbuf());
        capture.str("");   // Clear the buffer
        capture.clear();   // Clear any error flags

        // 2. Run the command silently
        compile_commands(cmd, current, args);

        // 3. Restore standard output back to the terminal
        cout.rdbuf(orig_cout);

        // 4. Analyze the captured output for known error phrases
        string output = capture.str();
        if (output.find("not found") != string::npos || 
            output.find("No such directory") != string::npos || 
            output.find("Error") != string::npos || 
            output.find("cannot find") != string::npos) {
            
            cout << "[FAIL] Test halted at line " << line_number << " -> " << line << endl;
            cout << "       Reason: " << output;
            passed = false;
            break; // Stop running tests on the first failure
        }
    }

    if (passed) {
        cout << "[SUCCESS] All commands in test.txt passed successfully!" << endl;
    }

    return 0;
}
