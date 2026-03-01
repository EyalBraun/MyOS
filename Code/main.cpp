#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../Headers/Commands.h"
#define ("sifu",hello);
using namespace std;







void base_display(string name,string dir){
  cout<<name<<":"<<dir<<endl;

}
int main(){
  build_commands();
  
  string name; cout<<"Whats the Owner Name ? : "; cin >> name;
  string dir = "root";
  
   while(true){
        base_display(name,dir);
    cout<<hello<<endl;


        

      


        
    }
  return 0;
    
  }
  


   



  
