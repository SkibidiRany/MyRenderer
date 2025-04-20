#pragma once
#define NOMINMAX
#include <windows.h>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>    
#include <queue>
#include "InputFields.h"
#include <thread>
#include <condition_variable>
#include <locale>
#include <memory>
#include <format>

using std::vector;
class PointManager;
class LineManager;

#define X 0
#define Y 1
#define Z 2

const bool ToRotate[] = { false, true, false };

PointManager* PointsToDraw;
LineManager* LinesToDraw;



COLORREF RED = RGB(255, 0, 0);
COLORREF GREEN = RGB(0, 255, 0);
COLORREF BLUE = RGB(0, 0, 255);
COLORREF WHITE = RGB(255, 255, 255);

HWND window_handle;
HWND rgb_window_handle;

const int screenWidth = 800;
const int screenHeight = 600;

const float PI = 3.14159265358979323846f;
bool quit = false;
const int PointBoldness = 10;
const int LineBoldness = 1;


double angle = 0;
double angleChangeSpeed = 0.01;
const bool AutoAngleChangeSpeed = false; // if false, the angle change speed will be set to the value in the input field

const bool ToDrawShape = true;
const bool AutoRotate = true;

const int drawingCapacity = 30;





// Enum for Drawings
enum Drawings {
    Rect,
    Pyramid,
    DoublePyramid,
    PolygonWith2Heads,
    Heart
};

Drawings WantedDrawing = Heart;




// Point Struct
struct Point {
    int x = 0;
    int y = 0;
    int z = 0;

    COLORREF color = WHITE;

	Point() : x(0), y(0), z(0), color(WHITE) {} // Default constructor
    // A constructor for implicit conversions
    Point(int _d) : x(_d), y(_d), z(_d), color(WHITE) {} 

	// default / custom constructor
    Point(int _x, int _y, int _z = 0, COLORREF col = WHITE)
        : x(_x), y(_y), z(_z), color(col) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    // += operator: adds x/y/z only (keeps first color)
    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    // -= operator: subtracts x/y/z only (keeps first color)
    Point& operator-=(const Point& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
};

// Hash for Point
struct PointHash {
    size_t operator()(const Point& p) const {
        return std::hash<int>()(p.x) ^ std::hash<int>()(p.y) ^ std::hash<int>()(p.z);
    }
};


double GetDistance(const Point& p1, const Point& p2) {
    double dx = std::abs(p1.x - p2.x);
    double dy = std::abs(p1.y - p2.y);
    double dz = std::abs(p1.z - p2.z);

    // Manual sort of dx, dy, dz to find max, mid, min
    double maxD = dx, midD = dy, minD = dz;

    if (maxD < midD) std::swap(maxD, midD);
    if (midD < minD) std::swap(midD, minD);
    if (maxD < midD) std::swap(maxD, midD);

    return 0.939 * maxD + 0.389 * midD + 0.298 * minD;
}




// Line Struct
struct Line {
    Point p1 = {}, p2 = {}, Middle = {};
    const char* text = "";
    COLORREF color = WHITE;
    Line(const Point& a, const Point& b, COLORREF col = WHITE, const char* t = "") : p1(a), p2(b), color(col),
        Middle({ (p1.x + p2.x) / 2 ,
                 (p1.y + p2.y) / 2 ,
                 (p1.z + p2.z) / 2 , col }),
        text(""/*(GetDistance(a, b))*/) {}


    bool operator==(const Line& other) const {
        return (p1 == other.p1 && p2 == other.p2) || (p1 == other.p2 && p2 == other.p1);
    }
};

// Hash for Line
struct LineHash {
    size_t operator()(const Line& line) const {
        return PointHash{}(line.p1) ^ PointHash {}(line.p2);
    }
};

Point Middle = { screenWidth / 2, screenHeight / 2, 0 };
Point pivot = { Middle.x , Middle.y, Middle.z };
Point LastCursPos = { 0, 0, 0 };

// Function Declarations
void DrawBoldPoint(HDC hdc, int x, int y, int boldness, COLORREF color = WHITE);
void DrawLine(HDC& hdc, const Line& line, const int boldness, const COLORREF& color = WHITE);
Point MultiplyMatrixByPoint(const double matrix[3][3], const Point& p);
Point RotatePointAround(Point p, Point pivot, double matrix[3][3]);
bool PositionIsLegal(Point p);
void RemovePoint(Point p);
void OnLeftMouseDown(HWND hwnd);
void OnLeftMouseHold(HWND hwnd, HDC hdc);
void OnLeftMouseUp(HWND hwnd);







// PointManager Class with stable multithreaded drawing
class PointManager {
private:
    std::unordered_map<Point, std::vector<Point>, PointHash> buckets;
    std::unordered_set<Point, PointHash> points;
    std::queue<Point> insertionOrder;
    int _capacity;
    int _c;

    // Thread pool and resources
    int numThreads;
    std::vector<std::thread> workerThreads;
    std::vector<HDC> threadHDCs;
    std::vector<HBITMAP> threadBitmaps;
    std::vector<HBITMAP> oldBitmaps;
    bool threadsInitialized = false;

    // Synchronization - using raw pointers to avoid copy issues with atomics
    std::atomic<bool> shouldTerminate{ false };
    std::vector<std::atomic<bool>*> frameReady;
    std::vector<std::atomic<bool>*> threadDone;
    std::condition_variable cv;
    std::mutex mutex;

    // Safety flag to prevent multiple simultaneous Draw calls
    std::atomic<bool> currentlyDrawing{ false };

    // Initialize thread resources
    void InitializeThreadResources(HDC targetHDC) {
        RECT rect;
        GetClientRect(WindowFromDC(targetHDC), &rect);
        int width = rect.right;
        int height = rect.bottom;

        threadHDCs.resize(numThreads);
        threadBitmaps.resize(numThreads);
        oldBitmaps.resize(numThreads);

        for (int i = 0; i < numThreads; i++) {
            threadHDCs[i] = CreateCompatibleDC(targetHDC);
            if (!threadHDCs[i]) continue; // Skip if creation failed

            threadBitmaps[i] = CreateCompatibleBitmap(targetHDC, width, height);
            if (!threadBitmaps[i]) {
                DeleteDC(threadHDCs[i]);
                threadHDCs[i] = nullptr;
                continue;
            }

            oldBitmaps[i] = (HBITMAP)SelectObject(threadHDCs[i], threadBitmaps[i]);
        }
    }

    // Clean up thread resources
    void CleanupThreadResources() {
        for (int i = 0; i < numThreads; i++) {
            if (threadHDCs[i]) {
                SelectObject(threadHDCs[i], oldBitmaps[i]);
                DeleteObject(threadBitmaps[i]);
                DeleteDC(threadHDCs[i]);
                threadHDCs[i] = nullptr;
            }
        }

        // Clean up the atomic pointers
        for (auto& ptr : frameReady) {
            delete ptr;
        }
        for (auto& ptr : threadDone) {
            delete ptr;
        }

        frameReady.clear();
        threadDone.clear();
        threadHDCs.clear();
        threadBitmaps.clear();
        oldBitmaps.clear();
    }

    // Worker thread function
    void WorkerThread(int threadId) {
        while (!shouldTerminate) {
            // Wait for work
            {
                std::unique_lock<std::mutex> lock(mutex);
                cv.wait(lock, [this, threadId]() {
                    return (frameReady[threadId] && *frameReady[threadId]) || shouldTerminate;
                    });

                if (shouldTerminate) break;
            }

            // Skip if we don't have valid DC
            if (!threadHDCs[threadId]) {
                *frameReady[threadId] = false;
                *threadDone[threadId] = true;
                cv.notify_one();
                continue;
            }

            // Get screen dimensions safely
            BITMAP bm;
            if (GetObject(threadBitmaps[threadId], sizeof(BITMAP), &bm) == 0) {
                *frameReady[threadId] = false;
                *threadDone[threadId] = true;
                cv.notify_one();
                continue;
            }

            int width = bm.bmWidth;
            int height = bm.bmHeight;

            // Clear bitmap
            RECT rc = { 0, 0, width, height };
            FillRect(threadHDCs[threadId], &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

            // Safe copy of points to avoid any race conditions
            std::vector<Point> pointsVector;
            {
                std::unique_lock<std::mutex> lock(mutex);
                pointsVector.assign(points.begin(), points.end());
            }

            // Calculate which points to process (divide points evenly among threads)
            int pointsPerThread = (pointsVector.size() + numThreads - 1) / numThreads;
            int startIdx = threadId * pointsPerThread;
            int endIdx = startIdx + pointsPerThread;

            // Prevent out of bounds access
            if (endIdx > pointsVector.size()) {
                endIdx = pointsVector.size();
            }

            // Draw assigned points
            for (int i = startIdx; i < endIdx && i < pointsVector.size(); i++) {
                const Point& p = pointsVector[i];
                DrawBoldPoint(threadHDCs[threadId], p.x, p.y, PointBoldness, p.color);
            }

            // Mark thread as done
            *frameReady[threadId] = false;
            *threadDone[threadId] = true;
            cv.notify_one(); // Notify main thread
        }
    }

    // Initialize thread pool
    void InitializeThreadPool(HDC targetHDC) {
        try {
            // Set up synchronization primitives
            frameReady.resize(numThreads, nullptr);
            threadDone.resize(numThreads, nullptr);

            for (int i = 0; i < numThreads; i++) {
                frameReady[i] = new std::atomic<bool>(false);
                threadDone[i] = new std::atomic<bool>(true);
            }

            // Create resources for each thread
            InitializeThreadResources(targetHDC);

            // Create worker threads
            for (int i = 0; i < numThreads; i++) {
                workerThreads.push_back(std::thread(&PointManager::WorkerThread, this, i));
            }

            threadsInitialized = true;
        }
        catch (const std::exception& e) {
            // Handle exceptions during initialization
            CleanupThreadResources();
            threadsInitialized = false;
            OutputDebugStringA(e.what());
        }
    }

public:
    Point LastDrawn = { 0, 0, 0 };

    // Constructor
    PointManager(int cap, int c, int threadCount = 8) :
        _capacity(cap), _c(c), numThreads(threadCount) {
        // Limit thread count to hardware concurrency
        int maxThreads = std::thread::hardware_concurrency();
        if (maxThreads > 0 && numThreads > maxThreads) {
            numThreads = maxThreads;
        }

        // Ensure at least one thread
        if (numThreads < 1) numThreads = 1;
    }

    // Destructor to clean up threads
    ~PointManager() {
        if (threadsInitialized) {
            shouldTerminate = true;

            // Signal all threads to terminate
            for (int i = 0; i < numThreads; i++) {
                if (frameReady[i]) *frameReady[i] = true;
            }
            cv.notify_all();

            // Join all threads
            for (auto& thread : workerThreads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }

            CleanupThreadResources();
        }
    }

    Point ToBucket(Point p) {
        int bucketX = p.x / _c;
        int bucketY = p.y / _c;
        int bucketZ = p.z / _c;
        return { bucketX, bucketY };
    }

    Point insert(Point p) {
        std::unique_lock<std::mutex> lock(mutex); // Lock during modification

        Point checker = CheckIntersection(p);
        LastDrawn = checker;
        if (checker != p) return checker; // if we found a point checker intersecting with p, return it 

        // if we didn't find such point, add p
        if (points.size() >= _capacity) {
            Point oldest = insertionOrder.front();
            insertionOrder.pop();
            RemovePoint(oldest);
        }

        p.color = GetColorFromInputs(rgb_window_handle);
        points.insert(p);
        insertionOrder.push(p);
        buckets[ToBucket(p)].push_back(p);
        return p;
    }

    void remove(Point p) {
        // No need for mutex here as this is called from insert which already has the lock
        points.erase(p);
        Point key = ToBucket(p);
        auto& vec = buckets[key];
        vec.erase(std::remove(vec.begin(), vec.end(), p), vec.end());
    }

    Point CheckIntersection(Point p) {
        // No need for mutex here as this is called from insert which already has the lock
        Point base = ToBucket(p);
        for (int dx = -1; dx <= 1; ++dx)
            for (int dy = -1; dy <= 1; ++dy)
                for (int dz = -1; dz <= 1; ++dz) {
                    Point neighbor = { base.x + dx, base.y + dy, base.z + dz };
                    auto it = buckets.find(neighbor);
                    if (it != buckets.end()) {
                        for (const Point& q : it->second) {
                            if (std::abs(q.x - p.x) <= _c &&
                                std::abs(q.y - p.y) <= _c &&
                                std::abs(q.z - p.z) <= _c) {
                                return q;  // Found nearby match
                            }
                        }
                    }
                }
        return p; // no match
    }

    // Safe direct drawing for small collections
    void DrawPointsDirect(HDC targetHDC) {
        std::unique_lock<std::mutex> lock(mutex);
        for (const auto& p : points) {
            DrawBoldPoint(targetHDC, p.x, p.y, PointBoldness, p.color);
        }
    }

    // Multithreaded drawing implementation with reentry protection
    void DrawPoints(HDC targetHDC) {
        // Only allow one thread to be drawing at a time
        if (currentlyDrawing.exchange(true)) {
            return; // Another thread is already drawing, skip this frame
        }

        // Guard for exception safety
        struct DrawGuard {
            std::atomic<bool>& flag;
            DrawGuard(std::atomic<bool>& f) : flag(f) {}
            ~DrawGuard() { flag = false; }
        } guard(currentlyDrawing);

        try {
            // Validate HDC
            if (!targetHDC) return;

            // Get the number of points (thread-safe)
            size_t numPoints;
            {
                std::unique_lock<std::mutex> lock(mutex);
                numPoints = points.size();
            }

            // For very small collections, just draw directly
            if (numPoints < 100) {
                DrawPointsDirect(targetHDC);
                return;
            }

            // Initialize thread pool if not already done
            if (!threadsInitialized) {
                InitializeThreadPool(targetHDC);
            }

            // Skip if threads failed to initialize
            if (!threadsInitialized) {
                DrawPointsDirect(targetHDC);
                return;
            }

            // Signal all threads to start working
            for (int i = 0; i < numThreads; i++) {
                *threadDone[i] = false;
                *frameReady[i] = true;
            }
            cv.notify_all();

            // Wait for all threads to complete (with timeout protection)
            bool allDone = false;
            {
                std::unique_lock<std::mutex> lock(mutex);
                allDone = cv.wait_for(lock, std::chrono::milliseconds(100), [this]() {
                    for (int i = 0; i < numThreads; i++) {
                        if (!*threadDone[i]) return false;
                    }
                    return true;
                    });
            }

            // If we timed out, some threads might still be working
            if (!allDone) {
                DrawPointsDirect(targetHDC);
                return;
            }

            // Get client dimensions
            RECT rect;
            if (!GetClientRect(WindowFromDC(targetHDC), &rect)) {
                DrawPointsDirect(targetHDC);
                return;
            }

            int width = rect.right;
            int height = rect.bottom;

            // Create temporary DC for blending
            HDC tempDC = CreateCompatibleDC(targetHDC);
            if (!tempDC) {
                DrawPointsDirect(targetHDC);
                return;
            }

            HBITMAP tempBitmap = CreateCompatibleBitmap(targetHDC, width, height);
            if (!tempBitmap) {
                DeleteDC(tempDC);
                DrawPointsDirect(targetHDC);
                return;
            }

            HBITMAP oldTempBitmap = (HBITMAP)SelectObject(tempDC, tempBitmap);

            // Clear the temp bitmap
            RECT rc = { 0, 0, width, height };
            FillRect(tempDC, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

            // Blend all thread bitmaps
            for (int i = 0; i < numThreads; i++) {
                if (threadHDCs[i]) {
                    BitBlt(tempDC, 0, 0, width, height, threadHDCs[i], 0, 0, SRCPAINT);
                }
            }

            // Copy to target
            BitBlt(targetHDC, 0, 0, width, height, tempDC, 0, 0, SRCCOPY);

            // Clean up temp resources
            SelectObject(tempDC, oldTempBitmap);
            DeleteObject(tempBitmap);
            DeleteDC(tempDC);
        }
        catch (const std::exception& e) {
            OutputDebugStringA(e.what());
            // If multithreaded drawing fails, fall back to direct drawing
            DrawPointsDirect(targetHDC);
        }
    }
};



// LineManager Class
class LineManager {
private:
    std::unordered_set<Line, LineHash> lines;

    PointManager* pointManager;

public:
    LineManager(PointManager* pm) : pointManager(pm) {}

    void addLine(Point P, Point Q) {
        Point intersectingPointP = pointManager->CheckIntersection(P);
        Point intersectingPointQ = pointManager->CheckIntersection(Q);

		if (intersectingPointP == intersectingPointQ) { // if they are the same point, we don't need to add a line
			return;
		}

        
        lines.insert(Line(intersectingPointP, intersectingPointQ, GetColorFromInputs(rgb_window_handle))); // else add needed line with wanted color        
    }
    void removeLine(Point P, Point Q) {
        lines.erase(Line(P, Q));
    }
    void removeLinesWithPoint(Point P) {
        for (auto it = lines.begin(); it != lines.end();) {
            if (it->p1 == P || it->p2 == P) {
                it = lines.erase(it);
            }
            else {
                ++it;
            }
        }
    }
    void DrawLines(HDC hdc) {
        for (const auto& line : lines) {
            DrawLine(hdc, line, LineBoldness, line.color);
        }
    }
};


// Function Definitions
void DrawBoldPoint(HDC hdc, int x, int y, int boldness, COLORREF color) {
	if (!PositionIsLegal({ x, y })) return; // Check if the position is legal before drawing

    for (int i = -boldness; i <= boldness; ++i) {
        for (int j = -boldness; j <= boldness; ++j) {
            SetPixel(hdc, x + i, y + j, color);
        }
    }
}

void DrawTextAtMiddle(HDC hdc, Line line, const char* text, COLORREF color) {

    // Set the text color
    SetTextColor(hdc, color);

    // Get the size of the text to center it
    SIZE textSize;
    GetTextExtentPoint32A(hdc, text, strlen(text), &textSize);

    // Adjust the Y-coordinate to be above the line
    int textY = line.Middle.y - textSize.cy - 5; // 5 pixels above the midpoint

    // Draw the text centered over the midpoint
    int textX = line.Middle.x - (textSize.cx / 2);
    TextOutA(hdc, textX, textY, text, strlen(text));
}

void DrawLine(HDC& hdc, const Line& line, const int boldness, const COLORREF& color) {
	Point p1 = line.p1;
	Point p2 = line.p2;

    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int sx = (p1.x < p2.x) ? 1 : -1;
    int sy = (p1.y < p2.y) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (PositionIsLegal(p1)) {
            DrawBoldPoint(hdc, p1.x, p1.y, boldness, color);
        }
        if (p1.x == p2.x && p1.y == p2.y) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            p1.x += sx;
        }
        if (e2 < dx) {
            err += dx;
            p1.y += sy;
        }
    }

    // Draw text above the line at the midpoint
    DrawTextAtMiddle(hdc, line, line.text, color);
}

Point MultiplyMatrixByPoint(const double matrix[3][3], const Point& p) {
	if (!matrix) return p; // if the matrix is null, return the point as is
    Point result;
    result.x = (int)(matrix[0][0] * p.x + matrix[0][1] * p.y + matrix[0][2] * p.z);
    result.y = (int)(matrix[1][0] * p.x + matrix[1][1] * p.y + matrix[1][2] * p.z);
    result.z = (int)(matrix[2][0] * p.x + matrix[2][1] * p.y + matrix[2][2] * p.z);
	result.color = p.color; // Keep the color of the original point
    return result;
}

Point RotatePointAround(Point p, Point pivot, double matrix[3][3]) {
	p -= pivot; // Translate point to origin
    Point result = MultiplyMatrixByPoint(matrix, p);
	result += pivot; // Translate point back
    return result;
}


bool PositionIsLegal(Point p) {
	return (p.x >= 0 && p.x < screenWidth && p.y >= 0 && p.y < screenHeight);
}



COLORREF lastColorFromInputs = WHITE;
void OnLeftMouseDown(HWND hwnd) {
    POINT cursPos;
    GetCursorPos(&cursPos);
    ScreenToClient(hwnd, &cursPos);
    Point toAdd = { cursPos.x, cursPos.y };
    if(!PositionIsLegal(toAdd)) return;
	PointsToDraw->insert(toAdd);
	lastColorFromInputs = GetColorFromInputs(rgb_window_handle);
}




void OnLeftMouseHold(HWND hwnd, HDC hdc) {
	POINT cursPos;
	GetCursorPos(&cursPos);
	ScreenToClient(hwnd, &cursPos);
	LastCursPos = { cursPos.x, cursPos.y };
	if (!PositionIsLegal(LastCursPos)) return;
    DrawLine(hdc, Line({ LastCursPos, PointsToDraw->LastDrawn }), LineBoldness, lastColorFromInputs);

}

             
void OnLeftMouseUp(HWND hwnd) {
	Point intersectionChecker = PointsToDraw->CheckIntersection(LastCursPos);
    Point lastPointDrawn = PointsToDraw->LastDrawn;
    if (!PositionIsLegal(intersectionChecker)) return;
    LinesToDraw->addLine(lastPointDrawn, intersectionChecker);
    PointsToDraw->insert(LastCursPos);
}

void RemovePoint(Point p) {
    PointsToDraw->remove(p);
	LinesToDraw->removeLinesWithPoint(p);
}

// Initialize the PointManager and LineManager objects
void InitializeManagers() {
    PointsToDraw = new PointManager(drawingCapacity, 2* PointBoldness);
    LinesToDraw = new LineManager(PointsToDraw);
}


