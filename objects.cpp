
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <wchar.h>
#include <windows.h>
#include <gdiplus.h>

#include "objects.h"
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

void Shapes::Object::SetColor(string s, float alpha){
    int r,g,b;
    if (s == "none")
        alpha = 0;

    StringToRGB(r,g,b,s);

    color.SetRGB(r,g,b);
    color.SetAlpha(alpha);
}

void Shapes::Object::SetStroke(string s, float alpha){
    int r,g,b;
    if (s == "none")
        alpha = 0;

    StringToRGB(r,g,b,s);

    stroke.SetRGB(r,g,b);
    stroke.SetAlpha(alpha);
}

void Shapes::Object::StringToRGB(int &r, int &g, int &b, string s){
    // cout << s << " ";
    size_t hex = s.find('#');
    if (hex != string::npos){
        // cout << "hex ";
        if (s.size() == 4){
            r = stoi(string(2, s[1]), nullptr, 16);
            g = stoi(string(2, s[2]), nullptr, 16);
            b = stoi(string(2, s[3]), nullptr, 16);
        }else if (s.size() == 7){
            r = stoi(s.substr(1,2), nullptr, 16);
            g = stoi(s.substr(3,2), nullptr, 16);
            b = stoi(s.substr(5,2), nullptr, 16);
        }
    }else if (namedColors.find(s) != namedColors.end()){
        // cout << "named ";
         tie(r, g, b) = namedColors[s];
    }else{
        // cout << "rgb ";
        s.erase(0,4);
        s.erase(s.size()-1, 1);
        for (int i = 0; i < s.size(); i++){
            if (s[i] == ',' && s[i+1] == ' ') s.erase(i,1);
            else if (s[i] == ',' && s[i+1] != ' ') s[i] = ' ';
        }

        stringstream ss(s);
        ss >> r >> g >> b;
    }
    // cout << r << " " << g << " " << b << endl;
    
}

Shapes::RGBA Shapes::Object::getColor(){
    return color;
}

Shapes::RGBA Shapes::Object::getStroke(){
    return stroke;
}

void Shapes::Object::setStrokeWidth(float width) {
    stroke_width = width;
}

float Shapes::Object::getStrokeWidth(){
    return stroke_width;
}

void Shapes::Object::setTransformString(const char* T){
    Transform = T;
}

void Shapes::Object::setTransform(Gdiplus::Matrix& M, float s, Gdiplus::PointF anchor){
    string type, para;
    float translate_x = 0, translate_y = 0, rotate = 0, scale_x = 1, scale_y = 1;
    stringstream ss(Transform);
    while(getline(ss, type, '(')){
        if(type == "translate"){
            getline(ss, para, ',');
            translate_x = stof(para);
            getline(ss, para, ')');
            translate_y = stof(para);
            getline(ss, para, ' ');
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
        }
        else if(type == "rotate"){
            getline(ss, para, ')');
            rotate = stof(para);
            float radian = rotate * 3.1415926 / (float)180;
            getline(ss, para, ' ');
        }
    }
    translate_x*=s;
    translate_y*=s;
    M.Translate(anchor.X*(1-s), anchor.Y*(1-s));
    M.Translate(translate_x, translate_y);
    M.Rotate(rotate);
    M.Scale(scale_x, scale_y);
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
    top.SetPoint(0,0);
    font_size = 0;
    text = "";
    cout << "Text constructor" << endl;
}

Shapes::Point Shapes::Text::getTop(){
    return top;
}

void Shapes::Text::setTop(Point& p) {
    top = p;
}

float Shapes::Text::getFontSize(){
    return font_size;
}

void Shapes::Text::setFontSize(float size) {
    font_size = size;
}


string Shapes::Text::getText(){
    return text;
}

void Shapes::Text::setText(string& str) {
    text = str;
}


Shapes::Path::Path(){
    cout << "Path constructed\n";
}

void Shapes::Path::addCmd(char c){
    cmd.push_back(c);
}

void Shapes::Path::addCoor(float coordinate){
    coor.push_back(coordinate);
}

vector<char> Shapes::Path::getCmd(){
    return cmd;
}

vector<float> Shapes::Path::getCoor(){
    return coor;
}



