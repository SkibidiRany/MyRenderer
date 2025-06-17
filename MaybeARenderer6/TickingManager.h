#ifndef TICKING_MANAGER_H
#define TICKING_MANAGER_H

#include <vector>
#include "MyClasses.h"

class TickingManager {
private:
    std::vector<ITick*> subscribers;

public:
    void Subscribe(ITick* sub);
    void Tick(float deltaTime);
};

#endif // TICKING_MANAGER_H