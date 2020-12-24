/*
使用条件变量（condition variable），标准库对条件变量提供了两种实现：std::condition_variable和std::condition_variable_any，前者仅限和std::mutex工作，
而后者可以与任何满足最低标准的mutex工作（因此加上_any的后缀），更通用也意味着更大的开销，因此一般首选使用前者

*/

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void f() {
    std::unique_lock<std::mutex> l(m); // 传给wait的只能是std::unique_lock
    cv.wait(l, [] { return ready; }); // 第二个参数为false时解锁mutex阻塞线程
    // 当收到其他线程notify_one时wait会被唤醒，重新检查条件
    data += " after processing";
    processed = true;
    l.unlock();
    cv.notify_one();
}

int main() {
    std::thread t(f);
    data = "data";
    {
        std::lock_guard<std::mutex> l(m);
        data += " ready";
        ready = true;
        cv.notify_one(); // 唤醒cv.wait，重新检查ready == true
    }
    
    {
        std::unique_lock<std::mutex> l(m);
        cv.wait(l, [] { return processed; });
    }
    std::cout << data; // data ready after processing
    t.join();
}