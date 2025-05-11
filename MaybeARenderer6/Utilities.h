#pragma once
#define NOMINMAX
#include <windows.h>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>    
#include <queue>
#include <format>
#include <string>

#include "Constants.h"
#include "InputFields.h"
#include "PointManager.h"
#include "LineManager.h"

using std::vector;
using std::string;

class PointManager;
class LineManager;

PointManager* PointsToDraw;
LineManager* LinesToDraw;

InputFieldsManager* inputFieldsManager;


















