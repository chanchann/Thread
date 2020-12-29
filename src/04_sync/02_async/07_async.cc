/*
https://www.cnblogs.com/wangshaowei/p/8875501.html

而std::async比std::promise, std::packaged_task和std::thread更高一层，它可以直接用来创建异步的task，
异步任务返回的结果也保存在future中。
可以说，std::async帮我们将std::future、std::promise和std::packaged_task三者结合了起来

std::launch::async：在调用async就开始创建线程。
std::launch::deferred：延迟加载方式创建线程。调用async时不创建线程，直到调用了future的get或者wait时才创建线程。

因为std::packaged_task禁用了拷贝构造函数，所以必须通过std::ref或std::move传递
*/

#include <iostream>
#include <future>
#include <thread>

int main() {
    // future from a packaged_task
    std::packaged_task<int()> task([]() { 
        std::cout << "packaged_task started" << std::endl;
        return 7; }); // wrap the function
    std::future<int> f1 = task.get_future();  // get a future
    std::thread(std::move(task)).detach(); // launch on a thread

    // future from an async()
    std::future<int> f2 = std::async(std::launch::deferred, []() { 
        std::cout << "Async task started" << std::endl;
        return 8; });

    // future from a promise
    std::promise<int> p;
    std::future<int> f3 = p.get_future();
    std::thread([&p] { p.set_value_at_thread_exit(9); }).detach();

    f1.wait();
    f2.wait();
    f3.wait();
    std::cout << "Done!\nResults are: "
        << f1.get() << ' ' << f2.get() << ' ' << f3.get() << '\n';
}