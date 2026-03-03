#include "../Headers/Commands.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// --- Helper Functions ---

/**
 * Generates a simple hash index for the command jump table
 */
int get_idx(string s) {
    if (s.empty()) return -1;
    return (unsigned(s[0] * 31 + s[s.size() - 1])) % 64;
}

/**
 * Maps command strings to their respective handler functions using X-Macros
 */
void build_commands() {
    #define X(name) \
        int idx_##name = get_idx(#name); \
        cmds_defs[idx_##name] = handle_##name; \
        commands[idx_##name] = #name; 
    
    COMMAND_LIST
    #undef X
}

/**
 * Finds and executes the command from the jump table
 */
void compile_commands(string s, File*& current, vector<string> args) {
    int idx = get_idx(s);
    if (idx != -1 && cmds_defs[idx] != nullptr) {
        cmds_defs[idx](current, args);
    } else {
        cout << s << ": command not found" << endl;
    }
}

/**
 * Recursively prints the current directory path
 */
void pwd(File* current) {
    if (current == nullptr) return;
    if (current->parent != nullptr) pwd(current->parent);
    if (current->name == "/") {
        cout << "/";
    } else {
        cout << current->name << "/";
    }
}

// --- Command Handlers ---

void handle_help(File*& current, const vector<string>& args) {
    cout << "ORBIT OS Supported commands:" << endl;
    for (int i = 0; i < 64; i++) {
        if (!commands[i].empty()) {
            cout << " - " << commands[i] << endl;
        }
    }
}

void handle_pwd(File*& current, const vector<string>& args) {
    pwd(current);
    cout << endl;
}

void handle_ls(File*& current, const vector<string>& args) {
    for (auto const& child : current->children) {
        string favPrefix = child->isFav ? "[*]" : "";
        cout << favPrefix << child->name << (child->isDir ? "/" : "") << "  ";
    }
    cout << endl;
}

void handle_mkdir(File*& current, const vector<string>& args) {
    if (args.empty()) return;
    bool isDir = true;
    for(auto c : args[0]) if(c == '.') isDir = false;
    // Creates a new File object: Name, isDir, isFav (false), Parent
    current->children.push_back(make_unique<File>(args[0], isDir, false, current));
}

void handle_cd(File*& current, const vector<string>& args) {
    if (args.empty()) return;
    
    // Quick jump to root
    if(args[0] == "root"){
        while(current->parent != nullptr) current = current->parent;
        return;
    }
    
    // Move to parent directory
    if (args[0] == ".." && current->parent != nullptr) {
        current = current->parent;
        return;
    }
    
    // Navigate to child directory
    for (auto const& child : current->children) {
        if (child->name == args[0] && child->isDir) {
            current = child.get();
            return;
        }
    }
    cout << "Directory not found." << endl;
}

void handle_fv(File*& current, const vector<string>& args) {
    if (args.empty()) return;
    for (auto& child : current->children) {
        if (child->name == args[0]) {
            child->isFav = !child->isFav;
            cout << "File '" << child->name << "' favorite status: " << (child->isFav ? "ON" : "OFF") << endl;
            return;
        }
    }
    cout << "fv: " << args[0] << ": No such file" << endl;
}

void handle_find(File*& current, const vector<string>& args) {
    if (args.empty()) return;
    bool isAuthorized = (args.size() > 1 && args.back() == "authorized");

    if (!isAuthorized && current->parent != nullptr) {
        cout << "Error: find must start from root (type 'cd root')" << endl;
        return;
    }

    static bool foundAny = false; 
    if (!isAuthorized) foundAny = false;

    if (current->name == args[0]) {
        cout << "Found: ";
        pwd(current);
        cout << endl;
        foundAny = true; 
    }

    vector<string> authorizedArgs = args; 
    if (!isAuthorized) authorizedArgs.push_back("authorized");

    // --- התיקון כאן ---
    for (auto const& child : current->children) {
        File* childPtr = child.get(); // מחלצים את המצביע למשתנה עזר (Lvalue)
        handle_find(childPtr, authorizedArgs); // עכשיו ניתן להעביר אותו כרפרנס
    }

    if (!isAuthorized && !foundAny) cout << "File not found." << endl;
}

void handle_rm(File*& current, const vector<string>& args) {
    if (args.empty()) return;
    bool recursive = false;
    string name = (args[0][0] == '-') ? (args.size() > 1 ? args[1] : "") : args[0];
    if (args[0][0] == '-' && args[0].find('r') != string::npos) recursive = true;

    auto it = current->children.begin();
    while (it != current->children.end()) {
        if ((*it)->name == name) {
            // Check favorite guard
            if ((*it)->isFav) { cout << "rm: Guarded file." << endl; return; }
            // Check directory removal without -r flag
            if ((*it)->isDir && !recursive) { cout << "rm: Is a directory." << endl; return; }
            
            current->children.erase(it);
            return;
        }
        it++;
    }
}

void tree_recursive(File* node, string indent, bool isLast) {
    string marker = isLast ? "└── " : "├── ";
    cout << indent << marker << node->name << (node->isDir ? "/" : "") << endl;
    
    string newIndent = indent + (isLast ? "    " : "│   ");
    for (size_t i = 0; i < node->children.size(); i++) {
        tree_recursive(node->children[i].get(), newIndent, i == node->children.size() - 1);
    }
}

void handle_tree(File*& current, const vector<string>& args) {
    cout << "." << endl; 
    for (size_t i = 0; i < current->children.size(); i++) {
        tree_recursive(current->children[i].get(), "", i == current->children.size() - 1);
    }
}

void handle_wtf(File*& current, const vector<string>& args) {
    if (args.empty()) return;

    File* target = nullptr;
    for (auto& child : current->children) {
        if (child->name == args[0]) {
            if (child->isDir) { 
                cout << "wtf: target is directory." << endl; 
                return; 
            }
            target = child.get();
            break;
        }
    }

    // Handle overwrite
    if (target != nullptr && !target->Content.empty()) {
        cout << "File exists. Overwrite? (y/n): ";
        char input; cin >> input; cin.ignore(1000, '\n');
        if (input == 'n') return;
    } 
    
    if (target == nullptr) {
        current->children.push_back(make_unique<File>(args[0], false, false, current));
        target = current->children.back().get();
    }

    char c;
    string buffer = "";
    cout << "Recording... (Type 'q' to stop)" << endl;

    while (cin.get(c)) {
        // Stop before 'q' is added to the buffer
        if (c == 'q') break; 
        buffer += c;
    }
    
    // Clear the remaining 'newline' to prevent duplicate prompt issues
    cin.ignore(1000, '\n'); 
    
    target->Content = buffer;
    cout << "\nFile Saved." << endl;
}
