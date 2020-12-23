/*
如果使用 C++ 面向对象的方式对线程函数进行封装，那么线程函数就不能是类的实例方法，即必须是类的静态方法。那么，为什么不能是类的实例方法呢？
void* threadFunc(Thread* this, void* arg);
这样的话，就不符合线程函数签名要求

如果是使用 C++ 11 的 std::thread 类就没有这个限制，即使类成员函数是类的实例方法也可以，
但是必须显式地将线程函数所属的类对象实例指针（在类的内部就是 this 指针）作为构造函数参数传递给 std::thread，还是需要传递类的 this 指针

*/

#include <thread>
#include <memory>
#include <stdio.h>

class Thread {
public:
    Thread() = default;

    ~Thread() = default;

    void Start() {
        m_stopped = false;
        //threadFunc 是类的非静态方法，因此作为线程函数，第一个参数必须传递类实例地址，即 this 指针
        m_spThread.reset(new std::thread(&Thread::threadFunc, this, 8888, 9999));
    }

    void Stop() {
        m_stopped = true;
        if (m_spThread) {
            if (m_spThread->joinable())
                m_spThread->join();
        }
    }

private:
    void threadFunc(int arg1, int arg2) {
        while (!m_stopped) {
            printf("Thread function use instance method.\n");
        }      
    }

private:
    // std::shared_ptr 类来包裹了一下 new 出来的 std::thread 对象，
    // 这样我们就不需要自己手动 delete 这个 std::thread 对象了。
    std::shared_ptr<std::thread>  m_spThread;
    bool                          m_stopped;
};

int main() {
    Thread mythread;
    mythread.Start();

    while (true)
    {
        //权宜之计，让主线程不要提前退出
    }

    return 0;
}
