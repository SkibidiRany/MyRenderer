#pragma once
#include "Utilities.h"
#include <vector>
#include <stdexcept>
using std::vector;

class Shape {
protected:
    vector<Point> Points;
    vector<Point> OriginalPoints; // Store the original positions

public:
    Shape(vector<Point> Ps) : Points(Ps), OriginalPoints(Ps) {} // Copy original points

    void RotatePoints(double Rx[3][3], double Ry[3][3], double Rz[3][3]) {
        Points = OriginalPoints; // Reset to original positions before rotating
        for (Point& p : Points) {
            p = RotatePointAround(p, pivot, Rx);
            p = RotatePointAround(p, pivot, Ry);
            p = RotatePointAround(p, pivot, Rz);
        }
    }


    virtual void DrawLines(HDC hdc) const = 0;

    void DrawPoints(HDC hdc) const {
        if (hdc == NULL) throw std::runtime_error("Device context not initialized");
        for (const Point& p : Points) {
            DrawBoldPoint(hdc, p.x, p.y, PointBoldness);
        }
    }


    vector<Point> GetPoints() const {
        return Points;
    }
};

class Rect3D : public Shape {
protected:
    int width;
    int height;
    int depth;
public:
    Rect3D(int w, int h, int d)
        : Shape({
            { Middle.x - w / 2, Middle.y - h / 2, Middle.z + d / 2 },
            { Middle.x + w / 2, Middle.y - h / 2, Middle.z + d / 2 },
            { Middle.x + w / 2, Middle.y + h / 2, Middle.z + d / 2 },
            { Middle.x - w / 2, Middle.y + h / 2, Middle.z + d / 2 },
            { Middle.x - w / 2, Middle.y - h / 2, Middle.z - d / 2 },
            { Middle.x + w / 2, Middle.y - h / 2, Middle.z - d / 2 },
            { Middle.x + w / 2, Middle.y + h / 2, Middle.z - d / 2 },
            { Middle.x - w / 2, Middle.y + h / 2, Middle.z - d / 2 }
            }), width(w), height(h), depth(d) {}

    void DrawLines(HDC hdc) const override {
        if (hdc == NULL) throw std::runtime_error("Device context not initialized");
        // Draw lines for the top face
        DrawLine(hdc, Points[0], Points[1], LineBoldness);
        DrawLine(hdc, Points[1], Points[2], LineBoldness);
        DrawLine(hdc, Points[2], Points[3], LineBoldness);
        DrawLine(hdc, Points[3], Points[0], LineBoldness);

        // Draw lines for the bottom face
        DrawLine(hdc, Points[4], Points[5], LineBoldness);
        DrawLine(hdc, Points[5], Points[6], LineBoldness);
        DrawLine(hdc, Points[6], Points[7], LineBoldness);
        DrawLine(hdc, Points[7], Points[4], LineBoldness);

        // Draw vertical lines connecting top and bottom faces
        DrawLine(hdc, Points[0], Points[4], LineBoldness);
        DrawLine(hdc, Points[1], Points[5], LineBoldness);
        DrawLine(hdc, Points[2], Points[6], LineBoldness);
        DrawLine(hdc, Points[3], Points[7], LineBoldness);
    }
};

class Pyramid3D : public Shape {
protected:
    int sideLength;
public:
    Pyramid3D(int l)
        : Shape({
            { Middle.x, Middle.y, Middle.z + (int)(sqrt(2) / sqrt(3) * l) },
            { Middle.x + l / 2, Middle.y - (int)(sqrt(3) / 6 * l), Middle.z - (int)(sqrt(6) / 6 * l) },
            { Middle.x - l / 2, Middle.y - (int)(sqrt(3) / 6 * l), Middle.z - (int)(sqrt(6) / 6 * l) },
            { Middle.x, Middle.y + (int)(sqrt(3) / 3 * l), Middle.z - (int)(sqrt(6) / 6 * l) }
            }), sideLength(l) {}

    void DrawLines(HDC hdc) const override {
        if (hdc == NULL) throw std::runtime_error("Device context not initialized");
        
        // Draw lines for the base
        DrawLine(hdc, Points[1], Points[2], LineBoldness);
        DrawLine(hdc, Points[2], Points[3], LineBoldness);
        DrawLine(hdc, Points[3], Points[1], LineBoldness);
        
        // Draw Lines for head vertex
		DrawLine(hdc, Points[0], Points[1], LineBoldness);
		DrawLine(hdc, Points[0], Points[2], LineBoldness);
		DrawLine(hdc, Points[0], Points[3], LineBoldness);

    }
};
