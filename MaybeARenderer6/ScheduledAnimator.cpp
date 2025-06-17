#include "ScheduledAnimator.h"

void ScheduledAnimator::Schedule(std::shared_ptr<Animation> anim, float time) {
    auto it = schedule.begin();
    while (it != schedule.end() && it->time <= time)
        ++it;
    schedule.insert(it, { time, anim });
}

const std::vector<std::shared_ptr<Animation>> ScheduledAnimator::CollectDueAnimations() {
    std::vector<std::shared_ptr<Animation>> due;

    while (!schedule.empty() && schedule.front().time <= currentTime) {
        due.push_back(schedule.front().anim);
        schedule.pop_front(); // Delete after collection
    }

    return due;
}

void ScheduledAnimator::Tick(float deltaTime) {
    currentTime += deltaTime;
}

void ScheduledAnimator::Clear() {
    schedule.clear();
    currentTime = 0.0f;
}
