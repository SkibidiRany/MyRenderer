#pragma once

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <vector>
#include <memory>
#include <functional>
#include "Animation.h"

class Animator {
private:
    std::vector<std::shared_ptr<Animation>> animations;

public:
    void Add(std::shared_ptr<Animation> animation);
    void Update(float deltaTime);
    size_t GetActiveAnimationCount() const;
    void Clear();

	HDC CallAnimations();
    void StartAnimations();
    void StopAnimations();
};

#endif // ANIMATOR_H