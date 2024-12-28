#include "Reader.h"


Reader::Reader() {
    cout << "Reader constructed\n";
}

void Reader::ReadRectangle(Shapes::Rectangle* rect, XMLElement* E) {
    Shapes::Point p;
    const char* x = E->Attribute("x");
    const char* y = E->Attribute("y");
    const char* w = E->Attribute("width");
    const char* h = E->Attribute("height");
    
    if (x)
        p.SetX(ConvertUnit(x));
    if (y)
        p.SetY(ConvertUnit(y));
    rect->setPoint(p);

    if (w)
        rect->setWidth(ConvertUnit(w));
    if (h)
        rect->setHeight(ConvertUnit(h));

    x = E->Attribute("rx");
    y = E->Attribute("ry");
    
    if (x)
        rect->setRx(ConvertUnit(x));
    if (y)
        rect->setRy(ConvertUnit(y));

    rect->SetAttribute(E);
}

void Reader::ReadLine(Shapes::Line* line, XMLElement* E) {
    Shapes::Point start, end;
    const char* x1 = E->Attribute("x1");
    const char* y1 = E->Attribute("y1");
    const char* x2 = E->Attribute("x2");
    const char* y2 = E->Attribute("y2");

    if (x1)
        start.SetX(ConvertUnit(x1));
    if (y1)
        start.SetY(ConvertUnit(y1));
    line->setStart(start);

    if (x2)
        end.SetX(ConvertUnit(x2));
    if (y2)
        end.SetY(ConvertUnit(y2));
    line->setEnd(end);

    line->SetAttribute(E);
}

void Reader::ReadCircle(Shapes::Circle* circle, XMLElement* E) {
    Shapes::Point center;
    const char* cx = E->Attribute("cx");
    const char* cy = E->Attribute("cy");
    const char* r = E->Attribute("r");

    if (cx)
        center.SetX(ConvertUnit(cx));
    if (cy)
        center.SetY(ConvertUnit(cy));
    circle->setCenter(center);

    if (r)
        circle->setRadius(ConvertUnit(r));

    circle->SetAttribute(E);
}

void Reader::ReadEllipse(Shapes::Ellipse* ellipse, XMLElement* E) {
    Shapes::Point center;
    const char* cx = E->Attribute("cx");
    const char* cy = E->Attribute("cy");
    const char* rx = E->Attribute("rx");
    const char* ry = E->Attribute("ry");

    if (cx)
        center.SetX(ConvertUnit(cx));
    if (cy)
        center.SetY(ConvertUnit(cy));
    ellipse->setCenter(center);

    if (rx)
        ellipse->setRadiusX(ConvertUnit(rx));
    if (ry)
        ellipse->setRadiusY(ConvertUnit(ry));

    ellipse->SetAttribute(E);
}

void Reader::ReadPolygon(Shapes::Polygon* polygon, XMLElement* E) {
    vector<Shapes::Point> points;
    string p = E->Attribute("points");

    addSpaces(p);
    removeSpareSpaces(p);

    stringstream ss(p);

    string x = "0";
    string y = "0";
    while (ss >> x >> y) {
        Shapes::Point p;
        p.SetX(ConvertUnit(x));
        p.SetY(ConvertUnit(y));
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

    string x = "0";
    string y = "0";
    while (ss >> x >> y) {
        Shapes::Point p;
        p.SetX(ConvertUnit(x));
        p.SetY(ConvertUnit(y));
        points.push_back(p);
    }
    polyline->setPoints(points);

    polyline->SetAttribute(E);
}

void Reader::ReadText(Shapes::Text* text, XMLElement* E) {
    float fontsize;
    if (E->Attribute("font-size")){
        fontsize = ConvertUnit(E->Attribute("font-size"));
        text->setFontSize(fontsize);
    }
    
    Shapes::Point top;

    const char* x = E->Attribute("x");
    const char* y = E->Attribute("y");
    
    const char* dx = E->Attribute("dx");
    const char* dy = E->Attribute("dy");
    
    if (dx){
        string tmp = dx;
        addSpaces(tmp);
        removeSpareSpaces(tmp);
        stringstream ss(tmp);
        string dx_tmp;
        while(ss >> dx_tmp)
            text->add_dx(ConvertUnit(dx_tmp));
    }

    if (dy){
        string tmp = dy;
        addSpaces(tmp);
        removeSpareSpaces(tmp);
        stringstream ss(tmp);
        string dy_tmp;
        while(ss >> dy_tmp)
            text->add_dy(ConvertUnit(dy_tmp));
    }

    if (x)
        top.SetX(ConvertUnit(x));
    if (y)
        top.SetY(ConvertUnit(y) - text->getFontSize());
    else top.SetY(0 - text->getFontSize());
    text->addTop(top);


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
                break;
            }
        }
        path->add(cmd);
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