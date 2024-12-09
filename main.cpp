#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <memory>
#include <fstream>
#include <iostream>

#include "tinyxml2.h"
#include "Drawer.h"
#include "Reader.h"

using namespace Gdiplus;
using namespace std;
#pragma comment(lib, "gdiplus.lib")

ULONG_PTR gdiToken;
float scale = 1;
bool scale_mouse = 0;
string filepath;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:{
            RECT pos;
            GetClientRect(hwnd, &pos);
            HWND ZoomInButton = CreateWindowEx(0, "BUTTON", "Zoom In", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, pos.right - 125, pos.bottom - 125, 100, 50, hwnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);
            HWND ZoomOutButton = CreateWindowEx(0, "BUTTON", "Zoom Out", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, pos.right - 125, pos.bottom - 75, 100, 50, hwnd, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);
            return 0;
        }
        case WM_MOUSEWHEEL:{
            // MessageBox(hwnd, "Mouse Wheel detected!", "Mouse Scroll", MB_OK);
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            // cout << delta << '\n';
            if (delta > 0){
                if (scale < 3){
                    if (scale == 0.25) scale += 0.25;
                    else scale += 0.5;
                    scale_mouse = 1;
                    InvalidateRect(hwnd, nullptr, TRUE);
                }
            }
            else if (delta < 0){
                if (scale > 0.25){
                    if (scale > 0.5) scale -= 0.5;
                    else scale -= 0.25;
                    scale_mouse = 1;
                    InvalidateRect(hwnd, nullptr, TRUE);
                }
            }
        }
        case WM_COMMAND:{
            if (LOWORD(wParam) == 1){
                if (scale < 3){
                    if (scale == 0.25) scale += 0.25;
                    else scale += 0.5;
                    scale_mouse = 0;
                    InvalidateRect(hwnd, nullptr, TRUE);
                }
            }
            else if (LOWORD(wParam) == 2){
                if (scale > 0.25){
                    if (scale > 0.5) scale -= 0.5;
                    else scale -= 0.25;
                    scale_mouse = 0;
                    InvalidateRect(hwnd, nullptr, TRUE);
                }
            }
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:{
            tinyxml2::XMLDocument doc;
<<<<<<< HEAD
            doc.LoadFile("sample.svg");
=======
            doc.LoadFile(filepath.c_str());
>>>>>>> 036faf459ef4e589e5ffb7bedf16d402b8a17144
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            Graphics graphics(hdc);
            graphics.Clear(Color(255, 255, 255, 255));
            graphics.SetSmoothingMode(SmoothingModeAntiAlias);

            PointF anchor;
            if (scale_mouse){
                POINT mouse_pos;
                GetCursorPos(&mouse_pos);
                anchor.X = mouse_pos.x;
                anchor.Y = mouse_pos.y;
            }
            else {
                RECT screen_center;
                GetClientRect(hwnd, &screen_center);
                anchor.X =  screen_center.right/2;
                anchor.Y = screen_center.bottom/2;
            }
            Shapes::LinearVector LG;
            vector <unique_ptr<Shapes::Object>> list;
            Reader reader;
            tinyxml2::XMLElement* root = doc.FirstChildElement("svg");
            tinyxml2::XMLElement* def = root->FirstChildElement("defs");
            if(def){
                LG.read_gradient(def);
            }
            root = root->FirstChildElement();

            for (; root != nullptr; root = root->NextSiblingElement()){
                string name = root->Name();
                if (name == "rect"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Rectangle>();
                    reader.ReadRectangle(dynamic_cast<Shapes::Rectangle*>(ptr.get()), root);
                    list.push_back(std::move(ptr));
                    cout << "Read Rect\n";
                }
                else if (name == "line"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Line>();
                    reader.ReadLine(dynamic_cast<Shapes::Line*>(ptr.get()), root);
                    list.push_back(std::move(ptr));
                }
                else if (name == "circle"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Circle>();
                    reader.ReadCircle(dynamic_cast<Shapes::Circle*>(ptr.get()), root);
                    list.push_back(std::move(ptr));
                }
                else if (name == "ellipse"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Ellipse>();
                    reader.ReadEllipse(dynamic_cast<Shapes::Ellipse*>(ptr.get()), root);
                    list.push_back(std::move(ptr));
                }
                else if (name == "polygon"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Polygon>();
                    reader.ReadPolygon(dynamic_cast<Shapes::Polygon*>(ptr.get()), root);
                    list.push_back(std::move(ptr));
                }
                else if (name == "polyline"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Polyline>();
                    reader.ReadPolyline(dynamic_cast<Shapes::Polyline*>(ptr.get()), root);
                    list.push_back(std::move(ptr));
                }
                else if (name == "text"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Text>();
                    reader.ReadText(dynamic_cast<Shapes::Text*>(ptr.get()), root);
                    list.push_back(std::move(ptr));
                }
<<<<<<< HEAD
                // else if (name == "path"){
                //     unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Path>();
                //     reader.ReadPath(dynamic_cast<Shapes::Path*>(ptr.get()), root);
                //     list.push_back(std::move(ptr));
                // }
=======
                else if (name == "path"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Path>();
                    reader.ReadPath(dynamic_cast<Shapes::Path*>(ptr.get()), root);
                    list.push_back(std::move(ptr));
                }
                else if (name == "g"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Group>();
                    reader.ReadGroup(dynamic_cast<Shapes::Group*>(ptr.get()), root);
                    list.push_back(std::move(ptr));
                }
>>>>>>> 036faf459ef4e589e5ffb7bedf16d402b8a17144
            }
            Drawer drawer(list, &graphics, scale, anchor);
            drawer.Draw();
            //graphics.SetSmoothingMode(SmoothingModeNone);

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_SIZE:{
            RECT pos;
            GetClientRect(hwnd, &pos);
            HWND MZIB = GetDlgItem(hwnd, 1);
            HWND MZOB = GetDlgItem(hwnd, 2);
            if (MZIB) MoveWindow(MZIB, pos.right - 125, pos.bottom - 125, 100, 50, TRUE);
            if (MZOB) MoveWindow(MZOB, pos.right - 125, pos.bottom - 75, 100, 50, TRUE);
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    GdiplusStartupInput gdiInput;
    GdiplusStartup(&gdiToken, &gdiInput, nullptr);
    
    // string CMD(lpCmdLine);
    filepath = lpCmdLine;

    if (filepath.empty()){
        MessageBox(nullptr, "Provide a file path!", "Error", MB_OK | MB_ICONERROR);
        return 0;
    }
    
    fstream f(filepath);
    if (!f.is_open()){
        f.close();
        MessageBox(nullptr, "File doesn't exist!", "Error", MB_OK | MB_ICONERROR);
        return 0;   
    }

    f.close();


    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "GDIPlusWindowClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "SVG Render", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, lpCmdLine);

    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, SW_MAXIMIZE);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiToken);

    return 0;
}
