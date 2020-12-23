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
    std::lock_guard<std::mutex> lock1(from.m); 
    from.i -= n;
    std::cout << "get the first mutex" << " in thread " << std::this_thread::get_id() << std::endl;
    
    // 这里sleep一下，让一个线程得到第一个锁后，另一个有充分的时间直接得到第二个，然后死锁，此处为模拟效果
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::lock_guard<std::mutex> lock2(to.m);
    to.i += n;
    std::cout << "get the second mutex" << " in thread " << std::this_thread::get_id() << std::endl;

}


int main() {
    A x(70);
    A y(30);

    std::thread t1(f, std::ref(x), std::ref(y), 1);
    std::thread t2(f, std::ref(y), std::ref(x), 11);

    t1.join();
    t2.join();
}