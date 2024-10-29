#ifndef TESTH_H
#define TESTH_H

#include "tinyxml2.h"
using namespace tinyxml2;

class RGBA{
private:
    int red,green,blue;
    float alpha;
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

namespace Shapes{
    class Rectangle{
    private:
        RGBA color;
        RGBA stroke;
        float stroke_width;
        Point A;
        float width, height;
    public:
        Rectangle();
        void ReadRect(XMLElement* R);
        void GetCoord();
        void GetSize();
    };
}

#endif