/*
c++标准库中提供了std::lock()函数，能够保证将多个互斥锁同时上锁，

std::lock(_mu, _mu2);
同时，lock_guard也需要做修改，因为互斥锁已经被上锁了，那么lock_guard构造的时候不应该上锁，只是需要在析构的时候释放锁就行了，使用std::adopt_lock表示无需上锁：

std::lock_guard<std::mutex> guard(_mu2, std::adopt_lock);
std::lock_guard<std::mutex> guard2(_mu, std::adopt_lock);


总结一下，对于避免死锁，有以下几点建议：

1. 建议尽量同时只对一个互斥锁上锁。

{
    std::lock_guard<std::mutex> guard(_mu2);
    //do something
    f << msg << id << endl;
}
{
    std::lock_guard<std::mutex> guard2(_mu);
    cout << msg << id << endl;
}

2. 不要在互斥锁保护的区域使用用户自定义的代码，因为用户的代码可能操作了其他的互斥锁。

{
    std::lock_guard<std::mutex> guard(_mu2);
    user_function(); // never do this!!!
    f << msg << id << endl;
}

3. 如果想同时对多个互斥锁上锁，要使用std::lock()。

4. 给锁定义顺序（使用层次锁，或者比较地址等），每次以同样的顺序进行上锁。详细介绍可看C++并发编程实战。
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
        std::lock(_mu, _mu2);
        std::lock_guard<std::mutex> guard(_mu, std::adopt_lock);
        std::lock_guard<std::mutex> guard2(_mu2, std::adopt_lock);
        f << msg << id << endl;
        cout << msg << id << endl;
    }
    void shared_print2(string msg, int id) {
        std::lock(_mu, _mu2);
        std::lock_guard<std::mutex> guard(_mu2, std::adopt_lock);
        std::lock_guard<std::mutex> guard2(_mu, std::adopt_lock);
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

    for(int i=0; i<1000; i++)
        log.shared_print(string("From main: "), i);

    t1.join();
    return 0;
}