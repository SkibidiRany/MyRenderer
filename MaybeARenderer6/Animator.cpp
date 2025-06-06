#include "Animator.h"
#include <algorithm>

void Animator::Add(std::shared_ptr<Animation> animation) {
    animations.push_back(animation);
}



void Animator::Update(float deltaTime) {
    // Update all running animations
    for (auto& anim : animations) {
        anim->Update(deltaTime);
    }

    // Remove finished animations
    animations.erase(
        std::remove_if(animations.begin(), animations.end(),
            [](const std::shared_ptr<Animation>& anim) {
                return anim->IsFinished();
            }),
        animations.end()
    );
}

size_t Animator::GetActiveAnimationCount() const {
    return std::count_if(animations.begin(), animations.end(),
        [](const std::shared_ptr<Animation>& anim) {
            return anim->IsRunning();
        });
}

void Animator::Clear() {
    for (auto& anim : animations) {
        anim->Stop();
    }
    animations.clear();
}