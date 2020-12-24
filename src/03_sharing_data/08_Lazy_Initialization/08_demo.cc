#include<iostream>
#include<thread>
#include<string>
#include<mutex>
#include<fstream>

class LogFile {
private:
    std::mutex m_mutex;
    std::once_flag m_flag;//once_flag标记
    std::ofstream f;
public:
    LogFile() {
        //f.open("log.txt");//不再需要！
    }
    //此种情况是：那个线程先要打印，则首次创建文件（都不需打印，则始终不创建文件）
    void shared_print(std::string id, int value) {
        std::call_once(m_flag, [&]() { f.open("log.txt"); });//call_once函数
        std::unique_lock<std::mutex> locker(m_mutex);
        f << id << ":" << value << std::endl;
        std::cout << id << ":" << value << std::endl;
    }

};

void function_1(LogFile& log) {
    for (int i = 0; i > -1000; i--)
        log.shared_print(std::string("from t1:"), i);
}

int main() {//主线程 
    LogFile log;
    std::thread t1(function_1, std::ref(log));//t1线程开始运行

    for (int i = 0; i < 1000; i++) {
        log.shared_print(std::string("from main:"), i);
    }

    t1.join();

    return 0;
}