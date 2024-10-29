#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<cstring>
using namespace std;

class Point{
private:
    float x, y;
public:
    Point(){
        x = 0;
        y = 0;
    }
    Point(float a, float b){
        x = a;
        y = b;
    }
    float GetX(){
        return x;
    }
    float GetY(){
        return y;
    }
};

int main(){
    /* string tmp = "rgb(200, 200, 200)";
    tmp.erase(0,4);
    tmp.erase(tmp.size()-1, 1);
    cout << tmp << '\n';
    for (int i = 0; i < tmp.size(); i++){
        if (tmp[i] == ',' && tmp[i+1] == ' '){
            tmp.erase(i,1);
        }
        else if (tmp[i] == ',' && tmp[i+1] != ' ') tmp[i] = ' ';
    }
    cout << tmp << '\n'; */

    string s = "350,75 379,161 469,161 397,215 423,301 350,250 277,301 303,215 231,161 321,161";
    vector<Point> Points;
    Points.push_back({2,0});

    stringstream ss (s);
    string tmp;
    while(ss >> tmp){
        //cout << tmp << '\n';
        string a, b;
        stringstream sss (tmp);
        getline(sss, a, ',');
        getline(sss, b);
        Points.push_back({(float)atoi(a.c_str()), (float)atoi(b.c_str())});
    }

    for (int i = 0; i < Points.size(); i++) cout << Points[i].GetX() <<  ' ' << Points[i].GetY() << '\n';

    // stringstream ss(tmp);
    // int r,g,b;
    // ss >> r >> g >> b;
    return 0;
}