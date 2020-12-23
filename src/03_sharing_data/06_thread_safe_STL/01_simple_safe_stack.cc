/*
即便在很简单的接口中，也可能遇到race condition

https://blog.csdn.net/u201012980/article/details/52077442
https://www.bookstack.cn/read/Cpp_Concurrency_In_Action/content-chapter3-3.2-chinese.md
*/

#include <exception>
#include <memory>
#include <mutex>
#include <stack>

struct emptyStack : std::exception {
    const char* what() const noexcept {
        return "empty stack!";
    }
};

template<typename T>
class A {
private:
    std::stack<T> s;
    mutable std::mutex m;
public:
    A() : s(std::stack<T>()) {}

    A(const A& rhs) {
        std::lock_guard<std::mutex> l(rhs.m);
        s = rhs.s;
    }

    A& operator=(const A&) = delete;

    void push(T n) {
        std::lock_guard<std::mutex> l(m);
        s.push(std::move(n));
    }

    std::shared_ptr<T> pop() { // 返回一个指向栈顶元素的指针
        std::lock_guard<std::mutex> l(m);
        if (s.empty()) throw emptyStack();
        const std::shared_ptr<T> res(std::make_shared<T>(std::move(s.top())));
        s.pop();
        return res;
    }

    void pop(T& n) {// 传引用获取结果
        std::lock_guard<std::mutex> l(m);
        if (s.empty()) throw emptyStack();
        n = std::move(s.top());
        s.pop();
    }

    bool empty() const {
        std::lock_guard<std::mutex> l(m);
        return s.empty();
    }
};


