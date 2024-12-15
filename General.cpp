#include "General.h"

// Helper function to calculate angle in degrees
double RadiansToDegrees(double radians) {
    return radians * 180.0 / M_PI;
}

double DegreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

// Utility function to normalize angles between 0 and 2*PI
float NormalizeAngle(float angle) {
    while (angle < 0) angle += 2 * M_PI;
    while (angle >= 2 * M_PI) angle -= 2 * M_PI;
    return angle;
}

// Helper function to calculate the angle between two vectors
float AngleBetweenVectors(float ux, float uy, float vx, float vy) {
    float dot = ux * vx + uy * vy;
    float det = ux * vy - uy * vx;
    return std::atan2(det, dot);
}

// void AddSvgArcToPath(Gdiplus::GraphicsPath& path, 
//                      float x0, float y0, 
//                      float x1, float y1, 
//                      float rx, float ry, 
//                      float xAxisRotation, 
//                      int largeArcFlag, 
//                      int sweepFlag) {
//     // Convert rotation angle from degrees to radians
//     float phi = xAxisRotation * (M_PI / 180.0f);

//     // Step 1: Handle zero radii case (arc reduces to a straight line)
//     if (rx == 0 || ry == 0) {
//         path.AddLine(Gdiplus::PointF(x0, y0), Gdiplus::PointF(x1, y1));
//         return;
//     }

//     // Step 2: Ensure radii are positive
//     rx = std::abs(rx);
//     ry = std::abs(ry);

//     // Step 3: Compute (x1', y1') in the transformed coordinate system
//     float dx = (x0 - x1) / 2.0f;
//     float dy = (y0 - y1) / 2.0f;

//     float x1Prime = std::cos(phi) * dx + std::sin(phi) * dy;
//     float y1Prime = -std::sin(phi) * dx + std::cos(phi) * dy;

//     // Step 4: Correct radii if necessary
//     float rxSquared = rx * rx;
//     float rySquared = ry * ry;
//     float x1PrimeSquared = x1Prime * x1Prime;
//     float y1PrimeSquared = y1Prime * y1Prime;

//     float radiusCorrection = (x1PrimeSquared / rxSquared) + (y1PrimeSquared / rySquared);
//     if (radiusCorrection > 1) {
//         float scale = std::sqrt(radiusCorrection);
//         rx *= scale;
//         ry *= scale;
//         rxSquared = rx * rx;
//         rySquared = ry * ry;
//     }

//     // Step 5: Compute the center of the ellipse in the transformed coordinate system
//     float numerator = rxSquared * rySquared - rxSquared * y1PrimeSquared - rySquared * x1PrimeSquared;
//     float denominator = rxSquared * y1PrimeSquared + rySquared * x1PrimeSquared;

//     float centerFactor = std::sqrt(std::max(0.0f, numerator / denominator));
//     if (largeArcFlag == sweepFlag) {
//         centerFactor = -centerFactor;
//     }

//     float cxPrime = centerFactor * (rx * y1Prime / ry);
//     float cyPrime = centerFactor * -(ry * x1Prime / rx);

//     // Step 6: Compute (cx, cy) in the original coordinate system
//     float cx = std::cos(phi) * cxPrime - std::sin(phi) * cyPrime + (x0 + x1) / 2.0f;
//     float cy = std::sin(phi) * cxPrime + std::cos(phi) * cyPrime + (y0 + y1) / 2.0f;

//     // Step 7: Compute start and sweep angles
//     float startVectorX = (x1Prime - cxPrime) / rx;
//     float startVectorY = (y1Prime - cyPrime) / ry;
//     float endVectorX = (-x1Prime - cxPrime) / rx;
//     float endVectorY = (-y1Prime - cyPrime) / ry;

//     float startAngle = std::atan2(startVectorY, startVectorX);
//     float endAngle = std::atan2(endVectorY, endVectorX);

//     // Compute the sweep angle
//     float sweepAngle = AngleBetweenVectors(startVectorX, startVectorY, endVectorX, endVectorY);

//     // Adjust for sweep flag and large arc flag
//     if (sweepFlag == 1 && sweepAngle < 0) {
//         sweepAngle += 2 * M_PI;
//     } else if (sweepFlag == 0 && sweepAngle > 0) {
//         sweepAngle -= 2 * M_PI;
//     }

//     if (largeArcFlag == 1) {
//         if (std::abs(sweepAngle) < M_PI) {
//             sweepAngle += (sweepAngle > 0 ? -2 * M_PI : 2 * M_PI);
//         }
//     }

//     // Convert angles to degrees
//     startAngle *= (180.0f / M_PI);
//     sweepAngle *= (180.0f / M_PI);

//     // Step 8: Add the arc to the GDI+ path
//     if (xAxisRotation < 0) {
//         // Handle the case where xAxisRotation is negative
//         Gdiplus::Matrix transform;
//         transform.RotateAt(xAxisRotation, Gdiplus::PointF(cx, cy));

//         // Define a bounding rectangle for the ellipse
//         Gdiplus::RectF arcRect(cx - rx, cy - ry, rx * 2, ry * 2);

//         // Apply rotation and add the arc to a temporary path
//         Gdiplus::GraphicsPath tempPath;
//         tempPath.AddArc(arcRect, startAngle, sweepAngle);
//         tempPath.Transform(&transform);

//         // Append the transformed arc to the main path
//         path.AddPath(&tempPath, FALSE);
//         return;
//     }

//     Gdiplus::Matrix transform;
//     transform.RotateAt(xAxisRotation, Gdiplus::PointF(cx, cy));

//     // Define a bounding rectangle for the ellipse
//     Gdiplus::RectF arcRect(cx - rx, cy - ry, rx * 2, ry * 2);

//     // Apply rotation and add the arc
//     Gdiplus::GraphicsPath tempPath;
//     tempPath.AddArc(arcRect, startAngle, sweepAngle);
//     tempPath.Transform(&transform);

//     path.AddPath(&tempPath, FALSE);
// }

void AddSvgArcToPath(Gdiplus::GraphicsPath& path, 
                     float x0, float y0, 
                     float x1, float y1, 
                     float rx, float ry, 
                     float xAxisRotation, 
                     int largeArcFlag, 
                     int sweepFlag) {
    // Convert rotation angle from degrees to radians
    float phi = xAxisRotation * (M_PI / 180.0f);

    // Step 1: Handle zero radii case (arc reduces to a straight line)
    if (rx == 0 || ry == 0) {
        path.AddLine(Gdiplus::PointF(x0, y0), Gdiplus::PointF(x1, y1));
        return;
    }

    // Step 2: Ensure radii are positive
    rx = std::abs(rx);
    ry = std::abs(ry);

    // Step 3: Compute (x1', y1') in the transformed coordinate system
    float dx = (x0 - x1) / 2.0f;
    float dy = (y0 - y1) / 2.0f;

    float x1Prime = std::cos(phi) * dx + std::sin(phi) * dy;
    float y1Prime = -std::sin(phi) * dx + std::cos(phi) * dy;

    // Step 4: Compute the center of the ellipse in the transformed coordinate system
    float rxSquared = rx * rx;
    float rySquared = ry * ry;
    float x1PrimeSquared = x1Prime * x1Prime;
    float y1PrimeSquared = y1Prime * y1Prime;

    // Correct radii if necessary
    float radiusCorrection = (x1PrimeSquared / rxSquared) + (y1PrimeSquared / rySquared);
    if (radiusCorrection > 1) {
        float scale = std::sqrt(radiusCorrection);
        rx *= scale;
        ry *= scale;
        rxSquared = rx * rx;
        rySquared = ry * ry;
    }

    float numerator = rxSquared * rySquared - rxSquared * y1PrimeSquared - rySquared * x1PrimeSquared;
    float denominator = rxSquared * y1PrimeSquared + rySquared * x1PrimeSquared;

    float centerFactor = std::sqrt(std::max(0.0f, numerator / denominator));
    if (largeArcFlag == sweepFlag) {
        centerFactor = -centerFactor;
    }

    float cxPrime = centerFactor * (rx * y1Prime / ry);
    float cyPrime = centerFactor * -(ry * x1Prime / rx);

    // Step 5: Compute (cx, cy) in the original coordinate system
    float cx = std::cos(phi) * cxPrime - std::sin(phi) * cyPrime + (x0 + x1) / 2.0f;
    float cy = std::sin(phi) * cxPrime + std::cos(phi) * cyPrime + (y0 + y1) / 2.0f;

    // Step 6: Compute start and sweep angles
    float startVectorX = (x1Prime - cxPrime) / rx;
    float startVectorY = (y1Prime - cyPrime) / ry;
    float endVectorX = (-x1Prime - cxPrime) / rx;
    float endVectorY = (-y1Prime - cyPrime) / ry;

    float startAngle = std::atan2(startVectorY, startVectorX);
    float endAngle = std::atan2(endVectorY, endVectorX);

    // Compute the sweep angle
    float sweepAngle = endAngle - startAngle;
    if (sweepFlag == 0 && sweepAngle > 0) {
        sweepAngle -= 2 * M_PI;
    } else if (sweepFlag == 1 && sweepAngle < 0) {
        sweepAngle += 2 * M_PI;
    }

    // Convert angles to degrees
    startAngle *= (180.0f / M_PI);
    sweepAngle *= (180.0f / M_PI);

    if (xAxisRotation != 0){
        Gdiplus::RectF arcRect(cx - rx, cy - ry, rx * 2, ry * 2);
        Gdiplus::GraphicsPath arcPath;
        arcPath.AddArc(arcRect, startAngle, sweepAngle);

        // Apply rotation transform
        Gdiplus::Matrix transform;
        transform.RotateAt(xAxisRotation, Gdiplus::PointF(cx, cy));
        arcPath.Transform(&transform);

        // Append the rotated arc path to the original path
        path.AddPath(&arcPath, FALSE);
        return;
    }

    // Define a bounding rectangle for the ellipse
    Gdiplus::RectF arcRect(cx - rx, cy - ry, rx * 2, ry * 2);

    // Apply rotation and add the arc
    path.AddArc(arcRect, startAngle, sweepAngle);
}