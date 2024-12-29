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
    void setTransform(Gdiplus::LinearGradientBrush* gb, float s, Gdiplus::PointF anchor);
};

class LinearGradient : public Gradient{
private:
    Shapes::Point end;
public:
    void read(XMLElement* gradientElem) override;
    Shapes::Point& get_end();
    void set_end(Shapes::Point p);
};

class RadialGradient : public Gradient{
private:
    float radius;
public:
    void read(XMLElement* gradientElem) override;
    float get_radius();
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
