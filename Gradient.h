#ifndef GRADIENT_H
#define GRADIENT_H

#include "objects.h"

class Gradient{
protected:
    string id;
    Shapes::Point start;
    double stops[50];
    Gdiplus::Color colors[50];
    int amount;
    string Transform;
    bool isObjectBoundingBox;
    Shapes::Point end;
public:
    Gradient();
    Gradient(string I, Shapes::Point S);
    Shapes::Point& get_start();
    int get_amount();
    void set_amount(int s);
    string& get_id();
    void set_start(Shapes::Point p);
    void set_id(std::string str);
    double* get_stops();
    Gdiplus::Color* get_colors();
    void virtual read(XMLElement* gradientElem) = 0;
    bool getIsBoundingBox();
    void setTransform(Gdiplus::LinearGradientBrush* gb, float s, Gdiplus::PointF anchor);
    Shapes::Point& get_end();
    void set_end(Shapes::Point p);
    virtual void addStops(int& stops_amount, float alpha, Gdiplus::Color color_array[50], float stop_array[50]);
};

class LinearGradient : public Gradient{
public:
    void read(XMLElement* gradientElem) override;
    void setBrush(Shapes::Object* obj, Gdiplus::LinearGradientBrush*& gb, float alpha, float s);
};

class RadialGradient : public Gradient{
private:
    float radius;
public:
    void read(XMLElement* gradientElem) override;
    float get_radius();
    void setBrush(GraphicsPath& path, Gdiplus::PathGradientBrush*& rgb, float alpha, float s);
    void addStops(int& stops_amount, float alpha, Gdiplus::Color color_array[50], float stop_array[50]);
};

class GradientVector{
private:
    vector <Gradient*> content;
public:
    vector <Gradient*>& get_content();
    void read_gradient(tinyxml2::XMLElement* defs);
    void clear();
};

#endif
