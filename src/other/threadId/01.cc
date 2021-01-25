#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
// 区分 syscall(SYS_gettid) and std::this_thread::get_id()
void threadFunc() {
    std::cout << "syscall id = " << (long int)syscall(SYS_gettid) << std::endl;
    std::cout << "pthread_self = " << pthread_self() << std::endl;
    std::cout << "std::thread::id = " << std::this_thread::get_id() << std::endl;
}

int main (int argc, char *argv[]) {
    std::thread t1(threadFunc);
    t1.join();
    return 0;
}