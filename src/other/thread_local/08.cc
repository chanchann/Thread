/*
http://cifangyiquan.net/programming/thread_local/

类成员变量的情况

thread_local作为类成员变量时必须是static

可以看出thread_local作为类成员时也是对于每个thread分别分配了一个。而static则是全局一个。
*/

#include <iostream>
#include <thread>
#include <mutex>
std::mutex cout_mutex;

class B {
public:
    B() {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "create B" << std::endl;
    }
    ~B() {}
    //thread_local static int b_key;
    thread_local static int b_key;
    int b_value = 24;
    static int b_static;
};

thread_local int B::b_key = 12;
int B::b_static = 36;

void thread_func(const std::string& thread_name) {
    B b;
    for (int i = 0; i < 3; ++i) {
        b.b_key--;
        b.b_value--;
        b.b_static--;   // not thread safe
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "thread[" << thread_name << "]: b_key:" << b.b_key << ", b_value:" << b.b_value << ", b_static:" << b.b_static << std::endl;
        std::cout << "thread[" << thread_name << "]: B::key:" << B::b_key << ", b_value:" << b.b_value << ", b_static: " << B::b_static << std::endl;
    }
}

int main() {
    std::thread t1(thread_func, "t1");
    std::thread t2(thread_func, "t2");
    t1.join();
    t2.join();
    return 0;
}