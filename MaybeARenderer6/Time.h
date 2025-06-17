#pragma once

#ifndef TIME_H
#define TIME_H

#include <chrono>
#include "TickingManager.h"

class Time {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    std::chrono::high_resolution_clock::time_point currentFrameTime;
    
	TickingManager& tickingManager;
    

public:
    Time(TickingManager& manager);

    void Initialize();
    float GetDeltaTime() const;
    void Update();
    float ElapsedTime() const;
};

#endif // TIME_H