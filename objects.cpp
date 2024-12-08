
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
using namespace Shapes;



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

RGBA::RGBA(){
    red = 0;
    green = 0;
    blue = 0;
    opacity = 0;
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
    return opacity;
}

void RGBA::SetRGB(int r, int g, int b){
    red = r;
    green = g;
    blue = b;
}

void RGBA::SetAlpha(float a){
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

Object::Object(){
    stroke_width = 0;
}

Object::Object(const Object& obj){
    this->color = obj.color;
    this->stroke = obj.stroke;
    this->stroke_width = obj.stroke_width;
    this->Transform = obj.Transform;
}

void Object::copy(const Object& obj){
    this->color = obj.color;
    this->stroke = obj.stroke;
    this->stroke_width = obj.stroke_width;
    this->Transform = obj.Transform;
    cout << "called copy\n";
}

void Object::SetColor(string s, float alpha){
    int r,g,b;
    if (s == "none")
        alpha = 0;

    StringToRGB(r,g,b,s);

    color.SetRGB(r,g,b);
    color.SetAlpha(alpha);
}

void Object::SetStroke(string s, float alpha){
    int r,g,b;
    if (s == "none")
        alpha = 0;

    StringToRGB(r,g,b,s);

    stroke.SetRGB(r,g,b);
    stroke.SetAlpha(alpha);
}

void Object::StringToRGB(int &r, int &g, int &b, string s){
    // cout << s << " ";
    if (s == "none"){
        r = b = g = 0;
    }else if (s.find('#') != string::npos){
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

RGBA Object::getColor(){
    return color;
}

RGBA Object::getStroke(){
    return stroke;
}

void Object::setStrokeWidth(float width) {
    stroke_width = width;
}

float Object::getStrokeWidth(){
    return stroke_width;
}

void Object::setTransformString(const char* T){
    Transform = T;
}

void Object::setTransform(Gdiplus::Matrix& M, float s, Gdiplus::PointF anchor){
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
Object* Object::clone() const {
    return new Object(*this);
}


Line::Line(){
    cout << "Line constructed\n";
}

// rectang 
Rectangle::Rectangle(){
    height = 0;
    width = 0;
    cout << "Rectangle constructed\n";
}

void Rectangle::setPoint(Shapes::Point& p) {
    A = p;
}

void Rectangle::setWidth(float w) {
    width = w;
}

void Rectangle::setHeight(float h) {
    height = h;
}

Shapes::Point Rectangle::getPoint(){
    return A;
}

float Rectangle::getWidth(){
    return width;
}

float Rectangle::getHeight(){
    return height;
}


Shapes::Rectangle* Rectangle::clone() const{
    return new Rectangle(*this);
}

// line


Shapes::Point Line::getStart() {
    return start;
}

Shapes::Point Line::getEnd() {
    return end;
}

void Line::setStart(Shapes::Point& s) {
    start = s;
}

void Line::setEnd(Shapes::Point& e) {
    end = e;
}

Line* Line::clone() const{
    return new Line(*this);
}

// circle


Circle::Circle(){
    cout << "Circle constructed\n";
    radius = 0;
}

Shapes::Point Circle::getCenter(){
    return center;
}

void Circle::setCenter(Shapes::Point& p) {
    center = p;
}

float Circle::getRadius(){
    return radius;
}

void Circle::setRadius(float r) {
    radius = r;
}

Circle* Circle::clone() const{
    return new Circle(*this);
}

// ellipse 
Ellipse::Ellipse(){
    cout << "Eclipse constructed\n";
    radius_x = 0;
    radius_y = 0;
}

Shapes::Point Ellipse::getCenter(){
    return center;
}

void Ellipse::setCenter(Shapes::Point& p) {
    center = p;
}

float Ellipse::getRadiusX(){
    return radius_x;
}

void Ellipse::setRadiusX(float rx) {
    radius_x = rx;
}

float Ellipse::getRadiusY(){
    return radius_y;
}

void Ellipse::setRadiusY(float ry) {
    radius_y = ry;
}

Shapes::Ellipse* Ellipse::clone() const{
    return new Ellipse(*this);
}

// polygon
Polygon::Polygon(){
    cout << "Polygon constructed\n";
}

vector<Shapes::Point> Polygon::getPoints(){
    return Points;
}

void Polygon::setPoints(vector<Shapes::Point>& pts) {
    Points = pts;
}

Shapes::Polygon* Polygon::clone() const{
    return new Polygon(*this);
}

// polyline
Polyline::Polyline(){
    cout << "Polyline constructed\n";
}

vector<Shapes::Point> Polyline::getPoints(){
    return Points;
}

void Polyline::setPoints(vector<Shapes::Point>& pts) {
    Points = pts;
}

Shapes::Polyline* Polyline::clone() const{
    return new Polyline(*this);
}

// text
Text::Text(){
    top.SetPoint(0,0);
    font_size = 0;
    text = "";
    cout << "Text constructor" << endl;
}

Shapes::Point Text::getTop(){
    return top;
}

void Text::setTop(Shapes::Point& p) {
    top = p;
}

float Text::getFontSize(){
    return font_size;
}

void Text::setFontSize(float size) {
    font_size = size;
}


string Text::getText(){
    return text;
}

void Text::setText(string& str) {
    text = str;
}

Shapes::Text* Text::clone() const{
    return new Text(*this);
}

//command
Command::Command(){
    cmd = ' ';
}

char Command::getCmd() { return cmd; }
void Command::setCmd(char c) { cmd = c; } 

vector<Gdiplus::PointF> Command::getPoints() { return points; }

void Command::addPoint(PointF p){
    points.push_back(p);
}

string Command::toString(){
    string s = "";
    s += cmd;
    for (int i = 0; i < points.size(); i++){
        s += to_string(points[i].X) + ' ' + to_string(points[i].Y) + ' ';
    }
    return s;
}

//path
Path::Path(){}

vector<Command> Path::getCmd(){ return cmd; }

Command Path::getCmdAt(int idx){
    if (idx <= cmd.size())
        return cmd[idx];
    return cmd[0];
}

void Path::add(Command c){
    cmd.push_back(c);
}

Shapes::Path* Path::clone() const{
    return new Path(*this);
}

//group
Group::Group(){}

void Group::addShapes(Object* obj){
    if (obj){
        Object* temp = obj->clone();
        shapes.push_back(temp);
    }
}

vector<Object*> Group::getShapes(){
    vector<Object*> g;
    for (int i = 0; i < shapes.size(); i++){
        Object* temp = new Object(*shapes[i]);
        g.push_back(temp);
    }
    return g;
}

int Group::getSize(){
    return shapes.size();
}

Object* Group::operator [] (int idx) const{
    if (idx < shapes.size()){
        return shapes[idx];
    }
    return NULL;
}

string Group::toString(){
    string s = "";
    for (int i = 0; i < shapes.size(); i++){
        Shapes::Object* rawPtr = shapes[i];
        if(dynamic_cast <Shapes::Rectangle*> (rawPtr)){
            s += "rect ";
        }
        else if(dynamic_cast <Shapes::Line*> (rawPtr)){
            s += "line ";
        }
        else if(dynamic_cast <Shapes::Circle*> (rawPtr)){
            s += "circle ";
        }
        else if(dynamic_cast <Shapes::Ellipse*> (rawPtr)){
            s += "ellipse ";
        }
        else if(dynamic_cast <Shapes::Polygon*> (rawPtr)){
            s += "polygon ";
        }
        else if(dynamic_cast <Shapes::Polyline*> (rawPtr)){
            s += "polyline ";
        }
        else if(dynamic_cast <Shapes::Text*> (rawPtr)){
            s += "text ";
        }
        else if(dynamic_cast <Shapes::Path*> (rawPtr)){
            s += "path ";
        }
        else if(dynamic_cast <Shapes::Group*> (rawPtr)){
            s += "group ";
        }
    }
    s += to_string(shapes.size()) + " \n";
    return s;
}

Shapes::Group* Group::clone() const{
    return new Group(*this);
}

Group::~Group(){
    for (int i = 0; i < shapes.size(); i++){
        if (shapes[i]){
            delete shapes[i];
            shapes[i] = NULL;
        }
    }
    shapes.clear();
}

