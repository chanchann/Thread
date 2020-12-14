/*
The move support in std::thread means that ownership can readily be transferred out of a function,
Returning a std::thread from a function:
*/

#include <thread>

void some_function() {}

void some_other_function(int) {}

std::thread f() {
    void some_function();
    return std::thread(some_function);
}
std::thread g() {
    void some_other_function(int);
    std::thread t(some_other_function, 42);
    return t;
}

int main() {
    std::thread t1 = f();
    t1.join();
    std::thread t2 = g();
    t2.join();
}

/*
Likewise, if ownership should be transferred into a function, it can accept an instance
of std::thread by value as one of the parameters, as shown here:

void f(std::thread t);
void g() {
 void some_function();
 f(std::thread(some_function));
 std::thread t(some_function);
 f(std::move(t));
}
*/