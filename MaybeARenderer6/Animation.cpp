#include "Animation.h"
#include <algorithm>

Animation::Animation(float dur)
    : duration(dur), progress(0.0f), isRunning(false), elapsedTime(0.0f) {}

Animation::~Animation() {
    Stop();
}

void Animation::Start() {
    if (isRunning) return;
    isRunning = true;
    elapsedTime = 0.0f;
    progress = 0.0f;
}

void Animation::Stop() {
    isRunning = false;
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
    progress = std::min(elapsedTime / duration, 1.0f);

    if (elapsedTime >= duration) {
        isRunning = false;
    }
}