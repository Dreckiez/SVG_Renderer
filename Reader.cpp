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

    rect->SetAttribute(E);
}

void Reader::ReadLine(Shapes::Line* line, XMLElement* E) {
    Shapes::Point start, end;

    start.SetX(E->FloatAttribute("x1"));
    start.SetY(E->FloatAttribute("y1"));
    line->setStart(start);

    end.SetX(E->FloatAttribute("x2"));
    end.SetY(E->FloatAttribute("y2"));
    line->setEnd(end);

    line->SetAttribute(E);
}

void Reader::ReadCircle(Shapes::Circle* circle, XMLElement* E) {
    Shapes::Point center;

    center.SetX(E->FloatAttribute("cx"));
    center.SetY(E->FloatAttribute("cy"));
    circle->setCenter(center);

    circle->setRadius(E->FloatAttribute("r"));

    circle->SetAttribute(E);
}

void Reader::ReadEllipse(Shapes::Ellipse* ellipse, XMLElement* E) {
    Shapes::Point center;

    center.SetX(E->FloatAttribute("cx"));
    center.SetY(E->FloatAttribute("cy"));
    ellipse->setCenter(center);

    ellipse->setRadiusX(E->FloatAttribute("rx"));
    ellipse->setRadiusY(E->FloatAttribute("ry"));

    ellipse->SetAttribute(E);
}

void Reader::ReadPolygon(Shapes::Polygon* polygon, XMLElement* E) {
    vector<Shapes::Point> points;
    string p = E->Attribute("points");

    addSpaces(p);
    removeSpareSpaces(p);

    stringstream ss(p);

    float x = 0;
    float y = 0;
    string tmp;
    while (ss >> x >> y) {
        Shapes::Point p;
        p.SetX(x);
        p.SetY(y);
        points.push_back(p);
    }
    polygon->setPoints(points);

    polygon->SetAttribute(E);
}

void Reader::ReadPolyline(Shapes::Polyline* polyline, XMLElement* E) {
    vector<Shapes::Point> points;
    string p = E->Attribute("points");

    addSpaces(p);
    removeSpareSpaces(p);

    stringstream ss(p);

    float x = 0;
    float y = 0;
    string tmp;
    while (ss >> x >> y) {
        Shapes::Point p;
        p.SetX(x);
        p.SetY(y);
        points.push_back(p);
    }
    polyline->setPoints(points);

    polyline->SetAttribute(E);
}

void Reader::ReadText(Shapes::Text* text, XMLElement* E) {
    float fontsize;
    if (E->Attribute("font-size")){
        fontsize = E->FloatAttribute("font-size");
        text->setFontSize(fontsize);
    }
    
    Shapes::Point top;

    float x = E->FloatAttribute("x");
    float y = (E->FloatAttribute("y"));
    float dx = 0, dy = 0;

    if (E->Attribute("dx")){
        dx = E->FloatAttribute("dx");
    }

    if (E->Attribute("dy")){
        dy = E->FloatAttribute("dy");
    }

    top.SetX(x + dx);
    top.SetY(y + dy - 1.33 * text->getFontSize());
    text->setTop(top);


    const char* FF = E->Attribute("font-family");
    const char* FS = E->Attribute("font-style");
    const char* TA = E->Attribute("text-anchor");

    if (FF){
        string tmp = FF;
        text->setFontFamily(tmp);
    }

    if (FS){
        string tmp = FS;
        text->setFontStyle(tmp);
    }

    if (TA){
        string tmp = TA;
        text->setTextAnchor(tmp);
    }

    const char* t = E->GetText();
    if (t) {
    	string textStr(t);
        text->setText(textStr);
    }

    text->SetAttribute(E);

    // text->CheckAtt();
}

void Reader::ReadPath(Shapes::Path* path, XMLElement *E){
    string d = E->Attribute("d");

    //replace all delimeter into spaces
    addSpaces(d);

    //insert spaces between command and numbers
    for (int i = 0; i < d.size() - 1; i++){
        if (isalpha(d[i]) && (isdigit(d[i + 1]) || d[i + 1] == '-')){
            d.insert(i + 1, " ");
        }else if ((isdigit(d[i]) || d[i] == '-') && isalpha(d[i + 1])){
            d.insert(i + 1, " ");
        }else if (isalpha(d[i] && isalpha(d[i + 1]))){
            d.insert(i + 1, " ");
        }
    }

    removeSpareSpaces(d);

    for (int i = 0; i < d.size() - 1; i++){
        if (d[i] == 'e'){
            d.erase(i - 1, 1);
            d.erase(i, 1);
        }
    }

    cout << d << endl;

    stringstream ss(d);
    
    char c;
    float num = 0;
    int n;

    while (ss >> c){
        if (ss.fail()){
            break;
        }
        Shapes::Command cmd;
        cmd.setCmd(c);
        while (ss){
            if (ss >> num){
                cmd.addNum(num);
            }else{
                ss.clear(); // Clear the fail state
                //ss.ignore(numeric_limits<streamsize>::max(), ' ');
                break;
            }
        }
        path->add(cmd);
    }
        // cout << ss.str() << endl;

    for (int i = 0; i < path->getCmd().size(); i++){
        cout << path->getCmdAt(i).toString() << endl;
    }
    

    path->SetAttribute(E);
}

void Reader::ReadGroup(Shapes::Group* group, XMLElement* E){
    group->SetAttribute(E);

    for (XMLElement* child = E->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
        unique_ptr<Shapes::Object> ptr;
        string name = child->Name();
        if (name == "rect"){
            ptr = make_unique<Shapes::Rectangle>();
            ptr->CopyAttribute(*group);
            ReadRectangle(dynamic_cast<Shapes::Rectangle*>(ptr.get()), child);
        }
        else if (name == "line"){
            ptr = make_unique<Shapes::Line>();
            ptr->CopyAttribute(*group);
            ReadLine(dynamic_cast<Shapes::Line*>(ptr.get()), child);
        }
        else if (name == "circle"){
            ptr = make_unique<Shapes::Circle>();
            ptr->CopyAttribute(*group);
            ReadCircle(dynamic_cast<Shapes::Circle*>(ptr.get()), child);
        }
        else if (name == "ellipse"){
            ptr = make_unique<Shapes::Ellipse>();
            ptr->CopyAttribute(*group);
            ReadEllipse(dynamic_cast<Shapes::Ellipse*>(ptr.get()), child);
        }
        else if (name == "polygon"){
            ptr = make_unique<Shapes::Polygon>();
            ptr->CopyAttribute(*group);
            ReadPolygon(dynamic_cast<Shapes::Polygon*>(ptr.get()), child);
        }
        else if (name == "polyline"){
            ptr = make_unique<Shapes::Polyline>();
            ptr->CopyAttribute(*group);
            ReadPolyline(dynamic_cast<Shapes::Polyline*>(ptr.get()), child);
        }
        else if (name == "text"){
            ptr = make_unique<Shapes::Text>();
            ptr->CopyAttribute(*group);
            ReadText(dynamic_cast<Shapes::Text*>(ptr.get()), child);
        }
        else if (name == "path"){
            ptr = make_unique<Shapes::Path>();
            ptr->CopyAttribute(*group);
            ReadPath(dynamic_cast<Shapes::Path*>(ptr.get()), child);
        }
        else if (name == "g"){
            ptr = make_unique<Shapes::Group>();
            ptr->CopyAttribute(*group);
            ReadGroup(dynamic_cast<Shapes::Group*>(ptr.get()), child);
        }
        group->AddShapes(ptr.release());
    }
}