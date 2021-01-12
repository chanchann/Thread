/*
 * @Author: Shiyu Yi
 * @Github: https://github.com/chanchann
 */

/*
https://github.com/isocpp/CppCoreGuidelines/issues/1272
*/

#include <iostream>
#include <thread>
#include <list>
#include <mutex>
#include <condition_variable>
#include <vector> 
#include <algorithm>

class Task {
public:
    Task(int taskID) : _taskID(taskID) {}
    void doTask() {
        std::cout << "handle a task, taskID: " << _taskID << ", threadID: " 
            << std::this_thread::get_id() << std::endl; 
    }
private:
    int _taskID;
};

std::mutex mymutex;
std::list<std::shared_ptr<Task> > tasks;
std::condition_variable cv;

void consumer() {
    std::shared_ptr<Task> pTask;
    while(true) {
        std::unique_lock<std::mutex> lk(mymutex);
        cv.wait(lk, [&] { return !tasks.empty(); });
        pTask = tasks.front();
        tasks.pop_front();
        pTask->doTask();
        lk.unlock();   // 这里需要我unlock吗ß
    }
}

void producer() {
    std::shared_ptr<Task> pTask;
    int taskID = 0;
    while(true) {
        pTask = std::make_shared<Task>(taskID);
        std::unique_lock<std::mutex> lk(mymutex); // 这里可以优化lockguard
        tasks.push_back(pTask);
        std::cout << "produce a task, taskID: "  << taskID++ << ", threadID: " 
            << std::this_thread::get_id() << std::endl; 
        /**
         * 这里需要手动unlock
         * https://en.cppreference.com/w/cpp/thread/condition_variable
         */
        lk.unlock();   
        cv.notify_one();  // notify_one vs notify_all
        std::this_thread::sleep_for(std::chrono::seconds(1)); // sleep 1s
    }
}

int main() {
    std::vector<std::thread> threads;
    // 创建5个消费者线程
    for(int i = 0; i < 5; i++) {
        threads.push_back(std::thread(consumer));
    }
    // 一个生产者
    threads.push_back(std::thread(producer));
    std::for_each(threads.begin(), threads.end(), 
                    std::mem_fn(&std::thread::join));
    return 0;
}