/*
死锁的四个必要条件：互斥、占有且等待、不可抢占、循环等待
避免死锁通常建议让两个mutex以相同顺序上锁，总是先锁A再锁B，但这并不适用所有情况
std::lock解决了此问题，它可以一次性锁住多个mutex，并且没有死锁风险
std::lock可能抛异常，此时就不会上锁，因此要么都锁住，要么都不锁

*/

#include <iostream>
#include <mutex>
#include <thread>

class A {
public:
    explicit A(int x) : i(x) {}
    int i;
    std::mutex m;
};

void f(A& from, A& to, int n) {
    std::lock(from.m, to.m);
    // 下面按固定顺序加锁，看似不会有死锁的问题
    // 但如果没有std::lock同时上锁
    // 另一线程中执行f(to, from, n)
    // 两个锁的顺序就反了过来，从而可能导致死锁
    std::lock_guard<std::mutex> lock1(from.m, std::adopt_lock); // std::adopt_lock表示获取m的所有权
    from.i -= n;
    std::cout << "get the first mutex" << " in thread " << std::this_thread::get_id() << std::endl;
    
    // 这里sleep一下，让一个线程得到第一个锁后，另一个有充分的时间直接得到第二个，然后死锁，此处为模拟效果
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    std::lock_guard<std::mutex> lock2(to.m, std::adopt_lock);
    to.i += n;
    std::cout << "get the second mutex" << " in thread " << std::this_thread::get_id() << std::endl;

}

int main() {
    A x(70);
    A y(30);

    std::thread t1(f, std::ref(x), std::ref(y), 5);
    std::thread t2(f, std::ref(y), std::ref(x), 10);

    t1.join();
    t2.join();
}