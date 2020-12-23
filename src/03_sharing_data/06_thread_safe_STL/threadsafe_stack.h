/*
简化接口更有利于数据控制，可以保证互斥量将一个操作完全锁住。
这里的核心思想就是合并接口


这里还是有死锁的可能：栈内元素是用户代码，若该元素在构造或析构时修改栈则可能发生死锁，当然这种设计本身就有一定问题，应该从设计本身下手
*/


#ifndef THREADSAFE_STACK_H 
#define THREADSAFE_STACK_H 
#include <exception>
#include <memory>
#include <mutex>
#include <stack>
struct empty_stack: std::exception {
    const char* what() const throw() {
        return "empty stack!";
    };
};

template<typename T>
class threadsafe_stack {
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack()
        : data(std::stack<T>()) {}
    
    threadsafe_stack(const threadsafe_stack& other) {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data; // 1 在构造函数体中的执行拷贝
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;
    
    void push(T new_value) {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }

    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack(); // 在调用pop前，检查栈是否为空
        std::shared_ptr<T> const res(std::make_shared<T>(data.top())); // 在修改堆栈前，分配出返回值
        data.pop();
        return res;
    }

    void pop(T& value) {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        value = data.top();
        data.pop();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

#endif	// THREADSAFE_STACK_H