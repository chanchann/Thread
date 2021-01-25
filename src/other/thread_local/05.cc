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

void thread_func(const std::string& thread_name) {
    for (int i = 0; i < 3; ++i) {
        thread_local A* a = new A();
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "thread[" << thread_name << "]: a.counter:" << a->get_value() << std::endl;
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