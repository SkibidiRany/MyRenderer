#pragma once
#include <vector>
#include <stdexcept>

#include "Constants.h"
#include "MyClasses.h"
#include "DrawingFunctions.h"
#include "BaseFunctions.h"
#include "InputFields.h"


using std::vector;

extern struct Point pivot;
extern struct Point Middle;

class Shape{
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

    virtual void DrawPoints(HDC hdc) const {
        if (hdc == NULL) throw std::runtime_error("Device context not initialized");
        for (const Point& p : Points) {
            DrawBoldPoint(hdc, p, PointBoldness, p.color);
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
        DrawLine(hdc, Line(Points[0], Points[1]), LineBoldness);
        DrawLine(hdc, Line(Points[1], Points[2]), LineBoldness);
        DrawLine(hdc, Line(Points[2], Points[3]), LineBoldness);
        DrawLine(hdc, Line(Points[3], Points[0]), LineBoldness);

        // Draw lines for the bottom face
        DrawLine(hdc, Line(Points[4], Points[5]), LineBoldness);
        DrawLine(hdc, Line(Points[5], Points[6]), LineBoldness);
        DrawLine(hdc, Line(Points[6], Points[7]), LineBoldness);
        DrawLine(hdc, Line(Points[7], Points[4]), LineBoldness);

        // Draw vertical lines connecting top and bottom faces
        DrawLine(hdc, Line(Points[0], Points[4]), LineBoldness);
        DrawLine(hdc, Line(Points[1], Points[5]), LineBoldness);
        DrawLine(hdc, Line(Points[2], Points[6]), LineBoldness);
        DrawLine(hdc, Line(Points[3], Points[7]), LineBoldness);

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
        DrawLine(hdc, Line(Points[1], Points[2]), LineBoldness);
        DrawLine(hdc, Line(Points[2], Points[3]), LineBoldness);
        DrawLine(hdc, Line(Points[3], Points[1]), LineBoldness);

        // Draw lines for head vertex
        DrawLine(hdc, Line(Points[0], Points[1]), LineBoldness);
        DrawLine(hdc, Line(Points[0], Points[2]), LineBoldness);
        DrawLine(hdc, Line(Points[0], Points[3]), LineBoldness);


    }
};


class Heart3d : public Shape {
protected:
    int _width;
    int _height;
    int _depth;
    
    enum {
		A, B, C, D, E, F, G, H
    };
public:
    Heart3d(int width, int height, int depth)
        : Shape({
            {Middle.x, Middle.y, Middle.z - depth / 2},  // A 
            {Middle.x, Middle.y, Middle.z + depth / 2},   // B
            {Middle.x, Middle.y - height / 4, Middle.z}, // C
            {Middle.x + width / 4, Middle.y - height / 2, Middle.z}, // D
            {Middle.x + width / 2, Middle.y - height / 4, Middle.z}, // E
            {Middle.x, Middle.y + height / 2, Middle.z},   // F
            {Middle.x - width / 2, Middle.y - height / 4, Middle.z}, // G
            {Middle.x - width / 4, Middle.y - height / 2, Middle.z}  // H
            }), _width(width), _height(height), _depth(depth) {}

    void DrawLines(HDC hdc) const override {
        if (hdc == NULL) throw std::runtime_error("Device context not initialized");

        // Draw lines from points A and B to points C, D, E, F, G, H
        DrawLine(hdc, Line(Points[A], Points[C]), LineBoldness);
        DrawLine(hdc, Line(Points[A], Points[D]), LineBoldness);
        DrawLine(hdc, Line(Points[A], Points[E]), LineBoldness);
        DrawLine(hdc, Line(Points[A], Points[F]), LineBoldness);
        DrawLine(hdc, Line(Points[A], Points[G]), LineBoldness);
        DrawLine(hdc, Line(Points[A], Points[H]), LineBoldness);

        DrawLine(hdc, Line(Points[B], Points[C]), LineBoldness);
        DrawLine(hdc, Line(Points[B], Points[D]), LineBoldness);
        DrawLine(hdc, Line(Points[B], Points[E]), LineBoldness);
        DrawLine(hdc, Line(Points[B], Points[F]), LineBoldness);
        DrawLine(hdc, Line(Points[B], Points[G]), LineBoldness);
        DrawLine(hdc, Line(Points[B], Points[H]), LineBoldness);

        // Draw lines between the remaining specified points
        DrawLine(hdc, Line(Points[C], Points[D]), LineBoldness);
        DrawLine(hdc, Line(Points[D], Points[E]), LineBoldness);
        DrawLine(hdc, Line(Points[E], Points[F]), LineBoldness);
        DrawLine(hdc, Line(Points[F], Points[G]), LineBoldness);
        DrawLine(hdc, Line(Points[G], Points[H]), LineBoldness);
        DrawLine(hdc, Line(Points[H], Points[C]), LineBoldness);

    }
};


class DoublePyramid3D : public Shape {

public:
    DoublePyramid3D(int l) : Shape(
        {
            { Middle.x + l / 2, Middle.y - (int)(sqrt(3) / 6 * l), Middle.z}, // right down base vertex
            { Middle.x - l / 2, Middle.y - (int)(sqrt(3) / 6 * l), Middle.z}, // left down base vertex
			{ Middle.x, Middle.y + (int)(sqrt(3) / 3 * l), Middle.z }, // top base vertex
	        { Middle.x, Middle.y, Middle.z - (int)(sqrt(6) / 3 * l) }, // behind head vertex
			{ Middle.x, Middle.y, Middle.z + (int)(sqrt(6) / 3 * l)} // front head vertex
        }
    ) {  }



	void DrawPoints(HDC hdc) const override {
		if (hdc == NULL) throw std::runtime_error("Device context not initialized");
        
        for (int i = 0; i < 3; i++) 
            DrawBoldPoint(hdc, Points[i], PointBoldness, MyColors.RED);
		
		for (int i = 3; i < 5; i++) 
			DrawBoldPoint(hdc, Points[i], PointBoldness, MyColors.GREEN);
	}


	void DrawLines(HDC hdc) const override {
		if (hdc == NULL) throw std::runtime_error("Device context not initialized");

        // Draw lines for the base
        DrawLine(hdc, Line(Points[0], Points[1]), LineBoldness);
        DrawLine(hdc, Line(Points[1], Points[2]), LineBoldness);
        DrawLine(hdc, Line(Points[2], Points[0]), LineBoldness);

        // Draw lines for behind head vertex
        DrawLine(hdc, Line(Points[3], Points[0]), LineBoldness);
        DrawLine(hdc, Line(Points[3], Points[1]), LineBoldness);
        DrawLine(hdc, Line(Points[3], Points[2]), LineBoldness);

        // Draw lines for front head vertex
        DrawLine(hdc, Line(Points[4], Points[0]), LineBoldness);
        DrawLine(hdc, Line(Points[4], Points[1]), LineBoldness);
        DrawLine(hdc, Line(Points[4], Points[2]), LineBoldness);

    }



};


class PolygonWith2Heads3D : public Shape {
protected: 
	COLORREF BASE_COLOR = MyColors.RED;
	COLORREF HEAD_COLOR = MyColors.GREEN;
    COLORREF LINE_COLOR = MyColors.BLUE;
private:
    std::vector<Point> GeneratePolygonVertices(int n, double r, double h) {
        std::vector<Point> vertices;
        double angleStep = 2 * PI / n;

        // Generate base vertices
        for (int i = 0; i < n; ++i) {
            double angle = i * angleStep;
            int x = r * cos(angle);
            int y = r * sin(angle);
            vertices.push_back({ Middle.x + x, Middle.y + y, Middle.z });
        }

        // Add front and back vertices
        vertices.push_back({ Middle.x, Middle.y, Middle.z + (int)(h) });  // Front vertex
        vertices.push_back({ Middle.x, Middle.y, Middle.z + (int)(-h) }); // Back vertex

        return vertices;
    }

public:
    PolygonWith2Heads3D(int numOfVertices, int radius, int height)
        : Shape(GeneratePolygonVertices(numOfVertices, radius, height)) { }

    void DrawLines(HDC hdc) const override {
        if (hdc == NULL) throw std::runtime_error("Device context not initialized");

        int n = Points.size() - 2; // Number of base vertices

        // Draw lines for the base
        for (int i = 0; i < n; ++i) {
            DrawLine(hdc, Line(Points[i], Points[(i + 1) % n]), LineBoldness, LINE_COLOR);
        }

        // Draw lines for the front vertex
        for (int i = 0; i < n; ++i) {
            DrawLine(hdc, Line(Points[i], Points[n]), LineBoldness, LINE_COLOR);
        }

        // Draw lines for the back vertex
        for (int i = 0; i < n; ++i) {
            DrawLine(hdc, Line(Points[i], Points[n + 1]), LineBoldness, LINE_COLOR);
        }

    }

    void DrawPoints(HDC hdc) const override {
        if (hdc == NULL) throw std::runtime_error("Device context not initialized");

        int n = Points.size() - 2; // Number of base vertices

        // Draw base vertices in the desired color
        for (int i = 0; i < n; ++i) {
            DrawBoldPoint(hdc, Points[i], PointBoldness, BASE_COLOR);
        }

        // Draw front and back vertices in another color
        DrawBoldPoint(hdc, Points[n], PointBoldness, HEAD_COLOR);
        DrawBoldPoint(hdc, Points[n + 1], PointBoldness, HEAD_COLOR);
    }
};


Shape* GetWantedDrawing(Drawings WantedDrawing) {
    switch (WantedDrawing) {
    case Rect:
        return new Rect3D(200, 200, 200);
        break;
    case Pyramid:
        return new Pyramid3D(200);
        break;
    case DoublePyramid:
        return new DoublePyramid3D(200);
        break;
    case PolygonWith2Heads:
        return new PolygonWith2Heads3D(8, 100, 120);
        break;
    case Heart:
		return new Heart3d(400, 400, 150);
    default:
        return NULL;
    }
}



class ShapeManager : public IDrawEachFrame, public IFlushable {
private:
	// Rotation settings
    const bool RotateAxis[3] = {false, true, false};
	const int X = 0, Y = 1, Z = 2; // Axis indices

    enum Mode {
        InputAngle,
        InputAngleChangeSpeed,
		Manual
	};

	Mode myRotationMode = Manual;

    bool isRotating = false;
    int lastMouseX = 0;
    int lastMouseY = 0;
    float yaw = 0.0f;
    float pitch = 0.0f;

    // Auto settings
    double angle = 0;
    double angleChangeSpeed = 0.001;
    const bool AutoAngleChange = true;
    const bool AutoRotate = true;

    Shape* shape = nullptr;

    float Clamp(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    void MakeRotationMatrixX(float angleDeg, double matrix[3][3]) {
        float rad = angleDeg * 3.14159265f / 180.0f;
        matrix[0][0] = 1; matrix[0][1] = 0;           matrix[0][2] = 0;
        matrix[1][0] = 0; matrix[1][1] = cos(rad);    matrix[1][2] = -sin(rad);
        matrix[2][0] = 0; matrix[2][1] = sin(rad);    matrix[2][2] = cos(rad);
    }

    void MakeRotationMatrixY(float angleDeg, double matrix[3][3]) {
        float rad = angleDeg * 3.14159265f / 180.0f;
        matrix[0][0] = cos(rad);  matrix[0][1] = 0; matrix[0][2] = sin(rad);
        matrix[1][0] = 0;         matrix[1][1] = 1; matrix[1][2] = 0;
        matrix[2][0] = -sin(rad); matrix[2][1] = 0; matrix[2][2] = cos(rad);
    }

public:


    ShapeManager() {
        SetShape(GetWantedDrawing(WantedDrawing));
    }

    void SetShape(Shape* s) {
        if (!shape) {
            shape = s;
            return;
        }

        free(shape);
        shape = s;
    }

    void OnRightMouseDown(int x, int y) {
            isRotating = true;
            lastMouseX = x;
            lastMouseY = y;
    }

    void OnRightMouseUp() {
        isRotating = false;
    }

    void OnRightMouseMove(int x, int y) {
        if (!isRotating || !shape) return;

        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        float sensitivity = 0.5f;
        yaw += dx * sensitivity;
        pitch += dy * sensitivity;

        pitch = Clamp(pitch, -89.0f, 89.0f);

        lastMouseX = x;
        lastMouseY = y;
    }

    void EachFrame(HDC& targethdc) override {
        if (!shape) return;


        double rotX[3][3];
        double rotY[3][3];
        double rotZ[3][3] = {
            {1, 0, 0}, // Identity matrix for Z � no roll
            {0, 1, 0},
            {0, 0, 1}
        };


        switch (myRotationMode) {
		case Mode::InputAngle:
			if (InputFieldsManager::g_angleChanged) {
				angle = GetAngleFromInputs(rgb_window_handle);
				InputFieldsManager::g_angleChanged = false;
			}

			break;
        case Mode::InputAngleChangeSpeed:
            if (InputFieldsManager::g_angleSpeedChanged) {
                angleChangeSpeed = GetAngleChangeSpeedFromInputs(rgb_window_handle);
                InputFieldsManager::g_angleSpeedChanged = false;
            }
            angle += angleChangeSpeed;

            break;
        case Manual:
            if (isRotating) {
                Point p = GetCursPos(window_handle);
                OnRightMouseMove(p.x, p.y);
            }
            break;
        }
		
        if (myRotationMode != Manual) {
            if (RotateAxis[X]) pitch = angle;
            if (RotateAxis[Y]) yaw = angle;
        }
        MakeRotationMatrixX(pitch, rotX);
        MakeRotationMatrixY(yaw, rotY);

        shape->RotatePoints(rotX, rotY, rotZ);
        shape->DrawLines(targethdc);
        shape->DrawPoints(targethdc);
    }

    void Flush() override {
        if (shape) {
            delete shape;
            shape = nullptr;
        }
    }
};




ShapeManager* shapeManager = new ShapeManager();


void OnRightMouseDown(HWND hwnd) {
	Point cursPos = GetCursPos(hwnd);
    shapeManager->OnRightMouseDown(cursPos.x, cursPos.y);
}



void OnRightMouseUp() {
    shapeManager->OnRightMouseUp();
}

