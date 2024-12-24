#ifndef VIEWBOX_H
#define VIEWBOX_H

#include "objects.h"
#include "tinyxml2.h"
#include "General.h"
using namespace tinyxml2;

class ViewBox{
private:
    Shapes::Point Top;
    float width, height;
public:
    ViewBox();
    ViewBox(float x, float y, float w, float h);
    Shapes::Point GetTop();
    float GetWidth();
    float GetHeight();
    void SetTop(float x, float y);
    void ReadViewBox(XMLElement* E);
};

class ViewPort{
private:
    float width, height;
public:
    ViewPort();
    ViewPort(float w, float h);
    float GetWidth();
    float GetHeight();
    void SetWidth(float w);
    void SetHeight(float h);
    void ReadViewPort(XMLElement* E);
};

#endif