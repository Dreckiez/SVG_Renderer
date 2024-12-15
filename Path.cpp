#include "Path.h"
using namespace Shapes;

//command

Command::Command(){
    cmd = ' ';
}

char Command::getCmd() { return cmd; }
void Command::setCmd(char c) { cmd = c; } 

vector<float> Command::getNums() { return nums; }

void Command::addNum(float p){
    nums.push_back(p);
}

string Command::toString(){
    string s = "";
    s += cmd;
    for (int i = 0; i < nums.size(); i++){
        s += to_string(nums[i]) + ' ';
    }
    return s;
}

//path
Path::Path(){
    cout << "Path constructed\n";
}

vector<Command> Path::getCmd(){ 
    return cmd;
}

Command Path::getCmdAt(int idx){
    if (idx <= cmd.size())
        return cmd[idx];
    return cmd[0];
}

void Path::add(Command c){
    cmd.push_back(c);
}