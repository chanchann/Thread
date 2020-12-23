/*
为了处理race condition，C++标准库提供了std::once_flag和std::call_once

每个线程只要使用std::call_once，在std::call_once结束时就能安全地知道指针已被其他线程初始化，
而且这比使用mutex的开销更小

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
    p->do_something();
}
 
int main() {
    foo();
}