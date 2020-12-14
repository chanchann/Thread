/*
在C++11中提供了4中互斥量

std::mutex;                  //非递归的互斥量
std::timed_mutex;            //带超时的非递归互斥量
std::recursive_mutex;        //递归互斥量
std::recursive_timed_mutex;  //带超时的递归互斥量

从各种互斥量的名字可以看出其具有的特性，在实际开发中，常用就是std::mutex，它就像是一把锁，我们需要做的就是对它进行加锁与解锁。

此例中没有加速，数目越多越容易看到顺序混乱
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

void func() {
    std::cout << "entry func test thread ID is : " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    std::cout << "leave func test thread ID is : " << std::this_thread::get_id() << std::endl;
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