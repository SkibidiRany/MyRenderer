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

void LineManager::OnMovePointDown(const Point& old) {
	oldPoint = old; // Store the old point for later use
}

void LineManager::OnMovePointUp()
{
    Point newP = GetCursPos(window_handle);
    OnMovePoint(oldPoint, newP);
}

void LineManager::OnMovePoint(const Point& oldP, const Point& newP) {

    auto linesToUpdate = getLinesWithPoint(oldP);

    // Remove lines connected to the old point
    removeLinesWithPoint(oldP);

    // Add lines with the new point
    for (auto& line : linesToUpdate) {
        if (line.p1 == oldP) line.p1 = newP;
        if (line.p2 == oldP) line.p2 = newP;
        addLine(line);  // Reinsert with updated points
    }

}

void LineManager::MoveLinesWithPoint(Point old, Point pos)
{
    for (const Line& l : lines) {
        if (l.p1 ==  old) {
            removeLine({ l.p1, l.p2 });
            addLine({ pos, l.p2 });
		}
		else if (l.p2 == old) {
			removeLine({l.p1, l.p2});
			addLine({l.p1, pos});
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