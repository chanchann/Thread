#include <iostream>
#include <thread>
#include <mutex>

std::once_flag flag;

void f() {
    std::call_once(flag, [] { std::cout << 1; });
    std::cout << 2;
}

int main() {
    std::thread t1(f);
    std::thread t2(f);
    std::thread t3(f);
    t1.join();
    t2.join();
    t3.join();
}

// output
//1222