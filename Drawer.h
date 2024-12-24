#ifndef DRAWER_H
#define DRAWER_H

#include "objects.h"
#include "Path.h"
#include "Gradient.h"
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
    LinearGradientBrush* gb;
    Gdiplus::Matrix Ma;
    LinearVector gradientList;
public:
    Drawer(vector<unique_ptr<Shapes::Object>>& list, Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor, LinearVector linear);
    void Reset();
    void setDrawer(Shapes::Object* obj);
    void DrawR(Shapes::Object* obj);
    void FillRectGradient(Shapes::Rectangle* R);

    void DrawL(Shapes::Object* obj);

    void DrawC(Shapes::Object* obj);
    void FillCircleGradient(Shapes::Circle* C);

    void DrawE(Shapes::Object* obj);
    void FillEllipseGradient(Shapes::Ellipse* E);

    void DrawPG(Shapes::Object* obj);
    void FillPGGradient(Shapes::Polygon* PG,  Gdiplus::GraphicsPath* path);

    void DrawPL(Shapes::Object* obj);
    void FillPLGradient(Shapes::Polyline* PL, vector <Gdiplus::PointF> pF);

    void DrawT(Shapes::Object* obj);
    void FillTextGradient(Shapes::Text* T, Gdiplus::GraphicsPath* text);

    void DrawP(Shapes::Object* obj);
    void FillPGradient(Shapes::Path* P, Gdiplus::GraphicsPath* path);

    void DrawG(Shapes::Object* obj);
    void Draw();
    void setGradientBrush(string r, float alpha);
};    
#endif
