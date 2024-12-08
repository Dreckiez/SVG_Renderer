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
        Object(const Object& obj);

        void copy(const Object& obj);

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

        virtual Object* clone() const;
        
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
        
	    Rectangle* clone() const override;
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

        Line* clone() const override;
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
        
        Circle* clone() const override;
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

        Ellipse* clone() const override;
    };

    class Polygon:public Object{
    private:
        vector<Point> Points;
    public:
        Polygon();
        
        vector<Point> getPoints();
        void setPoints(vector<Point>& pts);

        Polygon* clone() const override;
    };

    class Polyline:public Object{
    private:
        vector<Point> Points;
    public:
        Polyline();
       
        vector<Point> getPoints();
        void setPoints(vector<Point>& pts);

        Polyline* clone() const override;
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

        Text* clone() const override;
    };

    class Command{
    private:
        char cmd;
        vector<Gdiplus::PointF> points;
    public:
        Command();

        char getCmd();
        void setCmd(char c);

        vector<Gdiplus::PointF> getPoints();
        void addPoint(PointF p);

        string toString();
    };

    class Path:public Object{
    private:
        vector<Command> cmd;
    public:
        Path();

        vector<Command> getCmd();
        Command getCmdAt(int idx);

        void add(Command c);

        Path* clone() const override;
    };

    class Group : public Object{
    private:
        //can contain another Group
        vector<Object*> shapes; //kepp hold of shapes to draw later
    public:
        Group();

        void addShapes(Object* obj);

        int getSize();
        vector<Object*> getShapes();
        Object* operator [] (int idx) const;

        string toString();

        Group* clone() const override;

        ~Group();
    };

    class Transform{
    private:
        vector <Object*> content;
    public:

    };

};

#endif