#include "Reader.h"

#include <iostream>
using namespace std;


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

    setAttribute(E, rect);
    
}

void Reader::setAttribute(tinyxml2::XMLElement* E, Shapes::Object* obj){
    const char* C = E->Attribute("fill");
    const char* S = E->Attribute("stroke");
    const char* T = E->Attribute("transform");
    
    if (T != nullptr)
        obj->setTransformString(T);

    if (C != nullptr){
        float opacity = 1;
        if (E->Attribute("fill-opacity") != nullptr)
            opacity = E->FloatAttribute("fill-opacity");
        obj->SetColor(C, opacity);
    }

    if (S != nullptr){
        float opacity = 1;
        if (E->Attribute("stroke-opacity") != nullptr)
            opacity = E->FloatAttribute("stroke-opacity");
        obj->SetStroke(S, opacity);
    }

    if (E->Attribute("stroke-width"))
        obj->setStrokeWidth(E->FloatAttribute("stroke-width"));
}

void Reader::ReadLine(Shapes::Line* line, XMLElement* E) {
    Shapes::Point start, end;

    start.SetX(E->FloatAttribute("x1"));
    start.SetY(E->FloatAttribute("y1"));
    line->setStart(start);

    end.SetX(E->FloatAttribute("x2"));
    end.SetY(E->FloatAttribute("y2"));
    line->setEnd(end);

    setAttribute(E, line);
}



void Reader::ReadCircle(Shapes::Circle* circle, XMLElement* E) {
    Shapes::Point center;

    center.SetX(E->FloatAttribute("cx"));
    center.SetY(E->FloatAttribute("cy"));
    circle->setCenter(center);

    circle->setRadius(E->FloatAttribute("r"));

    setAttribute(E, circle);
}


void Reader::ReadEllipse(Shapes::Ellipse* ellipse, XMLElement* E) {
    Shapes::Point center;

    center.SetX(E->FloatAttribute("cx"));
    center.SetY(E->FloatAttribute("cy"));
    ellipse->setCenter(center);

    ellipse->setRadiusX(E->FloatAttribute("rx"));
    ellipse->setRadiusY(E->FloatAttribute("ry"));

    setAttribute(E, ellipse);
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

    setAttribute(E, polygon);
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

    setAttribute(E, polyline);
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

    setAttribute(E, text);
}

void Reader::ReadPath(Shapes::Path* path, XMLElement *E){
    string d = E->Attribute("d");

    //replace all delimeter into spaces
    for (int i = 0; i < d.size(); i++){
        if ((d[i] == ',' || d[i] == '.' || d[i] == '\n')){
            d[i]= ' ';
        }
    }

    //insert spaces between command and numbers
    for (int i = 0; i < d.size() - 1; i++){
        if (isalpha(d[i])  && (isdigit(d[i + 1]) || d[i + 1] == '-')){
            d.insert(i + 1, " ");
        }else if ((isdigit(d[i]) || d[i] == '-') && isalpha(d[i + 1])){
            d.insert(i + 1, " ");
        }else if (isalpha(d[i] && isalpha(d[i + 1]))){
            d.insert(i + 1, " ");
        }
    }

    //remove excessive spaces
    for (int i = 0; i < d.size() - 1; i++){
        if (d[i] == ' ' && d[i + 1] == ' '){
            d.erase(i, 1);
            i--;
        }
    }

    cout << d << endl;

    stringstream ss(d);
    
    char c;
    float x = 0, y = 0;;
    int n;

    while (ss >> c){
        if (ss.fail()){

            break;
        }
        
        Shapes::Command cmd;
        cmd.setCmd(c);
        if (c == 'Z' || c == 'z'){
            path->add(cmd);
            continue;
        }
        if (c == 'H' || c == 'h'){
            ss >> x;
            PointF p(x, 0);
            cmd.addPoint(p);
        }else if (c == 'V' || c == 'v'){
            ss >> y;
            PointF p(0, y);
            cmd.addPoint(p);
        }else{
            while (ss){
                if (ss >> x >> y){
                    PointF p(x, y);
                    cmd.addPoint(p);
                }else{
                    ss.clear(); // Clear the fail state
                    //ss.ignore(numeric_limits<streamsize>::max(), ' ');
                    break;
                }
            }
        }
        path->add(cmd);
        // cout << ss.str() << endl;
    }

    for (int i = 0; i < path->getCmd().size(); i++){
        cout << path->getCmdAt(i).toString() << endl;
    }
    
    // for (int i = 0; i < d.size(); i++){
    //     if (isalpha(d[i])){
    //         path->addCmd(d[i]);
    //         if (d[i] == 'Z' || d[i] == 'z') break;
    //     }
    //     else if (isdigit(d[i])){
    //         num += d[i];
    //         if (i + 1 < d.size() && !isdigit(d[i+1])){
    //             path->addCoor(atof(num.c_str()));
    //             num.clear();
    //         }
    //     }
    // }

    // for (int i = 0; i < path->getCmd().size(); i++){
    //     cout << path->getCmd()[i] << " ";
    // }
    // cout << endl;
    // for (int i = 0; i < path->getCoor().size(); i++){
    //     cout << path->getCoor()[i] << " ";
    // }
    // cout << endl;

    setAttribute(E, path);
}

void Reader::ReadGroup(Shapes::Group* group, XMLElement *E){
    setAttribute(E, group);
    Shapes::Object* temp = group;
    Shapes::Object obj(*temp);
    
    // cout << Child->Name();

    for (XMLElement* Child = E->FirstChildElement(); Child != nullptr; Child = Child->NextSiblingElement()){
        string name = Child->Name();
        if (name == "rect"){
            unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Rectangle>();
            ptr->copy(obj);
            ReadRectangle(dynamic_cast<Shapes::Rectangle*>(ptr.get()), Child);
            group->addShapes(ptr.get());
        }
        else if (name == "line"){
            unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Line>();
            ptr->copy(obj);
            ReadLine(dynamic_cast<Shapes::Line*>(ptr.get()), Child);
            group->addShapes(move(ptr.get()));
        }
        else if (name == "circle"){
            unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Circle>();
            ptr->copy(obj);
            ReadCircle(dynamic_cast<Shapes::Circle*>(ptr.get()), Child); 
            group->addShapes(move(ptr.get()));
        }
        else if (name == "ellipse"){
            unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Ellipse>();
            ptr->copy(obj);
            ReadEllipse(dynamic_cast<Shapes::Ellipse*>(ptr.get()), Child); 
            group->addShapes(move(ptr.get()));  
        }
        else if (name == "polygon"){
            unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Polygon>();
            ptr->copy(obj);
            ReadPolygon(dynamic_cast<Shapes::Polygon*>(ptr.get()), Child);
            group->addShapes(move(ptr.get())); 
        }
        else if (name == "polyline"){
            unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Polyline>();
            ptr->copy(obj);
            ReadPolyline(dynamic_cast<Shapes::Polyline*>(ptr.get()), Child); 
            group->addShapes(move(ptr.get()));
        }
        else if (name == "text"){
            unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Text>();
            ptr->copy(obj);
            ReadText(dynamic_cast<Shapes::Text*>(ptr.get()), Child);
            group->addShapes(move(ptr.get()));
        }
        else if (name == "path"){
            unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Path>();
            ptr->copy(obj);
            ReadPath(dynamic_cast<Shapes::Path*>(ptr.get()), Child);
            group->addShapes(move(ptr.get()));
        }else if (name == "g"){
            unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Group>();
            ptr->copy(obj);
            ReadGroup(dynamic_cast<Shapes::Group*>(ptr.get()), Child);
            group->addShapes(move(ptr.get()));
        }
    }
    cout << group->getShapes().size() << " ";
    cout << group->toString();
}