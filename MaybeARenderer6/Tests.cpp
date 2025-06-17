#include "Tests.h"
#include "MyClasses.h"
#include "Animator.h"
#include "Lerp.h"


// Static registry definition
std::vector<std::function<void()>>& Tests::GetTestRegistry() {
    static std::vector<std::function<void()>> tests;
    return tests;
}

void Tests::NewTest(std::function<void()> testFunc) {
    GetTestRegistry().push_back(testFunc);
}

void Tests::Run() {
    for (auto& test : GetTestRegistry()) {
        test();
    }
}







