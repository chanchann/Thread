/*
 join 与 detach 的 std::thread::id

 TODO : 这里detach后
*/
#include <iostream>
#include <thread>
#include <chrono>
#include <assert.h>

using namespace std;

void foo() {
    cout << "foo start\n";
    std::thread::id tid = std::this_thread::get_id();
    cout << "foo thread id : " << tid << "\n";
    cout << "foo end\n";
}

int main() {
    std::thread t1(foo);

    std::thread::id tid = t1.get_id();
    if (t1.joinable())
        t1.join();

    cout << "Thread from Main : " << tid << endl;

    std::thread t2(foo);

    t2.detach();

    std::thread::id tid2 = t2.get_id();
    cout << "tid2 : " << tid2 << endl;
    cout << "Thread from Main : " << std::thread::id() << endl;
    assert(tid2 == std::thread::id());

    cout << "sleep 2s\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    cout << "Thread from Main : " << tid2 << endl;

    return 0;
}