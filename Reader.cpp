#include "Reader.h"

Reader::Reader() {
    cout << "Reader constructed\n";
}

void Reader::ReadRectangle(Shapes::Rectangle* rect, XMLElement* E) {
    Shapes::Point p;
    p.SetX(E->FloatAttribute("x"));
    p.SetY(E->FloatAttribute("y"));
    rect->setPoint(p);

    rect->setWidth(E->FloatAttribute("width"));
    rect->setHeight(E->FloatAttribute("height"));

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    
    if (T != nullptr) rect->setTransformString(T);
    if (C != nullptr) rect->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    if (S != nullptr) rect->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));
    rect->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
    
}
void Reader::ReadLine(Shapes::Line* line, XMLElement* E) {
    Shapes::Point start, end;

    start.SetX(E->FloatAttribute("x1"));
    start.SetY(E->FloatAttribute("y1"));
    line->setStart(start);

    end.SetX(E->FloatAttribute("x2"));
    end.SetY(E->FloatAttribute("y2"));
    line->setEnd(end);

    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr) line->setTransformString(T);
    if (S != nullptr) line->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));
    line->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));

}



void Reader::ReadCircle(Shapes::Circle* circle, XMLElement* E) {
    Shapes::Point center;

    center.SetX(E->FloatAttribute("cx"));
    center.SetY(E->FloatAttribute("cy"));
    circle->setCenter(center);

    circle->setRadius(E->FloatAttribute("r"));

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr) circle->setTransformString(T);
    if (C != nullptr) circle->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    if (S != nullptr) circle->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));

    circle->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
}




void Reader::ReadEllipse(Shapes::Ellipse* ellipse, XMLElement* E) {
    Shapes::Point center;

    center.SetX(E->FloatAttribute("cx"));
    center.SetY(E->FloatAttribute("cy"));
    ellipse->setCenter(center);

    ellipse->setRadiusX(E->FloatAttribute("rx"));
    ellipse->setRadiusY(E->FloatAttribute("ry"));

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr) ellipse->setTransformString(T);
    if (C != nullptr) ellipse->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    if (S != nullptr) ellipse->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));

    ellipse->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
}




void Reader::ReadPolygon(Shapes::Polygon* polygon, XMLElement* E) {
    vector<Shapes::Point> points;
    stringstream ss(E->Attribute("points"));

    string tmp;
    while (ss >> tmp) {
        stringstream pointStream(tmp);
        string xStr, yStr;
        getline(pointStream, xStr, ',');
        getline(pointStream, yStr);

        Shapes::Point p;
        p.SetX(stof(xStr));
        p.SetY(stof(yStr));
        points.push_back(p);
    }
    polygon->setPoints(points);

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr) polygon->setTransformString(T);
    if (C != nullptr) polygon->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    if (S != nullptr) polygon->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));

    polygon->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
}



void Reader::ReadPolyline(Shapes::Polyline* polyline, XMLElement* E) {
    vector<Shapes::Point> points;
    stringstream ss(E->Attribute("points"));

    string tmp;
    while (ss >> tmp) {
        stringstream pointStream(tmp);
        string xStr, yStr;
        getline(pointStream, xStr, ',');
        getline(pointStream, yStr);

        Shapes::Point p;
        p.SetX(stof(xStr));
        p.SetY(stof(yStr));
        points.push_back(p);
    }
    polyline->setPoints(points);

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr) polyline->setTransformString(T);
    if (C != nullptr) polyline->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    if (S != nullptr) polyline->SetStroke(S, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));

    polyline->setStrokeWidth(E->Attribute("stroke-width") == nullptr ? 0 : E->FloatAttribute("stroke-width"));
}




void Reader::ReadText(Shapes::Text* text, XMLElement* E) {
    Shapes::Point top;

    top.SetX(E->FloatAttribute("x"));
    top.SetY(E->FloatAttribute("y"));
    text->setTop(top);

    float fontSize = E->FloatAttribute("font-size");
    text->setFontSize(fontSize);

    const char* t = E->GetText();
    if (t) {
    	std::string textStr(t);
        text->setText(textStr);
    }

    const char* T = E->Attribute("transform");
    if (T != nullptr) text->setTransformString(T);

    const char* C = E->Attribute("fill");
    if (C != nullptr) text->SetColor(C, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));

}

void Reader::ReadPath(Shapes::Path* path, XMLElement *E){
    string d = E->Attribute("d");
    
    cout << d << endl;

    char c;
    string num = "";
    int n;
    
    for (int i = 0; i < d.size(); i++){
        if (isalpha(d[i])){
            path->addCmd(d[i]);
            if (d[i] == 'Z' || d[i] == 'z') break;
        }
        else if (isdigit(d[i])){
            num += d[i];
            if (i + 1 < d.size() && !isdigit(d[i+1])){
                path->addCoor(atof(num.c_str()));
                num.clear();
            }
        }
    }

    for (int i = 0; i < path->getCmd().size(); i++){
        cout << path->getCmd()[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < path->getCoor().size(); i++){
        cout << path->getCoor()[i] << " ";
    }
    cout << endl;

    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    
    if (T != nullptr)
        path->setTransformString(T);
    if (C != nullptr){
        string tmp = C;
        path->SetColor(tmp, E->Attribute("fill-opacity") == nullptr ? 1 : E->FloatAttribute("fill-opacity"));
    }
    if (S != nullptr){
        string tmp = S;
        path->SetStroke(tmp, E->Attribute("stroke-opacity") == nullptr ? 1 : E->FloatAttribute("stroke-opacity"));
    }
    if (E->Attribute("stroke-width"))
        path->setStrokeWidth(E->FloatAttribute("stroke-width"));
}