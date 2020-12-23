/*
https://blog.csdn.net/yamingwu/article/details/47860695

声名狼藉的Double-Checked Locking模式。
第一次访问指针时，不用获取锁，只有当指针为空时，才执行加锁操作，加锁以后再次检查指针是否为空。
因为其它线程可能在我们第一次检查指针后到锁定mutex之前已经初始化了数据。

但这个方案也存在潜在的race condition，第一次的检查没上锁，可能与其他线程中被保护的reset操作产生竞争。
如果当前线程看见其他线程写入了指针，但没看到新创建的对象实例，调用doSomething就会出错

https://www.zhihu.com/question/31551646
*/


#include <memory>
#include <mutex>
 
struct some_resource {
    void do_something() {}
};
 
 
std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;


void undefined_behaviour_with_double_checked_locking() {
    // 读操作和写操作没有同步
    if(!resource_ptr) {
        std::lock_guard<std::mutex> lk(resource_mutex);
        if (!resource_ptr) {
            resource_ptr.reset(new some_resource);
        }
    }
    // 可能导致do_something产生不正确的结果
    resource_ptr->do_something();
}
 
int main() {
    undefined_behaviour_with_double_checked_locking();
}