#include "Drawer.h"
#include "General.h"

void Drawer::Reset(){
    g->ResetTransform();
    Ma.Reset();
}

void Drawer::setDrawer(Shapes::Object* obj){
    p->SetColor(Gdiplus::Color(obj->getStroke().GetAlpha()*255, obj->getStroke().GetRed(), obj->getStroke().GetGreen(), obj->getStroke().GetBlue()));
    p->SetWidth(obj->getStrokeWidth() * s);
    b->SetColor(Gdiplus::Color(obj->getColor().GetAlpha()*255, obj->getColor().GetRed(), obj->getColor().GetGreen(), obj->getColor().GetBlue()));

    p->SetMiterLimit(obj->getStrokeMiterLimit());
}

Drawer::Drawer(vector <unique_ptr<Shapes::Object>>& list, Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor, LinearVector linear){
    int n = list.size();
    for(int i = 0; i < n; i++){
        shapeList.push_back(std::move(list[i]));
    }

    this->g = g;
    this->s = s;
    this->anchor = anchor;
    p = new Pen(Color(0,0,0,0), 0);
    b = new SolidBrush(Color(0,0,0,0));
    gradientList = linear;
}

void Drawer::setGradientBrush(string r, float alpha){
    int default_opacity = 100;
    for(int i = 0; i < gradientList.get_content().size(); i++){
        if(r == gradientList.get_content()[i].get_id()){
            Gdiplus::Color color_array[50];
            float stop_array[50];
            LinearGradient LG = gradientList.get_content()[i];
            int stops_amount = LG.get_amount();
            Gdiplus::PointF p1(LG.get_start().GetX() * s, LG.get_start().GetY() * s), p2(LG.get_end().GetX() * s, LG.get_end().GetY() * s);
            gb = new Gdiplus::LinearGradientBrush(p1, p2, LG.get_colors()[0], LG.get_colors()[LG.get_amount()]);
            int idx = 0;
            for(int i = 0; i < stops_amount; i++){
                float opacity = alpha * ((float)LG.get_colors()[i].GetAlpha());
                if(opacity < 50 && idx != 0){
                    Gdiplus::Color backup(alpha * (LG.get_colors()[idx].GetAlpha() + LG.get_colors()[idx-1].GetAlpha())/2,  LG.get_colors()[idx-1].GetRed()*0.5 + LG.get_colors()[idx].GetRed()*0.5, LG.get_colors()[idx-1].GetGreen()*0.5 + LG.get_colors()[idx].GetGreen()*0.5, LG.get_colors()[idx-1].GetBlue()*0.5 + LG.get_colors()[idx].GetBlue()*0.5);
                    color_array[i] = backup;
                    stop_array[i] = LG.get_stops()[idx] - ((LG.get_stops()[idx] - LG.get_stops()[idx-1]) / 2);
                    i++;
                    stops_amount++;
                }

                Gdiplus::Color c(opacity, LG.get_colors()[idx].GetRed(), LG.get_colors()[idx].GetGreen(), LG.get_colors()[idx].GetBlue());
                color_array[i] = c;
                stop_array[i] = LG.get_stops()[idx];
                
                if(opacity  < 50 && idx < LG.get_amount() - 1){
                    i++;
                    Gdiplus::Color backup(alpha * (LG.get_colors()[idx].GetAlpha() + LG.get_colors()[idx+1].GetAlpha())/2,  LG.get_colors()[idx+1].GetRed()*0.5 + LG.get_colors()[idx].GetRed()*0.5, LG.get_colors()[idx+1].GetGreen()*0.5 + LG.get_colors()[idx].GetGreen()*0.5, LG.get_colors()[idx+1].GetBlue()*0.5 + LG.get_colors()[idx].GetBlue()*0.5);
                    color_array[i] = backup;
                    stop_array[i] = LG.get_stops()[idx] + ((LG.get_stops()[idx+1] - LG.get_stops()[idx]) / 2);
                    stops_amount++;
                }
                idx++;
            }
            gb->SetInterpolationColors(color_array, stop_array, stops_amount);
            break;
        }
    }
}

void Drawer::FillRectGradient(Shapes::Rectangle* R){
    setGradientBrush(R->getColor().GetGradient(), R->getColor().GetAlpha());
    g->FillRectangle(gb, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
    delete gb;
}


void Drawer::DrawR(Shapes::Object* obj){
    setDrawer(obj);
    Gdiplus::Matrix Ma;
    obj->setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);
    Shapes::Rectangle* R = dynamic_cast<Shapes::Rectangle*>(obj);

    if(obj->getColor().GetGradient() != ""){
        FillRectGradient(R);
    }
    else{
        g->FillRectangle(b, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
    }
    g->DrawRectangle(p, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
    Reset();
}

void Drawer::DrawL(Shapes::Object* obj){
   setDrawer(obj);
   Gdiplus::Matrix Ma;
    obj->setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);
    Shapes::Line* L = dynamic_cast<Shapes::Line*>(obj);

    g->DrawLine(p, L->getStart().GetX() * s, L->getStart().GetY() * s, L->getEnd().GetX() * s, L->getEnd().GetY() * s);
    Reset();
}

void Drawer::FillCircleGradient(Shapes::Circle* C){
    setGradientBrush(C->getColor().GetGradient(), C->getColor().GetAlpha());
    g->FillEllipse(gb, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
    delete gb;
}

void Drawer::DrawC(Shapes::Object* obj){
    setDrawer(obj);
    Gdiplus::Matrix Ma;
    obj->setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);
    Shapes::Circle* C = dynamic_cast<Shapes::Circle*>(obj);
    
    if(obj->getColor().GetGradient() != "")    FillCircleGradient(C);
    else    g->FillEllipse(b, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
    g->DrawEllipse(p, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
    Reset();
}

 void Drawer::FillEllipseGradient(Shapes::Ellipse* E){
    setGradientBrush(E->getColor().GetGradient(), E->getColor().GetAlpha());
    g->FillEllipse(gb, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
    delete gb;
 }

void Drawer::DrawE(Shapes::Object* obj){
    setDrawer(obj);
    Gdiplus::Matrix Ma;
    obj->setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);
    Shapes::Ellipse* E = dynamic_cast<Shapes::Ellipse*>(obj);

    if(obj->getColor().GetGradient() != "")    FillEllipseGradient(E);
    else   g->FillEllipse(b, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
    g->DrawEllipse(p, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
    Reset();
}

void Drawer::FillPGGradient(Shapes::Polygon* PG, Gdiplus::GraphicsPath* path){
    setGradientBrush(PG->getColor().GetGradient(), PG->getColor().GetAlpha());
    g->FillPath(gb, path);
    delete gb;
}

void Drawer::DrawPG(Shapes::Object* obj){
    setDrawer(obj);
    Gdiplus::Matrix Ma;
    obj->setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);
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
    if(PG->getColor().GetGradient() != "")  FillPGGradient(PG, &path);
    else   g->FillPath(b, &path);
    g->DrawPath(p, &path);
    Reset();
}

void Drawer::DrawPL(Shapes::Object* obj){
    setDrawer(obj);
    Gdiplus::Matrix Ma;
    obj->setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);
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
    Gdiplus::Matrix Ma;
    obj->setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);
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

void Drawer::FillPGradient(Shapes::Path* P, Gdiplus::GraphicsPath* path){
    setGradientBrush(P->getColor().GetGradient(), P->getColor().GetAlpha());
    g->FillPath(gb, path);
    delete gb;
}

void Drawer::DrawP(Shapes::Object* obj){
    setDrawer(obj);
    Gdiplus::Matrix Ma;
    obj->setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);
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

        vector<float> coor = cmd[i].getNums();

        int pSize = coor.size();
        for (int i = 0; i < pSize; i++){
            coor[i] *= s;
        }

        if (c == 'M' || c == 'm'){
            
            path.StartFigure();
            if (c == 'M'){
                pre = cur = {coor[pSize - 2], coor.back()};
                if (pSize > 2){
                    for (int j = 0; j < pSize - 3; j += 2){
                        path.AddLine(coor[j], coor[j + 1], coor[j + 2], coor[j + 3]);
                    }
                }
                cout << "Move (absolute)\n";
            }
            else {
                pre = PointF(coor.front(), coor[1]) + pre;
                if (pSize > 2){
                    for (int j = 0; j < pSize - 3; j += 2){
                        path.AddLine(pre, (PointF (coor[j + 2], coor[j + 3]) + pre));
                        pre = pre + PointF (coor[j + 2], coor[j + 3]);
                    }
                }
                cur = pre;
                cout << "Move (relatively)\n";
            }
        }
        else if (c == 'L' || c == 'l'){
            if (c == 'L'){
                path.AddLine(pre, PointF(coor[0], coor[1]));
                pre = {coor[0], coor[1]};
                for (int j = 2; j < pSize - 1; j += 2){
                    path.AddLine(pre, {coor[j], coor[j + 1]});
                    pre = {coor[j],coor[j + 1]};
                }
                cur = pre;
                cout << "Line (absolute)\n";
            }
            else {
                path.AddLine(pre, PointF(coor[0], coor[1]) + pre);
                pre = pre + PointF(coor[0], coor[1]);
                for (int j = 2; j < pSize - 1; j += 2){
                    path.AddLine(pre, PointF(coor[j], coor[j + 1]) + pre);
                    pre = pre + PointF(coor[j], coor[j + 1]);
                }
                cur = pre;
                cout << "Line (relative)\n";
            }
        }
        else if (c == 'H' || c == 'h'){
            cur.X = coor.front();
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
            cur.Y = coor.front();
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
            for (int j = 0; j < pSize -  5; j++){
                Control1 = {coor[j++], coor[j++]};
                Control2 = {coor[j++], coor[j++]};
                cur = {coor[j++], coor[j]};
                path.AddBezier(pre, Control1, Control2, cur);
                pre = cur;
            }
            preCurve = Control2;
            cout << "Cubic Bezier (absolute)\n";
        }
        else if (c == 'c'){
            Gdiplus::PointF Control1;
            Gdiplus::PointF Control2;
            for (int j = 0; j < pSize; j++){
                Control1 = PointF(coor[j], coor[j + 1]) + pre;
                Control2 = PointF(coor[j + 2], coor[j + 3]) + pre;
                cur = PointF(coor[j + 4], coor[j + 5]) + pre;
                j += 5;
                path.AddBezier(pre, Control1, Control2, cur);
                pre = cur;
            }            
            preCurve = Control2;
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
            Control2 = {coor[0], coor[1]};

            cur = {coor[2], coor[3]};
            
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
            Control2 = pre + PointF(coor[0], coor[1]);
            cur = pre + PointF(coor[2], coor[3]);
            
            path.AddBezier(pre, Control1, Control2, cur);
            
            preCurve = Control2;
            pre = cur;

            cout << "Smooth Cubic Bezier (relative)\n";
        }
        else if (c == 'Q'){
            Gdiplus::PointF Quad = {coor[0], coor[1]};
            cur = {coor[2], coor[3]};

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
            Gdiplus::PointF Quad = pre + PointF(coor[0], coor[1]);
            cur = pre + PointF(coor[2], coor[3]);

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
            cur = {coor[0], coor[1]};

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
            cur = pre + PointF(coor[0], coor[1]);

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
        }else if (c == 'A' || c == 'a'){
            
            if (c == 'A'){
                for (int i = 0; i < pSize; i += 7){
                    cout << "Arc from (" << pre.X << ", " << pre.Y << ") to ("
          << coor[i + 5] << ", " << coor[i + 6] << ") with rx=" 
          << coor[i] << ", ry=" << coor[i + 1] 
          << ", rotation=" << coor[i + 2]
          << ", largeArcFlag=" << coor[i + 3] 
          << ", sweepFlag=" << coor[i + 4] << std::endl;
                    AddSvgArcToPath(path, pre.X, pre.Y, coor[i + 5], coor[i + 6], coor[i], coor[i + 1], coor[i + 2], int(coor[i + 3]), int(coor[i  +4]));
                    pre = {coor[i + 5], coor[i + 6]};
                }
            }else{
                for (int i = 0; i < pSize; i += 7){
                    cout << "Arc from (" << pre.X << ", " << pre.Y << ") to ("
          << coor[i + 5] + pre.X << ", " << coor[i + 6] + pre.Y << ") with rx=" 
          << coor[i] << ", ry=" << coor[i + 1] 
          << ", rotation=" << coor[i + 2]
          << ", largeArcFlag=" << coor[i + 3] 
          << ", sweepFlag=" << coor[i + 4] << std::endl;
                    AddSvgArcToPath(path, pre.X, pre.Y, coor[i + 5] + pre.X, coor[i + 6] + pre.Y, coor[i], coor[i + 1], coor[i + 2], int(coor[i + 3]), int(coor[i  +4]));
                    pre = {coor[i + 5] + pre.X, coor[i + 6] + pre.Y};
                }
            }
            cur = pre;
        }
        else if (c == 'Z' || c == 'z'){
            path.CloseFigure();
            path.SetFillMode(Gdiplus::FillModeWinding);
            cout << "Close\n";
        }
    }
    char end = cmd.back().getCmd();
    g->DrawPath(p, &path);
    if(P->getColor().GetGradient() != "")   FillPGradient(P, &path);
    else    g->FillPath(b, &path);
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
            DrawR(rawPtr);
        }
        else if(dynamic_cast <Shapes::Line*> (rawPtr)){
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
            DrawP(rawPtr);
        }
        else if (dynamic_cast<Shapes::Group*> (rawPtr)){
            DrawG(rawPtr);
        }
    }
}