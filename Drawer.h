#ifndef DRAWER_H
#define DRAWER_H

#include "objects.h"


class Drawer{
private:
    vector <unique_ptr<Shapes::Object>> shapeList;
    float s;
    Gdiplus::PointF anchor;
    Gdiplus::Graphics* g;
public:
    Drawer(vector<unique_ptr<Shapes::Object>>& list, Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor);
    void DrawR(Shapes::Rectangle* R);
    void DrawL(Shapes::Line* L);
    void DrawC(Shapes::Circle* C);
    void DrawE(Shapes::Ellipse* E);
    void DrawPG(Shapes::Polygon* PG);
    void DrawPL(Shapes::Polyline* PL);
    void DrawT(Shapes::Text* T);
    void DrawP(Shapes::Path* P);
    void DrawG(Shapes::Group* G);
    
    void Draw();
};    



#endif
