#include "Time.h"

void Time::Initialize() {
    auto now = std::chrono::high_resolution_clock::now();
    startTime = now;
    lastFrameTime = now;
    currentFrameTime = now;
}

float Time::GetDeltaTime() const {
    auto duration = currentFrameTime - lastFrameTime;
    return std::chrono::duration<float>(duration).count();
}

void Time::Update() {
    lastFrameTime = currentFrameTime;
    currentFrameTime = std::chrono::high_resolution_clock::now();
}

float Time::ElapsedTime() const {
    auto duration = currentFrameTime - startTime;
    return std::chrono::duration<float>(duration).count();
}