#ifndef DRAWER_H
#define DRAWER_H

#include "objects.h"


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



#endif
