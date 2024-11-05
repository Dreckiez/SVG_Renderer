#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <memory>
#include "tinyxml2.h"
#include "objects.h"
using namespace Gdiplus;
using namespace std;
#pragma comment(lib, "gdiplus.lib")

ULONG_PTR gdiToken;
float scale = 1;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:{
            RECT pos;
            GetClientRect(hwnd, &pos);
            HWND ZoomInButton = CreateWindowEx(0, "BUTTON", "Zoom In", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, pos.right - 125, pos.bottom - 125, 100, 50, hwnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);
            HWND ZoomOutButton = CreateWindowEx(0, "BUTTON", "Zoom Out", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, pos.right - 125, pos.bottom - 75, 100, 50, hwnd, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), nullptr);
            return 0;
        }
        case WM_COMMAND:{
            if (LOWORD(wParam) == 1){
                if (scale < 3){
                    scale += 0.5;
                    InvalidateRect(hwnd, nullptr, TRUE);
                }
            }
            else if (LOWORD(wParam) == 2){
                if (scale > 0.25){
                    if (scale > 0.5) scale -= 0.5;
                    else scale -= 0.25;
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
            doc.LoadFile("sample.svg");
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            Graphics graphics(hdc);
            graphics.Clear(Color(255, 255, 255, 255));
            graphics.SetSmoothingMode(SmoothingModeAntiAlias);

            for (tinyxml2::XMLElement* root = doc.FirstChildElement()->FirstChildElement(); root != nullptr; root = root->NextSiblingElement()){
                string name = root->Name();
                if (name == "rect"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Rectangle>();
                    ptr->Read(root);
                    ptr->Draw(&graphics, scale);
                }
                else if (name == "line"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Line>();
                    ptr->Read(root);
                    ptr->Draw(&graphics, scale);
                }
                else if (name == "circle"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Circle>();
                    ptr->Read(root);
                    ptr->Draw(&graphics, scale);
                }
                else if (name == "ellipse"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Ellipse>();
                    ptr->Read(root);
                    ptr->Draw(&graphics, scale);
                }
                else if (name == "polygon"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Polygon>();
                    ptr->Read(root);
                    ptr->Draw(&graphics, scale);
                }
                else if (name == "polyline"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Polyline>();
                    ptr->Read(root);
                    ptr->Draw(&graphics, scale);
                }
                else if (name == "text"){
                    unique_ptr<Shapes::Object> ptr = make_unique<Shapes::Text>();
                    ptr->Read(root);
                    ptr->Draw(&graphics, scale);
                }
            }
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

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "GDIPlusWindowClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "SVG Render", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

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
