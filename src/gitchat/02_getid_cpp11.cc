/*
C++11 的获取当前线程 ID 的方法

 get_id 方法返回的是一个包装类型的 std::thread::id 对象，不可以直接强转成整型，也没有提供任何转换成整型的接口。
 所以，我们一般使用 std::cout 这样的输出流来输出，
 或者先转换为 std::ostringstream 对象，再转换成字符串类型，然后把字符串类型转换成我们需要的整型。

*/

//test_cpp11_thread_id.cpp
#include <thread>
#include <iostream>
#include <sstream>

void worker_thread_func() {
    while (true) { }
}

int main()
{
    std::thread t(worker_thread_func);
    //获取线程 t 的 ID
    std::thread::id worker_thread_id = t.get_id();
    std::cout << "worker thread id: " << worker_thread_id << std::endl;

    //获取主线程的线程 ID
    std::thread::id main_thread_id = std::this_thread::get_id();
    //先将 std::thread::id 转换成 std::ostringstream 对象
    std::ostringstream oss;
    oss << main_thread_id;
    //再将 std::ostringstream 对象转换成std::string
    std::string str = oss.str();
    std::cout << "main thread id: " << str << std::endl;
    //最后将 std::string 转换成整型值
    unsigned long long threadid = std::stoull(str);
    std::cout << "main thread id: " << threadid << std::endl;

    while (true)
    {
        //权宜之计，让主线程不要提前退出
    }

    return 0;
}

