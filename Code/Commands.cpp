#include "../Headers/Commands.h"
#include <iostream>

using namespace std;

int get_idx(string s) {
    if (s.empty()) return -1;
    // Using unsigned prevents overflow issues with small arrays
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

void compile_commands(string s, node*& current, vector<string> args) {
    int idx = get_idx(s);
    if (idx != -1 && cmds_defs[idx] != nullptr) {
        cmds_defs[idx](current, args);
    } else {
        cout << s << ": command not found" << endl;
    }
}

void handle_help(node*& current, const vector<string>& args) {
    cout << "Supported commands:" << endl;
    for (int i = 0; i < 64; i++) {
        if (!commands[i].empty()) {
            cout << " - " << commands[i] << endl;
        }
    }
}

void handle_pwd(node*& current, const vector<string>& args) {
    pwd(current);
    cout << endl;
}

void handle_ls(node*& current, const vector<string>& args) {
    for (auto const& child : current->children) {
        cout << child->name << (child->isdir ? "/" : "") << "  ";
    }
    cout << endl;
}

void handle_mkdir(node*& current, const vector<string>& args) {
    if (args.empty()) return;
    bool isDir = true;
    for(auto c : args[0]) if(c == '.') isDir = false;
  current->children.push_back(make_unique<node>(args[0], isDir,current));
}

void handle_cd(node*& current, const vector<string>& args) {
    if (args.empty()) return;
    if (args[0] == ".." && current->parent != nullptr) {
        current = current->parent;
        return;
    }
    for (auto const& child : current->children) {
        if (child->name == args[0] && child->isdir) {
            current = child.get();
            return;
        }
    }
    cout << "Directory not found." << endl;
}

void pwd(node* current) {
    if (current == nullptr) return;
    if (current->parent != nullptr) pwd(current->parent);
    cout << "/" << current->name;
}
void handle_rm(node*& current, const vector<string>& args) {
    if (args.empty()) {
        cout << "rm: missing operand" << endl;
        return;
    }
 bool isFlag = false;
  string name;
  if(args[0][0] == '-') {
   name = args[1];
    isFlag = true;
  }
  else name = args[0] ; 

        auto it = current->children.begin();
    while (it != current->children.end()) {
        if ((*it)->name == name) {
                       if ((*it)->isdir &&isFlag) {
                bool is_r = false;
                for(auto x:args[0]) if(x == 'r'){ is_r = true ; break;}
                if(is_r){
          current-> children.erase(it);

          return;

        }         else{
           cout << "rm: cannot remove '" << name << "': Is a directory" << endl;
                return;


        }
                            
      }
      else if((*it)->isdir&& !isFlag) {
         cout << "rm: cannot remove '" << name << "': Is a directory" << endl;
        return;

      }                 

        current->children.erase(it);
            return;
        }
        it++;
    }

    cout << "rm: cannot remove '" << args[0] << "': No such file" << endl;
}
