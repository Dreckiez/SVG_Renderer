#include "Drawer.h"

void Drawer::Reset(){
    g->ResetTransform();
    Ma.Reset();
}

void Drawer::setDrawer(Shapes::Object* obj){
    obj->setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);
    p->SetColor(Gdiplus::Color(obj->getStroke().GetAlpha()*255, obj->getStroke().GetRed(), obj->getStroke().GetGreen(), obj->getStroke().GetBlue()));
    p->SetWidth(obj->getStrokeWidth() * s);
    b->SetColor(Gdiplus::Color(obj->getColor().GetAlpha()*255, obj->getColor().GetRed(), obj->getColor().GetGreen(), obj->getColor().GetBlue()));
}

Drawer::Drawer(vector <unique_ptr<Shapes::Object>>& list, Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    int n = list.size();
    for(int i = 0; i < n; i++){
        shapeList.push_back(std::move(list[i]));
    }

    this->g = g;
    this->s = s;
    this->anchor = anchor;
    p = new Pen(Color(0,0,0,0), 0);
    b = new SolidBrush(Color(0,0,0,0));
}

void Drawer::DrawR(Shapes::Object* obj){
    setDrawer(obj);
    Shapes::Rectangle* R = dynamic_cast<Shapes::Rectangle*>(obj);

    g->FillRectangle(b, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
    g->DrawRectangle(p, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
    Reset();
}

void Drawer::DrawL(Shapes::Object* obj){
   setDrawer(obj);
    Shapes::Line* L = dynamic_cast<Shapes::Line*>(obj);

    g->DrawLine(p, L->getStart().GetX() * s, L->getStart().GetY() * s, L->getEnd().GetX() * s, L->getEnd().GetY() * s);
    Reset();
}

void Drawer::DrawC(Shapes::Object* obj){
    setDrawer(obj);
    Shapes::Circle* C = dynamic_cast<Shapes::Circle*>(obj);
    
    g->FillEllipse(b, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
    g->DrawEllipse(p, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
    Reset();
}

void Drawer::DrawE(Shapes::Object* obj){
    setDrawer(obj);
    Shapes::Ellipse* E = dynamic_cast<Shapes::Ellipse*>(obj);

    g->FillEllipse(b, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
    g->DrawEllipse(p, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
    Reset();
}

void Drawer::DrawPG(Shapes::Object* obj){
    setDrawer(obj);
    vector<Gdiplus::PointF> list;
    Shapes::Polygon* PG = dynamic_cast <Shapes::Polygon*> (obj);
    int n = PG->getPoints().size();
    for (int i = 0; i < n; i++){
        list.push_back({PG->getPoints()[i].GetX() * s, PG->getPoints()[i].GetY() * s});
    }

    GraphicsPath path;
    if (obj->getFillRule() == "nonzero"){
        path.SetFillMode(FillModeWinding);
    }else{
        path.SetFillMode(FillModeAlternate);
    }
    path.AddPolygon(list.data(), PG->getPoints().size());
    g->FillPath(b, &path);
    g->DrawPath(p, &path);
    Reset();
}

void Drawer::DrawPL(Shapes::Object* obj){
    setDrawer(obj);
    vector <Gdiplus::PointF> pF;
    Shapes::Polyline* PL = dynamic_cast <Shapes::Polyline*> (obj);
    int size = PL->getPoints().size();
    for (int i = 0; i < size; i++){
        pF.push_back({PL->getPoints()[i].GetX() * s, PL->getPoints()[i].GetY() * s});
    }
    

    g->FillPolygon(b, pF.data(), static_cast<int> (pF.size()));
    g->DrawLines(p, pF.data(), static_cast<int>(pF.size()));
    Reset();
}

void Drawer::DrawT(Shapes::Object* obj){
    setDrawer(obj);
    Shapes::Text* T = dynamic_cast<Shapes::Text*>(obj);

    // Create a wide string for Font Family 
    size_t size_needed = mbstowcs(nullptr, T->getFontFamily().c_str(), 0);
    if (size_needed == static_cast<size_t>(-1)) {
        std::wcerr << L"Error converting string to wide string." << endl;
        return;
    }
    wstring wff(size_needed, L'\0');
    mbstowcs(&wff[0], T->getFontFamily().c_str(), size_needed);

    //Create a wide string for Text
    size_needed = mbstowcs(nullptr, T->getText().c_str(), 0);
    if (size_needed == static_cast<size_t>(-1)) {
        std::wcerr << L"Error converting string to wide string." << endl;
        return;
    }
    wstring wtext(size_needed, L'\0');
    mbstowcs(&wtext[0], T->getText().c_str(), size_needed);

    // Font Family
    Gdiplus::FontFamily* ff = new Gdiplus::FontFamily(wff.c_str());

    // Check if it can load the Font Family 
    if (!ff->IsAvailable()){
        delete ff;
        ff = new Gdiplus::FontFamily(L"Times New Roman"); // If no then it will load the default
    }

    // Recalculating the Top-Left Point because of the Text-anchor attribute
    if (!T->getTextAnchor().empty() && T->getTextAnchor() != "start"){
        Shapes::Point TA(T->getTop());
        Gdiplus::Font font(ff, T->getFontSize(), T->getFontStyle(), Gdiplus::UnitPixel);
        Gdiplus::RectF Bounding_Box;
        g->MeasureString(wtext.c_str(), T->getText().size(), &font, (Gdiplus::PointF){TA.GetX(), TA.GetY()}, &Bounding_Box);
        
        if (T->getTextAnchor() == "middle")
            TA.SetX(TA.GetX() - Bounding_Box.Width/2);
        else if (T->getTextAnchor() == "end")
            TA.SetX(TA.GetX() - Bounding_Box.Width);
    }


    Gdiplus::GraphicsPath text;
    text.StartFigure();
    text.AddString(wtext.c_str(), T->getText().size(), ff, T->getFontStyle(), T->getFontSize(), (Gdiplus::PointF){T->getTop().GetX() * s, T->getTop().GetY() * s}, nullptr);
    text.CloseFigure();

    g->DrawPath(p, &text);
    g->FillPath(b, &text);
    Reset();

    delete ff;
}

void Drawer::DrawP(Shapes::Object* obj){
    setDrawer(obj);
    Shapes::Path* P = dynamic_cast<Shapes::Path*>(obj);

    Gdiplus::GraphicsPath path;
    if (obj->getFillRule() == "nonzero"){
        path.SetFillMode(FillModeWinding);
    }else{
        path.SetFillMode(FillModeAlternate);
    }
    
    int size = P->getCmd().size();

    vector<Shapes::Command> cmd = P->getCmd();

    int pos = 0;
    //all command has to update cur and pre accordingly
    Gdiplus::PointF cur(0,0);     //current position of pen
    Gdiplus::PointF pre(0,0);     // 1 point before cur. cur will be updated first then AddLine or watever before updating pre
    Gdiplus::PointF preCurve(0,0);    //previous control point, used and updated when handle command curve, quaratic,...


    for (int i = 0; i < size; i++){
        char c = cmd[i].getCmd();

        vector<Gdiplus::PointF> coor = cmd[i].getPoints();

        for (int i = 0; i < coor.size(); i++){
            coor[i].X *= s;
            coor[i].Y *= s;
        }

        if (c == 'M' || c == 'm'){
            int pSize = coor.size();
            
            path.StartFigure();
            if (c == 'M'){
                pre = cur = coor.back();
                if (pSize != 1){
                    for (int j = 0; j < coor.size() - 1; j++){
                        path.AddLine(coor[j], coor[j + 1]);
                    }
                }
                cout << "Move (absolute)\n";
            }
            else {
                pre = coor.front() + pre;
                if (pSize != 1){
                    for (int j = 0; j < coor.size() - 1; j++){
                        path.AddLine(coor[j] + pre, coor[j + 1] + pre);
                        pre = pre + coor[j+1];
                    }
                }
                cur = pre;
                cout << "Move (relatively)\n";
            }
        }
        else if (c == 'L' || c == 'l'){
            if (c == 'L'){
                path.AddLine(pre, coor[0]);
                pre = coor[0];
                for (int j = 0; j < coor.size() - 1; j++){
                    path.AddLine(pre, coor[j + 1]);
                    pre = coor[j + 1];
                }
                cur = pre;
                cout << "Line (absolute)\n";
            }
            else {
                path.AddLine(pre, coor[0] + pre);
                pre = pre + coor[0];
                for (int j = 0; j < coor.size() - 1; j++){
                    path.AddLine(pre, coor[j + 1] + pre);
                    pre = pre + coor[j + 1];
                }
                cur = pre;
                cout << "Line (relative)\n";
            }
        }
        else if (c == 'H' || c == 'h'){
            cur = coor.back();
            cur.Y = pre.Y;
            if (c == 'H'){
                
                path.AddLine(pre, cur);

                pre = cur;
                cout << "Horizontal Line (absolute)\n";
            }
            else {
                cur.X = cur.X + pre.X;
                
                path.AddLine(pre, cur);
                
                pre = cur;
                cout << "Horizontal Line (relative)\n";
            }
        }
        else if (c == 'V' || c == 'v'){
            cur = coor.back();
            cur.X = pre.X;
            if (c == 'V'){
                path.AddLine(pre, cur);
                pre = cur;

                cout << "Vertical Line (absolute)\n";
            }
            else {
                cur.Y = cur.Y + pre.Y;
                path.AddLine(pre, cur);
                pre = cur;

                cout << "Vertical (relative)\n";
            }
        }
        else if (c == 'C'){
            Gdiplus::PointF Control1;
            Gdiplus::PointF Control2;
            for (int j = 0; j < coor.size(); j++){
                Control1 = coor[j++];
                Control2 = coor[j++];
                cur = coor[j];
                path.AddBezier(pre, Control1, Control2, cur);
                pre = cur;
            }
            preCurve = Control2;

            cout << "Cubic Bezier (absolute)\n";
        }
        else if (c == 'c'){
            Gdiplus::PointF Control1;
            Gdiplus::PointF Control2;
            for (int j = 0; j < coor.size(); j++){
                Control1 = coor[j++] + pre;
                Control2 = coor[j++] + pre;
                cur = coor[j] + pre;
                path.AddBezier(pre, Control1, Control2, cur);
                pre = cur;
            }
                        
            preCurve = Control2;
            cur = pre;
            cout << "Cubic Bezier (relative)\n";
        }
        else if (c == 'S'){
            Gdiplus::PointF Control1 (0,0);
            Gdiplus::PointF Control2 (0,0);

            if (i > 0 && (cmd[i - 1].getCmd() == 'C' || cmd[i - 1].getCmd() == 'c' || cmd[i - 1].getCmd() == 's' || cmd[i - 1].getCmd() == 'S')){
                Control1 = pre + pre - preCurve;
            }else{
                Control1 = pre;
            }
            Control2 = coor[0];

            cur = coor[1];
            
            path.AddBezier(pre, Control1, Control2, cur);
            
            pre = cur;
            preCurve = Control2;

            cout << "Smooth Cubic Bezier (absolute)\n";
        }
        else if (c == 's'){
            Gdiplus::PointF Control1 (0,0);
            Gdiplus::PointF Control2 (0,0);
            if (i > 0 && (cmd[i - 1].getCmd() == 'C' || cmd[i - 1].getCmd() == 'c' || cmd[i - 1].getCmd() == 's' || cmd[i - 1].getCmd() == 'S')){
                Control1 = pre + pre - preCurve;
            }else{
                Control1 = pre;
            }
            Control2 = pre + coor[0];
            cur = pre + coor[1];
            
            path.AddBezier(pre, Control1, Control2, cur);
            
            preCurve = Control2;
            pre = cur;

            cout << "Smooth Cubic Bezier (relative)\n";
        }
        else if (c == 'Q'){
            Gdiplus::PointF Quad = coor[0];
            cur = coor[1];

            Gdiplus::PointF Control1 = pre;
            Control1.X += 2 * (Quad.X - pre.X) / 3.0;
            Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

            Gdiplus::PointF Control2 = cur;
            Control2.X += 2 * (Quad.X - cur.X) / 3.0;
            Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;            

            path.AddBezier(pre, Control1, Control2, cur);

            pre = cur;
            preCurve = Control2;
            cout << "Quadratic Bezier (absolute)\n";
        }
        else if (c == 'q'){
            Gdiplus::PointF Quad = pre + coor[0];
            cur = pre + coor[1];

            Gdiplus::PointF Control1 = pre;
            Control1.X += 2 * (Quad.X - pre.X) / 3.0;
            Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

            Gdiplus::PointF Control2 = cur;
            Control2.X += 2 * (Quad.X - cur.X) / 3.0;
            Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;

            path.AddBezier(pre, Control1, Control2, cur);

            preCurve = Control2;
            pre = cur;

            cout << "Quadratic Bezier (relative)\n";
        }
        else if (c == 'T'){
            Gdiplus::PointF Quad = pre + pre - preCurve;
            cur = coor[0];

            Gdiplus::PointF Control1 = pre;
            Control1.X += 2 * (Quad.X - pre.X) / 3.0;
            Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

            Gdiplus::PointF Control2 = cur;
            Control2.X += 2 * (Quad.X - cur.X) / 3.0;
            Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;

            path.AddBezier(pre, Control1, Control2, cur);

            pre = cur;
            preCurve = Control2;

            cout << "Smooth Quadratic Bezier (absolute)\n";
        }
        else if (c == 't'){
            Gdiplus::PointF Quad = pre + pre - preCurve;
            cur = pre + coor[0];

            Gdiplus::PointF Control1 = pre;
            Control1.X += 2 * (Quad.X - pre.X) / 3.0;
            Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

            Gdiplus::PointF Control2 = cur;
            Control2.X += 2 * (Quad.X - cur.X) / 3.0;
            Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;

            path.AddBezier(pre, Control1, Control2, cur);

            pre = cur;
            preCurve = Control2;

            cout << "Smooth Quadratic Bezier (relative)\n";
        }
        else if (c == 'Z' || c == 'z'){
            path.CloseFigure();
            path.SetFillMode(Gdiplus::FillModeWinding);
            cout << "Close\n";
        }
    }
    char end = cmd.back().getCmd();
    g->DrawPath(p, &path);
    g->FillPath(b, &path);
    Reset();
}

void Drawer::DrawG(Shapes::Object* obj){
    Shapes::Group* G = dynamic_cast<Shapes::Group*>(obj);
    for (int i = 0; i < G->GetSize(); i++){
        if(dynamic_cast <Shapes::Rectangle*> (G->GetShape(i))){
            DrawR(G->GetShape(i));
        }
        else if(dynamic_cast <Shapes::Line*> (G->GetShape(i))){
            DrawL(G->GetShape(i));
        }
        else if(dynamic_cast <Shapes::Circle*> (G->GetShape(i))){
            DrawC(G->GetShape(i));
        }
        else if(dynamic_cast <Shapes::Ellipse*> (G->GetShape(i))){
            DrawE(G->GetShape(i));
        }
        else if(dynamic_cast <Shapes::Polygon*> (G->GetShape(i))){
            DrawPG(G->GetShape(i));
        }
        else if(dynamic_cast <Shapes::Polyline*> (G->GetShape(i))){
            DrawPL(G->GetShape(i));
        }
        else if(dynamic_cast <Shapes::Text*> (G->GetShape(i))){
            DrawT(G->GetShape(i));
        }else if(dynamic_cast <Shapes::Path*> (G->GetShape(i))){
            DrawP(G->GetShape(i));
        }
        else if (dynamic_cast<Shapes::Group*> (G->GetShape(i))){
            DrawG(G->GetShape(i));
        }
    }
    Reset();
}

void Drawer::Draw(){
    for (int i = 0; i < shapeList.size(); i++){
        Shapes::Object* rawPtr = shapeList[i].get();
        if(dynamic_cast <Shapes::Rectangle*> (rawPtr)){
            cout << "rect";
            DrawR(rawPtr);
        }
        else if(dynamic_cast <Shapes::Line*> (rawPtr)){
            cout << "line";
            DrawL(rawPtr);
        }
        else if(dynamic_cast <Shapes::Circle*> (rawPtr)){
            DrawC(rawPtr);
        }
        else if(dynamic_cast <Shapes::Ellipse*> (rawPtr)){
            DrawE(rawPtr);
        }
        else if(dynamic_cast <Shapes::Polygon*> (rawPtr)){
            DrawPG(rawPtr);
        }
        else if(dynamic_cast <Shapes::Polyline*> (rawPtr)){
            DrawPL(rawPtr);
        }
        else if(dynamic_cast <Shapes::Text*> (rawPtr)){
            DrawT(rawPtr);
        }else if(dynamic_cast <Shapes::Path*> (rawPtr)){
            cout << "path\n";
            DrawP(rawPtr);
        }
        else if (dynamic_cast<Shapes::Group*> (rawPtr)){
            DrawG(rawPtr);
        }
    }
}