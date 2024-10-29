#include<iostream>
#include<string>
#include<vector>
#include<cstring>
#include<sstream>
#include<windows.h>
#include<gdiplus.h>
#include "objects.h"
#include "tinyxml2.h"
using namespace std;
using namespace Gdiplus;
using namespace tinyxml2;

Shapes::RGBA::RGBA(){
    red = 0;
    green = 0;
    blue = 0;
    opacity = 0;
}

int Shapes::RGBA::GetRed(){
    return red;
}

int Shapes::RGBA::GetGreen(){
    return green;
}

int Shapes::RGBA::GetBlue(){
    return blue;
}

float Shapes::RGBA::GetAlpha(){
    return opacity;
}

void Shapes::RGBA::SetRGB(int r, int g, int b){
    red = r;
    green = g;
    blue = b;
}

void Shapes::RGBA::SetAlpha(float a){
    opacity = a;
}

Shapes::Point::Point(){
    x = 0;
    y = 0;
}

Shapes::Point::Point(float a, float b){
    x = a;
    y = b;
}

float Shapes::Point::GetX(){
    return x;
}

float Shapes::Point::GetY(){
    return y;
}

void Shapes::Point::SetX(float a){
    x = a;
}

void Shapes::Point::SetY(float a){
    y = a;
}

void Shapes::Point::SetPoint(float a, float b){
    x = a;
    y = b;
}

Shapes::Object::Object(){
    stroke_width = 0;
}

Shapes::Rectangle::Rectangle(){
    height = 0;
    width = 0;
    cout << "Rectangle constructed\n";
}

void Shapes::Rectangle::GetCoord(){
    cout << "X: " << A.GetX() << '\n';
    cout << "Y: " << A.GetY() << '\n';
}

void Shapes::Rectangle::GetSize(){
    cout << "Width: " << width << '\n';
    cout << "Height: " << height << '\n';
}

void Shapes::Object::GetColor(){
    cout << "RGB(" << color.GetRed() << ", " << color.GetGreen() << ", " << color.GetBlue() << ")\n";
    cout << "Opacity: " << color.GetAlpha() << '\n';
}

void Shapes::Object::GetStroke(){
    cout << "Stroke RGB(" << stroke.GetRed() << ", " << stroke.GetGreen() << ", " << stroke.GetBlue() << ")\n";
    cout << "Stroke Opacity: " << stroke.GetAlpha() << '\n';
    cout << "Stroke Width: " << stroke_width << '\n';
}

void Shapes::Object::SetColor(string s){
    s.erase(0,4);
    s.erase(s.size()-1, 1);
    for (int i = 0; i < s.size(); i++){
        if (s[i] == ',' && s[i+1] == ' ') s.erase(i,1);
        else if (s[i] == ',' && s[i+1] != ' ') s[i] = ' ';
    }
    stringstream ss(s);
    int r,g,b;
    ss >> r >> g >> b;
    color.SetRGB(r,g,b);
    color.SetAlpha(1);
}

void Shapes::Object::SetStroke(string s){
    s.erase(0,4);
    s.erase(s.size()-1, 1);
    for (int i = 0; i < s.size(); i++){
        if (s[i] == ',' && s[i+1] == ' ') s.erase(i,1);
        else if (s[i] == ',' && s[i+1] != ' ') s[i] = ' ';
    }
    stringstream ss(s);
    int r,g,b;
    ss >> r >> g >> b;
    stroke.SetRGB(r,g,b);
    stroke.SetAlpha(1);
}

void Shapes::Rectangle::ReadRect(XMLElement* R){
    A.SetX(R->FloatAttribute("x"));
    A.SetY(R->FloatAttribute("y"));
    width = R->FloatAttribute("width");
    height = R->FloatAttribute("height");

    const char* C = R->Attribute("fill");
    const char* S = R->Attribute("stroke"); 
    if (C != nullptr){
        string tmp = C;
        SetColor(tmp);
    }
    if (S != nullptr){
        string tmp = S;
        SetStroke(tmp);
    }
    stroke_width = R->Attribute("stroke-width") == nullptr ? 0 : R->FloatAttribute("stroke-width");
    color.SetAlpha(R->Attribute("fill-opacity") == nullptr ? 1 : R->FloatAttribute("fill-opacity"));
    stroke.SetAlpha(R->Attribute("stroke-opacity") == nullptr ? 1 : R->FloatAttribute("stroke-opacity"));

}

void Shapes::Rectangle::DrawR(Graphics* g){
    /* Pen p(Color((unsigned char)(stroke.GetAlpha()*255), (unsigned char)stroke.GetRed(), (unsigned char)stroke.GetGreen(), (unsigned char)stroke.GetBlue()), stroke_width);
    SolidBrush b(Color((unsigned char)color.GetAlpha(), (unsigned char)color.GetRed(), (unsigned char)color.GetGreen(), (unsigned char)color.GetBlue())); */
    Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width);
    SolidBrush b(Color(color.GetAlpha()*255, color.GetRed(), color.GetGreen(), color.GetBlue()));
    g->DrawRectangle(&p, A.GetX(), A.GetY(), width, height);
    g->FillRectangle(&b, A.GetX(), A.GetY(), width, height);
}


Shapes::Line::Line(){
    cout << "Line constructed\n";
}

void Shapes::Line::ReadLine(XMLElement* L){
    start.SetX(L->FloatAttribute("x1"));
    start.SetY(L->FloatAttribute("y1"));
    end.SetX(L->FloatAttribute("x2"));
    end.SetY(L->FloatAttribute("y2"));

    const char* S = L->Attribute("stroke");

    if (S != nullptr){
        string tmp = S;
        SetStroke(tmp);
    }
    stroke_width = L->Attribute("stroke-width") == nullptr ? 0 : L->FloatAttribute("stroke-width");
    stroke.SetAlpha(L->Attribute("stroke-opacity") == nullptr ? 1 : L->FloatAttribute("stroke-opacity"));
}

void Shapes::Line::DrawL(Graphics* g){
    Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width);
    g->DrawLine(&p, start.GetX(), start.GetY(), end.GetX(), end.GetY());
}

Shapes::Circle::Circle(){
    cout << "Circle constructed\n";
    radius = 0;
}

void Shapes::Circle::ReadCircle(XMLElement* C){
    center.SetX(C->FloatAttribute("cx"));
    center.SetY(C->FloatAttribute("cy"));
    radius = C->FloatAttribute("r");

    const char* Col = C->Attribute("fill");
    const char* S = C->Attribute("stroke"); 
    if (Col != nullptr){
        string tmp = Col;
        SetColor(tmp);
    }
    if (S != nullptr){
        string tmp = S;
        SetStroke(tmp);
    }
    stroke_width = C->Attribute("stroke-width") == nullptr ? 0 : C->FloatAttribute("stroke-width");
    color.SetAlpha(C->Attribute("fill-opacity") == nullptr ? 1 : C->FloatAttribute("fill-opacity"));
    stroke.SetAlpha(C->Attribute("stroke-opacity") == nullptr ? 1 : C->FloatAttribute("stroke-opacity"));
}

void Shapes::Circle::DrawC(Graphics* g){
    Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width);
    SolidBrush b(Color(color.GetAlpha()*255, color.GetRed(), color.GetGreen(), color.GetBlue()));
    g->DrawEllipse(&p, center.GetX()-radius-stroke_width/2, center.GetY()-radius-stroke_width/2, radius*2+stroke_width, radius*2+stroke_width);
    g->FillEllipse(&b, center.GetX()-radius, center.GetY()-radius, radius*2, radius*2);
}

Shapes::Ellipse::Ellipse(){
    cout << "Eclipse constructed\n";
    radius_x = 0;
    radius_y = 0;
}

void Shapes::Ellipse::ReadEllipse(XMLElement* E){
    center.SetX(E->FloatAttribute("cx"));
    center.SetY(E->FloatAttribute("cy"));
    radius_x = E->FloatAttribute("rx");
    radius_y = E->FloatAttribute("ry");

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke"); 
    if (C != nullptr){
        string tmp = C;
        SetColor(tmp);
    }
    if (S != nullptr){
        string tmp = S;
        SetStroke(tmp);
    }
    stroke_width = E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width");
    color.SetAlpha(E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    stroke.SetAlpha(E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));
}

void Shapes::Ellipse::DrawE(Graphics* g){
    Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width);
    SolidBrush b(Color(color.GetAlpha()*255, color.GetRed(), color.GetGreen(), color.GetBlue()));
    g->DrawEllipse(&p, center.GetX()-radius_x-stroke_width/2, center.GetY()-radius_y-stroke_width/2, radius_x*2+stroke_width, radius_y*2+stroke_width);
    g->FillEllipse(&b, center.GetX()-radius_x, center.GetY()-radius_y, radius_x*2, radius_y*2);
}

Shapes::Polygon::Polygon(){
    cout << "Polygon constructed\n";
}

void Shapes::Polygon::ReadPolygon(XMLElement* PG){

    stringstream ss(PG->Attribute("points"));

    string tmp;
    while(ss >> tmp){
        stringstream sss(tmp);
        string a, b;
        getline(sss, a, ',');
        getline(sss, b);
        Points.push_back({(float) atof(a.c_str()), (float) atof(b.c_str())});
    } 

    const char* C = PG->Attribute("fill");
    const char* S = PG->Attribute("stroke"); 
    if (C != nullptr){
        string tmp = C;
        SetColor(tmp);
    }
    if (S != nullptr){
        string tmp = S;
        SetStroke(tmp);
    }
    stroke_width = PG->Attribute("stroke-width") == nullptr ? 0 : PG->FloatAttribute("stroke-width");
    color.SetAlpha(PG->Attribute("fill-opacity") == nullptr ? 1 : PG->FloatAttribute("fill-opacity"));
    stroke.SetAlpha(PG->Attribute("stroke-opacity") == nullptr ? 1 : PG->FloatAttribute("stroke-opacity"));
}

void Shapes::Polygon::DrawPG(Graphics* g){
    vector<PointF> list;
    for (int i = 0; i < Points.size(); i++){
        list[i].X = Points[i].GetX();
        list[i].Y = Points[i].GetY();
        cout << list[i].X << ' ' << list[i].Y << '\n';
    }
    /* Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width);
    SolidBrush b(Color(color.GetAlpha()*255, color.GetRed(), color.GetGreen(), color.GetBlue()));

    g->DrawPolygon(&p, list.data(), static_cast<int> (Points.size()));
    g->FillPolygon(&b, list.data(), static_cast<int> (Points.size())); */
}

void Shapes::Polygon::GetCoords(){
    for (int i = 0; i < Points.size(); i++){
        cout << Points[i].GetX() << ' ' << Points[i].GetY() << '\n';
    }
}