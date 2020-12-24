/*
https://blog.csdn.net/yamingwu/article/details/47860695

多线程情况下，使用mutex来保护初始化过程的代码如下：


降低程序执行的效率，因为即使共享数据已经初始化好，
每次调用foo()函数，还是回去锁定mutex，所有调用这个函数的线程，都会被不必要的串行化起来
*/


#include <memory>
#include <mutex>
#include <iostream>

struct some_resource {
    void do_something() { std::cout << "do something" << std::endl; }
};
 
 
std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;

void foo() {
    // 所有调用foo()的线程在锁定mutex时都被串行化而降低了程序执行的效率
    std::unique_lock<std::mutex> lk(resource_mutex);
    if(!resource_ptr) {
        // 实际上，只有初始化代码才需要被保护
        resource_ptr.reset(new some_resource);
    }
    lk.unlock();
    resource_ptr->do_something();
}
 
int main() {
    foo();
}