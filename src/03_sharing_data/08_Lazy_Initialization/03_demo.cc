/*
为了处理race condition，C++标准库提供了std::once_flag和std::call_once

每个线程只要使用std::call_once，在std::call_once结束时就能安全地知道指针已被其他线程初始化，
而且这比使用mutex的开销更小


另一种方法是使用静态局部变量。
在C++11中，静态局部变量的初始化只会发生在一个确定的线程中，其它线程会等待该变量初始化完成才能继续执行，
这样race condition就从一个麻烦的问题转换为选择一个线程来进行初始化操作。在只需要一个单独的全局变量时
，可以使用这种方法代替std::call_once：

lass my_class;
my_class& get_my_class_instance()
{
    // 初始化保证了线程间的安全性
    static my_class instance;
    return instance;
}

单例模式
*/


#include <memory>
#include <mutex>
std::once_flag flag;

struct some_resource {
    void do_something() {}
};
 
 
std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;

void init() {
    resource_ptr.reset(new some_resource);
}

void foo() {
    std::call_once(flag, init);
    resource_ptr->do_something();
}
 
int main() {
    foo();
}