#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include "FileSystem.h" 

#define COMMAND_LIST \
    X(pwd)           \
    X(cd)            \
    X(mkdir)         \
    X(ls)            \
    X(help)          \
    X(rm)        

typedef void (*CommandFunc)(node*& current, const std::vector<std::string>& args);

// Automated declarations
#define X(name) void handle_##name(node*& current, const std::vector<std::string>& args);
COMMAND_LIST
#undef X

inline CommandFunc cmds_defs[64];
inline std::string commands[64]; 

void build_commands();
int get_idx(std::string s);
void compile_commands(std::string s, node*& current, std::vector<std::string> args);
void pwd(node* current);

#endif
