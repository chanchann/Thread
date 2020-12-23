/*
https://blog.csdn.net/u012507022/article/details/85909567

- std::adopt_lock

不希望再unique_lock()的构造函数中lock这个mutex。
用std::adopt_lock的前提是，自己需要先把mutex lock上；用法与lock_guard相同

- std::try_to_lock

我们会尝试用mutex的lock()去锁定这个mutex,但如果没有锁定成功，我也会立即返回，并不会阻塞在那里；

用这个try_to_lock的前提是你自己不能先lock

- std::defer_lock
用std::defer_lock的前提是，你不能自己先lock,否则会报异常
std::defer_lock的意思就是并没有给mutex加锁：初始化了一个没有加锁的mutex。
*/


#include<iostream>
#include<thread>
#include<string>
#include<vector>
#include<list>
#include<mutex>
 
using namespace std;
 
class A {
public:
	void inMsgRecvQueue() {
		for (int i = 0; i < 10000; i++) {
			cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl; 
            { 
				std::unique_lock<std::mutex> sbguard(my_mutex, std::try_to_lock);
				if (sbguard.owns_lock()) {
					//拿到了锁
					msgRecvQueue.push_back(i); 
					//...
					//其他处理代码
				} else {
					//没拿到锁
					cout << "inMsgRecvQueue()执行，但没拿到锁头，只能干点别的事" << i << endl;
				}
			}
		}
	}
 
	bool outMsgLULProc(int &command) {
		my_mutex.lock();//要先lock(),后续才能用unique_lock的std::adopt_lock参数
		std::unique_lock<std::mutex> sbguard(my_mutex, std::adopt_lock);
 
		std::chrono::milliseconds dura(2000);
		std::this_thread::sleep_for(dura);  //休息2s
 
		if (!msgRecvQueue.empty()) {
			//消息不为空
			int command = msgRecvQueue.front();//返回第一个元素，但不检查元素是否存在
			msgRecvQueue.pop_front();//移除第一个元素。但不返回；
			
			return true;
		}
		return false;
	}
	//把数据从消息队列取出的线程
	void outMsgRecvQueue() {
		int command = 0;
		for (int i = 0; i < 10000; i++) {
			bool result = outMsgLULProc(command);
 
			if (result == true) {
				cout << "outMsgRecvQueue()执行，取出一个元素" << endl;
				//处理数据
			}
			else
			{
				//消息队列为空
				cout << "inMsgRecvQueue()执行，但目前消息队列中为空！" << i << endl;
			}
		}
		cout << "end!" << endl;
	}
 
private:
	std::list<int> msgRecvQueue;//容器（消息队列），代表玩家发送过来的命令。
	std::mutex my_mutex;//创建一个互斥量（一把锁）
};
 
int main() {
	A myobja;
 
	std::thread myOutMsgObj(&A::outMsgRecvQueue, &myobja);
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
 
	myOutMsgObj.join();
	myInMsgObj.join();
 
	cout << "主线程执行！" << endl;
 
	return 0;
}