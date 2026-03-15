#include "../include/Commands.h"
#include "../include/orbitCommands.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <cstdio> // Added for std::remove

using namespace std;

// Define a dedicated disk file for testing
const string TEST_DISK = "data/test_disk.bin";

int main() {
    // 1. Wipe the test disk before starting to ensure a clean state
    std::remove(TEST_DISK.c_str());

    build_commands();
    build_orb_commands();
    
    // Note: Ensure your save/load logic in orbitCommands.cpp 
    // uses TEST_DISK when running in test mode.
    
    auto root = make_unique<File>("/", true, false, false, nullptr, "");
    File* current = root.get();

    ifstream file("tests/test.txt");
    if (!file.is_open()) {
        cerr << "[FATAL] Could not find tests/test.txt" << endl;
        return 1;
    }

    // --- Redirection Setup ---
    streambuf* terminal_cout = cout.rdbuf(); 
    stringstream silent_buffer;             
    // -------------------------

    string line;
    int line_num = 0;
    int fail_count = 0;

    cout << "--- OrbitOS Automated Diagnostic Report ---" << endl;
    cout << "Using Test Disk: " << TEST_DISK << endl;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; 
        line_num++;

        stringstream ss(line);
        string cmd;
        ss >> cmd;
        vector<string> args;
        string arg;
        while (ss >> arg) args.push_back(arg);

        // Redirect output
        cout.rdbuf(silent_buffer.rdbuf());

        // Execute
        compile_commands(cmd, current, args);

        // Switch back to terminal
        string output = silent_buffer.str();
        cout.rdbuf(terminal_cout);

        if (output.find("Error") != string::npos || output.find("not found") != string::npos) {
            cout << "[FAIL] Line " << line_num << ": " << line << endl;
            cout << "      Reason: " << output; 
            fail_count++;
        }

        silent_buffer.str("");
        silent_buffer.clear();
    }

    cout << "-------------------------------------------" << endl;
    if (fail_count == 0) {
        cout << ">> Result: [SUCCESS] All commands executed perfectly." << endl;
    } else {
        cout << ">> Result: [FAILED] " << fail_count << " issues detected." << endl;
    }

    return (fail_count == 0) ? 0 : 1;
}
