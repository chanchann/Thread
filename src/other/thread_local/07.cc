/*
http://cifangyiquan.net/programming/thread_local/

类对象


*/

#include <iostream>
#include <thread>
#include <mutex>
std::mutex cout_mutex;

//定义类
class A {
public:
    A() {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "create A" << std::endl;
    }

    ~A() {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "destroy A" << std::endl;
    }

    int counter = 0;
    int get_value() {
        return counter++;
    }
};

A* creatA() {
    return new A();
}

void loopin_func(const std::string& thread_name) {
    // 但是如果没有在声明时进行赋值，就不一样了
    // 所有一般情况要求我们：thread_local对象声明时赋值。
    thread_local A* a;
    a = creatA();
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "thread[" << thread_name << "]: a.counter:" << a->get_value() << std::endl;
    return;
}

void thread_func(const std::string& thread_name) {
    for (int i = 0; i < 3; ++i) {    
        loopin_func(thread_name);
    }
    return;
}

int main() {
    std::thread t1(thread_func, "t1");
    std::thread t2(thread_func, "t2");
    t1.join();
    t2.join();
    return 0;
}