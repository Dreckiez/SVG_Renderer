#include<iostream>
#include<string>
#include<vector>
#include<cstring>
#include<sstream>
#include<wchar.h>
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

void Shapes::Rectangle::Read(XMLElement* E){
    A.SetX(E->FloatAttribute("x"));
    A.SetY(E->FloatAttribute("y"));
    width = E->FloatAttribute("width");
    height = E->FloatAttribute("height");

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

void Shapes::Rectangle::Draw(Graphics* g, float s, PointF anchor){
    /* Pen p(Color((unsigned char)(stroke.GetAlpha()*255), (unsigned char)stroke.GetRed(), (unsigned char)stroke.GetGreen(), (unsigned char)stroke.GetBlue()), stroke_width);
    SolidBrush b(Color((unsigned char)color.GetAlpha(), (unsigned char)color.GetRed(), (unsigned char)color.GetGreen(), (unsigned char)color.GetBlue())); */
    Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width * s);
    SolidBrush b(Color(color.GetAlpha()*255, color.GetRed(), color.GetGreen(), color.GetBlue()));
    g->FillRectangle(&b, (A.GetX() - anchor.X) * s + anchor.X, (A.GetY() - anchor.Y) * s + anchor.Y, width * s, height * s);
    g->DrawRectangle(&p, (A.GetX() - anchor.X) * s + anchor.X, (A.GetY() - anchor.Y) * s + anchor.Y, width * s, height * s);
}


Shapes::Line::Line(){
    cout << "Line constructed\n";
}

void Shapes::Line::Read(XMLElement* E){
    start.SetX(E->FloatAttribute("x1"));
    start.SetY(E->FloatAttribute("y1"));
    end.SetX(E->FloatAttribute("x2"));
    end.SetY(E->FloatAttribute("y2"));

    const char* S = E->Attribute("stroke");

    if (S != nullptr){
        string tmp = S;
        SetStroke(tmp);
    }
    stroke_width = E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width");
    stroke.SetAlpha(E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));
}

void Shapes::Line::Draw(Graphics* g, float s, PointF anchor){
    Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width * s);
    g->DrawLine(&p, (start.GetX() - anchor.X) * s + anchor.X, (start.GetY() - anchor.Y) * s + anchor.Y, (end.GetX() - anchor.X) * s + anchor.X, (end.GetY() - anchor.Y) * s + anchor.Y);
}

Shapes::Circle::Circle(){
    cout << "Circle constructed\n";
    radius = 0;
}

void Shapes::Circle::Read(XMLElement* E){
    center.SetX(E->FloatAttribute("cx"));
    center.SetY(E->FloatAttribute("cy"));
    radius = E->FloatAttribute("r");

    const char* Col = E->Attribute("fill");
    const char* S = E->Attribute("stroke"); 
    if (Col != nullptr){
        string tmp = Col;
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

void Shapes::Circle::Draw(Graphics* g, float s, PointF anchor){
    Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width * s);
    SolidBrush b(Color(color.GetAlpha()*255, color.GetRed(), color.GetGreen(), color.GetBlue()));
    g->FillEllipse(&b, (center.GetX() - radius - anchor.X) * s + anchor.X, (center.GetY() - radius - anchor.Y) * s + anchor.Y, radius*2 * s, radius*2 * s);
    g->DrawEllipse(&p, (center.GetX() - radius - anchor.X) * s + anchor.X, (center.GetY() - radius - anchor.Y) * s + anchor.Y, radius*2 * s, radius*2 * s);
}

Shapes::Ellipse::Ellipse(){
    cout << "Eclipse constructed\n";
    radius_x = 0;
    radius_y = 0;
}

void Shapes::Ellipse::Read(XMLElement* E){
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

void Shapes::Ellipse::Draw(Graphics* g, float s, PointF anchor){
    Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width * s);
    SolidBrush b(Color(color.GetAlpha()*255, color.GetRed(), color.GetGreen(), color.GetBlue()));
    g->FillEllipse(&b, (center.GetX() - radius_x - anchor.X) * s + anchor.X, (center.GetY() - radius_y - anchor.Y) * s + anchor.Y, radius_x*2 * s, radius_y*2 * s);
    g->DrawEllipse(&p, (center.GetX() - radius_x - anchor.X) * s + anchor.X, (center.GetY() - radius_y - anchor.Y) * s + anchor.Y, radius_x*2 * s, radius_y*2 * s);
}

Shapes::Polygon::Polygon(){
    cout << "Polygon constructed\n";
}

void Shapes::Polygon::Read(XMLElement* E){

    stringstream ss(E->Attribute("points"));

    string tmp;
    while(ss >> tmp){
        stringstream sss(tmp);
        string a, b;
        getline(sss, a, ',');
        getline(sss, b);
        Points.push_back({(float) atof(a.c_str()), (float) atof(b.c_str())});
    }

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

void Shapes::Polygon::Draw(Graphics* g, float s, PointF anchor){
    vector<PointF> list;
    for (int i = 0; i < Points.size(); i++){
        list.push_back({(Points[i].GetX() - anchor.X) * s + anchor.X, (Points[i].GetY() - anchor.Y) * s + anchor.Y});
    }
    Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width * s);
    SolidBrush b(Color(color.GetAlpha()*255, color.GetRed(), color.GetGreen(), color.GetBlue()));

    g->FillPolygon(&b, list.data(), static_cast<int> (Points.size()));
    g->DrawPolygon(&p, list.data(), static_cast<int> (Points.size()));
}

Shapes::Polyline::Polyline(){
    cout << "Polyline constructed\n";
}

void Shapes::Polyline::Read(XMLElement* E){
    stringstream ss(E->Attribute("points"));

    string tmp;
    while(ss >> tmp){
        stringstream sss(tmp);
        string a, b;
        getline(sss, a, ',');
        getline(sss, b);
        Points.push_back({(float) atof(a.c_str()), (float) atof(b.c_str())});
    } 

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

void Shapes::Polyline::Draw(Graphics* g, float s, PointF anchor){
    vector <PointF> pF;
    int size = Points.size();
    for (int i = 0; i < size; i++){
        pF.push_back({(Points[i].GetX() - anchor.X) * s + anchor.X, (Points[i].GetY() - anchor.Y) * s + anchor.Y});
    }

    Pen p(Color(stroke.GetAlpha()*255, stroke.GetRed(), stroke.GetGreen(), stroke.GetBlue()), stroke_width * s);
    SolidBrush b(Color(color.GetAlpha()*255, color.GetRed(), color.GetGreen(), color.GetBlue()));

    g->FillPolygon(&b, pF.data(), static_cast<int> (pF.size()));

    if (stroke_width != 0) g->DrawLines(&p, pF.data(), static_cast<int>(pF.size()));
}

Shapes::Text::Text(){
    top.SetPoint(0,0);
    font_size = 0;
    text = "";
    cout << "Text constructor" << endl;
}

void Shapes::Text::Read(XMLElement* E){
    top.SetX(E->FloatAttribute("x"));
    top.SetY(E->FloatAttribute("y"));
    font_size = E->FloatAttribute("font-size");

    const char* t = E->GetText();

    top.SetY(top.GetY()-font_size);
    font_size /= float(4.0 / 3.0);

    const char* C = E->Attribute("fill");
    if (C != nullptr){
        string tmp = C;
        SetColor(tmp);
    }

    if (t){
        text = t;
    }
}

void Shapes::Text::Draw(Graphics *g, float s, PointF anchor){
    SolidBrush b(Color(color.GetAlpha()*255, color.GetRed(), color.GetGreen(), color.GetBlue()));
    Font TNR(L"Times New Roman", int(font_size * s));

    size_t size_needed = mbstowcs(nullptr, text.c_str(), 0);
    if (size_needed == static_cast<size_t>(-1)) {
        std::wcerr << L"Error converting string to wide string." << endl;
        return;
    }
    wstring wstr(size_needed, L'\0');
    mbstowcs(&wstr[0], text.c_str(), size_needed);

    g->DrawString(wstr.c_str(), -1, &TNR, {(top.GetX() - anchor.X) * s + anchor.X, (top.GetY() - anchor.Y) * s + anchor.Y}, &b);
}