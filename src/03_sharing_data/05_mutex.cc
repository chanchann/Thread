/*
C++17提供了加强版的std::scoped_lock，它可以接受任意数量的std::mutex，可完全取代std::lock_guard

使用互斥量来保护数据，并不是仅仅在每一个成员函数中都加入一个std::lock_guard对象那么简单；一个迷失的指针或引用，将会让这种保护形同虚设

检查迷失指针或引用是很容易的，只要没有成员函数通过返回值或者输出参数的形式向其调用者返回指向受保护数据的指针或引用，数据就是安全的

不能将被保护数据的指针或引用以函数返回值的形式，返回给外部不可控的调用者；
不能将被保护数据的指针或引用意函数参数的形式，传递给外部不可控的调用者。
*/

// Accidentally passing out a reference to protected data

#include <mutex>

class some_data {
private:
    int a;
    std::string b;
public:
    void do_something() {}
};

class data_wrapper {
private:
    some_data data;
    std::mutex m;
public:
    template<typename Function>
    void process_data(Function func) {  // 接收的函数 func 是一个外部函数,不可控
        std::lock_guard<std::mutex> l(m);
        func(data);  // 1 传递“保护”数据给用户函数
    }
};

some_data* unprotected;

void malicious_function(some_data& protected_data) {
    unprotected = &protected_data;  // 将传递进来的指针赋值给了一个没有任何保护的全局指针变量 unprotected
}

data_wrapper x;

void foo() {
    x.process_data(malicious_function); // 2 传递一个恶意函数
    unprotected->do_something(); // 3 在无保护的情况下访问保护数据
}

int main() {
    foo();
}
