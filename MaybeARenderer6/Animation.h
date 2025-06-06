#pragma once

#ifndef ANIMATION_H
#define ANIMATION_H
#define NOMINMAX

#include <functional>
#include <thread>
#include <Windows.h>


class Animation {
protected:
    float duration;
    float progress;
    bool isRunning;
    std::thread animThread;
    float elapsedTime;

public:
    Animation(float dur);
    ~Animation();

    void Start();
    void Stop();
    bool IsFinished() const;
    bool IsRunning() const;
    void Update(float deltaTime);

    virtual void DoAnimation(HDC hdc, float deltaTime) = 0;
};

#endif // ANIMATION_H