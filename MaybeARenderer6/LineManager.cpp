#include "LineManager.h"
#include "MyFunctions.h"
#include "Constants.h"
#include "InputFields.h"

LineManager::LineManager() = default;

void LineManager::addLine(Point& P, Point& Q) {
    lines.insert({ P, Q, lastColorFromInputs });
}

void LineManager::removeLine(Point& P, Point& Q) {
    lines.erase(Line{ P, Q });
}

void LineManager::removeLinesWithPoint(Point& P) {
    for (auto it = lines.begin(); it != lines.end();) {
        if (it->p1 == P || it->p2 == P) {
            it = lines.erase(it);
        }
        else {
            ++it;
        }
    }
}

void LineManager::DrawLines(HDC& targetHDC) {
    for (const auto& l : lines) {
        DrawLine(targetHDC, l, LineBoldness, l.color);
    }
}

void LineManager::EachFrame(HDC& targetHDC) {
    DrawLines(targetHDC);
}

void LineManager::Flush() {
    lines.clear();
}