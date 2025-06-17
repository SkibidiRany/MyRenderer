#include "Animator.h"
#include <algorithm>
#include "Constants.h"
#include <Windows.h>
#include <memory>
#include <vector>
#include <iostream>

Animator::Animator(ScheduledAnimator& schedulerRef)
    : scheduler(schedulerRef) {}

void Animator::Add(std::shared_ptr<Animation> animation) {
    //// Prevent duplicate
    //if (std::find(animations.begin(), animations.end(), animation) == animations.end()) {
    //    animations.push_back(animation);
    //}
    
	// Always add the animation, even if it is already present, because they might be added at a different time.
    animations.push_back(animation);

}

void Animator::Tick(float deltaTime) {
    auto dueAnims = scheduler.CollectDueAnimations();

    for (auto& anim : dueAnims) {
        anim->Start();
        if (std::find(animations.begin(), animations.end(), anim) == animations.end()) {
            animations.push_back(anim);
        }
    }

    for (auto& anim : animations) {
        anim->Update(deltaTime);
    }

    animations.erase(
        std::remove_if(animations.begin(), animations.end(),
            [](const std::shared_ptr<Animation>& anim) {
                return anim->IsFinished();
            }),
        animations.end()
    );
}

void Animator::At(std::shared_ptr<Animation> anim, float time) {
    scheduler.At(anim, time);
}

void Animator::After(std::shared_ptr<Animation> anim, float time)
{
	scheduler.After(anim, time);
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

HDC Animator::CallAnimations() {
    HDC screenDC = GetDC(0);
    HDC finalDC = CreateCompatibleDC(screenDC);
    HBITMAP finalBitmap = CreateCompatibleBitmap(screenDC, screenWidth, screenHeight);
    HBITMAP oldFinalBitmap = (HBITMAP)SelectObject(finalDC, finalBitmap);
    ReleaseDC(0, screenDC);

    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = { 0, 0, screenWidth, screenHeight };
    FillRect(finalDC, &rect, brush);
    DeleteObject(brush);

    std::vector<std::shared_ptr<Animation>> processed;

    for (const auto& anim : animations) {
        if (!anim || !anim->IsRunning()) continue;

        if (std::find(processed.begin(), processed.end(), anim) != processed.end()) {
            continue;
        }
        processed.push_back(anim);

        HDC animDC = CreateCompatibleDC(finalDC);
        HBITMAP animBitmap = CreateCompatibleBitmap(finalDC, screenWidth, screenHeight);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(animDC, animBitmap);

        HBRUSH b = CreateSolidBrush(RGB(0, 0, 0));
        RECT r = { 0, 0, screenWidth, screenHeight };
        FillRect(animDC, &r, b);
        DeleteObject(b);

        anim->DoAnimation(animDC, 0.0f);

        TransparentBlt(
            finalDC, 0, 0, screenWidth, screenHeight,
            animDC, 0, 0, screenWidth, screenHeight,
            RGB(0, 0, 0)
        );

        SelectObject(animDC, oldBitmap);
        DeleteObject(animBitmap);
        DeleteDC(animDC);
    }

    return finalDC;
}

void Animator::StartAnimations() {
    for (auto& anim : animations) {
        if (!anim->IsRunning()) {
            anim->Start();
        }
    }
}

void Animator::StopAnimations() {
    for (auto& anim : animations) {
        if (anim->IsRunning()) {
            anim->Stop();
        }
    }
}
