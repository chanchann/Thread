/*
https://zhuanlan.zhihu.com/p/77585472

*/

#include <iostream>
#include <string>
#include <thread>

class A {
public:
    A() {}
    ~A() {}

    void test(const std::string &name) {
        thread_local int count = 0;
        ++count;
        std::cout << name << ": " <<  count << std::endl;
    }
};

void func(const std::string &name) {
    A a1;
    a1.test(name);
    a1.test(name);
    A a2;
    a2.test(name);
    a2.test(name);
}

int main(int argc, char* argv[]) {
    std::thread t1(func, "t1");
    t1.join();
    std::thread t2(func, "t2");
    t2.join();
    return 0;
}