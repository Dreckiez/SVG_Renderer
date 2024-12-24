#ifndef READER_H
#define READER_H

#include <cctype>
#include <sstream>

#include "objects.h"
#include "Path.h"
#include "General.h"


void addSpaces(string &s);
void removeSpareSpaces(string &s);

class Reader{
public:
    Reader();
    void ReadRectangle(Shapes::Rectangle* rect, XMLElement* E);
    void ReadLine(Shapes::Line* line, XMLElement* E);
    void ReadCircle(Shapes::Circle* circle, XMLElement* E);
    void ReadEllipse(Shapes::Ellipse* ellipse, XMLElement* E);
    void ReadPolygon(Shapes::Polygon* polygon, XMLElement* E);
    void ReadPolyline(Shapes::Polyline* polyline, XMLElement* E);
    void ReadText(Shapes::Text* text, XMLElement* E);
    void ReadPath(Shapes::Path* path, XMLElement *E);
    void ReadGroup(Shapes::Group* group, XMLElement* E);
};


#endif