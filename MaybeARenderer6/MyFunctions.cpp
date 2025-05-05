#include "MyFunctions.h"
#include "Constants.h"
#include "MyClasses.h"
#include "PointManager.h"
#include "LineManager.h"
#include "Utilities.h"
using std::string;

void RemovePoint(Point p) {
    PointsToDraw->remove(p);
    LinesToDraw->removeLinesWithPoint(p);
}

void FlushScreen(PointManager* pM, LineManager* lM) {
    lM->Flush();
    pM->Flush();
}
