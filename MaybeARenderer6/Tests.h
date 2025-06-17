#ifndef TESTS_H
#define TESTS_H

#include <vector>
#include <functional>



class Tests {
private:
    static std::vector<std::function<void()>>& GetTestRegistry();

public:
    static void NewTest(std::function<void()> testFunc);
    static void Run();
};

#endif // TESTS_H


