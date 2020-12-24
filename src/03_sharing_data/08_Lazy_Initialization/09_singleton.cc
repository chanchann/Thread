/*
class A {
public:
    static A& getInstance();
    A(const A&) = delete;
    A& operator(const A&) = delete;
private:
    A() = default;
    ~A() = default;
};

A& A::getInstance()
{
    static A instance; // 线程安全的初始化
    return instance;
}

C++11规定static变量的初始化只完全发生在一个线程中，直到初始化完成前其他线程都不会做处理，从而避免了race condition。只有一个全局实例时可以不使用std::call_once而直接用static
*/