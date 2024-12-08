#include "Drawer.h"
#include "gdiplus.h"

using namespace Gdiplus;

#include <iostream>
using namespace std;
void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}
template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ','; __print(x.second); cerr << '}';}
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? "," : ""), __print(i); cerr << "}";}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {__print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}
#ifndef ONLINE_JUDGE
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif

Drawer::Drawer(vector <unique_ptr<Shapes::Object>>& list, Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor){
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

void Drawer::DrawE(Gdiplus::Graphics* g, float s, Gdiplus::PointF anchor, Shapes::Ellipse* E){
        Gdiplus::Matrix Ma;
        E->setTransform(Ma, s, anchor);
        g->SetTransform(&Ma);
        
        Pen p(Gdiplus::Color(E->getStroke().GetAlpha()*255, E->getStroke().GetRed(), E->getStroke().GetGreen(), E->getStroke().GetBlue()), E->getStrokeWidth() * s);
        SolidBrush b(Gdiplus::Color(E->getColor().GetAlpha()*255, E->getColor().GetRed(), E->getColor().GetGreen(), E->getColor().GetBlue()));
        g->FillEllipse(&b, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
        g->DrawEllipse(&p, (E->getCenter().GetX() - E->getRadiusX()) * s, (E->getCenter().GetY() - E->getRadiusY()) * s, E->getRadiusX()*2 * s, E->getRadiusY()*2 * s);
        g->ResetTransform();
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

        GraphicsPath path;
        path.StartFigure();
        
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

        FontFamily FFamily(L"Times New Roman");

        // path.AddString(wstr.c_str(), -1, &FFamily, 0, REAL(T->getFontSize()), {p.GetX() * s, p.GetY() * s}, NULL);

        g->DrawString(wstr.c_str(), -1, &TNR, {p.GetX() * s, p.GetY() * s}, &b);

        g->ResetTransform();
    }
}

void Drawer::DrawP(Gdiplus::Graphics* g, float s, PointF anchor){
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

    int size = P.getCmd().size();

    vector<Shapes::Command> cmd = P.getCmd();

    int pos = 0;
    //all command has to update cur and pre accordingly
    PointF cur;     //current position of pen
    PointF pre;     // 1 point before cur. cur will be updated first then AddLine or watever before updating pre
    PointF preCurve;    //previous control point, used and updated when handle command curve, quaratic,...

    path.StartFigure();
    
    char end = cmd.back().getCmd();

    for (int i = 0; i < size; i++){
        char c = cmd[i].getCmd();

        vector<PointF> coor = cmd[i].getPoints();

        for (int i = 0; i < coor.size(); i++){
            coor[i].X *= s;
            coor[i].Y *= s;
        }

        if (c == 'M' || c == 'm'){
            int pSize = coor.size();
            if (c == 'M'){
                pre = cur = coor.front();
                if (pSize == 1){
                    path.AddLine(coor[0], coor[0]);
                }else{
                    for (int j = 0; j < coor.size() - 1; j++){
                        path.AddLine(coor[j], coor[j + 1]);
                    }
                }
            }
            else {
                if (pSize == 1){
                    path.AddLine(coor[0], coor[0]);
                    cur = coor[0];
                }else{
                    path.AddLine(coor[0], coor[1] + coor[0]);
                    for (int j = 1; j < coor.size() - 1; j++){
                        cur = coor[j + 1] + coor[j];
                        path.AddLine(coor[j] + coor[j - 1], cur);
                    }
                }
                pre = cur;
            }
        }
        else if (c == 'L' || c == 'l'){
            if (c == 'L'){
                pre = cur = coor.back();
                path.AddLine(pre, coor[0]);
            }
            else {
                cur = coor.back() + pre;
                path.AddLine(pre, cur);

                pre = cur;
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
            Gdiplus::PointF Control1 = coor[0];
            Gdiplus::PointF Control2 = coor[1];
            cur = coor[2];

            path.AddBezier(pre, Control1, Control2, cur);
            
            pre = cur;
            preCurve = Control2;

            cout << "Cubic Bezier (absolute)\n";
        }
        else if (c == 'c'){
            // debug(pre.X, pre.Y);
            PointF Control1 = pre + coor[0];
            PointF Control2 = pre + coor[1];
            cur = pre + coor[2];
            // debug (cur.X, cur.Y, Control1.X, Control1.Y);
            // debug(Control2.X, Control2.Y);
            path.AddBezier(pre, Control1, Control2, cur);
            
            preCurve = Control2;
            pre = cur;
            cout << "Cubic Bezier (relative)\n";
        }
        else if (c == 'S'){
            PointF Control1 = pre + pre - preCurve;
            PointF Control2 = coor[0];
            cur = coor[1];
            
            path.AddBezier(pre, Control1, Control2, cur);
            
            pre = cur;
            preCurve = Control2;

            cout << "Smooth Cubic Bezier (absolute)\n";
        }
        else if (c == 's'){
            PointF Control1 = pre + pre - preCurve;
            PointF Control2 = pre + coor[0];
            PointF cur = pre + coor[1];
            
            path.AddBezier(pre, Control1, Control2, cur);
            
            preCurve = Control2;
            pre = cur;

            cout << "Smooth Cubic Bezier (relative)\n";
        }
        else if (c == 'Q'){
            PointF Quad = coor[0];
            cur = coor[1];

            PointF Control1 = pre;
            Control1.X += 2 * (Quad.X - pre.X) / 3.0;
            Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

            PointF Control2 = cur;
            Control2.X += 2 * (Quad.X - cur.X) / 3.0;
            Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;
            
            // cout << PrevX << ' ' << PrevY << ' ' << Prev_CurveX << ' ' << Prev_CurveY << ' ' << Control_P1_X << ' ' << Control_P1_Y << ' ' << Control_P2_X << ' ' << Control_P2_Y << ' ' << CurrX << ' ' << CurrY << '\n';

            path.AddBezier(pre, Control1, Control2, cur);

            pre = cur;
            preCurve = Control2;
            cout << "Quadratic Bezier (absolute)\n";
        }
        else if (c == 'q'){
            PointF Quad = pre + coor[0];
            cur = pre + coor[1];

            PointF Control1 = pre;
            Control1.X += 2 * (Quad.X - pre.X) / 3.0;
            Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

            PointF Control2 = cur;
            Control2.X += 2 * (Quad.X - cur.X) / 3.0;
            Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;

            path.AddBezier(pre, Control1, Control2, cur);

            preCurve = Control2;
            pre = cur;

            cout << "Quadratic Bezier (relative)\n";
        }
        else if (c == 'T'){
            PointF Quad = pre + pre - preCurve;
            cur = coor[0];

            PointF Control1 = pre;
            Control1.X += 2 * (Quad.X - pre.X) / 3.0;
            Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

            PointF Control2 = cur;
            Control2.X += 2 * (Quad.X - cur.X) / 3.0;
            Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;

            path.AddBezier(pre, Control1, Control2, cur);

            pre = cur;
            preCurve = Control2;

            cout << "Smooth Quadratic Bezier (absolute)\n";
        }
        else if (c == 't'){
            PointF Quad = pre + pre - preCurve;
            cur = pre + coor[0];

            PointF Control1 = pre;
            Control1.X += 2 * (Quad.X - pre.X) / 3.0;
            Control1.Y += 2 * (Quad.Y - pre.Y) / 3.0;

            PointF Control2 = cur;
            Control2.X += 2 * (Quad.X - cur.X) / 3.0;
            Control2.Y += 2 * (Quad.Y - cur.Y) / 3.0;

            path.AddBezier(pre, Control1, Control2, cur);

            pre = cur;
            preCurve = Control2;

            cout << "Smooth Quadratic Bezier (relative)\n";
        }
        else if (c == 'Z' || c == 'z'){
            path.CloseFigure();
            g->DrawPath(&p, &path);
            g->FillPath(&b, &path);
            g->ResetTransform();
            path.Reset();
            path.StartFigure();
            cout << "Close\n";
        }
    }
    if (end != 'z' && end != 'Z'){
        g->DrawPath(&p, &path);
        g->FillPath(&b, &path);
        g->ResetTransform();
    }
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
        }
        else if(dynamic_cast <Shapes::Path*> (rawPtr)){
            DrawP(g, s, anchor);
        }
        else if(dynamic_cast <Shapes::Group*> (rawPtr)){
            Shapes::Group group = *(dynamic_cast<Shapes::Group*> (rawPtr));
            cout << "draw group ";
            for (int i = 0; i < group.getSize(); i++){
                cout << i << " ";
                Shapes::Object* raw = group[i];
                if (group[i] == NULL)
                    cout << "null";
                if(dynamic_cast <Shapes::Rectangle*> (raw)){
                    unique_ptr<Shapes::Object> ptr(group[i]->clone());
                    shapeList.insert(shapeList.begin(), move(ptr));
                    DrawR(g,s,anchor);
                }
                else if(dynamic_cast <Shapes::Line*> (raw)){
                    unique_ptr<Shapes::Object> ptr(group[i]->clone());
                    shapeList.insert(shapeList.begin(), move(ptr));
                    DrawL(g, s, anchor);
                }
                else if(dynamic_cast <Shapes::Circle*> (raw)){
                    cout << "c ";
                    unique_ptr<Shapes::Object> ptr(group[i]->clone());
                    shapeList.insert(shapeList.begin(), move(ptr));
                    DrawC(g, s, anchor);
                }
                else if(dynamic_cast <Shapes::Ellipse*> (raw)){
                    unique_ptr<Shapes::Object> ptr(group[i]->clone());
                    shapeList.insert(shapeList.begin(), move(ptr));
                    DrawE(g, s, anchor);
                }
                else if(dynamic_cast <Shapes::Polygon*> (raw)){
                    unique_ptr<Shapes::Object> ptr(group[i]->clone());
                    shapeList.insert(shapeList.begin(), move(ptr));
                    DrawPG(g, s, anchor);
                }
                else if(dynamic_cast <Shapes::Polyline*> (raw)){
                    unique_ptr<Shapes::Object> ptr(group[i]->clone());
                    shapeList.insert(shapeList.begin(), move(ptr));
                    DrawPL(g, s, anchor);
                }
                else if(dynamic_cast <Shapes::Text*> (raw)){
                    unique_ptr<Shapes::Object> ptr(group[i]->clone());
                    shapeList.insert(shapeList.begin(), move(ptr));
                    DrawT(g, s, anchor);
                }
                else if(dynamic_cast <Shapes::Path*> (raw)){
                    unique_ptr<Shapes::Object> ptr(group[i]->clone());
                    shapeList.insert(shapeList.begin(), move(ptr));
                    DrawP(g, s, anchor);
                }else if(dynamic_cast <Shapes::Group*> (raw)){
                    cout << "g ";
                    unique_ptr<Shapes::Object> ptr(group[i]->clone());
                    shapeList.insert(shapeList.begin(), move(ptr));
                    break;
                }
                shapeList.erase(shapeList.begin());
            }
            cout << group.toString();
        }
        if(dynamic_cast <Shapes::Rectangle*> (shapeList.front().get())){
            cout << "Group1\n";
        }
        else if(dynamic_cast <Shapes::Line*> (shapeList.front().get())){
            cout << "Group2\n";
        }
        else if(dynamic_cast <Shapes::Circle*> (shapeList.front().get())){
            cout << "Group3\n";
        }
        else if(dynamic_cast <Shapes::Ellipse*> (shapeList.front().get())){
            cout << "Group4\n";
        }
        else if(dynamic_cast <Shapes::Polygon*> (shapeList.front().get())){
            cout << "Group5\n";
        }
        else if(dynamic_cast <Shapes::Polyline*> (shapeList.front().get())){
            cout << "Group6\n";
        }
        else if(dynamic_cast <Shapes::Text*> (shapeList.front().get())){
            cout << "Group7\n";
        }
        else if(dynamic_cast <Shapes::Path*> (shapeList.front().get())){
            cout << "Group8\n";
        }
        if (dynamic_cast<Shapes::Group*> (shapeList.front().get())){
            cout << "Group9\n";
        }
        if (shapeList.front() == NULL)
            cout << "null";
        else{
            cout << "sizeip : " << shapeList.size() << endl;
            shapeList.erase(shapeList.begin());
            cout << "erased";
        }
    }
    cout << "lop end";
}

