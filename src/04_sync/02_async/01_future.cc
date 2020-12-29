/*
https://thispointer.com/c11-multithreading-part-8-stdfuture-stdpromise-and-returning-values-from-thread/

Many times we encounter a situation where we want a thread to return a result.

1.) Old Way : Share data among threads using pointer

2.) C++11 Way : Using std::future and std::promise

*/

#include <iostream>
#include <thread>
#include <future>
void initiazer(std::promise<int> * promObj) {
    std::cout<<"Inside Thread"<<std::endl;     promObj->set_value(35);
}

int main() {
    std::promise<int> promiseObj;
    std::future<int> futureObj = promiseObj.get_future();
    std::thread th(initiazer, &promiseObj);  // 这里应该std::ref把？
    std::cout << futureObj.get() << std::endl;
    th.join();
    return 0;
}