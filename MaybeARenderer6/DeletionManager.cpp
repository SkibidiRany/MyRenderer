#include "DeletionManager.h"

DeletionManager::DeletionManager() {}
DeletionManager::~DeletionManager() {}

DeletionManager& DeletionManager::Get() {
    static DeletionManager instance;
    return instance;
}

void DeletionManager::Register(IDeletable* ptr) {
    deletables.push_back(ptr);
}

void DeletionManager::Cleanup() {
    for (IDeletable* ptr : deletables) {
        if (ptr) {
            ptr->Delete();  // user-defined cleanup
            delete ptr;     // destroy object
        }
    }
    deletables.clear();
}