/*
https://zh.cppreference.com/w/cpp/atomic/atomic

*/

#include <atomic>
#include <stdio.h>

int main() {
    std::atomic<int> value;
    value = 99;
    printf("%d\n", (int)value);

    //自增1，原子操作
    value++;
    printf("%d\n", (int)value);

    return 0;
}
