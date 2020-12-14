/*
用std::mutex进行控制：
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <mutex>
std::mutex g_mutex;

void func() {
    g_mutex.lock();
    std::cout << "entry func test thread ID is : " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    std::cout << "leave func test thread ID is : " << std::this_thread::get_id() << std::endl;
    g_mutex.unlock();
}

int main() {
    std::vector<std::thread> threads;
    for(unsigned i = 0; i < 100; ++i) {
        threads.push_back(std::thread(func)); // 产生线程
    } 
    std::for_each(threads.begin(),threads.end(),
                    std::mem_fn(&std::thread::join)); // 对每个线程调用join()
    return 0;
}