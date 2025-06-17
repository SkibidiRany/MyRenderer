#include "TickingManager.h"
#include "MyClasses.h"

void TickingManager::Subscribe(ITick* sub) {
    subscribers.push_back(sub);
}

void TickingManager::Tick(float deltaTime) {
    for (auto* sub : subscribers) {
        sub->Tick(deltaTime);
    }
}
