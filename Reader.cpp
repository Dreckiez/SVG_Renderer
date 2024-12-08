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
    if (C != nullptr){
        string tmp = C;
        rect->SetColor(tmp);
    }
    
    const char* check = E->Attribute("fill-opacity");
    if (check != nullptr)
        rect->SetColorAlpha((float)atof(check));
    
    if (S != nullptr){
        string tmp = S;
        rect->SetStroke(tmp);
        
    }

    check = E->Attribute("stroke-opacity");
    if (check != nullptr)
        rect->SetStrokeAlpha((float)atof(check));
        
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
    if (S != nullptr){
        string tmp = S;
        line->SetStroke(tmp);
    }

    const char* check = E->Attribute("stroke-opacity");
    if (check != nullptr)
        line->SetStrokeAlpha((float)atof(check));

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
    if (C != nullptr){
        string tmp = C;
        circle->SetColor(tmp);
    }
    
    const char* check = E->Attribute("fill-opacity");
    if (check != nullptr)
        circle->SetColorAlpha((float)atof(check));

    if (S != nullptr){
        string tmp = S;
        circle->SetStroke(tmp);
    }
    
    check = E->Attribute("stroke-opacity");
    if (check != nullptr)
        circle->SetStrokeAlpha((float)atof(check));

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
    if (C != nullptr){
        string tmp = C;
        ellipse->SetColor(tmp);
    }
    
    const char* check = E->Attribute("fill-opacity");
    if (check != nullptr)
        ellipse->SetColorAlpha((float)atof(check));
    
    if (S != nullptr){
        string tmp = S;
        ellipse->SetStroke(tmp);
    }
    
    check = E->Attribute("stroke-opacity");
    if (check != nullptr)
        ellipse->SetStrokeAlpha((float)atof(check));

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
    if (C != nullptr){
        string tmp = C;
        polygon->SetColor(tmp);
    }
    
    const char* check = E->Attribute("fill-opacity");
    if (check != nullptr)
        polygon->SetColorAlpha((float)atof(check));
    
    if (S != nullptr){
        string tmp = S;
        polygon->SetStroke(tmp);
    }
    
    check = E->Attribute("stroke-opacity");
    if (check != nullptr)
        polygon->SetStrokeAlpha((float)atof(check));

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
    if (C != nullptr){
        string tmp = C;
        polyline->SetColor(tmp);
    }
    
    const char* check = E->Attribute("fill-opacity");
    if (check != nullptr)
        polyline->SetColorAlpha((float)atof(check));
    
    if (S != nullptr){
        string tmp = S;
        polyline->SetStroke(tmp);
    }
    
    check = E->Attribute("stroke-opacity");
    if (check != nullptr)
        polyline->SetStrokeAlpha((float)atof(check));

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
    if (C != nullptr){
        string tmp = C;
        text->SetColor(tmp);
    }
    
    const char* check = E->Attribute("fill-opacity");
    if (check != nullptr)
        text->SetColorAlpha((float)atof(check));

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
        path->SetColor(tmp);
    }
    
    const char* check = E->Attribute("fill-opacity");
    if (check != nullptr)
        path->SetColorAlpha((float)atof(check));
    
    if (S != nullptr){
        string tmp = S;
        path->SetStroke(tmp);
    }
    
    check = E->Attribute("stroke-opacity");
    if (check != nullptr)
        path->SetStrokeAlpha((float)atof(check));
    
    if (E->Attribute("stroke-width"))
        path->setStrokeWidth(E->FloatAttribute("stroke-width"));
}