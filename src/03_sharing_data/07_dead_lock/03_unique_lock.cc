/*
unique_lock比lock_guard灵活很多，效率上差一点，内存占用多一点。

2. unique_lock的第二个参数，实现其它功能

unique_lock的defer_lock参数，即先不加锁，只是先绑定unique_lock与mutex，
另外，可以 随时进行加锁、解锁操作，某些情况下可提高效率
（注意此时的加、解锁是通过unique_lock的成员函数.lock() 与 .unlock()实现的）

到真正需要同步的地方再加锁,用完就立马释放,无需等到程序执行出当前范围，从而支持了更加细粒度的加锁。

unique_lock还可以 交换管理权（unique_lock可以被移动，lock_guard不可以）

https://zhuanlan.zhihu.com/p/146392103
https://zhuanlan.zhihu.com/p/146981951
*/
#include<iostream>
#include<thread>
#include<string>
#include<mutex>
#include<fstream>

class LogFile {
private:
    std::mutex m_mutex;//锁
    std::ofstream f;
public:
    LogFile() {
        f.open("log.txt");
    }
    void shared_print(std::string id, int value) {
        std::unique_lock<std::mutex> locker(m_mutex, std::defer_lock);//先不加锁
        //...此处代码不需要保护
        locker.lock();
        f << id << ":" << value << std::endl;//此处才是保护区
        std::cout << id << ":" << value << std::endl;
        locker.unlock();
        //...此处代码也不需要保护
        locker.lock();
        //以下可能又需要保护了

        //unique_lock可以被移动，lock_guard不可以
        std::unique_lock<std::mutex> locker2 = std::move(locker);
        //将unique_lock管理的lock交给新的unique_lock的管理
        //即将m_mutex与locker2绑定在一起！此时locker已无用
        // release in the deconstructor of locker2

    }

};

void function_1(LogFile& log) {
    for (int i = 0; i > -1000; i--)
        log.shared_print(std::string("from t1:"), i);
}

int main() { 
    LogFile log;
    std::thread t1(function_1, std::ref(log));//t1线程开始运行

    for (int i = 0; i < 1000; i++) {
        log.shared_print(std::string("from main:"), i);
    }

    t1.join();

    return 0;
}