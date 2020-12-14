/*
可以通过对线程实例调用成员函数get_id或在当前线程中调用std::this_thread::get_id获取线程id
线程id允许拷贝和比较，因此可以将其作为容器的键值。如果两个线程id相等，则两者是同一线程或都无线程
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <assert.h>

using namespace std;

void foo() {
    std::thread::id tid = std::this_thread::get_id();
    cout << "foo thread id : " << tid << "\n";
}

int main() {
    std::thread t1(foo);

    std::thread::id tid = t1.get_id();
    std::thread::id mainTid = std::this_thread::get_id();

    if (t1.joinable())
        t1.join();

    cout << "t1 tid: " << tid << endl;
    cout << "main tid: " << mainTid << endl;

    return 0;
}