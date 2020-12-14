/*
hardware_concurrency会返回支持的并发线程数
*/

#include <iostream>
#include <thread>

int main() {
    std::cout << std::thread::hardware_concurrency() << std::endl;
    return 0;
}