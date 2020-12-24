/*
需要的变量(variable)容器(container)只需要初始化一次，以容器(containter)为例子，可能只是需要向其中注入一次元素. C++11开始标准库提供了std::call_once()和std::once_flag的组合可以帮助我们做到这一点.
 在多线程编程中，有一个常见的情景是某个任务仅仅须要运行一次。

https://www.cnblogs.com/blfshiye/p/4717646.html


truct once_flag {
    constexpr once_flag() noexcept;
    once_flag(const once_flag&) = delete;
    once_flag& operator=(const once_flag&) = delete;
};
template<class Callable, class ...Args>
void call_once(once_flag& flag, Callable&& func, Args&&... args);

}  // std
可以看到once_flag是不允许修改的，拷贝构造函数和operator=函数都声明为delete，这样防止程序员乱用。
另外，call_once也是很简单的，只要传进一个once_flag，回调函数，和参数列表就可以了。
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::vector<int> vec;
std::mutex mutex;
std::once_flag flag;

static int val = 0;

void initialized_vec(std::vector<int>& vec) {
    for (int i = 0; i < 10; ++i) {
        vec.push_back(val++);
    }
}
 
void print_vec() {
    std::call_once(flag, initialized_vec, std::ref(vec));
    std::lock_guard<std::mutex> auto_lock(mutex);
    for (const int& number : vec) {
        std::cout << number << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::thread threads[5];
    for (int i = 0; i < 5; ++i) {
        threads[i] = std::thread(print_vec);
    }

    for (std::thread& ref_thread : threads) {
        ref_thread.join();
    }

    return 0;
}