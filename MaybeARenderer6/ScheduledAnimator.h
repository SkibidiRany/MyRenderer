#pragma once

#ifndef SCHEDULED_ANIMATOR_H
#define SCHEDULED_ANIMATOR_H

#include <list>
#include <memory>
#include "Animation.h"

class ScheduledAnimator {
private:
    struct ScheduledItem {
        float time;
        std::shared_ptr<Animation> anim;
    };

    std::list<ScheduledItem> schedule;
    float currentTime = 0.0f;

public:
    void Schedule(std::shared_ptr<Animation> anim, float time);
    const std::vector<std::shared_ptr<Animation>> CollectDueAnimations();
    void Tick(float deltaTime);

    void Clear();
};

#endif // SCHEDULED_ANIMATOR_H
