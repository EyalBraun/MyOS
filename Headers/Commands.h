#ifndef COMMANDS_H
#define COMMANDS_H
#include <string>
#include <vector>
using namespace std;
typedef void (*CommandFunc)(const std::vector<std::string>& args);
inline CommandFunc cmds_defs[64];
inline string commands[64];
void build_commands();
int get_idx(string);
void compile_commands(string, vector<string>);
#endif



