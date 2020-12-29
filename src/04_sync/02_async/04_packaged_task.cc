/*
https://www.cnblogs.com/wangshaowei/p/8875501.html
std::packaged_task包装一个可调用对象的包装类（如function，lambda表达式（C++11之lambda表达式），将函数与future绑定起来。std::packaged_task与std::promise都有get_future()接口，
但是std::packaged_task包装的是一个异步操作，而std::promise包装的是一个值。
*/

#include <iostream>
#include <type_traits>
#include <future>
#include <thread>

int main() {
    std::packaged_task<int()> task([]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));// 线程睡眠2s
        return 4; });
    std::thread t1(std::ref(task));
    std::future<int> f1 = task.get_future();
    
    auto r = f1.get();// 线程外阻塞等待
    std::cout << r << std::endl;
    t1.join();
    return 0;
}