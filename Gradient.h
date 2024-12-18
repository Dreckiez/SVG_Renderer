#ifndef GRADIENT_H
#define GRADIENT_H

#include "objects.h"

class LinearGradient{
private:
    string id;
    Shapes::Point start, end;
    float stops[50];
    Gdiplus::Color colors[50];
    int amount;
public:
    LinearGradient();
    LinearGradient(string I, Shapes::Point S, Shapes::Point E);
    Shapes::Point& get_start();
    Shapes::Point& get_end();
    int get_amount();
    void set_amount(int s);
    string& get_id();
    void set_start(Shapes::Point p);
    void set_end(Shapes::Point p);
    void set_id(std::string str);
    float* get_stops();
    Gdiplus::Color* get_colors();
};

class LinearVector{
private:
    vector <LinearGradient> content;
public:
    vector <LinearGradient>& get_content();
    void read_gradient(tinyxml2::XMLElement* defs);
};

#endif
