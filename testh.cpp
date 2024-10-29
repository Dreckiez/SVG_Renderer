#include "testh.h"
#include "tinyxml2.h"
#include<iostream>
using namespace std;
using namespace tinyxml2;

RGBA::RGBA(){
    red = 0;
    green = 0;
    blue = 0;
    alpha = 0;
}

int RGBA::GetRed(){
    return red;
}

int RGBA::GetGreen(){
    return green;
}

int RGBA::GetBlue(){
    return blue;
}

float RGBA::GetAlpha(){
    return alpha;
}

void RGBA::SetRGB(int r, int g, int b){
    red = r;
    green = g;
    blue = b;
}

void RGBA::SetAlpha(float a){
    alpha = a;
}


Point::Point(){
    x = 0;
    y = 0;
}

Point::Point(float a, float b){
    x = a;
    y = b;
}

float Point::GetX(){
    return x;
}

float Point::GetY(){
    return y;
}

void Point::SetX(float a){
    x = a;
}

void Point::SetY(float a){
    y = a;
}

void Point::SetPoint(float a, float b){
    x = a;
    y = b;
}

Shapes::Rectangle::Rectangle(){
    cout << "Rectangle constructed\n";
    stroke_width = 0;
    width = 0;
    height = 0;
}

void Shapes::Rectangle::GetCoord(){
    cout << "X: " << A.GetX() << '\n';
    cout << "Y: " << A.GetY() << '\n';
}

void Shapes::Rectangle::GetSize(){
    cout << "Width: " << width << '\n';
    cout << "Height: " << height << '\n';
}

void Shapes::Rectangle::ReadRect(XMLElement* R){
    A.SetX(R->FloatAttribute("x"));
    A.SetY(R->FloatAttribute("y"));
    width = R->FloatAttribute("width");
    height = R->FloatAttribute("height");

    stroke_width = R->Attribute("stroke-width") == nullptr ? 0 : R->FloatAttribute("stroke-width");
    color.SetAlpha(R->Attribute("fill-opacity") == nullptr ? 1 : R->FloatAttribute("fill-opacity"));
    stroke.SetAlpha(R->Attribute("stroke-opacity") == nullptr ? 1 : R->FloatAttribute("stroke-opacity"));
}