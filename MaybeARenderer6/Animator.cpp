#include "Animator.h"
#include <algorithm>

#include "Constants.h"

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

HDC Animator::CallAnimations() {
    // Create final composed HDC
    HDC screenDC = GetDC(0);
    HDC finalDC = CreateCompatibleDC(screenDC);
    HBITMAP finalBitmap = CreateCompatibleBitmap(screenDC, screenWidth, screenHeight);
    HBITMAP oldFinalBitmap = (HBITMAP)SelectObject(finalDC, finalBitmap);
    ReleaseDC(0, screenDC);

    // Clear finalDC
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
    RECT rect = { 0, 0, screenWidth, screenHeight };
    FillRect(finalDC, &rect, brush);
    DeleteObject(brush);

    // Create thread-safe container for animation HDCs
    std::vector<HDC> animDCs(animations.size(), nullptr);
    std::vector<HBITMAP> animBitmaps(animations.size(), nullptr);
    std::vector<HBITMAP> oldBitmaps(animations.size(), nullptr);
    std::vector<std::thread> threads;

    for (size_t i = 0; i < animations.size(); ++i) {
        if (!animations[i]->IsRunning()) continue;

        threads.emplace_back([&, i]() {
            HDC screenDC = GetDC(0);
            HDC animDC = CreateCompatibleDC(screenDC);
            HBITMAP animBitmap = CreateCompatibleBitmap(screenDC, screenWidth, screenHeight);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(animDC, animBitmap);
            ReleaseDC(0, screenDC);

            // Clear animDC
            HBRUSH b = CreateSolidBrush(RGB(0, 0, 0));
            RECT r = { 0, 0, screenWidth, screenHeight };
            FillRect(animDC, &r, b);
            DeleteObject(b);

            // Let the animation draw on its own surface
            // Don't pass deltaTime here - animations should already be updated
            animations[i]->DoAnimation(animDC, 0.0f);

            animDCs[i] = animDC;
            animBitmaps[i] = animBitmap;
            oldBitmaps[i] = oldBitmap;
            });
    }

    // Wait for all animation threads to finish
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    // Merge all animation HDCs into finalDC
    for (size_t i = 0; i < animDCs.size(); ++i) {
        if (animDCs[i]) {
            TransparentBlt(
                finalDC, 0, 0, screenWidth, screenHeight,
                animDCs[i], 0, 0, screenWidth, screenHeight,
                RGB(0, 0, 0)  // color treated as transparent
            );
        }
    }

    // Clean up temporary HDCs and bitmaps
    for (size_t i = 0; i < animDCs.size(); ++i) {
        if (animDCs[i]) {
            SelectObject(animDCs[i], oldBitmaps[i]);
            DeleteObject(animBitmaps[i]);
            DeleteDC(animDCs[i]);
        }
    }

    return finalDC;
}