
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
    opacity = 1;
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
    // cout << s << " ";
    if (s == "none"){
        opacity = 0;
    }
    else if (s.find('#') != string::npos){
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

    // cout << red << ' ' << green << ' ' << blue << ' ' << opacity << '\n';
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
}

void Shapes::Object::CopyAttribute(const Object &other){
    color = other.color;
    stroke = other.stroke;
    stroke_width = other.stroke_width;
    Transform = other.Transform;
}

void Shapes::Object::SetAttribute(XMLElement* E){
    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    // cout << S << '\n';
    // cout << C << '\n';
    
    if (T != nullptr){
        setTransformString(T);
    }
    if (C != nullptr){
        SetColor(C);
    }
    if (S != nullptr){
        SetStroke(S);
    }

    const char* check = E->Attribute("fill-opacity");
    if (check != nullptr)
        SetColorAlpha((float)atof(check));

    check = E->Attribute("stroke-opacity");

    if (check != nullptr)
        SetStrokeAlpha((float)atof(check));
        
    setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
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
    string str = T;
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
    translate_x*=s;
    translate_y*=s;
    M.Translate(anchor.X*(1-s), anchor.Y*(1-s));
    while(getline(ss, type, '(')){
        if(type == "translate"){
            getline(ss, para, ',');
            translate_x = stof(para);
            getline(ss, para, ')');
            translate_y = stof(para);
            getline(ss, para, ' ');
            M.Translate(translate_x, translate_y);
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
    top.SetPoint(0,0);
    font_size = 0;
    text = "";
    cout << "Text constructed\n";
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

void Shapes::Point::SetPoint(Point p){
    x = p.GetX();
    y = p.GetY();
}

Shapes::LinearGradient::LinearGradient(){
    id = "";
    start.SetPoint(0,0);
    end.SetPoint(0,0);
}

Shapes::LinearGradient::LinearGradient(string I, Point S, Point E){
    id = I;
    start.SetPoint(S);
    end.SetPoint(E);
}

Shapes::Point& Shapes::LinearGradient::get_start(){
    return start;
}

Shapes::Point& Shapes::LinearGradient::get_end(){
    return end;
}

std::string& Shapes::LinearGradient::get_id(){
    return id;
}

void Shapes::LinearGradient::set_start(Point p){
    start.SetPoint(p);
}

void Shapes::LinearGradient::set_end(Point p){
    end.SetPoint(p);
}

vector <float>& Shapes::LinearGradient::get_stops(){
    return stops;
}

vector <Shapes::RGBA>& Shapes::LinearGradient::get_colors(){
    return colors;
}

vector <Shapes::LinearGradient>& Shapes::LinearVector::get_content(){
    return content;
}

void Shapes::LinearGradient::set_id(std::string str){
    id = str;
}

void Shapes::LinearVector::read_gradient(tinyxml2::XMLElement* defs){
    Shapes::LinearGradient LG;
    for (XMLElement* gradientElem = defs->FirstChildElement("linearGradient");
         gradientElem; gradientElem = gradientElem->NextSiblingElement("linearGradient")) {
        LG.set_id(gradientElem->Attribute("id"));
        LG.get_colors().clear();
        LG.get_stops().clear();
        // Set start and end pos;
        if(gradientElem->Attribute("x1")){
            float x1 = atof(gradientElem->Attribute("x1")), y1 = atof(gradientElem->Attribute("y1"));
            float x2 = atof(gradientElem->Attribute("x2")), y2 = atof(gradientElem->Attribute("y2"));
            Point s(x1,y1), e(x2,y2);
            LG.set_start(s);
            LG.set_end(e);
        }
        else{
            int screenWidth = GetSystemMetrics(SM_CXSCREEN);
            Point s(0,0), e(screenWidth,0);
            LG.set_start(s);
            LG.set_end(e);
        }

        // Read all <stop> elements
        for (XMLElement* stopElem = gradientElem->FirstChildElement("stop");
             stopElem; stopElem = stopElem->NextSiblingElement("stop")) {
            float offset = 0.0f;
            stopElem->QueryFloatAttribute("offset", &offset);
            LG.get_stops().push_back(offset);

            string colorHex = stopElem->Attribute("stop-color");
            float stopOpacity = 1.0f;
            stopElem->QueryFloatAttribute("stop-opacity", &stopOpacity);
            RGBA color;
            color.SetRGB(colorHex);
            color.SetAlpha(stopOpacity);
            LG.get_colors().push_back(color);
        }
        // Add the gradient to the vector
        content.push_back(LG);
    }

    return;
}

string Shapes::Object::get_transform(){
    return Transform;
}