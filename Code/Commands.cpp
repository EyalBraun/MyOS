#include "../Headers/Commands.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

// --- Helper Functions ---

int get_idx(string s) {
    if (s.empty()) return -1;
    return (unsigned(s[0] * 31 + s[s.size() - 1])) % 64;
}

void build_commands() {
    #define X(name) \
        int idx_##name = get_idx(#name); \
        cmds_defs[idx_##name] = handle_##name; \
        commands[idx_##name] = #name; 
    
    COMMAND_LIST
    #undef X
}

void compile_commands(string s, File*& current, vector<string> args) {
    int idx = get_idx(s);
    if (idx != -1 && cmds_defs[idx] != nullptr) {
        cmds_defs[idx](current, args);
    } else {
        cout << s << ": command not found" << endl;
    }
}

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
    current->children.push_back(make_unique<File>(args[0], isDir, current));
}

void handle_cd(File*& current, const vector<string>& args) {
    if (args.empty()) return;
    
    if(args[0] == "root"){
        while(current -> parent != nullptr) {
            current = current -> parent;
        }
        return;
    }
    
    if (args[0] == ".." && current->parent != nullptr) {
        current = current->parent;
        return;
    }
    
    for (auto const& child : current->children) {
        if (child->name == args[0] && child->isDir) {
            current = child.get();
            return;
        }
    }
    cout << "Directory not found." << endl;
}

void handle_fv(File*& current, const vector<string>& args) {
    if (args.empty()) {
        cout << "fv: missing filename" << endl;
        return;
    }
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
    bool isAuthorized = (args.size() > 1 && args.back() == "authorized");

    // 1. The Guard
    if (!isAuthorized && current->parent != nullptr) {
        cout << "Cannot Look if not in the root directory ! to get to root type cd root" << endl;
        return;
    }

    // 2. Search Logic
    // We use a static variable (or pass a bool by reference) 
    // to track if we found it across ALL recursive calls.
    static bool foundAny = false; 
    if (!isAuthorized) foundAny = false; // Reset for a brand new search

    if (current->name == args[0]) {
        cout << "The File has been Found ! in the path : ";
        pwd(current);
        cout << endl;
        foundAny = true; 
        return; // Found it in this branch, can stop looking deeper here
    }

    // 3. Prepare badge
    vector<string> authorizedArgs = args; 
    if (!isAuthorized) {
        authorizedArgs.push_back("authorized");
    }

    // 4. Recurse
    for (int i = 0; i < current->children.size(); i++) {
        File* childPtr = current->children[i].get();
        handle_find(childPtr, authorizedArgs);
    }

    // 5. THE FIX: Only the "Original Caller" (Root) prints the failure message
    if (!isAuthorized && !foundAny) {
        cout << "the file does not exist" << endl;
    }
}

void handle_rm(File*& current, const vector<string>& args) {
    if (args.empty()) {
        cout << "rm: missing operand" << endl;
        return;
    }
    
    bool isFlag = false;
    string name;
    if(args[0][0] == '-') {
        if(args.size() < 2) return;
        name = args[1];
        isFlag = true;
    }
    else name = args[0]; 

    auto it = current->children.begin();
    while (it != current->children.end()) {
        if ((*it)->name == name) {
            if ((*it)->isDir && isFlag) {
                bool is_r = false;
                for(auto x : args[0]) if(x == 'r'){ is_r = true; break; }
                
                if(is_r) {
                    if((*it)->isFav) {
                        cout << "rm: cannot remove '" << name << "': File is Guarded" << endl;
                        return;
                    }
                    current->children.erase(it);
                    return;
                } else {
                    cout << "rm: cannot remove '" << name << "': Is a directory" << endl;
                    return;
                }
            }
            else if((*it)->isDir && !isFlag) {
                cout << "rm: cannot remove '" << name << "': Is a directory" << endl;
                return;
            }                 

            if((*it)->isFav) {
                cout << "rm: cannot remove '" << name << "': File is Guarded" << endl;
                return;
            }

            current->children.erase(it);
            return;
        }
        it++;
    }
    cout << "rm: cannot remove '" << name << "': No such file" << endl;
}
