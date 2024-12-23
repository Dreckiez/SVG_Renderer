#include "ViewBox.h"
#include<string>
#include<sstream>
#include "Reader.h"
using namespace std;

ViewBox::ViewBox(){
    width = height = 1;
}

ViewBox::ViewBox(float x, float y, float w, float h){
    Top.SetX(x);
    Top.SetY(y);
    width = w;
    height = h;
}

Shapes::Point ViewBox::GetTop(){
    return Top;
}

float ViewBox::GetWidth(){
    return width;
}

float ViewBox::GetHeight(){
    return height;
}

void ViewBox::SetTop(float x, float y){
    Top.SetX(x);
    Top.SetY(y);
}

void ViewBox::ReadViewBox(XMLElement* E){
    const char* VB = E->Attribute("viewBox");
    string tmp(VB);

    addSpaces(tmp);
    removeSpareSpaces(tmp);

    stringstream ss(tmp);
    float x = 0, y = 0;

    ss >> x >> y;
    Top.SetX(x);
    Top.SetY(y);
    ss >> width >> height;
}

ViewPort::ViewPort(){
    width = 300;
    height = 150;
}

ViewPort::ViewPort(float w, float h){
    width = w;
    height = h;
}

float ViewPort::GetWidth(){
    return width;
}

float ViewPort::GetHeight(){
    return height;
}

void ViewPort::SetWidth(float w){
    width = w;
}

void ViewPort::SetHeight(float h){
    height = h;
}

void ViewPort::ReadViewPort(XMLElement* E){
    const char* W = E->Attribute("width");
    const char* H = E->Attribute("height");
    string tmp_W(W);
    string tmp_H(H);

    if (W && H){
        width = ConvertUnit(tmp_W);
        height = ConvertUnit(tmp_H);
    }
    else if (W && !H){
        width = ConvertUnit(tmp_W);
        if (E->Attribute("viewBox")){
            ViewBox VB;
            VB.ReadViewBox(E);
            height = (VB.GetWidth()/VB.GetHeight()) * width;
        }
    }
    else if (!W && H){
        height = ConvertUnit(tmp_H);
        if (E->Attribute("viewBox")){
            ViewBox VB;
            VB.ReadViewBox(E);
            width = (VB.GetWidth()/VB.GetHeight()) * height;
        }
    }
}