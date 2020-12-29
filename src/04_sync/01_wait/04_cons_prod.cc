/*
简单的生产者消费者模型

生产者消费者模型可以通过 queue， mutex 和 condition_variable 来实现

单生产者-单消费者模型

这里还有unlock问题细节还没完全搞清，参考01.cc

TODO : 多次运行有阻塞情况 neet to fix
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <atomic>

int main() {
  std::queue<int> production;
  std::mutex mtx;
  std::condition_variable cv;
  bool ready = false;  // 是否有产品可供消费
  bool done = false;   // 生产结束

  std::thread producer(
        [&] () -> void {
            for (int i = 1; i < 10; ++i) {
                // 模拟实际生产过程
                std::this_thread ::sleep_for(std::chrono::milliseconds(10));
                std::cout << "producing " << i << std::endl;

                std::unique_lock<std::mutex> lock(mtx);
                production.push(i);

                // 有产品可以消费了
                ready = true;
                // lock.unlock();  需要加吗
                cv.notify_one();
            }
            // 生产结束了
            done = true;
        }
  );

  std::thread consumer (
        [&] () -> void {
            std::unique_lock<std::mutex> lock(mtx);
            // 如果生成没有结束或者队列中还有产品没有消费，则继续消费，否则结束消费
            while(!done || !production.empty()) {
                // std::unique_lock<std::mutex> lock(mtx);
                // 防止误唤醒
                // while(!ready) {   
                //     cv.wait(lock);
                // }
                cv.wait(lock, [&] { return ready; });

                while(!production.empty()) {
                    // 模拟消费过程
                    std::cout << "consuming " << production.front() << std::endl;
                    production.pop();
                }

                // 没有产品了
                ready = false;
            }
        }
    );

    producer.join();
    consumer.join();

  return 0;
}