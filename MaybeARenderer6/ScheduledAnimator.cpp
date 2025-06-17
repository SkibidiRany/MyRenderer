#include "ScheduledAnimator.h"

void ScheduledAnimator::At(std::shared_ptr<Animation> anim, float time) {
    auto it = schedule.begin();
    while (it != schedule.end() && it->time <= time)
        ++it;
    schedule.insert(it, { time, anim });
}

void ScheduledAnimator::After(std::shared_ptr<Animation> anim, float time)
{
	float targetTime = currentTime + time;
	At(anim, targetTime);
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
}
