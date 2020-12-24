/*
模仿建立数据库的连接，只有在放生send_data或者receive_data的时候，才去连接数据库，并且只连接了一次。即使，既调用了send_data也调用了receive_data，但是open_connection只被执行了一次，
也就是说数据库的连接只建立一次就够了，不管你是要接收，还是要发送。

std::call_once也可以用在类中

class A {
private:
    std::once_flag flag;
    void init() { ... }
public:
    void f()
    {
        std::call_once(flag, &A::init, this);
        ...
    }
};
*/

#include <mutex>
#include <thread>
#include <iostream>
class X{
private:
    int connect_detail;
    std::once_flag connect_init_flag;
    void open_connection(){
        std::cout << "open:" << connect_detail << std::endl;
        //open();
    }
public:
    X(int detail) : connect_detail(detail) {}
    void send_data() {
        std::call_once(connect_init_flag, &X::open_connection, this);
        //send();
    }
    void receive_data() {
        std::call_once(connect_init_flag, &X::open_connection, this);
        //receive();
    }
};
int main() {
    X x(10);
    x.send_data();
    x.receive_data();
}