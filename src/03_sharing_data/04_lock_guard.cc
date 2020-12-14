/* 
lock_guard是类模板，在其构造函数中自动给std::mutex加锁，在退出作用域的时候自动解锁，这样就可以保证std::mutex的正确操作，这也是RAII（获取资源便初始化）技术的体现。
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <mutex>
std::mutex g_mutex;

void func() {
    std::lock_guard<std::mutex> lock(g_mutex);   //加锁
    std::cout << "entry func test thread ID is : " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    std::cout << "leave func test thread ID is : " << std::this_thread::get_id() << std::endl;
    //退出作用域后，lock_guard对象析构就自动解锁
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