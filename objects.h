#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <windows.h>
#include <gdiplus.h>
#include <cmath>
#include "tinyxml2.h"
#include <memory>

using namespace std;
using namespace Gdiplus;
using namespace tinyxml2;

#include "NamedColors.cpp"

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
        void HexToRGB(string s);
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
        void SetPoint(Point p);
    };

    class LinearGradient{
    private:
        string id;
        Point start, end;
        vector <float> stops;
        vector <RGBA> colors;
    public:
        LinearGradient();
        LinearGradient(string I, Point S, Point E);
        Point& get_start();
        Point& get_end();
        string& get_id();
        void set_start(Point p);
        void set_end(Point p);
        void set_id(std::string str);
        vector <float>& get_stops();
        vector <RGBA>& get_colors();
    };

    class LinearVector{
    private:
        vector <LinearGradient> content;
    public:
        vector <LinearGradient>& get_content();
        void read_gradient(tinyxml2::XMLElement* defs);
    };

    class Object{
    protected:
        RGBA color;
        RGBA stroke;
        float stroke_width;
        string Transform;
    public:
        Object();
        virtual ~Object() = default;
        void SetColor(string s, float alpha);
        void SetStroke(string s, float alpha);

        void StringToRGB(int &r, int &g, int &b, string s);
        Shapes::RGBA getColor();
        Shapes::RGBA getStroke();

        void setStrokeWidth(float width);
        float getStrokeWidth();

        void setTransformString(const char* T);
        void setTransform(Gdiplus::Matrix& M, float s, Gdiplus::PointF anchor);
    };

    class Rectangle:public Object{
    private:
        Point A;
        float width, height;
    public:
        Rectangle();
        
        void setPoint(Point& p);
        Point getPoint();
        
        void setWidth(float w);
        float getWidth();
        
        void setHeight(float h);
        float getHeight();
        
	    
    };

    class Line:public Object{
    private:
        Point start, end;
    public:
        Line();
        
        void setStart(Point& s);
        Point getStart();
        
        void setEnd(Point& e);
        Point getEnd();
    };

    class Circle:public Object{
    private:
        Point center;
        float radius;
    public:
        Circle();
        
        Point getCenter();
        void setCenter(Point& p);
        
        float getRadius();
        void setRadius(float r);
        
    };

    class Ellipse:public Object{
    private:
        Point center;
        float radius_x, radius_y;
    public:
        Ellipse();
        
        Point getCenter();
        void setCenter(Point& p);
        
        float getRadiusX();
        void setRadiusX(float rx);
        
        float getRadiusY();
        void setRadiusY(float ry);
    };

    class Polygon:public Object{
    private:
        vector<Point> Points;
    public:
        Polygon();
        
        vector<Point> getPoints();
        void setPoints(vector<Point>& pts);
    };

    class Polyline:public Object{
    private:
        vector<Point> Points;
    public:
        Polyline();
       
        vector<Point> getPoints();
        void setPoints(vector<Point>& pts);
    };

    class Text:public Object{
    private:
        Point top;
        float font_size;
        string text;
    public:
        Text();
        
    	Point getTop();
    	void setTop(Point& p);
    	
        float getFontSize();
        void setFontSize(float size);
        
        string getText();
        void setText(string& str);
    };

    class Path:public Object{
    private:
        vector<char> cmd;
        vector<float> coor;
    public:
        Path();

        vector<char> getCmd();
        vector<float> getCoor();

        void addCmd(char c);
        void addCoor(float coordinate);
    };

    

    


    class Transform{
    private:
        vector <Object*> content;
    public:

    };

};

#endif