#ifndef OBJECTS_H
#define OBJECTS_H

#include<string>
#include<vector>
#include<windows.h>
#include<gdiplus.h>
#include "tinyxml2.h"
using namespace std;
using namespace Gdiplus;
using namespace tinyxml2;

namespace Shapes{
    class RGBA{
    private:    
        int red;
        int green;
        int blue;
        float opacity;
    public:
        RGBA();
        int GetRed();
        int GetGreen();
        int GetBlue();
        float GetAlpha();
        void SetRGB(int r, int g, int b);
        void SetAlpha(float a);
    };

    class Point{
    private:
        float x,y;
    public:
        Point();
        Point(float a, float b);
        float GetX();
        float GetY();
        void SetX(float a);
        void SetY(float a);
        void SetPoint(float a, float b);
    };

    class Object{
    protected:
        RGBA color;
        RGBA stroke;
        float stroke_width;
    public:
        Object();
        void SetColor(string s);
        void SetStroke(string s);
        void GetColor();
        void GetStroke();
    };

    class Rectangle:Object{
    private:
        Point A;
        float width, height;
    public:
        Rectangle();
        void ReadRect(XMLElement* R);
        void GetCoord();
        void GetSize();
        void DrawR(Graphics* g);
    };

    class Line:Object{
    private:
        Point start, end;
    public:
        Line();
        void ReadLine(XMLElement* L);
        void DrawL(Graphics* g);
    };

    class Circle:Object{
    private:
        Point center;
        float radius;
    public:
        Circle();
        void ReadCircle(XMLElement* C);
        void DrawC(Graphics* g);
    };

    class Ellipse:Object{
    private:
        Point center;
        float radius_x, radius_y;
    public:
        Ellipse();
        void ReadEllipse(XMLElement* E);
        void DrawE(Graphics* g);
    };

    class Polygon:Object{
    private:
        vector<Point> Points;
    public:
        Polygon();
        void ReadPolygon(XMLElement* PG);
        void GetCoords();
        void DrawPG(Graphics* g);
    };

    class Polyline:Object{
    private:
        vector<Point> Points;
    public:
        Polyline();
        void ReadPolyLine(XMLElement* PL);
        void DrawPL(Graphics* g);
    };
}

#endif