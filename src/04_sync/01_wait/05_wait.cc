/*
https://blog.csdn.net/yapian8/article/details/46638155

在线程被阻塞时，该函数会自动调用 lck.unlock() 释放锁，使得其他被阻塞在锁竞争上的线程得以继续执行。
另外，一旦当前线程获得通知(notified，通常是另外某个线程调用 notify_* 唤醒了当前线程)，
wait()函数也是自动调用 lck.lock()，使得lck的状态和 wait 函数被调用时相同


template <class Predicate>
void wait (unique_lock<mutex>& lck, Predicate pred)

相当于

while (!pred()) wait(lck);

std::this_thread::yield

https://www.zhihu.com/question/52892878
*/

#include <iostream>                // std::cout
#include <thread>                // std::thread, std::this_thread::yield
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable

std::mutex mtx;
std::condition_variable cv;

int cargo = 0;

// 消费者线程.
void consume(int n)
{
    for (int i = 0; i < n; ++i) {
        std::unique_lock <std::mutex> lck(mtx);
        cv.wait(lck, [&] { return cargo; });
        std::cout << cargo << '\n';
        cargo = 0;
    }
}

int main() {
    std::thread consumer_thread(consume, 10); // 消费者线程.

    // 主线程为生产者线程, 生产 10 个物品.
    for (int i = 0; i < 10; ++i) {
        while (cargo != 0)
            std::this_thread::yield();   //是将当前线程所抢到的CPU”时间片A”让渡给其他线程
        std::unique_lock <std::mutex> lck(mtx);
        cargo = i + 1;
        cv.notify_one();
    }

    consumer_thread.join();

    return 0;
}