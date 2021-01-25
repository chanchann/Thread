/*
http://cifangyiquan.net/programming/thread_local/

局部变量

thread_local的局部变量没有因为for循环作用域而重新赋值。这是因为线程存储期的变量都是和线程绑定的，所以只有第一次声明时被赋值。
可以理解为线程的static变量。不过变量的作用域依然是在本身的作用域内
在for循环外使用x就会编译时错误
*/

#include <iostream>
#include <thread>
#include <mutex>
std::mutex cout_mutex;    //方便多线程打印

void thread_func(const std::string& thread_name) {
    for (int i = 0; i < 3; ++i) {
        thread_local int x = 1;   // 1
        // int x = 1;  // 2
        x++;
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "thread[" << thread_name << "]: x = " << x << std::endl;
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