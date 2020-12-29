#include <iostream>
#include <string>
#include <thread>
#include <list>
#include <mutex>
 #include <condition_variable>

class MesProcess {
public:
	void Inmsglist() {
		for (int i = 0; i < 10000; i++) {
			std::unique_lock<std::mutex> uniqlock(my_mutex);
			std::cout << "Inmsglist线程正在执行，插入数字为：" << i << std::endl;
			msglist.push_back(i);
			my_cond.notify_one();  //尝试把wait和线程唤醒，执行完这里wait就会被唤醒
			//如果添加线程Outmsg_thread2则需要将此处更换为notify_all更稳定
		}
	}
 
	
	void Outmsglist() {
		int command = 0;
		while (true) {
			std::unique_lock < std::mutex > uniqlock(my_mutex);
			my_cond.wait(uniqlock, [this] {           //这里的判断条件为lambda表达式，判断信息队列是否为空，为空返回false，否则返回true
				if (!msglist.empty())                 //调用wait函数
					return true;
				return false;
			});
			command = msglist.front();
			msglist.pop_front();
			std::cout << "Outmsglist线程"<<std::this_thread::get_id()<<"正在执行，取出数字为：" << command << std::endl;
			uniqlock.unlock();
			
		}
	}
private:
	std::list<int> msglist;
	std::mutex my_mutex;
	std::condition_variable my_cond;   //生成一个条件变量对象
};
 
int main() {
	std::cout << "主线程的线程id为： " << std::this_thread::get_id() << std::endl;
 
	MesProcess mpobj;
	std::thread Outmsg_thread(&MesProcess::Outmsglist, &mpobj);
	//std::thread Outmsg_thread2(&MesProcess::Outmsglist, &mpobj);
	std::thread Inmsg_thread(&MesProcess::Inmsglist, &mpobj);
 
	Inmsg_thread.join();
	Outmsg_thread.join();
	//Outmsg_thread2.join();
 
	std::cout << "主线程运行结束" << std::endl;
	return 0;
}