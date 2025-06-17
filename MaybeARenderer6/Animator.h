#pragma once

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <vector>
#include <memory>
#include <functional>
#include "Animation.h"
#include "ScheduledAnimator.h"


struct AnimationResources {
    HDC dc = nullptr;
    HBITMAP bmp = nullptr;
    HBITMAP oldBmp = nullptr;
    void* bits = nullptr;  // pointer to bitmap bits for clearing
};

class Animator : public ITick {
private:
    std::vector<std::shared_ptr<Animation>> animations;
    std::unordered_map<std::shared_ptr<Animation>, AnimationResources> animationCache;

	ScheduledAnimator& scheduler;
public:
    Animator(ScheduledAnimator& schedulerRef);

    void Add(std::shared_ptr<Animation> animation);
    void Tick(float deltaTime) override;

    void At(std::shared_ptr<Animation> anim, float time);
	void After(std::shared_ptr<Animation> anim, float time);

    size_t GetActiveAnimationCount() const;
    void Clear();

	HDC CallAnimations();
    void StartAnimations();
    void StopAnimations();
};

#endif // ANIMATOR_H