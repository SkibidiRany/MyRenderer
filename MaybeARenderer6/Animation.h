#pragma once

#ifndef ANIMATION_H
#define ANIMATION_H

#include <functional>
#include <thread>

class Animation {
private:
    std::function<void(float)> func;
    float duration;
    bool isRunning;
    std::thread animThread;
    float elapsedTime;

public:
    Animation(float dur, std::function<void(float)> f);
    ~Animation();

    void Start();
    void Stop();
    bool IsFinished() const;
    bool IsRunning() const;
    void Update(float deltaTime);
};

#endif // ANIMATION_H