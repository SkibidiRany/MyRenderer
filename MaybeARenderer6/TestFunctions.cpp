#include "TestFunctions.h"
#include "Animator.h"
#include "Lerp.h"








void TestFunctions::LerpPressureTest(Animator* animator) {
    double delayTest = 5.0, runAnimsFor = 4.0;

    Point a = { 0, 0 };
    Point b = { 400, 400 };
    Point c = { 400, 200 };
    Point d = { 800 - 1, 600 - 1 };

    auto ab = std::make_shared<Lerp>(a, b, runAnimsFor);
    auto ac = std::make_shared<Lerp>(a, c, runAnimsFor);
    auto ad = std::make_shared<Lerp>(a, d, runAnimsFor);

    auto ba = std::make_shared<Lerp>(b, a, runAnimsFor);
    auto bc = std::make_shared<Lerp>(b, c, runAnimsFor);
    auto bd = std::make_shared<Lerp>(b, d, runAnimsFor);

    auto ca = std::make_shared<Lerp>(c, a, runAnimsFor);
    auto cb = std::make_shared<Lerp>(c, b, runAnimsFor);
    auto cd = std::make_shared<Lerp>(c, d, runAnimsFor);

    auto da = std::make_shared<Lerp>(d, a, runAnimsFor);
    auto db = std::make_shared<Lerp>(d, b, runAnimsFor);
    auto dc = std::make_shared<Lerp>(d, c, runAnimsFor);

    // Add and schedule all animations with increasing delay
    std::vector<std::shared_ptr<Animation>> allAnims = {
        ab, ac, ad, ba, bc, bd, ca, cb, cd, da, db, dc
    };

    for (auto anim : allAnims) {
        animator->Add(anim);
    }

    animator->StartAnimations();
}

void TestFunctions::AnimationAfterFunctionalityTest(Animator* animator)
{
    animator->After(std::make_shared<Lerp>(Point(0, 0), Point(550, 450), 4), 4);
}





