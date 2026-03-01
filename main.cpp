#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "."
using namespace std;
//
string commands[64];
vector <void (*) (vector <string>)> cmds_defs;

void compile_commands(string s, vector <string >  args){
  cmds_defs[get_idx(s)](args);


}


void base_display(string name,string dir){
  cout<<name<<":"<<dir<<endl;
}
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
int main(){
  build_commands();
  
  string name; cout<<"Whats the Owner Name ? : "; cin >> name;
  string dir = "root";
  for(int i = 0;i<64;i++){
    if(commands[i] != "")cout<<"name : " << commands[i] << "index : " << i<< endl;
  }
  while(true){
        base_display(name,dir);

        string com; cin >> com;

      


        
    }
  return 0;
    
  }
  


   



  
