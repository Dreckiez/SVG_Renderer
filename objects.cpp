
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <wchar.h>
#include <windows.h>
#include <gdiplus.h>
#include <locale>
#include <codecvt>

#include "objects.h"
#include "Reader.h"
#include "tinyxml2.h"
using namespace std;
using namespace Gdiplus;
using namespace tinyxml2;



bool isAlpha(char c){
    if (c >= 'a' && c <= 'z')
        return true;
    if (c >= 'A' && c <= 'Z')
        return true;
    return false;
}

bool isNum(char c){
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

Shapes::RGBA::RGBA(){
    red = 0;
    green = 0;
    blue = 0;
    opacity = 1;
    gradient_name = "";
}

string Shapes::RGBA::GetGradient(){
    return gradient_name;
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

void Shapes::RGBA::SetAlpha(float a){
    opacity = a;
}

void Shapes::RGBA::SetRGB(string s){
    if (s == ""){
    }
    if (s == "none" || s == "transparent"){
        opacity = 0.0;
    }
    else if (s.find('#') != string::npos){
        // Gradient format
        if(s.find("url(#") != string::npos){
            int start = s.find("#") + 1, end = s.size() - 1;
            gradient_name = s.substr(start, end - start);
            return;
        }
        // HEX CODE format
        if (s.size() == 4){
            red = stoi(string(2, s[1]), nullptr, 16);
            green = stoi(string(2, s[2]), nullptr, 16);
            blue = stoi(string(2, s[3]), nullptr, 16);
        }
        else if (s.size() == 7){
            red = stoi(s.substr(1,2), nullptr, 16);
            green = stoi(s.substr(3,2), nullptr, 16);
            blue = stoi(s.substr(5,2), nullptr, 16);
        }
    }
    else if (namedColors.find(s) != namedColors.end()){
        //Color Name
        tie(red, green, blue) = namedColors[s];
    }
    else if (s.find("hsl") != string::npos){
        s.erase(0,4);
        s.erase(s.size()-1, 1);
        for (int i = 0; i < s.size(); i++){
            if (s[i] == ',' && s[i+1] == ' ') s.erase(i,1);
            else if (s[i] == ',' && s[i+1] != ' ') s[i] = ' ';
        }

        int H,S,L;
        double C, X, m, tmp_r, tmp_g, tmp_b;

        stringstream ss(s);
        ss >> H >> S >> L;

        C = (1 - abs(2 * L/100.0 - 1)) * S/100.0;
        double tmp = H/60.0;
        tmp = tmp - (int)(tmp/2) * 2;
        X = C * (1 - abs(tmp - 1));

    }
    else if (s.find("rgba") != string::npos){
        // RGBA format
        s.erase(0,5);
        s.erase(s.size()-1, 1);
        for (int i = 0; i < s.size(); i++){
            if (s[i] == ',' && s[i+1] == ' ') s.erase(i,1);
            else if (s[i] == ',' && s[i+1] != ' ') s[i] = ' ';
        }
        stringstream ss(s);
        ss >> red >> green >> blue >> opacity;
    }
    else {
        // RGB format
        s.erase(0,4);
        s.erase(s.size()-1, 1);
        for (int i = 0; i < s.size(); i++){
            if (s[i] == ',' && s[i+1] == ' ') s.erase(i,1);
            else if (s[i] == ',' && s[i+1] != ' ') s[i] = ' ';
        }
        stringstream ss(s);
        ss >> red >> green >> blue;
    }
    if (red > 255)
        red = 255;
    if (green > 255)
        green = 255;
    if (blue > 255)
        blue = 255;
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
    stroke.SetAlpha(0);
    Transform = "";
    stroke_miterlimit = 4;
    fillRule = "nonzero";
}

void Shapes::Object::CopyAttribute(const Object &other){
    color = other.color;
    stroke = other.stroke;
    stroke_width = other.stroke_width;
    Transform = other.Transform;
}

void Shapes::Object::SetStyle(string s){
    removeSpareSpaces(s);
    string type;
    stringstream ss(s);
    while(!ss.eof()){
        getline(ss, type, ':');
        if(type == "fill"){
            string C;
            getline(ss, C, ';');
            color.SetRGB(C);
        }
        else if(type == "stroke"){
            string S;
            getline(ss, S, ';');
            stroke.SetRGB(S);
        }
    }
}

void Shapes::Object::SetAttribute(XMLElement* E){
    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    const char* Style = E->Attribute("style");

    if (T != nullptr) setTransformString(T);

    const char* check = E->Attribute("fill-opacity");
    if (check != nullptr)
        SetColorAlpha((float)atof(check));

    check = E->Attribute("stroke-opacity");

    if (check != nullptr)
        SetStrokeAlpha((float)atof(check));
        
    if(E->Attribute("stroke-width") != nullptr)
        setStrokeWidth(E->FloatAttribute("stroke-width"));

    if (C != nullptr){
        SetColor(C);
    }
     
    if (S != nullptr){
        SetStroke(S);
    }
    
    if(Style != nullptr){
        SetStyle(Style);
    }

    const char* FR = E->Attribute("fill-rule");
    if (FR){
        SetFillRule(FR);
    }

    FR = E->Attribute("stroke-miterlimit");
    if (FR){
        setStrokeMiterLimit(E->FloatAttribute("stroke-miterlimit"));
    }
}

void Shapes::Object::SetColor(string s){
    color.SetRGB(s);
}

void Shapes::Object::SetStroke(string s){
    stroke.SetAlpha(1);
    stroke.SetRGB(s);
}

void Shapes::Object::SetColorAlpha(float alpha){
    color.SetAlpha(alpha);
}

void Shapes::Object::SetStrokeAlpha(float alpha){
    stroke.SetAlpha(alpha);
}

void Shapes::Object::SetFillRule(string fillRule){
    this->fillRule = fillRule;
}

string Shapes::Object::getFillRule(){
    return fillRule;
}

Shapes::RGBA Shapes::Object::getColor(){
    return color;
}

Shapes::RGBA Shapes::Object::getStroke(){
    return stroke;
}

void Shapes::Object::setStrokeMiterLimit(int lim){
    stroke_miterlimit = lim;
}

int Shapes::Object::getStrokeMiterLimit(){
    return stroke_miterlimit;
}

void Shapes::Object::setStrokeWidth(float width) {
    stroke_width = width;
}

float Shapes::Object::getStrokeWidth(){
    return stroke_width;
}

string Shapes::Object::get_transform(){
    return Transform;
}

void Shapes::Object::setTransformString(const char* T){
    if(Transform != ""){
        Transform = Transform + " " + T;
    }
    else{
        Transform = T;
    }
}

void Shapes::Object::setTransform(Gdiplus::Matrix& M, float s, Gdiplus::PointF anchor){
    string type, para;
    float translate_x = 0, translate_y = 0, rotate = 0, scale_x = 1, scale_y = 1;
    stringstream ss(Transform);
    M.Translate(anchor.X*(1-s), anchor.Y*(1-s));
    while(getline(ss, type, '(')){
        if(type == "translate"){
            getline(ss, para, ')');
            int size = para.length();
            string number_string;
            for(int i = 0; i < size; i++){
                if(para[i] != ',' && para[i] != ' '){
                    number_string += para[i];
                }
                else if(translate_x == 0){
                    translate_x = stof(number_string);
                    number_string = "";
                    if(para[i+1] == ' ' || para[i+1] == ','){
                        i++;
                    }
                }
                if(i == size-1){
                    translate_y = stof(number_string);
                }
            }
            getline(ss, para, ' ');
            translate_x*=s;
            translate_y*=s;
            M.Translate(translate_x, translate_y);
            translate_x = 0;
            translate_y = 0;
        }
        else if(type == "scale"){
            string temp;
            getline(ss, temp, ')');
            stringstream scale_stream(temp);
            if (temp.find(',') != std::string::npos){
                getline(scale_stream, para, ',');
                scale_x = stof(para);
                getline(scale_stream, para, ')');
                scale_y = stof(para);
                getline(ss, para, ' ');
            }
            else{
                getline(scale_stream, para, ')');
                scale_x = stof(para);
                scale_y = stof(para);
                getline(ss, para, ' ');
            }
            M.Scale(scale_x, scale_y);
        }
        else if(type == "rotate"){
            getline(ss, para, ')');
            rotate = stof(para);
            float radian = rotate * 3.1415926 / (float)180;
            getline(ss, para, ' ');
            M.Rotate(rotate);
        }
    }
}


Shapes::Line::Line(){
    cout << "Line constructed\n";
}

// rectang 
Shapes::Rectangle::Rectangle(){
    height = 0;
    width = 0;
    cout << "Rectangle constructed\n";
}

void Shapes::Rectangle::setPoint(Point& p) {
    A = p;
}

void Shapes::Rectangle::setWidth(float w) {
    width = w;
}

void Shapes::Rectangle::setHeight(float h) {
    height = h;
}

Shapes::Point Shapes::Rectangle::getPoint(){
    return A;
}

float Shapes::Rectangle::getWidth(){
    return width;
}

float Shapes::Rectangle::getHeight(){
    return height;
}


// line


Shapes::Point Shapes::Line::getStart() {
    return start;
}

Shapes::Point Shapes::Line::getEnd() {
    return end;
}

void Shapes::Line::setStart(Point& s) {
    start = s;
}

void Shapes::Line::setEnd(Point& e) {
    end = e;
}

// circle


Shapes::Circle::Circle(){
    cout << "Circle constructed\n";
    radius = 0;
}

Shapes::Point Shapes::Circle::getCenter(){
    return center;
}

void Shapes::Circle::setCenter(Point& p) {
    center = p;
}

float Shapes::Circle::getRadius(){
    return radius;
}

void Shapes::Circle::setRadius(float r) {
    radius = r;
}
// ellipse 
Shapes::Ellipse::Ellipse(){
    cout << "Eclipse constructed\n";
    radius_x = 0;
    radius_y = 0;
}

Shapes::Point Shapes::Ellipse::getCenter(){
    return center;
}

void Shapes::Ellipse::setCenter(Point& p) {
    center = p;
}

float Shapes::Ellipse::getRadiusX(){
    return radius_x;
}

void Shapes::Ellipse::setRadiusX(float rx) {
    radius_x = rx;
}

float Shapes::Ellipse::getRadiusY(){
    return radius_y;
}

void Shapes::Ellipse::setRadiusY(float ry) {
    radius_y = ry;
}

// polygon
Shapes::Polygon::Polygon(){
    cout << "Polygon constructed\n";
}

vector<Shapes::Point> Shapes::Polygon::getPoints(){
    return Points;
}

void Shapes::Polygon::setPoints(vector<Point>& pts) {
    Points = pts;
}

// polyline
Shapes::Polyline::Polyline(){
    cout << "Polyline constructed\n";
}

vector<Shapes::Point> Shapes::Polyline::getPoints(){
    return Points;
}

void Shapes::Polyline::setPoints(vector<Point>& pts) {
    Points = pts;
}

// text
Shapes::Text::Text(){
    font_size = 30;
    text = L"";
    font_family = "Times New Roman";
    font_style = "";
    text_achor = "";
    cout << "Text constructed\n";
}

void Shapes::Text::add_dx(float dx){
    offset_x.push_back(dx);
}

void Shapes::Text::add_dy(float dy){
    offset_y.push_back(dy);
}

vector<float> Shapes::Text::Get_dx(){
    return offset_x;
}

vector<float> Shapes::Text::Get_dy(){
    return offset_y;
}

Shapes::Point Shapes::Text::getTop(int idx){
    if (idx < Top.size())
        return Top[idx];
    return (Shapes::Point){0,0};
}

void Shapes::Text::addTop(Point& p) {
    Top.push_back(p);
}

void Shapes::Text::setTop_X(int idx, float x){
    if (idx < Top.size())
        Top[idx].SetX(x);
}

void Shapes::Text::setTop_Y(int idx, float y){
    if (idx < Top.size())
        Top[idx].SetY(y);
}

float Shapes::Text::getFontSize(){
    return font_size;
}

void Shapes::Text::setFontSize(float size) {
    font_size = size;
}

string Shapes::Text::getFontFamily(){
    if (!font_family.empty()){
        if (font_family.find(',') == string::npos)
            return font_family;
        else{
            string tmp;
            stringstream ss(font_family);
            getline(ss, tmp, ',');
            return tmp;
        }
    }
    return "times new roman";
}

void Shapes::Text::setFontFamily(string ff){
    font_family = ff;
}

int Shapes::Text::getFontStyle(){
    if (font_style == "italic" || font_style == "oblique") return Gdiplus::FontStyleItalic;
    return Gdiplus::FontStyleRegular;
}

void Shapes::Text::setFontStyle(string fs){
    font_style = fs;
}

string Shapes::Text::getTextAnchor(){
    return text_achor;
}

void Shapes::Text::setTextAnchor(string ta){
    text_achor = ta;
}


wstring Shapes::Text::getText(){
    return text;
}

wstring Shapes::Text::StringToWstring(string& str) {
    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

void Shapes::Text::setText(string &str){
    text = StringToWstring(str);
}

Shapes::Group::Group(){
    cout << "Group constructed\n";
}

void Shapes::Group::AddShapes(Shapes::Object* obj){
    Shapes_List.push_back(obj);
}

Shapes::Object* Shapes::Group::GetShape(int idx){
    if (idx >= Shapes_List.size()) return nullptr;
    return Shapes_List[idx];
}

int Shapes::Group::GetSize(){
    return Shapes_List.size();
}

Shapes::Group::~Group(){
    for (int i = 0; i < Shapes_List.size(); i++){
        delete Shapes_List[i];
    }
    Shapes_List.clear();
}