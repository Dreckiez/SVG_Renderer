#include "Drawer.h"
#include "General.h"
#include <locale>
#include <iostream>
#include <codecvt>

void Drawer::Reset(){
    g->ResetTransform();
    Ma.Reset();
}

void Drawer::setDrawer(Shapes::Object* obj){
    Gdiplus::Matrix MVB;
    float scale_x = 1;
    float scale_y = 1;
    float scale_factor;

    if (VB.GetWidth() != 1){
        if (VP.GetWidth() != 300){
                scale_x = VP.GetWidth()/VB.GetWidth();
        }
        else {
            Gdiplus::RectF bounds;
            g->GetClipBounds(&bounds);
            scale_x = bounds.Width/VB.GetWidth();
        }
    }
    
    if (VB.GetHeight() != 1){
        if (VP.GetHeight() != 150){
                scale_y = VP.GetHeight()/VB.GetHeight();
        }
        else {
            Gdiplus::RectF bounds;
            g->GetClipBounds(&bounds);
            scale_y = bounds.Height/VB.GetHeight();
        }
    }
    
    scale_factor = min (scale_x, scale_y);
    MVB.Scale(scale_factor, scale_factor);
    MVB.Translate(VB.GetTop().GetX(), VB.GetTop().GetY());

    obj->setTransform(Ma, s, anchor);

    Ma.Multiply(&MVB, Gdiplus::MatrixOrderAppend);

    g->SetTransform(&Ma);
    p->SetColor(Gdiplus::Color(obj->getStroke().GetAlpha()*255, obj->getStroke().GetRed(), obj->getStroke().GetGreen(), obj->getStroke().GetBlue()));
    p->SetWidth(obj->getStrokeWidth() * s);
    b->SetColor(Gdiplus::Color(obj->getColor().GetAlpha()*255, obj->getColor().GetRed(), obj->getColor().GetGreen(), obj->getColor().GetBlue()));
    if(obj->getColor().GetGradient() != ""){
        setGradientBrush(obj);
    }
    p->SetMiterLimit(obj->getStrokeMiterLimit());
}


Drawer::Drawer(vector <unique_ptr<Shapes::Object>>& list, Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor, GradientVector linear, ViewBox vb, ViewPort vp){
    int n = list.size();
    for(int i = 0; i < n; i++){
        shapeList.push_back(std::move(list[i]));
    }

    this->g = g;
    this->s = s;
    this->anchor = anchor;
    p = new Gdiplus::Pen(Gdiplus::Color(0,0,0,0), 0);
    b = new Gdiplus::SolidBrush(Gdiplus::Color(0,0,0,0));
    gradientList = linear;
    VB = vb;
    VP = vp;
    gb = nullptr;
    rgb = nullptr;
}

void Drawer::setGradientBrush(Shapes::Object* obj){
    string r = obj->getColor().GetGradient();
    float alpha = obj->getColor().GetAlpha();
    for(int i = 0; i < gradientList.get_content().size(); i++){
        if(r == gradientList.get_content()[i]->get_id()){
            if(LinearGradient* LG = dynamic_cast<LinearGradient*> (gradientList.get_content()[i])){
                LG->setBrush(obj, gb, alpha, s);
            }
            if(RadialGradient* RG = dynamic_cast<RadialGradient*> (gradientList.get_content()[i])){
                Gdiplus::GraphicsPath path;
                Gdiplus::RectF boundingBox;
                float radius = RG->get_radius()*1.25;
                if(!RG->getIsBoundingBox()){
                    boundingBox.X = (RG->get_start().GetX() - radius) * s;
                    boundingBox.Y = (RG->get_start().GetY() - radius) * s;
                    boundingBox.Width = radius*2*s;
                    boundingBox.Height = radius*2*s;
                    path.AddEllipse(boundingBox);
                }
                else{
                    obj->setBoundingBox(boundingBox);
                    boundingBox.X*=s; boundingBox.Y*=s; boundingBox.Width*=s; boundingBox.Height*=s;
                    if(dynamic_cast <Shapes::Rectangle*> (obj)){
                        path.AddRectangle(boundingBox);
                    }
                    else if(dynamic_cast <Shapes::Circle*> (obj)){
                        path.AddEllipse(boundingBox);
                    }
                    else if(dynamic_cast <Shapes::Ellipse*> (obj)){
                        path.AddEllipse(boundingBox);
                    }
                    else if(Shapes::Polygon* PG = dynamic_cast <Shapes::Polygon*> (obj)){
                        path.AddPath(&PG->getPath(), true);
                    }
                    else if(Shapes::Polyline* PL = dynamic_cast <Shapes::Polyline*> (obj)){
                        for(int i = 0; i < PL->getPoints().size() - 1; i++){
                            path.AddLine(PointF(PL->getPoints()[i].GetX(), PL->getPoints()[i].GetY()), PointF(PL->getPoints()[i+1].GetX(), PL->getPoints()[i+1].GetY()));
                        }
                    }
                    else if(Shapes::Text* T = dynamic_cast <Shapes::Text*> (obj)){
                        path.AddPath(&T->getPath(), true);
                    }else if(Shapes::Path* P = dynamic_cast <Shapes::Path*> (obj)){
                        path.AddPath(&P->getPath(), true);
                    }
                }
                RG->setBrush(path, rgb, alpha, s);
            }
            return;
        }
    }
    gb = new Gdiplus::LinearGradientBrush(Gdiplus::Rect(0, 0, 1000, 1000), Gdiplus::Color(255, 0, 0), Gdiplus::Color(0, 0, 255), Gdiplus::LinearGradientModeHorizontal);
}

void Drawer::FillRectGradient(Shapes::Rectangle* R){
    if(gb){
        g->FillRectangle(gb, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
        gb->ResetTransform();
        delete gb;
        gb = nullptr;
    }
    if(rgb){
        g->FillRectangle(rgb, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
        delete rgb;
        rgb = nullptr;
    }  
}


void Drawer::DrawR(Shapes::Object* obj){
    setDrawer(obj);
    Shapes::Rectangle* R = dynamic_cast<Shapes::Rectangle*>(obj);
    if (R->getRx() != 0 || R->getRy() != 0){
        float rx = min(R->getRx(), R->getWidth() / 2) * s;
        float ry = min(R->getRy(), R->getHeight() / 2) * s;
        float x = R->getPoint().GetX() * s;
        float y = R->getPoint().GetY() * s;
        float width = R->getWidth() * s;
        float height = R->getHeight() * s;
        GraphicsPath path;

        if (obj->getFillRule() == "nonzero"){
            path.SetFillMode(FillModeWinding);
        }else{
            path.SetFillMode(FillModeAlternate);
        }

        path.AddArc(x, y, rx * 2, ry * 2, 180, 90);                     // Top-left corner
        path.AddLine(x + rx, y, x + width - rx, y);                     // Top edge                 
        path.AddArc(x + width - rx * 2, y, rx * 2, ry * 2, 270, 90);    // Top-right corner
        path.AddLine(x + width, y + ry, x + width, y + height - ry);    // Right edge
        path.AddArc(x + width - rx * 2, y + height - ry * 2, rx * 2, ry * 2, 0, 90);    // Bottom-right corner
        path.AddLine(x + width - rx, y + height, x + rx, y + height);   // Bottom edge
        path.AddArc(x, y + height - ry * 2, rx * 2, ry * 2, 90, 90);    // Bottom-left corner
        path.AddLine(x, y + height - ry, x, y + ry);                    // Left edge

        path.CloseFigure();

        g->DrawPath(p, &path);

        g->FillPath(b, &path);
    }else{
            if(obj->getColor().GetGradient() != ""){
            FillRectGradient(R);
        }
        else{
            g->FillRectangle(b, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
        }
        g->DrawRectangle(p, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
    }

    Reset();
}

void Drawer::DrawL(Shapes::Object* obj){
    setDrawer(obj);
    Shapes::Line* L = dynamic_cast<Shapes::Line*>(obj);

    g->DrawLine(p, L->getStart().GetX() * s, L->getStart().GetY() * s, L->getEnd().GetX() * s, L->getEnd().GetY() * s);
    Reset();
}

void Drawer::FillCircleGradient(Shapes::Circle* C){
    if(gb){
        g->FillEllipse(gb, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
        delete gb;
        gb = nullptr;
    }
    if(rgb){
        g->FillEllipse(rgb, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
        delete rgb;
        rgb = nullptr;
    }
}

void Drawer::DrawC(Shapes::Object* obj){
    setDrawer(obj);
    Shapes::Circle* C = dynamic_cast<Shapes::Circle*>(obj);
    
    if(obj->getColor().GetGradient() != "")    FillCircleGradient(C);
    else    g->FillEllipse(b, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
    g->DrawEllipse(p, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
    Reset();
}

 void Drawer::FillEllipseGradient(Shapes::Ellipse* E){
    if(gb){
        g->FillEllipse(gb, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
        delete gb;
        gb = nullptr;
    }
    else if(rgb){
        g->FillEllipse(rgb, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
        delete rgb;
        rgb = nullptr;
    }
 }

void Drawer::DrawE(Shapes::Object* obj){
    setDrawer(obj);
    Shapes::Ellipse* E = dynamic_cast<Shapes::Ellipse*>(obj);

    if(obj->getColor().GetGradient() != "")    FillEllipseGradient(E);
    else   g->FillEllipse(b, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
    g->DrawEllipse(p, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
    Reset();
}

void Drawer::FillPGGradient(Gdiplus::GraphicsPath* path){
    if(gb){
        g->FillPath(gb, path);
        delete gb;
        gb = nullptr;
    }
    else if(rgb){
        g->FillPath(rgb, path);
        delete rgb;
        rgb = nullptr;
    }
}

void Drawer::DrawPG(Shapes::Object* obj){
    vector<Gdiplus::PointF> list;
    Shapes::Polygon* PG = dynamic_cast <Shapes::Polygon*> (obj);
    int n = PG->getPoints().size();
    for (int i = 0; i < n; i++){
        list.push_back({PG->getPoints()[i].GetX() * s, PG->getPoints()[i].GetY() * s});
    }

    Gdiplus::GraphicsPath path;
    if (obj->getFillRule() == "nonzero"){
        path.SetFillMode(FillModeWinding);
    }else{
        path.SetFillMode(FillModeAlternate);
    }
    path.AddPolygon(list.data(), PG->getPoints().size());
    PG->setPath(&path);
    setDrawer(obj);
    if(PG->getColor().GetGradient() != "")  FillPGGradient(&path);
    else   g->FillPath(b, &path);
    g->DrawPath(p, &path);
    Reset();
}

void Drawer::FillPLGradient(vector <Gdiplus::PointF> pF){
    if(gb){
        g->FillPolygon(gb, pF.data(), static_cast<int> (pF.size()));
        delete gb;
        gb = nullptr;
    }
    else if(rgb){
        g->FillPolygon(rgb, pF.data(), static_cast<int> (pF.size()));
        delete rgb;
        rgb = nullptr;
    }
}


void Drawer::DrawPL(Shapes::Object* obj){
    setDrawer(obj);
    vector <Gdiplus::PointF> pF;
    Shapes::Polyline* PL = dynamic_cast <Shapes::Polyline*> (obj);
    int size = PL->getPoints().size();
    for (int i = 0; i < size; i++){
        pF.push_back({PL->getPoints()[i].GetX() * s, PL->getPoints()[i].GetY() * s});
    }

    if(PL->getColor().GetGradient() != "") FillPLGradient(pF);
    else    g->FillPolygon(b, pF.data(), static_cast<int> (pF.size()));
    g->DrawLines(p, pF.data(), static_cast<int>(pF.size()));
    Reset();
}

void Drawer::FillTextGradient(Gdiplus::GraphicsPath* text){
    if(gb){
        g->FillPath(gb, text);
        delete gb;
        gb = nullptr;
    }
    else if(rgb){
        g->FillPath(rgb, text);
        delete rgb;
        rgb = nullptr;
    }
}


void Drawer::DrawT(Shapes::Object* obj){
    Shapes::Text* T = dynamic_cast<Shapes::Text*>(obj);

    // Create a wide string for Font Family 
    wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    wstring wff = converter.from_bytes(T->getFontFamily());

    // Font Family & Font
    Gdiplus::FontFamily* ff = new Gdiplus::FontFamily(wff.c_str());
    Gdiplus::Font font(ff, T->getFontSize(), T->getFontStyle(), Gdiplus::UnitPixel);

    // Check if it can load the Font Family 
    if (!ff->IsAvailable()){
        delete ff;
        ff = new Gdiplus::FontFamily(L"Times New Roman"); // If no then it will load the default
    }


    // Recalculating the Top-Left Point because of the Text-anchor attribute
    if (!T->getTextAnchor().empty() && T->getTextAnchor() != "start"){
        Shapes::Point TA(T->getTop(0));
        Gdiplus::RectF Bounding_Box;
        g->MeasureString(T->getText().c_str(), T->getText().size(), &font, (Gdiplus::PointF){TA.GetX(), TA.GetY()}, &Bounding_Box);

        if (T->getTextAnchor() == "middle")
            TA.SetX(TA.GetX() - Bounding_Box.Width/2);
        else if (T->getTextAnchor() == "end")
            TA.SetX(TA.GetX() - Bounding_Box.Width);
        
        T->setTop_X(0, TA.GetX());
    }

    // Calculate each letter Top-Left corner
    vector<float> dx = T->Get_dx();
    vector<float> dy = T->Get_dy();

    Shapes::Point Prev(T->getTop(0));
    for (int i = 0; i < T->getText().size(); i++){
        if (i == 0){
            if (!dx.empty()){
                float tmp = T->getTop(i).GetX() + dx[i];
                T->setTop_X(i, tmp);
            }
            if (!dy.empty()){
                float tmp = T->getTop(i).GetY() + dy[i];
                T->setTop_Y(i, tmp);
            }
        }
        else {
            Gdiplus::RectF Bounding_Box;
            g->MeasureString(&T->getText()[i], T->getText().size(), &font, (Gdiplus::PointF){Prev.GetX(), Prev.GetY()}, &Bounding_Box);
            Prev.SetX(Prev.GetX() + Bounding_Box.Width);
            Shapes::Point p(Prev);
            if (i < dx.size()){
                float tmp = p.GetX() + dx[i];
                p.SetX(tmp);
            }

            if (i < dy.size()){
                float tmp = p.GetY() + dy[i];
                p.SetY(tmp);
            }
            T->addTop(p);
        }
    }


    Gdiplus::GraphicsPath text;
    text.StartFigure();
    
    if (dx.empty() && dy.empty())
        text.AddString(T->getText().c_str(), T->getText().size(), ff, T->getFontStyle(), T->getFontSize() * s, (Gdiplus::PointF){T->getTop(0).GetX() * s, T->getTop(0).GetY() * s}, nullptr);
    else {
        for (int i = 0; i < T->getText().size(); i++){
            text.AddString(&T->getText()[i], 1, ff, T->getFontStyle(), T->getFontSize() * s, (Gdiplus::PointF){T->getTop(i).GetX() * s, T->getTop(i).GetY() * s}, nullptr);
        }
    }

    text.CloseFigure();
    T->setPath(&text);
    setDrawer(obj);
    g->DrawPath(p, &text);
    if(T->getColor().GetGradient() != "")   FillTextGradient(&text);
    else    g->FillPath(b, &text);
    Reset();

    delete ff;
}

void Drawer::FillPGradient(Gdiplus::GraphicsPath* path){
    if(gb){
        g->FillPath(gb, path);
        delete gb;
        gb = nullptr;
    }
    else if(rgb){
        g->FillPath(rgb, path);
        delete rgb;
        rgb = nullptr;
    }
}

void Drawer::DrawP(Shapes::Object* obj){
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
    Gdiplus::PointF pathStart(0,0);

    for (int i = 0; i < size; i++){
        char c = cmd[i].getCmd();

        vector<float> coor = cmd[i].getNums();

        int pSize = coor.size();
        for (int j = 0; j < pSize; j++){
            coor[j] *= s;
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
            }
            if (i == 0 || cmd[i - 1].getCmd() == 'Z' || cmd[i - 1].getCmd() == 'z'){
                pathStart = cur;
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
            }
            else {
                path.AddLine(pre, PointF(coor[0], coor[1]) + pre);
                pre = pre + PointF(coor[0], coor[1]);
                for (int j = 2; j < pSize - 1; j += 2){
                    path.AddLine(pre, PointF(coor[j], coor[j + 1]) + pre);
                    pre = pre + PointF(coor[j], coor[j + 1]);
                }
                cur = pre;
            }
        }
        else if (c == 'H' || c == 'h'){
            cur.X = coor.front();
            cur.Y = pre.Y;
            if (c == 'H'){
                
                path.AddLine(pre, cur);

                pre = cur;
            }
            else {
                cur.X = cur.X + pre.X;
                
                path.AddLine(pre, cur);
                
                pre = cur;
            }
        }
        else if (c == 'V' || c == 'v'){           
            cur.Y = coor.front();
            cur.X = pre.X;
            if (c == 'V'){
                path.AddLine(pre, cur);
                pre = cur;
            }
            else {
                cur.Y = cur.Y + pre.Y;
                path.AddLine(pre, cur);
                pre = cur;
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
        }
        else if (c == 'S'){
            Gdiplus::PointF Control1 (0,0);
            Gdiplus::PointF Control2 (0,0);

            for (int j = 0; j < pSize - 3; j++){
                if (j == 0){
                    if (i > 0 && (cmd[i - 1].getCmd() == 'C' || cmd[i - 1].getCmd() == 'c' || cmd[i - 1].getCmd() == 's' || cmd[i - 1].getCmd() == 'S')){
                        Control1 = pre + pre - preCurve;
                    }else{
                        Control1 = pre;
                    }
                }else{
                    Control1 = pre + pre - preCurve;
                }
                Control2 = {coor[j], coor[j + 1]};
                cur = {coor[j + 2], coor[j + 3]};
                j += 3;
                path.AddBezier(pre, Control1, Control2, cur);
                
                pre = cur;
                preCurve = Control2;
            }
        }
        else if (c == 's'){
            Gdiplus::PointF Control1 (0,0);
            Gdiplus::PointF Control2 (0,0);
            for (int j = 0; j < pSize - 3; j++){
                if (j == 0){
                    if (i > 0 && (cmd[i - 1].getCmd() == 'C' || cmd[i - 1].getCmd() == 'c' || cmd[i - 1].getCmd() == 's' || cmd[i - 1].getCmd() == 'S')){
                        Control1 = pre + pre - preCurve;
                    }else{
                        Control1 = pre;
                    }
                }else{
                    Control1 = pre + pre - preCurve;
                }
                Control2 = pre + Gdiplus::PointF(coor[j], coor[j + 1]);
                cur = pre + Gdiplus::PointF(coor[j + 2], coor[j + 3]);
                j += 3;
                path.AddBezier(pre, Control1, Control2, cur);
                
                preCurve = Control2;
                pre = cur;
            }
        }
        else if (c == 'Q'){
            for (int j = 0; j < coor.size(); j++){
                Gdiplus::PointF Quad = {coor[j], coor[j + 1]};
                cur = {coor[j + 2], coor[j + 3]};
                j += 3;

                Gdiplus::PointF Control1 = pre;
                Control1.X += 2 * (Quad.X - pre.X) / 3.0;
                Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

                Gdiplus::PointF Control2 = cur;
                Control2.X += 2 * (Quad.X - cur.X) / 3.0;
                Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;            

                path.AddBezier(pre, Control1, Control2, cur);

                pre = cur;
                preCurve = Control2;
            }
        }
        else if (c == 'q'){
            for (int j = 0; j < coor.size(); j++){
                Gdiplus::PointF Quad = pre + Gdiplus::PointF(coor[j], coor[j + 1]);
                cur = pre + Gdiplus::PointF(coor[j + 2], coor[j + 3]);
                j += 3;
                
                Gdiplus::PointF Control1 = pre;
                Control1.X += 2 * (Quad.X - pre.X) / 3.0;
                Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

                Gdiplus::PointF Control2 = cur;
                Control2.X += 2 * (Quad.X - cur.X) / 3.0;
                Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;

                path.AddBezier(pre, Control1, Control2, cur);
                
                preCurve = Control2;
                pre = cur;
            }
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
        }
        else if (c == 't'){
            Gdiplus::PointF Quad = pre + pre - preCurve;
            cur = pre + Gdiplus::PointF(coor[0], coor[1]);

            Gdiplus::PointF Control1 = pre;
            Control1.X += 2 * (Quad.X - pre.X) / 3.0;
            Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

            Gdiplus::PointF Control2 = cur;
            Control2.X += 2 * (Quad.X - cur.X) / 3.0;
            Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;

            path.AddBezier(pre, Control1, Control2, cur);

            pre = cur;
            preCurve = Control2;
        }else if (c == 'A' || c == 'a'){
            
            if (c == 'A'){
                for (int j = 0; j < pSize; j += 7){
                    AddSvgArcToPath(path, pre.X, pre.Y, coor[j + 5], coor[j + 6], coor[j], coor[j + 1], coor[j + 2], int(coor[j + 3]), int(coor[j  +4]));
                    pre = {coor[j + 5], coor[j + 6]};
                }
            }else{
                for (int j = 0; j < pSize; j += 7){
                    AddSvgArcToPath(path, pre.X, pre.Y, coor[j + 5] + pre.X, coor[j + 6] + pre.Y, coor[j], coor[j + 1], coor[j + 2], int(coor[j + 3]), int(coor[j  +4]));
                    pre = {coor[j + 5] + pre.X, coor[j + 6] + pre.Y};
                }
            }
            cur = pre;
        }
        else if (c == 'Z' || c == 'z'){
            path.CloseFigure();
            pre = cur = pathStart;
        }
    }
    char end = cmd.back().getCmd();
    P->setPath(&path);
    setDrawer(obj);
    g->DrawPath(p, &path);
    if(P->getColor().GetGradient() != "")   FillPGradient(&path);
    else    g->FillPath(b, &path);
    Reset();
}

void Drawer::DrawG(Shapes::Object* obj){
    Shapes::Group* G = dynamic_cast<Shapes::Group*>(obj);
    int count = 0;

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
            count++;
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