#include "LineManager.h"
#include "Constants.h"
#include "InputFields.h"
#include "BaseFunctions.h"

LineManager::LineManager() = default;

void LineManager::addLine(Line l) {
    lines.insert(l);
}

void LineManager::removeLine(Line l) {
    lines.erase(l);
}

void LineManager::removeLinesWithPoint(const Point& P) {
    for (auto it = lines.begin(); it != lines.end();) {
        if (it->p1 == P || it->p2 == P) {
            it = lines.erase(it);
        }
        else {
            ++it;
        }
    }
}


std::vector<Line> LineManager::getLinesWithPoint(const Point& p) const {
    std::vector<Line> result;
    for (const Line& l : lines) {
        if (l.p1 == p || l.p2 == p) {
            result.push_back(l);
        }
    }
    return result; 
}



void LineManager::MoveLinesWithPoint(Point old, Point pos)
{
    auto linesToUpdate = getLinesWithPoint(old);

    // Remove lines connected to the old point
    removeLinesWithPoint(old);

    // Add lines with the new point
    for (auto& line : linesToUpdate) {
        if (line.p1 == old) line.p1 = pos;
        if (line.p2 == old) line.p2 = pos;
        addLine(line);  // Reinsert with updated points
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