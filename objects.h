#ifndef OBJECTS_H
#define OBJECTS_H

#include<string>
#include<vector>
#include<windows.h>
#include<gdiplus.h>
#include<cmath>
#include "tinyxml2.h"
#include <memory>
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

    class Reader{
	public:
		Reader(); 
	    void ReadRectangle(Rectangle* rect, XMLElement* E);
	    void ReadLine(Line* line, XMLElement* E);
	    void ReadCircle(Circle* circle, XMLElement* E);
	    void ReadEllipse(Ellipse* ellipse, XMLElement* E);
	    void ReadPolygon(Polygon* polygon, XMLElement* E);
	    void ReadPolyline(Polyline* polyline, XMLElement* E);
	    void ReadText(Text* text, XMLElement* E);
        void ReadPath(Path* path, XMLElement *E);
	};

    class Drawer{
    private:
        vector <unique_ptr<Shapes::Object>> shapeList;
    public:
        Drawer(vector<unique_ptr<Shapes::Object>>& list);
        void DrawR(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
        void DrawL(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
        void DrawC(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
        void DrawE(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
        void DrawPG(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
        void DrawPL(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
        void DrawT(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
        void DrawP(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
        void Draw(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
    };    


    class Transform{
    private:
        vector <Object*> content;
    public:

    };

};

#endif