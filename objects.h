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
        virtual void Read(XMLElement* E) = 0;
        virtual void Draw(Graphics* g, float s) = 0;
    };

    class Rectangle:public Object{
    private:
        Point A;
        float width, height;
    public:
        Rectangle();
        void Read(XMLElement* E) override;
        void Draw(Graphics* g, float s) override;
    };

    class Line:public Object{
    private:
        Point start, end;
    public:
        Line();
        void Read(XMLElement* E) override;
        void Draw(Graphics* g, float s) override;
    };

    class Circle:public Object{
    private:
        Point center;
        float radius;
    public:
        Circle();
        void Read(XMLElement* E) override;
        void Draw(Graphics* g, float s) override;
    };

    class Ellipse:public Object{
    private:
        Point center;
        float radius_x, radius_y;
    public:
        Ellipse();
        void Read(XMLElement* E) override;
        void Draw(Graphics* g, float s) override;
    };

    class Polygon:public Object{
    private:
        vector<Point> Points;
    public:
        Polygon();
        void Read(XMLElement* E) override;
        void Draw(Graphics* g, float s) override;
    };

    class Polyline:public Object{
    private:
        vector<Point> Points;
    public:
        Polyline();
        void Read(XMLElement* E) override;
        void Draw(Graphics* g, float s) override;
    };

    class Text:public Object{
    private:
        Point top;
        float font_size;
        string text;
    public:
        Text();
        void Read(XMLElement* E) override;
        void Draw(Graphics* g, float s) override;
    };
}

#endif