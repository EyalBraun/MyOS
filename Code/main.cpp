#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "../Headers/Commands.h"

using namespace std;

int main() {
    build_commands();

    auto root = make_unique<node>("root", true, nullptr);
    node* current = root.get();
    string owner;
    
    cout << "owner name: ";
    cin >> owner;
    cin.ignore();

    while (true) {
        cout << owner << "@os:";
        pwd(current);
        cout << "$ ";

        string input;
        getline(cin, input);
        if (input == "exit") break;
        if (input.empty()) continue;

        vector<string> args;
        string cmd = "";
        string word = "";
        int count = 0;

        for (int i = 0; i < input.size(); i++) {
            if (input[i] != ' ') {
                word += input[i];
            } else if (!word.empty()) {
                if (count == 0) cmd = word;
                else args.push_back(word);
                word = "";
                count++;
            }
        }
        // Handle the last word (no space after it)
        if (!word.empty()) {
            if (count == 0) cmd = word;
            else args.push_back(word);
        }

        if (!cmd.empty()) {
            compile_commands(cmd, current, args);
        }
    }
    return 0;
}
