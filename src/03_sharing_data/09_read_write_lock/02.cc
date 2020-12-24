/*
shared_mutex 这个名字并不十分贴切，不如 pthread 直呼「读写锁」。

所谓「读写锁」，就是同时可以被多个读者拥有，但是只能被一个写者拥有的锁。
而所谓「多个读者、单个写者」，并非指程序中只有一个写者（线程），而是说不能有多个写者同时去写。

shared_mutex 比一般的 mutex 多了函数 lock_shared() / unlock_shared()，
允许多个（读者）线程同时加锁、解锁，而 shared_lock 则相当于共享版的 lock_guard。

对 shared_mutex 使用 lock_guard 或 unique_lock 就达到了写者独占的目的。

于计数器来说，原子类型 std::atomic<> 也许是更好的选择。
*/
#include <iostream>
//std::unique_lock
#include <mutex> 
#include <boost/thread.hpp>
#include <thread>
 
class ThreadSafeCounter {
public:
    ThreadSafeCounter() = default;
  
    // 多个线程/读者能同时读计数器的值。
    unsigned int get() const {
        boost::shared_lock<boost::shared_mutex> lock(mutex_);
        return value_;
    }
  
    // 只有一个线程/写者能增加/写线程的值。
    void increment() {
        boost::unique_lock<boost::shared_mutex> lock(mutex_);
        value_++;
    }
  
    // 只有一个线程/写者能重置/写线程的值。
    void reset() {
        boost::unique_lock<boost::shared_mutex> lock(mutex_);
        value_ = 0;
    }
  
private:
    mutable boost::shared_mutex mutex_;
    unsigned int value_ = 0;
};
 
int main() {
    ThreadSafeCounter counter;
  
    auto increment_and_print = [&counter]() {
        for (int i = 0; i < 3; i++) {
            counter.increment();
            std::cout << std::this_thread::get_id() << '\t' << counter.get() << std::endl;
        }
    };
  
    std::thread thread1(increment_and_print);
    std::thread thread2(increment_and_print);
  
    thread1.join();
    thread2.join();
  
    return 0;
}