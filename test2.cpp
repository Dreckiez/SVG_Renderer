#include<iostream>
#include<vector>
#include<string>
#include "objects.h"
#include "tinyxml2.h"
#include "raylib.h"
using namespace std;
using namespace tinyxml2;

int main(){
    XMLDocument doc;
    doc.LoadFile("sample.svg");
    XMLElement* root = doc.FirstChildElement()->FirstChildElement();
    
    // const XMLAttribute* firstAtt = root->FirstAttribute();
    // T1.ReadRect(firstAtt);
    // root = root->NextSiblingElement();
    // firstAtt = root->FirstAttribute();
    // T2.ReadRect(firstAtt);
    // T1.GetCoord();
    // T1.GetSize();
    // T1.GetColor();
    // T2.GetCoord();
    // T2.GetSize();
    // T2.GetColor();
    while (root)
    {   
        string name = root->Name();
        if (name == "rect"){
            Objects::Rectangle R;
            R.ReadRect(root);
            R.GetCoord();
            R.GetSize();
            R.GetColor();
            R.GetStroke();
        }
        
        root = root->NextSiblingElement();
    }
    
    return 0;
}