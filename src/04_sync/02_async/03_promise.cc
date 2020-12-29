/*
https://www.cnblogs.com/wangshaowei/p/8875501.html


*/

#include <iostream>
#include <type_traits>
#include <future>
#include <thread>

using namespace std;
int main() {
    std::promise<int> promiseParam;
    std::thread t([](std::promise<int>& p) {
        std::this_thread::sleep_for(std::chrono::seconds(3));// 线程睡眠3s
        p.set_value_at_thread_exit(4);//
    }, std::ref(promiseParam));
    std::future<int> futureParam = promiseParam.get_future();

    auto r = futureParam.get();// 线程外阻塞等待
    std::cout << r << std::endl;
    t.join();
    return 0;
}