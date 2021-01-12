/*
https://downdemo.gitbook.io/cpp-concurrency-in-action-2ed/3.-tong-bu-bing-fa-cao-zuo-synchronizing-concurrent-operation/xian-ding-deng-dai-shi-jian/shi-zhong-clock

https://en.cppreference.com/w/cpp/chrono
*/

#include <iostream>
#include <chrono>
#include <iomanip>

void test01() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now); // 转为整数
    std::cout << std::put_time(std::localtime(&now_c), "%F %T"); // %F即%Y-%m-%d，%T即%H:%M:%S
}

long fibonacci(unsigned n) {
    if (n < 2) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}
void test02() {
    auto start = std::chrono::steady_clock::now();
    std::cout << "f(42) = " << fibonacci(42) << '\n';
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}

int main() {
    // test01();
    test02();
}
