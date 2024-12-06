#include "Drawer.h"
#include <gdiplus.h>

Drawer::Drawer(vector <unique_ptr<Shapes::Object>>& list){
    int n = list.size();
    for(int i = 0; i < n; i++){
        shapeList.push_back(std::move(list[i]));
    }
}

void Drawer::DrawR(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    if(Shapes::Rectangle* R = dynamic_cast <Shapes::Rectangle*> (shapeList.front().get())){
        Gdiplus::Matrix Ma;
        R->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        Pen p(Gdiplus::Color(R->getStroke().GetAlpha()*255, R->getStroke().GetRed(), R->getStroke().GetGreen(), R->getStroke().GetBlue()), R->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(R->getColor().GetAlpha()*255, R->getColor().GetRed(), R->getColor().GetGreen(), R->getColor().GetBlue()));
        g->FillRectangle(&b, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
        g->DrawRectangle(&p, R->getPoint().GetX() * s, R->getPoint().GetY() * s, R->getWidth() * s, R->getHeight() * s);
        g->ResetTransform();
    }
}

void Drawer::DrawL(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    if(Shapes::Line* L = dynamic_cast <Shapes::Line*> (shapeList.front().get())){
        Gdiplus::Matrix Ma;
        L->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);

        Pen p(Gdiplus::Color(L->getStroke().GetAlpha()*255, L->getStroke().GetRed(), L->getStroke().GetGreen(), L->getStroke().GetBlue()), L->getStrokeWidth() * s);
        g->DrawLine(&p, L->getStart().GetX() * s, L->getStart().GetY() * s, L->getEnd().GetX() * s, L->getEnd().GetY() * s);
        g->ResetTransform();
    }
}

void Drawer::DrawC(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    if(Shapes::Circle* C = dynamic_cast <Shapes::Circle*> (shapeList.front().get())){
        Gdiplus::Matrix Ma;
        C->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        Pen p(Gdiplus::Color(C->getStroke().GetAlpha()*255, C->getStroke().GetRed(), C->getStroke().GetGreen(), C->getStroke().GetBlue()), C->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(C->getColor().GetAlpha()*255, C->getColor().GetRed(), C->getColor().GetGreen(), C->getColor().GetBlue()));
        g->FillEllipse(&b, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
        g->DrawEllipse(&p, (C->getCenter().GetX() - C->getRadius()) * s, (C->getCenter().GetY() - C->getRadius()) * s, C->getRadius()*2 * s, C->getRadius()*2 * s);
        g->ResetTransform();
    }
}

void Drawer::DrawE(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    if(Shapes::Ellipse* E = dynamic_cast <Shapes::Ellipse*> (shapeList.front().get())){
        Gdiplus::Matrix Ma;
        E->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        Pen p(Gdiplus::Color(E->getStroke().GetAlpha()*255, E->getStroke().GetRed(), E->getStroke().GetGreen(), E->getStroke().GetBlue()), E->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(E->getColor().GetAlpha()*255, E->getColor().GetRed(), E->getColor().GetGreen(), E->getColor().GetBlue()));
        g->FillEllipse(&b, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
        g->DrawEllipse(&p, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
        g->ResetTransform();
    }
}

void Drawer::DrawPG(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    vector<Gdiplus::PointF> list;
    if(Shapes::Polygon* PG = dynamic_cast <Shapes::Polygon*> (shapeList.front().get())){
        int n = PG->getPoints().size();
        for (int i = 0; i < n; i++){
            list.push_back({PG->getPoints()[i].GetX() * s, PG->getPoints()[i].GetY() * s});
        }
        Gdiplus::Matrix Ma;
        PG->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        Pen p(Gdiplus::Color(PG->getStroke().GetAlpha()*255, PG->getStroke().GetRed(), PG->getStroke().GetGreen(), PG->getStroke().GetBlue()), PG->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(PG->getColor().GetAlpha()*255, PG->getColor().GetRed(), PG->getColor().GetGreen(), PG->getColor().GetBlue()));

        g->FillPolygon(&b, list.data(), static_cast<int> (PG->getPoints().size()));
        g->DrawPolygon(&p, list.data(), static_cast<int> (PG->getPoints().size()));
        g->ResetTransform();
    }
}

void Drawer::DrawPL(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    vector <Gdiplus::PointF> pF;
    if(Shapes::Polyline* PL = dynamic_cast <Shapes::Polyline*> (shapeList.front().get())){
        int size = PL->getPoints().size();
        for (int i = 0; i < size; i++){
            pF.push_back({PL->getPoints()[i].GetX() * s, PL->getPoints()[i].GetY() * s});
        }
        Gdiplus::Matrix Ma;
        PL->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        Pen p(Gdiplus::Color(PL->getStroke().GetAlpha()*255, PL->getStroke().GetRed(), PL->getStroke().GetGreen(), PL->getStroke().GetBlue()), PL->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(PL->getColor().GetAlpha()*255, PL->getColor().GetRed(), PL->getColor().GetGreen(), PL->getColor().GetBlue()));
        g->FillPolygon(&b, pF.data(), static_cast<int> (pF.size()));
        if (PL->getStrokeWidth() != 0) g->DrawLines(&p, pF.data(), static_cast<int>(pF.size()));
        g->ResetTransform();
    }
}

void Drawer::DrawT(Gdiplus::Graphics *g, float s, Gdiplus::PointF anchor){
    if(Shapes::Text* T = dynamic_cast <Shapes::Text*> (shapeList.front().get())){
        Gdiplus::Matrix Ma;
        T->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        SolidBrush b(Gdiplus::Color(T->getColor().GetAlpha()*255, T->getColor().GetRed(), T->getColor().GetGreen(), T->getColor().GetBlue()));
        Font TNR(L"Times New Roman", int(T->getFontSize() * s));

        size_t size_needed = mbstowcs(nullptr, T->getText().c_str(), 0);
        if (size_needed == static_cast<size_t>(-1)) {
            std::wcerr << L"Error converting string to wide string." << endl;
            return;
        }
        wstring wstr(size_needed, L'\0');
        mbstowcs(&wstr[0], T->getText().c_str(), size_needed);
        Shapes::Point p = T->getTop();
        p.SetY(p.GetY() - 1.33 * T->getFontSize());
        g->DrawString(wstr.c_str(), -1, &TNR, {p.GetX() * s, p.GetY() * s}, &b);
        g->ResetTransform();
    }
}

void Drawer::DrawP(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    Shapes::Path* PathPtr = dynamic_cast <Shapes::Path*> (shapeList.front().get());
    if(PathPtr == NULL)
        return;
    Shapes::Path P = *PathPtr;
    GraphicsPath path;

    Gdiplus::Matrix Ma;
    P.setTransform(Ma, s, anchor);
    g->SetTransform(&Ma);

    Pen p(Color(P.getStroke().GetAlpha()*255, P.getStroke().GetRed(), P.getStroke().GetGreen(), P.getStroke().GetBlue()), P.getStrokeWidth() * s);
    SolidBrush b(Color(P.getColor().GetAlpha()*255, P.getColor().GetRed(), P.getColor().GetGreen(), P.getColor().GetBlue()));

    vector<char> cmd = P.getCmd();
    vector<float> coor = P.getCoor();

    int pos = 0;
    float CurrX = 0;
    float CurrY = 0;
    float PrevX = 0;
    float PrevY = 0;
    float Prev_CurveX = 0;
    float Prev_CurveY = 0;

    path.StartFigure();
    
    for (int i = 0; i < cmd.size(); i++){
        cout << cmd[i] << '\n';
        if (cmd[i] == 'M' || cmd[i] == 'm'){
            CurrX = coor[pos];
            CurrY = coor[pos+1];

            if (cmd[i] == 'M'){
                path.AddLine((Gdiplus::PointF){CurrX, CurrY}, (Gdiplus::PointF){CurrX, CurrY});

                PrevX = CurrX;
                PrevY = CurrY;
                cout << "Move (absolute)\n";
            }
            else {
                path.AddLine((Gdiplus::PointF){PrevX + CurrX, PrevY + CurrY}, (Gdiplus::PointF){PrevX + CurrX, PrevY + CurrY});

                PrevX += CurrX;
                PrevY += CurrY;
                cout << "Move (relative)\n";
            }
            
            pos+=2;
        }
        else if (cmd[i] == 'L' || cmd[i] == 'l'){
            CurrX = coor[pos];
            CurrY = coor[pos+1];

            if (cmd[i] == 'L'){
                path.AddLine((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){CurrX, CurrY});

                PrevX = CurrX;
                PrevY = CurrY;
                cout << "Line (absolute)\n";
            }
            else {
                path.AddLine((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){PrevX + CurrX, PrevY + CurrY});

                PrevX += CurrX;
                PrevY += CurrY;
                cout << "Line (relative)\n";
            }

            pos += 2;
        }
        else if (cmd[i] == 'H' || cmd[i] == 'h'){
            CurrX = coor[pos];

            if (cmd[i] == 'H'){
                path.AddLine((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){CurrX, PrevY});

                PrevX = CurrX;
                cout << "Horizontal Line (absolute)\n";
            }
            else {
                path.AddLine((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){PrevX + CurrX, PrevY});
                
                PrevX += CurrX;
                cout << "Horizontal Line (relative)\n";
            }

            pos++;
        }
        else if (cmd[i] == 'V' || cmd[i] == 'v'){
            CurrY = coor[pos];
            
            if (cmd[i] == 'V'){
                path.AddLine((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){PrevX, CurrY});

                PrevY = CurrY;
                cout << "Vertical Line (absolute)\n";
            }
            else {
                path.AddLine((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){PrevX, PrevY + CurrY});
                
                PrevY += CurrY;
                cout << "Vertical (relative)\n";
            }

            pos++;
        }
        else if (cmd[i] == 'C'){
            float Control_P1_X = coor[pos++];
            float Control_P1_Y = coor[pos++];
            float Control_P2_X = coor[pos++];
            float Control_P2_Y = coor[pos++];
            CurrX = coor[pos];
            CurrY = coor[pos+1];

            path.AddBezier((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){Control_P1_X, Control_P1_Y}, (Gdiplus::PointF){Control_P2_X, Control_P2_Y}, (Gdiplus::PointF){CurrX, CurrY});
            
            PrevX = CurrX;
            PrevY = CurrY;
            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y;
            pos+=2;
            cout << "Cubic Bezier (absolute)\n";
        }
        else if (cmd[i] == 'c'){
            float Offset_P1_X = coor[pos++];
            float Offset_P1_Y = coor[pos++];
            float Offset_P2_X = coor[pos++];
            float Offset_P2_Y = coor[pos++];
            CurrX = coor[pos];
            CurrY = coor[pos+1];

            path.AddBezier((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){PrevX + Offset_P1_X, PrevY + Offset_P1_Y}, (Gdiplus::PointF){PrevX + Offset_P2_X, PrevY + Offset_P2_Y}, (Gdiplus::PointF){PrevX + CurrX, PrevY + CurrY});
            
            Prev_CurveX = PrevX + Offset_P2_X;
            Prev_CurveY = PrevY + Offset_P2_Y;
            PrevX += CurrX;
            PrevY += CurrY;
            pos+=2;
            cout << "Cubic Bezier (relative)\n";
        }
        else if (cmd[i] == 'S'){
            float Control_P1_X = 2 * PrevX - Prev_CurveX;
            float Control_P1_Y = 2 * PrevY - Prev_CurveY;
            float Control_P2_X = coor[pos++];
            float Control_P2_Y = coor[pos++];
            CurrX = coor[pos];
            CurrY = coor[pos+1];
            
            path.AddBezier((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){Control_P1_X, Control_P1_Y}, (Gdiplus::PointF){Control_P2_X, Control_P2_Y}, (Gdiplus::PointF){CurrX, CurrY});
            
            PrevX = CurrX;
            PrevY = CurrY;
            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y;
            pos+=2;
            cout << "Smooth Cubic Bezier (absolute)\n";
        }
        else if (cmd[i] == 's'){
            float Control_P1_X = 2 * PrevX - Prev_CurveX;
            float Control_P1_Y = 2 * PrevY - Prev_CurveY;
            float Offset_P2_X = coor[pos++];
            float Offset_P2_Y = coor[pos++];
            CurrX = coor[pos];
            CurrY = coor[pos+1];
            
            path.AddBezier((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){Control_P1_X, Control_P1_Y}, (Gdiplus::PointF){PrevX + Offset_P2_X, PrevY + Offset_P2_Y}, (Gdiplus::PointF){PrevX + CurrX, PrevY + CurrY});
            
            Prev_CurveX = PrevX + Offset_P2_X;
            Prev_CurveY = PrevY + Offset_P2_Y;
            PrevX += CurrX;
            PrevY += CurrY;
            pos+=2;
            cout << "Smooth Cubic Bezier (relative)\n";
        }
        else if (cmd[i] == 'Q'){
            float Quad_X = coor[pos++];
            float Quad_Y = coor[pos++];
            CurrX = coor[pos];
            CurrY = coor[pos+1];
            float Control_P1_X = PrevX + (2 * (Quad_X - PrevX))/3.0;
            float Control_P1_Y = PrevY + (2 * (Quad_Y - PrevY))/3.0;
            float Control_P2_X = CurrX + (2 * (Quad_X - CurrX))/3.0;
            float Control_P2_Y = CurrY + (2 * (Quad_Y - CurrY))/3.0;
            // cout << PrevX << ' ' << PrevY << ' ' << Prev_CurveX << ' ' << Prev_CurveY << ' ' << Control_P1_X << ' ' << Control_P1_Y << ' ' << Control_P2_X << ' ' << Control_P2_Y << ' ' << CurrX << ' ' << CurrY << '\n';

            path.AddBezier((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){Control_P1_X, Control_P1_Y}, (Gdiplus::PointF){Control_P2_X, Control_P2_Y}, (Gdiplus::PointF){CurrX, CurrY});

            PrevX = CurrX;
            PrevY = CurrY;
            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y;
            pos+=2;
            cout << "Quadratic Bezier (absolute)\n";
        }
        else if (cmd[i] == 'q'){
            float Quad_X = PrevX + coor[pos++];
            float Quad_Y = PrevY + coor[pos++];
            CurrX = PrevX + coor[pos];
            CurrY = PrevY + coor[pos+1];
            float Control_P1_X = PrevX + (2 * (Quad_X - PrevX))/3.0;
            float Control_P1_Y = PrevY + (2 * (Quad_Y - PrevY))/3.0;
            float Control_P2_X = CurrX + (2 * (Quad_X - CurrX))/3.0;
            float Control_P2_Y = CurrY + (2 * (Quad_Y - CurrY))/3.0;


            path.AddBezier((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){Control_P1_X, Control_P1_Y}, (Gdiplus::PointF){Control_P2_X, Control_P2_Y}, (Gdiplus::PointF){CurrX, CurrY});

            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y; 
            PrevX = CurrX;
            PrevY = CurrY;
            pos+=2;
            cout << "Quadratic Bezier (relative)\n";
        }
        else if (cmd[i] == 'T'){
            float Quad_X = 2 * PrevX - Prev_CurveX;
            float Quad_Y = 2 * PrevY - Prev_CurveY;
            CurrX = coor[pos];
            CurrY = coor[pos+1];
            float Control_P1_X = PrevX + (2 * (Quad_X - PrevX))/3.0;
            float Control_P1_Y = PrevY + (2 * (Quad_Y - PrevY))/3.0;
            float Control_P2_X = CurrX + (2 * (Quad_X - CurrX))/3.0;
            float Control_P2_Y = CurrY + (2 * (Quad_Y - CurrY))/3.0;

            path.AddBezier((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){Control_P1_X, Control_P1_Y}, (Gdiplus::PointF){Control_P2_X, Control_P2_Y}, (Gdiplus::PointF){CurrX, CurrY});

            PrevX = CurrX;
            PrevY = CurrY;
            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y;
            pos+=2;
            cout << "Smooth Quadratic Bezier (absolute)\n";
        }
        else if (cmd[i] == 't'){
            float Quad_X = 2 * PrevX - Prev_CurveX;
            float Quad_Y = 2 * PrevY - Prev_CurveY;
            CurrX = PrevX + coor[pos];
            CurrY = PrevY + coor[pos+1];
            float Control_P1_X = PrevX + (2 * (Quad_X - PrevX))/3.0;
            float Control_P1_Y = PrevY + (2 * (Quad_Y - PrevY))/3.0;
            float Control_P2_X = CurrX + (2 * (Quad_X - CurrX))/3.0;
            float Control_P2_Y = CurrY + (2 * (Quad_Y - CurrY))/3.0;

            path.AddBezier((Gdiplus::PointF){PrevX, PrevY}, (Gdiplus::PointF){Control_P1_X, Control_P1_Y}, (Gdiplus::PointF){Control_P2_X, Control_P2_Y}, (Gdiplus::PointF){CurrX, CurrY});

            PrevX = CurrX;
            PrevY = CurrY;
            Prev_CurveX = Control_P2_X;
            Prev_CurveY = Control_P2_Y;
            pos+=2;
            cout << "Smooth Quadratic Bezier (relative)\n";
        }
        else if (cmd[i] == 'Z' || cmd[i] == 'z'){
            path.CloseFigure();
            cout << "Close\n";
        }
    }

    g->DrawPath(&p, &path);
    g->FillPath(&b, &path);
    g->ResetTransform();
}

void Drawer::Draw(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
    while(!shapeList.empty()){
        Shapes::Object* rawPtr = shapeList.front().get();
        if(dynamic_cast <Shapes::Rectangle*> (rawPtr)){
            DrawR(g, s, anchor);
        }
        else if(dynamic_cast <Shapes::Line*> (rawPtr)){
            DrawL(g, s, anchor);
        }
        else if(dynamic_cast <Shapes::Circle*> (rawPtr)){
            DrawC(g, s, anchor);
        }
        else if(dynamic_cast <Shapes::Ellipse*> (rawPtr)){
            DrawE(g, s, anchor);
        }
        else if(dynamic_cast <Shapes::Polygon*> (rawPtr)){
            DrawPG(g, s, anchor);
        }
        else if(dynamic_cast <Shapes::Polyline*> (rawPtr)){
            DrawPL(g, s, anchor);
        }
        else if(dynamic_cast <Shapes::Text*> (rawPtr)){
            DrawT(g, s, anchor);
        }else if(dynamic_cast <Shapes::Path*> (rawPtr)){
            DrawP(g, s, anchor);
        }
        shapeList.erase(shapeList.begin());
    }
}

