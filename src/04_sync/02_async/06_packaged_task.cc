/*
https://www.cnblogs.com/wangshaowei/p/8875501.html

*/
#include <chrono>
#include <functional>
#include <thread>
#include <future>
#include <iostream>

int Test_Fun(int a, int b, int &c) {
    // a=1,b=2,c=0
    // 突出效果，休眠2s
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // c=233
    c = a + b + 230;
    return c;
}

int main() {
    //声明一个std::packaged_task对象pt1，包装函数Test_Fun
    std::packaged_task<int(int, int, int&)> pt1(Test_Fun);
    //声明一个std::future对象fu1，包装Test_Fun的返回结果类型，并与pt1关联
    std::future<int> fu1 = pt1.get_future();

    //声明一个变量
    int c = 0;

    //创建一个线程t1，将pt1及对应的参数放到线程里面执行
    std::thread t1(std::move(pt1), 1, 2, std::ref(c)); //这必须用转移,或std::ref(pt1)
    t1.join();
    //阻塞至线程t1结束(函数Test_Fun返回结果)
    int iResult = fu1.get();

    std::cout << "执行结果：" << iResult << std::endl; //执行结果：233
    std::cout << "c：" << c << std::endl; //c：233

    return 0;
}