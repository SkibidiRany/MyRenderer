#pragma once 

#include <vector>
#include "MyClasses.h"

class DeletionManager {
public:
    static DeletionManager& Get();

    void Register(IDeletable* ptr);
    void Cleanup();

private:
    std::vector<IDeletable*> deletables;

    DeletionManager();
    ~DeletionManager();

    // Non-copyable, non-movable
    DeletionManager(const DeletionManager&) = delete;
    DeletionManager& operator=(const DeletionManager&) = delete;
};