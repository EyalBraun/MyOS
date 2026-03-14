#include "../Headers/orbitCommands.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// הגדרת המערכים הגלובליים
string orb_commands[64];
string orb_command_descs[64];
void (*orb_cmds_defs[64])(vector<string> Code) = {nullptr};

int get_orb_idx(string s) {
    if (s.empty()) return -1;
    return (unsigned((s[0] * 31) ^ (s[s.size() - 1] * 91) ^ (s.size() * 13))) % 64;
}

// תיקון: הפונקציה מקבלת vector כחלק מהחתימה של השפה
void handle_ptc(vector<string> Code) {
    // נניח ש-Code[0] זה שם הפקודה ו-Code[1] זה התוכן
    if (Code.size() > 1) {
        for (size_t i = 1; i < Code.size(); ++i) {
            cout << Code[i] << " ";
        }
        cout << endl;
    }
}

void handle_gi(vector<string> Code) {
    string input;
    cout << "> ";
    getline(cin, input);
}

void build_orb_commands() {
    #define X(name, desc) \
        { \
            int idx = get_orb_idx(#name); \
            while (!orb_commands[idx].empty()) idx = (idx + 1) % 64; \
            orb_cmds_defs[idx] = handle_##name; \
            orb_commands[idx] = #name; \
            orb_command_descs[idx] = desc; \
        }
   
  ORB_COMMAND_LIST
    #undef X
}

vector<string> Parse_Code(string Content) {
    vector<string> Code;
    string Line;
    for (auto c : Content) {
        if (c != ';') Line += c;
        else {
            if (!Line.empty()) Code.push_back(Line);
            Line = "";
        }
    }
    return Code;
}


