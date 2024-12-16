#ifndef GENERAL_H
#define GENERAL_H

#include <cmath>
#include <windows.h>  // Includes core Windows definitions
#include <propidl.h>
#include <gdiplus.h>
#include <algorithm>
using namespace Gdiplus;

// Helper function to calculate angle in degrees
double RadiansToDegrees(double radians);

double DegreesToRadians(double degrees);

float AngleBetweenVectors(float ux, float uy, float vx, float vy);

// Function to add an SVG arc to a GraphicsPath
// void AddSvgArcToPath(GraphicsPath &path, 
//                      float x1, float y1, 
//                      float x2, float y2, 
//                      float rx, float ry, 
//                      float xAxisRotation,
//                      bool largeArcFlag, 
//                      bool sweepFlag);

void AddSvgArcToPath(Gdiplus::GraphicsPath& path, 
                     float x0, float y0, 
                     float x1, float y1, 
                     float rx, float ry, 
                     float xAxisRotation, 
                     int largeArcFlag, 
                     int sweepFlag);

#endif