#include <windows.h>
#include <gdiplus.h>
#include <string>
#include "tinyxml2.h"
#include "objects.h"
using namespace Gdiplus;
using namespace std;
#pragma comment(lib, "gdiplus.lib")

ULONG_PTR gdiToken;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
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

            for (tinyxml2::XMLElement* root = doc.FirstChildElement()->FirstChildElement(); root != nullptr; root = root->NextSiblingElement()){
                string name = root->Name();
                if (name == "rect"){
                    Shapes::Rectangle R;
                    R.ReadRect(root);
                    R.DrawR(&graphics);
                }
                else if (name == "line"){
                    Shapes::Line L;
                    L.ReadLine(root);
                    L.DrawL(&graphics);
                }
                else if (name == "circle"){
                    Shapes::Circle C;
                    C.ReadCircle(root);
                    C.DrawC(&graphics);
                }
                else if (name == "ellipse"){
                    Shapes::Ellipse E;
                    E.ReadEllipse(root);
                    E.DrawE(&graphics);
                }
                else if (name == "polygon"){
                    Shapes::Polygon PG;
                    PG.ReadPolygon(root);
                    PG.DrawPG(&graphics);
                    // PG.GetCoords();
                    // PG.GetColor();
                    // PG.GetStroke();
                }
            }
            

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_SIZE:
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
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
