#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../Headers/Commands.h"
int get_idx(string s){
  return ((s[0]*31) + s[s.size()-1]) %64;
  
}
void build_commands(){
  fstream file("commands.txt");
string line;
while (getline(file, line)) {
       int idx = get_idx(line);
        commands[idx] = line;

     
    }

}
void compile_commands(string s, vector <string >  args){
  cmds_defs[get_idx(s)](args);


}
