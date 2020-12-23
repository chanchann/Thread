/*
即便在很简单的接口中，也可能遇到race condition

std::stack<int> s；
if (!s.empty())
{
    int n = s.top();
    s.pop();
}


在单线程中是安全的，但在多线程中，检查非空之后，如果其他线程先pop，就会导致当前线程top出错。
这是一个经典的race condition，即使用mutex也不能阻止，这就是接口固有的问题，解决方法是改变接口的设计

另一个潜在的竞争是，如果两个线程都还没pop，而是分别获取了top，虽然不会产生未定义行为，
但这种对同一值处理了两次的行为更为严重，因为看起来没有任何错误，很难定位bug

思考一个问题，既然如此，为什么不直接让pop返回栈顶元素。
原因在于，假设有一个stack<vector<int>>，拷贝vector时需要在堆上分配内存，如果系统负载严重或资源有限（比如vector有大量元素），
vector的拷贝构造函数就会抛出std::bad_alloc异常。如果pop可以返回栈顶元素值，返回一定是最后执行的语句，
stack在返回前已经弹出了元素，但如果拷贝返回值时抛出异常，就会导致弹出的数据丢失（从栈上移除但拷贝失败）。
因此std::stack的设计者将这个操作分解为top和pop两部分，但这样的分割却造成了race condition

下面思考几种把top和pop合为一步的方法。

1. 第一种方法是传入一个参数获取结果值
std::vector<int> res;
s.pop(res);
这种方式的明显缺点是，需要构造一个栈元素类型的实例，这是不现实的：
为了获取结果而临时构造一个对象并不划算、元素类型可能不支持赋值（比如用户自定义某个类型）、构造函数还需要一些参数......


2. 第二种方案是为元素类型设置不抛异常的拷贝或移动构造函数，
使用std::is_nothrow_copy_constructible和std::is_nothrow_move_constructible即可保证不抛异常，因为pop返回值时只担心该过程抛异常。
但这种方式过于局限，抛异常的构造函数还是更常见的，这些类型也希望能存入stack

3. 第三种方案是返回指向弹出元素的指针，指针可以自由拷贝且不会抛异常。
这需要管理对象的内存分配，使用std::shared_ptr是个不错的选择，但这个方案的开销太大，尤其是对于内置类型

4. 第四种方案是结合方案一二或者一三，比如结合方案一三实现一个线程安全的stack
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


