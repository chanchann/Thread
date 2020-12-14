/*
虽然get_id()可以获取线程的ID，但是其返回类型是thread::id，
通过std::cout可以输出线程ID，但是这样使用似乎不太方面，要是能转换为整形就好了。
其实可以将得到的线程ID写入到ostreamstring流中，转换成string类型，再转换成整形
*/

#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <sstream>

class A {
public:
    void threadfunc() {
        std::cout << "bind thread func" << std::endl;
    }
};

int main() {
    A a;
    std::thread t1(std::bind(&A::threadfunc, &a));
    
    std::ostringstream os1;
    os1 << t1.get_id() << std::endl;
    std::string strID = os1.str();            //转换成string类型
    unsigned long int threadID = std::stoul(strID);       //转换成unsigned long int类型
    
    std::cout << "t1 thread ID is : " << threadID << std::endl;

    t1.join();
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));//睡眠1000毫秒
        break;
    }
    return 0;
}