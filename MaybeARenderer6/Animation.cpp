#include "Animation.h"
#include <chrono>
#include <algorithm>

Animation::Animation(float dur)
    : duration(dur), isRunning(false), elapsedTime(0.0f) {}

Animation::~Animation() {
    if (animThread.joinable()) {
        animThread.join();
    }
}

void Animation::Start() {
    if (isRunning) return;

    isRunning = true;
    elapsedTime = 0.0f;

    animThread = std::thread([this]() {
        auto startTime = std::chrono::high_resolution_clock::now();

        while (isRunning && elapsedTime < duration) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();

            float progress = std::min(elapsedTime / duration, 1.0f);
            

            if (elapsedTime >= duration) {
                isRunning = false;
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        });
}

void Animation::Stop() {
    isRunning = false;
    if (animThread.joinable()) {
        animThread.join();
    }
}

bool Animation::IsFinished() const {
    return !isRunning && elapsedTime >= duration;
}

bool Animation::IsRunning() const {
    return isRunning;
}

void Animation::Update(float deltaTime) {
    if (!isRunning) return;

    elapsedTime += deltaTime;
    float progress = std::min(elapsedTime / duration, 1.0f);
    
    if (elapsedTime >= duration) {
        isRunning = false;
    }
}