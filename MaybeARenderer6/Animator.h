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
    void Add(float duration, std::function<void(float)> func);
    void Update(float deltaTime);
    size_t GetActiveAnimationCount() const;
    void Clear();
};

#endif // ANIMATOR_H