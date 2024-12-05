
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
    if (s == "none" || s == "black")
        s = "rgb(0,0,0)";
    else if (s == "white")
        s = "rgb(255,255,255)";
    else if (s == "red")
        s = "rgb(255,0,0)";
    else if (s == "green")
        s = "rgb(0,255,0)";
    else if (s == "blue"){
        s = "rgb(0,0,255)";
    }else if (s == "yellow")
        s = "rgb(255,255,0)";



    s.erase(0,4);
    s.erase(s.size()-1, 1);
    for (int i = 0; i < s.size(); i++){
        if (s[i] == ',' && s[i+1] == ' ') s.erase(i,1);
        else if (s[i] == ',' && s[i+1] != ' ') s[i] = ' ';
    }

    stringstream ss(s);
    ss >> r >> g >> b;
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


Shapes::Reader::Reader() {
    cout << "Reader constructed\n";
}

void Shapes::Reader::ReadRectangle(Rectangle* rect, XMLElement* E) {
    Point p;
    p.SetX(E->FloatAttribute("x"));
    p.SetY(E->FloatAttribute("y"));
    rect->setPoint(p);

    rect->setWidth(E->FloatAttribute("width"));
    rect->setHeight(E->FloatAttribute("height"));

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    
    if (T != nullptr) rect->setTransformString(T);
    if (C != nullptr) rect->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    if (S != nullptr) rect->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));
    rect->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
    
}
void Shapes::Reader::ReadLine(Line* line, XMLElement* E) {
    Point start, end;

    start.SetX(E->FloatAttribute("x1"));
    start.SetY(E->FloatAttribute("y1"));
    line->setStart(start);

    end.SetX(E->FloatAttribute("x2"));
    end.SetY(E->FloatAttribute("y2"));
    line->setEnd(end);

    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr) line->setTransformString(T);
    if (S != nullptr) line->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));
    line->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));

}



void Shapes::Reader::ReadCircle(Circle* circle, XMLElement* E) {
    Point center;

    center.SetX(E->FloatAttribute("cx"));
    center.SetY(E->FloatAttribute("cy"));
    circle->setCenter(center);

    circle->setRadius(E->FloatAttribute("r"));

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr) circle->setTransformString(T);
    if (C != nullptr) circle->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    if (S != nullptr) circle->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));

    circle->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
}




void Shapes::Reader::ReadEllipse(Ellipse* ellipse, XMLElement* E) {
    Point center;

    center.SetX(E->FloatAttribute("cx"));
    center.SetY(E->FloatAttribute("cy"));
    ellipse->setCenter(center);

    ellipse->setRadiusX(E->FloatAttribute("rx"));
    ellipse->setRadiusY(E->FloatAttribute("ry"));

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr) ellipse->setTransformString(T);
    if (C != nullptr) ellipse->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    if (S != nullptr) ellipse->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));

    ellipse->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
}




void Shapes::Reader::ReadPolygon(Polygon* polygon, XMLElement* E) {
    vector<Point> points;
    stringstream ss(E->Attribute("points"));

    string tmp;
    while (ss >> tmp) {
        stringstream pointStream(tmp);
        string xStr, yStr;
        getline(pointStream, xStr, ',');
        getline(pointStream, yStr);

        Point p;
        p.SetX(stof(xStr));
        p.SetY(stof(yStr));
        points.push_back(p);
    }
    polygon->setPoints(points);

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr) polygon->setTransformString(T);
    if (C != nullptr) polygon->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    if (S != nullptr) polygon->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));

    polygon->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
}



void Shapes::Reader::ReadPolyline(Polyline* polyline, XMLElement* E) {
    vector<Point> points;
    stringstream ss(E->Attribute("points"));

    string tmp;
    while (ss >> tmp) {
        stringstream pointStream(tmp);
        string xStr, yStr;
        getline(pointStream, xStr, ',');
        getline(pointStream, yStr);

        Point p;
        p.SetX(stof(xStr));
        p.SetY(stof(yStr));
        points.push_back(p);
    }
    polyline->setPoints(points);

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr) polyline->setTransformString(T);
    if (C != nullptr) polyline->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    if (S != nullptr) polyline->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));

    polyline->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
}




void Shapes::Reader::ReadText(Text* text, XMLElement* E) {
    Point top;

    top.SetX(E->FloatAttribute("x"));
    top.SetY(E->FloatAttribute("y"));
    text->setTop(top);

    float fontSize = E->FloatAttribute("font-size");
    text->setFontSize(fontSize);

    const char* t = E->GetText();
    if (t) {
    	std::string textStr(t);
        text->setText(textStr);
    }

    const char* T = E->Attribute("transform");
    if (T != nullptr) text->setTransformString(T);

    const char* C = E->Attribute("fill");
    if (C != nullptr) text->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));

}

void Shapes::Reader::ReadPath(Path* path, XMLElement *E){
    string dAttri = E->Attribute("d");
    
    cout << dAttri << endl;

    char c;
    string num = "";
    int n;
    
    for (int i = 0; i < dAttri.size(); i++){
        if (isAlpha(dAttri[i])){
            path->addCmd(dAttri[i]);
            if (dAttri[i] == 'Z' || dAttri[i] == 'z') break;
        }
        else if (isNum(dAttri[i])){
            num += dAttri[i];
            if (i + 1 < dAttri.size() && !isNum(dAttri[i+1])){
                path->addCoor(atof(num.c_str()));
                num.clear();
            }
        }
    }

    for (int i = 0; i < path->getCmd().size(); i++){
        cout << path->getCmd()[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < path->getCoor().size(); i++){
        cout << path->getCoor()[i] << " ";
    }
    cout << endl;

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    
    if (T != nullptr)
        path->setTransformString(T);
    if (C != nullptr){
        string tmp = C;
        path->SetColor(tmp, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    }
    if (S != nullptr){
        string tmp = S;
        path->SetStroke(tmp, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));
    }
    if (E->Attribute("stroke-width"))
        path->setStrokeWidth(E->FloatAttribute("stroke-width"));
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

void Shapes::Drawer::DrawP(Gdiplus::Graphics* g, float s, PointF anchor){
    Shapes::Path* PathPtr = dynamic_cast <Path*> (shapeList.front().get());
    if(PathPtr == NULL)
        return;
    Shapes::Path P = *PathPtr;
    GraphicsPath path;

    Gdiplus::Matrix Ma;
    P.setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);

    Pen p(Color(P.getStroke().GetAlpha()*255, P.getStroke().GetRed(), P.getStroke().GetGreen(), P.getStroke().GetBlue()), P.getStrokeWidth() * s);
    SolidBrush b(Color(P.getColor().GetAlpha()*255, P.getColor().GetRed(), P.getColor().GetGreen(), P.getColor().GetBlue()));

    vector<char> cmd = P.getCmd();
    vector<float> coor = P.getCoor();

    int pos = 0;
    float CurrX = 0;
    float CurrY = 0;
    float PrevX = 0;
    float PrevY = 0;
    float Prev_CurveX = 0;
    float Prev_CurveY = 0;

    path.StartFigure();
    
    for (int i = 0; i < cmd.size(); i++){
        cout << cmd[i] << '\n';
        if (cmd[i] == 'M' || cmd[i] == 'm'){
            CurrX = coor[pos];
            CurrY = coor[pos+1];

            if (cmd[i] == 'M'){
                path.AddLine((PointF){CurrX, CurrY}, (PointF){CurrX, CurrY});

                PrevX = CurrX;
                PrevY = CurrY;
                cout << "Move (absolute)\n";
            }
            else {
                path.AddLine((PointF){PrevX + CurrX, PrevY + CurrY}, (PointF){PrevX + CurrX, PrevY + CurrY});

                PrevX += CurrX;
                PrevY += CurrY;
                cout << "Move (relative)\n";
            }
            
            pos+=2;
        }
        else if (cmd[i] == 'L' || cmd[i] == 'l'){
            CurrX = coor[pos];
            CurrY = coor[pos+1];

            if (cmd[i] == 'L'){
                path.AddLine((PointF){PrevX, PrevY}, (PointF){CurrX, CurrY});

                PrevX = CurrX;
                PrevY = CurrY;
                cout << "Line (absolute)\n";
            }
            else {
                path.AddLine((PointF){PrevX, PrevY}, (PointF){PrevX + CurrX, PrevY + CurrY});

                PrevX += CurrX;
                PrevY += CurrY;
                cout << "Line (relative)\n";
            }

            pos += 2;
        }
        else if (cmd[i] == 'H' || cmd[i] == 'h'){
            CurrX = coor[pos];

            if (cmd[i] == 'H'){
                path.AddLine((PointF){PrevX, PrevY}, (PointF){CurrX, PrevY});

                PrevX = CurrX;
                cout << "Horizontal Line (absolute)\n";
            }
            else {
                path.AddLine((PointF){PrevX, PrevY}, (PointF){PrevX + CurrX, PrevY});
                
                PrevX += CurrX;
                cout << "Horizontal Line (relative)\n";
            }

            pos++;
        }
        else if (cmd[i] == 'V' || cmd[i] == 'v'){
            CurrY = coor[pos];
            
            if (cmd[i] == 'V'){
                path.AddLine((PointF){PrevX, PrevY}, (PointF){PrevX, CurrY});

                PrevY = CurrY;
                cout << "Vertical Line (absolute)\n";
            }
            else {
                path.AddLine((PointF){PrevX, PrevY}, (PointF){PrevX, PrevY + CurrY});
                
                PrevY += CurrY;
                cout << "Vertical (relative)\n";
            }

            pos++;
        }
        else if (cmd[i] == 'C'){
            float Control_P1_X = coor[pos++];
            float Control_P1_Y = coor[pos++];
            float Control_P2_X = coor[pos++];
            float Control_P2_Y = coor[pos++];
            CurrX = coor[pos];
            CurrY = coor[pos+1];

            path.AddBezier((PointF){PrevX, PrevY}, (PointF){Control_P1_X, Control_P1_Y}, (PointF){Control_P2_X, Control_P2_Y}, (PointF){CurrX, CurrY});
            
            PrevX = CurrX;
            PrevY = CurrY;
            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y;
            pos+=2;
            cout << "Cubic Bezier (absolute)\n";
        }
        else if (cmd[i] == 'c'){
            float Offset_P1_X = coor[pos++];
            float Offset_P1_Y = coor[pos++];
            float Offset_P2_X = coor[pos++];
            float Offset_P2_Y = coor[pos++];
            CurrX = coor[pos];
            CurrY = coor[pos+1];

            path.AddBezier((PointF){PrevX, PrevY}, (PointF){PrevX + Offset_P1_X, PrevY + Offset_P1_Y}, (PointF){PrevX + Offset_P2_X, PrevY + Offset_P2_Y}, (PointF){PrevX + CurrX, PrevY + CurrY});
            
            Prev_CurveX = PrevX + Offset_P2_X;
            Prev_CurveY = PrevY + Offset_P2_Y;
            PrevX += CurrX;
            PrevY += CurrY;
            pos+=2;
            cout << "Cubic Bezier (relative)\n";
        }
        else if (cmd[i] == 'S'){
            float Control_P1_X = 2 * PrevX - Prev_CurveX;
            float Control_P1_Y = 2 * PrevY - Prev_CurveY;
            float Control_P2_X = coor[pos++];
            float Control_P2_Y = coor[pos++];
            CurrX = coor[pos];
            CurrY = coor[pos+1];
            
            path.AddBezier((PointF){PrevX, PrevY}, (PointF){Control_P1_X, Control_P1_Y}, (PointF){Control_P2_X, Control_P2_Y}, (PointF){CurrX, CurrY});
            
            PrevX = CurrX;
            PrevY = CurrY;
            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y;
            pos+=2;
            cout << "Smooth Cubic Bezier (absolute)\n";
        }
        else if (cmd[i] == 's'){
            float Control_P1_X = 2 * PrevX - Prev_CurveX;
            float Control_P1_Y = 2 * PrevY - Prev_CurveY;
            float Offset_P2_X = coor[pos++];
            float Offset_P2_Y = coor[pos++];
            CurrX = coor[pos];
            CurrY = coor[pos+1];
            
            path.AddBezier((PointF){PrevX, PrevY}, (PointF){Control_P1_X, Control_P1_Y}, (PointF){PrevX + Offset_P2_X, PrevY + Offset_P2_Y}, (PointF){PrevX + CurrX, PrevY + CurrY});
            
            Prev_CurveX = PrevX + Offset_P2_X;
            Prev_CurveY = PrevY + Offset_P2_Y;
            PrevX += CurrX;
            PrevY += CurrY;
            pos+=2;
            cout << "Smooth Cubic Bezier (relative)\n";
        }
        else if (cmd[i] == 'Q'){
            float Quad_X = coor[pos++];
            float Quad_Y = coor[pos++];
            CurrX = coor[pos];
            CurrY = coor[pos+1];
            float Control_P1_X = PrevX + (2 * (Quad_X - PrevX))/3.0;
            float Control_P1_Y = PrevY + (2 * (Quad_Y - PrevY))/3.0;
            float Control_P2_X = CurrX + (2 * (Quad_X - CurrX))/3.0;
            float Control_P2_Y = CurrY + (2 * (Quad_Y - CurrY))/3.0;
            // cout << PrevX << ' ' << PrevY << ' ' << Prev_CurveX << ' ' << Prev_CurveY << ' ' << Control_P1_X << ' ' << Control_P1_Y << ' ' << Control_P2_X << ' ' << Control_P2_Y << ' ' << CurrX << ' ' << CurrY << '\n';

            path.AddBezier((PointF){PrevX, PrevY}, (PointF){Control_P1_X, Control_P1_Y}, (PointF){Control_P2_X, Control_P2_Y}, (PointF){CurrX, CurrY});

            PrevX = CurrX;
            PrevY = CurrY;
            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y;
            pos+=2;
            cout << "Quadratic Bezier (absolute)\n";
        }
        else if (cmd[i] == 'q'){
            float Quad_X = PrevX + coor[pos++];
            float Quad_Y = PrevY + coor[pos++];
            CurrX = PrevX + coor[pos];
            CurrY = PrevY + coor[pos+1];
            float Control_P1_X = PrevX + (2 * (Quad_X - PrevX))/3.0;
            float Control_P1_Y = PrevY + (2 * (Quad_Y - PrevY))/3.0;
            float Control_P2_X = CurrX + (2 * (Quad_X - CurrX))/3.0;
            float Control_P2_Y = CurrY + (2 * (Quad_Y - CurrY))/3.0;


            path.AddBezier((PointF){PrevX, PrevY}, (PointF){Control_P1_X, Control_P1_Y}, (PointF){Control_P2_X, Control_P2_Y}, (PointF){CurrX, CurrY});

            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y; 
            PrevX = CurrX;
            PrevY = CurrY;
            pos+=2;
            cout << "Quadratic Bezier (relative)\n";
        }
        else if (cmd[i] == 'T'){
            float Quad_X = 2 * PrevX - Prev_CurveX;
            float Quad_Y = 2 * PrevY - Prev_CurveY;
            CurrX = coor[pos];
            CurrY = coor[pos+1];
            float Control_P1_X = PrevX + (2 * (Quad_X - PrevX))/3.0;
            float Control_P1_Y = PrevY + (2 * (Quad_Y - PrevY))/3.0;
            float Control_P2_X = CurrX + (2 * (Quad_X - CurrX))/3.0;
            float Control_P2_Y = CurrY + (2 * (Quad_Y - CurrY))/3.0;

            path.AddBezier((PointF){PrevX, PrevY}, (PointF){Control_P1_X, Control_P1_Y}, (PointF){Control_P2_X, Control_P2_Y}, (PointF){CurrX, CurrY});

            PrevX = CurrX;
            PrevY = CurrY;
            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y;
            pos+=2;
            cout << "Smooth Quadratic Bezier (absolute)\n";
        }
        else if (cmd[i] == 't'){
            float Quad_X = 2 * PrevX - Prev_CurveX;
            float Quad_Y = 2 * PrevY - Prev_CurveY;
            CurrX = PrevX + coor[pos];
            CurrY = PrevY + coor[pos+1];
            float Control_P1_X = PrevX + (2 * (Quad_X - PrevX))/3.0;
            float Control_P1_Y = PrevY + (2 * (Quad_Y - PrevY))/3.0;
            float Control_P2_X = CurrX + (2 * (Quad_X - CurrX))/3.0;
            float Control_P2_Y = CurrY + (2 * (Quad_Y - CurrY))/3.0;

            path.AddBezier((PointF){PrevX, PrevY}, (PointF){Control_P1_X, Control_P1_Y}, (PointF){Control_P2_X, Control_P2_Y}, (PointF){CurrX, CurrY});

            PrevX = CurrX;
            PrevY = CurrY;
            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y;
            pos+=2;
            cout << "Smooth Quadratic Bezier (relative)\n";
        }
        else if (cmd[i] == 'Z' || cmd[i] == 'z'){
            path.CloseFigure();
            cout << "Close\n";
        }
    }

    g->DrawPath(&p, &path);
    g->FillPath(&b, &path);
    g->ResetTransform();
}

Shapes::Drawer::Drawer(vector <unique_ptr<Shapes::Object>>& list){
    int n = list.size();
    for(int i = 0; i < n; i++){
        shapeList.push_back(std::move(list[i]));
    }
}

void Shapes::Drawer::DrawR(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    if(Shapes::Rectangle* R = dynamic_cast <Rectangle*> (shapeList.front().get())){
        Gdiplus::Matrix Ma;
        R->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        Pen p(Gdiplus::Color(R->getStroke().GetAlpha()*255, R->getStroke().GetRed(), R->getStroke().GetGreen(), R->getStroke().GetBlue()), R->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(R->getColor().GetAlpha()*255, R->getColor().GetRed(), R->getColor().GetGreen(), R->getColor().GetBlue()));
        g->FillRectangle(&b, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
        g->DrawRectangle(&p, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
        g->ResetTransform();
    }
}

void Shapes::Drawer::DrawL(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    if(Shapes::Line* L = dynamic_cast <Line*> (shapeList.front().get())){
        Gdiplus::Matrix Ma;
        L->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);

        Pen p(Gdiplus::Color(L->getStroke().GetAlpha()*255, L->getStroke().GetRed(), L->getStroke().GetGreen(), L->getStroke().GetBlue()), L->getStrokeWidth() * s);
        g->DrawLine(&p, L->getStart().GetX() * s, L->getStart().GetY() * s, L->getEnd().GetX() * s, L->getEnd().GetY() * s);
        g->ResetTransform();
    }
}

void Shapes::Drawer::DrawC(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    if(Shapes::Circle* C = dynamic_cast <Circle*> (shapeList.front().get())){
        Gdiplus::Matrix Ma;
        C->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        Pen p(Gdiplus::Color(C->getStroke().GetAlpha()*255, C->getStroke().GetRed(), C->getStroke().GetGreen(), C->getStroke().GetBlue()), C->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(C->getColor().GetAlpha()*255, C->getColor().GetRed(), C->getColor().GetGreen(), C->getColor().GetBlue()));
        g->FillEllipse(&b, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
        g->DrawEllipse(&p, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
        g->ResetTransform();
    }
}

void Shapes::Drawer::DrawE(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    if(Shapes::Ellipse* E = dynamic_cast <Ellipse*> (shapeList.front().get())){
        Gdiplus::Matrix Ma;
        E->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        Pen p(Gdiplus::Color(E->getStroke().GetAlpha()*255, E->getStroke().GetRed(), E->getStroke().GetGreen(), E->getStroke().GetBlue()), E->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(E->getColor().GetAlpha()*255, E->getColor().GetRed(), E->getColor().GetGreen(), E->getColor().GetBlue()));
        g->FillEllipse(&b, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
        g->DrawEllipse(&p, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
        g->ResetTransform();
    }
}

void Shapes::Drawer::DrawPG(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    vector<Gdiplus::PointF> list;
    if(Shapes::Polygon* PG = dynamic_cast <Polygon*> (shapeList.front().get())){
        int n = PG->getPoints().size();
        for (int i = 0; i < n; i++){
            list.push_back({PG->getPoints()[i].GetX() * s, PG->getPoints()[i].GetY() * s});
        }
        Gdiplus::Matrix Ma;
        PG->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        Pen p(Gdiplus::Color(PG->getStroke().GetAlpha()*255, PG->getStroke().GetRed(), PG->getStroke().GetGreen(), PG->getStroke().GetBlue()), PG->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(PG->getColor().GetAlpha()*255, PG->getColor().GetRed(), PG->getColor().GetGreen(), PG->getColor().GetBlue()));

        g->FillPolygon(&b, list.data(), static_cast<int> (PG->getPoints().size()));
        g->DrawPolygon(&p, list.data(), static_cast<int> (PG->getPoints().size()));
        g->ResetTransform();
    }
}

void Shapes::Drawer::DrawPL(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    vector <Gdiplus::PointF> pF;
    if(Shapes::Polyline* PL = dynamic_cast <Polyline*> (shapeList.front().get())){
        int size = PL->getPoints().size();
        for (int i = 0; i < size; i++){
            pF.push_back({PL->getPoints()[i].GetX() * s, PL->getPoints()[i].GetY() * s});
        }
        Gdiplus::Matrix Ma;
        PL->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        Pen p(Gdiplus::Color(PL->getStroke().GetAlpha()*255, PL->getStroke().GetRed(), PL->getStroke().GetGreen(), PL->getStroke().GetBlue()), PL->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(PL->getColor().GetAlpha()*255, PL->getColor().GetRed(), PL->getColor().GetGreen(), PL->getColor().GetBlue()));
        g->FillPolygon(&b, pF.data(), static_cast<int> (pF.size()));
        if (PL->getStrokeWidth() != 0) g->DrawLines(&p, pF.data(), static_cast<int>(pF.size()));
        g->ResetTransform();
    }
}

void Shapes::Drawer::DrawT(Gdiplus::Graphics *g, float s, Gdiplus::PointF anchor){
    if(Shapes::Text* T = dynamic_cast <Text*> (shapeList.front().get())){
        Gdiplus::Matrix Ma;
        T->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        SolidBrush b(Gdiplus::Color(T->getColor().GetAlpha()*255, T->getColor().GetRed(), T->getColor().GetGreen(), T->getColor().GetBlue()));
        Font TNR(L"Times New Roman", int(T->getFontSize() * s));

        size_t size_needed = mbstowcs(nullptr, T->getText().c_str(), 0);
        if (size_needed == static_cast<size_t>(-1)) {
            std::wcerr << L"Error converting string to wide string." << endl;
            return;
        }
        wstring wstr(size_needed, L'\0');
        mbstowcs(&wstr[0], T->getText().c_str(), size_needed);
        Point p = T->getTop();
        p.SetY(p.GetY() - 1.33 * T->getFontSize());
        g->DrawString(wstr.c_str(), -1, &TNR, {p.GetX() * s, p.GetY() * s}, &b);
        g->ResetTransform();
    }
}

void Shapes::Drawer::Draw(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    while(!shapeList.empty()){
        Object* rawPtr = shapeList.front().get();
        if(dynamic_cast <Rectangle*> (rawPtr)){
            DrawR(g, s, anchor);
        }
        else if(dynamic_cast <Line*> (rawPtr)){
            DrawL(g, s, anchor);
        }
        else if(dynamic_cast <Circle*> (rawPtr)){
            DrawC(g, s, anchor);
        }
        else if(dynamic_cast <Ellipse*> (rawPtr)){
            DrawE(g, s, anchor);
        }
        else if(dynamic_cast <Polygon*> (rawPtr)){
            DrawPG(g, s, anchor);
        }
        else if(dynamic_cast <Polyline*> (rawPtr)){
            DrawPL(g, s, anchor);
        }
        else if(dynamic_cast <Text*> (rawPtr)){
            DrawT(g, s, anchor);
        }else if(dynamic_cast <Path*> (rawPtr)){
            DrawP(g, s, anchor);
        }
        shapeList.erase(shapeList.begin());
    }
}