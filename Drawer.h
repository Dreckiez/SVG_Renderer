#ifndef DRAWER_H
#define DRAWER_H

#include "objects.h"
#include "Path.h"
#include "gdiplus.h"

using namespace Gdiplus;

class Drawer{
private:
    vector <unique_ptr<Shapes::Object>> shapeList;
    Gdiplus::Graphics* g;
    float s;
    Gdiplus::PointF anchor;
    Pen* p;
    SolidBrush* b;
    Gdiplus::Matrix Ma;
public:
    Drawer(vector<unique_ptr<Shapes::Object>>& list, Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
    
    void setDrawer(Shapes::Object* obj);
    void DrawR(Shapes::Object* obj);
    void DrawL(Shapes::Object* obj);
    void DrawC(Shapes::Object* obj);
    void DrawE(Shapes::Object* obj);
    void DrawPG(Shapes::Object* obj);
    void DrawPL(Shapes::Object* obj);
    void DrawT(Shapes::Object* obj);
    void DrawP(Shapes::Object* obj);
    void DrawG(Shapes::Object* obj);
    void Draw();
};    



#endif
