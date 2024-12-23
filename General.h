#ifndef GENERAL_H
#define GENERAL_H

#include <cmath>
#include <windows.h>  // Includes core Windows definitions
#include <propidl.h>
#include <gdiplus.h>
#include <algorithm>
#include <string>
using namespace Gdiplus;
using namespace std;

// Helper function to calculate angle in degrees
double RadiansToDegrees(double radians);

double DegreesToRadians(double degrees);

float AngleBetweenVectors(float ux, float uy, float vx, float vy);

void AddSvgArcToPath(Gdiplus::GraphicsPath& path, 
                     float x0, float y0, 
                     float x1, float y1, 
                     float rx, float ry, 
                     float xAxisRotation, 
                     int largeArcFlag, 
                     int sweepFlag);

float ConvertUnit(string s);

#endif