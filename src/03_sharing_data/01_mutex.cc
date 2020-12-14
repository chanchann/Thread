/*
用mutex保护共享数据

使用mutex在访问共享数据前加锁，访问结束后解锁。一个线程用特定的mutex锁定后，其他线程必须等待该线程的mutex解锁才能访问共享数据
C++提供了std::mutex来创建一个mutex，可通过std::mutex::lock加锁，通过std::unlock解锁，但一般不直接使用这两个函数
std::lock_guard是一个用std::mutex构造的RAII模板类

list是由双向链表实现的，因此内存空间是不连续的。
只能通过指针访问数据，所以list的随机存取非常没有效率，时间复杂度为o(n);
但由于链表的特点，能高效地进行插入和删除。
*/

#include <list>
#include <mutex>
#include <algorithm>

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int new_value) {
    std::lock_guard<std::mutex> guard(some_mutex); // C++17中引入了类模板实参推断，可简写为std::lock_guard guard(some_mutex);
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find) {
    std::lock_guard<std::mutex> guard(some_mutex); 
    return std::find(some_list.begin(),some_list.end(),value_to_find)
        != some_list.end();
}

#include <iostream>

int main() {
    add_to_list(42);
    std::cout << "contains(1)=" << list_contains(1) << ", contains(42)=" << list_contains(42) << std::endl;
}
