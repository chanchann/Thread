/*
如果你将某个mutex上锁了，却一直不释放，另一个线程访问该锁保护的资源的时候，就会发生死锁，
这种情况下使用lock_guard可以保证析构的时候能够释放锁，
然而，当一个操作需要使用两个互斥元的时候，仅仅使用lock_guard并不能保证不会发生死锁，


程序卡住

Thread A              Thread B
_mu.lock()             _mu2.lock()
   //死锁               //死锁
_mu2.lock()         _mu.lock()

c++标准库中提供了std::lock()函数，能够保证将多个互斥锁同时上锁，

*/

#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <fstream>
using namespace std;

class LogFile {
private:
    std::mutex _mu;
    std::mutex _mu2;
    ofstream f;
public:
    LogFile() {
        f.open("log.txt");
    }
    ~LogFile() {
        f.close();
    }
    void shared_print(string msg, int id) {
        std::lock_guard<std::mutex> guard(_mu);
        std::lock_guard<std::mutex> guard2(_mu2);
        f << msg << id << endl;
        cout << msg << id << endl;
    }
    void shared_print2(string msg, int id) {
        std::lock_guard<std::mutex> guard(_mu2);
        std::lock_guard<std::mutex> guard2(_mu);
        f << msg << id << endl;
        cout << msg << id << endl;
    }
};

void function_1(LogFile& log) {
    for(int i = 0; i > -1000; i--)
        log.shared_print2(string("From t1: "), i);
}

int main() {
    LogFile log;
    std::thread t1(function_1, std::ref(log));

    for(int i = 0; i < 1000; i++)
        log.shared_print(string("From main: "), i);

    t1.join();
    return 0;
}