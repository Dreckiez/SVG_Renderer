#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <vector>
#include <windows.h>
#include <gdiplus.h>
#include <sstream>
#include <cmath>
#include "tinyxml2.h"
#include <memory>
#include "General.h"

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
        string gradient_name;
    public:
        RGBA();
        int GetRed();
        int GetGreen();
        int GetBlue();
        string GetGradient();
        float GetAlpha();
        void SetAlpha(float a);
        void SetRGB(string s);
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
        int stroke_miterlimit;
        string Transform;
        string fillRule;
    public:
        Object();
        Object(const Object& other);
        virtual ~Object() = default;

        Object& operator=(const Object& other);

        void SetAttribute(XMLElement* E);
        void CopyAttribute(const Object &other);

        void SetColor(string s);
        void SetStroke(string s);
        void SetColorAlpha(float alpha);
        void SetStrokeAlpha(float alpha);

        void SetFillRule(string s);
        string getFillRule();

        Shapes::RGBA getColor();
        Shapes::RGBA getStroke();

        void setStrokeMiterLimit(int lim);
        void setStrokeWidth(float width);
        int getStrokeMiterLimit();
        float getStrokeWidth();

        void setTransformString(const char* T);
        void setTransform(Gdiplus::Matrix& M, float s, Gdiplus::PointF anchor);
        string get_transform();
        virtual void setBoundingBox(Gdiplus::RectF& box);
    };

    class Rectangle:public Object{
    private:
        Point A;
        float width, height;
        float rx, ry;
    public:
        Rectangle();
        
        void setPoint(Point& p);
        Point getPoint();
        
        void setWidth(float w);
        float getWidth();
        
        void setHeight(float h);
        float getHeight();
        
        void setRx(float rx);
        void setRy(float ry);
        float getRx();
        float getRy();
        void setBoundingBox(Gdiplus::RectF& box) override;
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
        void setBoundingBox(Gdiplus::RectF& box) override;
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
        void setBoundingBox(Gdiplus::RectF& box) override;
    };

    class Polygon:public Object{
    private:
        vector<Point> Points;
        GraphicsPath path;
    public:
        Polygon();
        
        vector<Point> getPoints();
        void setPoints(vector<Point>& pts);
        void setBoundingBox(Gdiplus::RectF& box) override;
        void setPath(Gdiplus::GraphicsPath* p2);
        Gdiplus::GraphicsPath& getPath();
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
        vector<Point> Top;
        float font_size;
        wstring text;
        string text_achor;
        string font_family;
        string font_style;
        vector<float> offset_x;
        vector<float> offset_y;
        GraphicsPath path;
    public:
        Text();
        
        void add_dx(float dx);
        void add_dy(float dy);
        vector<float> Get_dx();
        vector<float> Get_dy();

    	Point getTop(int idx);
    	void addTop(Point& p);
        void setTop_X(int idx, float x);
        void setTop_Y(int idx, float y);
    	
        float getFontSize();
        void setFontSize(float size);
        
        string getFontFamily();
        void setFontFamily(string ff);
        int getFontStyle();
        void setFontStyle(string fs);
        string getTextAnchor();
        void setTextAnchor(string ta);

        wstring getText();
        wstring StringToWstring(string& str);
        void setText(string &str);
        void setPath(Gdiplus::GraphicsPath* p2);
        Gdiplus::GraphicsPath& getPath();
        void setBoundingBox(Gdiplus::RectF& box) override;
    };

    class Group:public Object{
    private:
        vector<Object*> Shapes_List;
    public:
        Group();
        void AddShapes(Object* obj);

        Object* GetShape(int idx);

        int GetSize();

        ~Group();
    };

};

#endif