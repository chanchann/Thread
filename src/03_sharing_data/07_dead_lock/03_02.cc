#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <list>
#include <map>
#include <string>

using namespace std;

class A{
public:
	std::unique_lock<std::mutex> rtn_unique_lock(){
		std::unique_lock<std::mutex> tmpguard(my_mutex1);
		return tmpguard; //从函数返回一个局部的unique_lock对象时可以的。
						//返回这种局部对象tmpguard会导致系统生成临时unique_lock对象，并调用unique_lock的移动构造函数
	}


	//把收到的消息 （玩家命令） 入到一个队列的线程

	void inMsgRecvQueue(){
		for (int i = 0; i < 10000; ++i) {
			cout << "inMsgRecvQueue() 执行，插入一个元素" << i << endl;
			//std::lock_guard<std::mutex> sbguar(my_mutex);  
			// my_mutex1.lock();
			std::unique_lock<std::mutex> sbguard1(my_mutex1, std::try_to_lock);
			if (sbguard1.owns_lock())
			{
				//拿到了锁
				msgRecvQueue.push_back(i); //假设这个数字i 就是我收到的命令， 我直接弄到消息队列里边来；
				//其他处理代码
			}
			else{
				//没拿到锁
				cout << "inMsgRecvQueue() 执行，但没有拿到锁，只能干点别的事" << i << endl;
			}
		}
	}

	void inMsgRecvQueue1(){
		for (int i = 0; i < 10000; ++i) {
			cout << "inMsgRecvQueue() 执行，插入一个元素" << i << endl;

			std::unique_lock<std::mutex> sbguard1(my_mutex1, std::defer_lock); //没有加锁的my_mutex1
			//sbguard1.lock(); //咱们不用自己unlock
			//
			//处理共享代码
			//
			//sbguard1.unlock();
			//.... 处理一些非共享代码
			//sbguard1.lock();

			if (sbguard1.try_lock() == true) //返回true 表示拿到了锁了
			{
				//拿到了锁
				msgRecvQueue.push_back(i); //假设这个数字i 就是我收到的命令， 我直接弄到消息队列里边来；
				//其他处理代码
				//sbguard1.unlock(); //画蛇添足，但也可以
			}
			else{
				//没拿到锁
				cout << "inMsgRecvQueue() 执行，但没有拿到锁，只能干点别的事" << i << endl;
			}
		}
	}

	void inMsgRecvQueue2(){
		for (int i = 0; i < 10000; ++i) {
			cout << "inMsgRecvQueue() 执行，插入一个元素" << i << endl;

			std::unique_lock<std::mutex> sbguard1(my_mutex1);

			//std::unique_lock<std::mutex> sbguard2(sbguard1); //复制所有权是非法的
			//std::unique_lock<std::mutex> sbguard2(std::move(sbguard1)); //移动语义，现在相当于sbguard2和my_mutex1绑定到一起了， 
																		//现在sbguard1指向空, sbguard2指向了my_mutex1
			std::mutex *ptx = sbguard1.release();// 现在你有责任自己解锁这个my_mutex1；

			msgRecvQueue.push_back(i); //假设这个数字i 就是我收到的命令， 我直接弄到消息队列里边来；
			ptx->unlock(); //自己负责mutex的unlock了
		}
	}



	bool outMsgLULProc(int &command){

		//std::lock_guard<std::mutex> sbguar(my_mutex);
		std::unique_lock<std::mutex> sbguard1(my_mutex1);

		std::chrono::milliseconds dura(2000); //1秒 = 1000毫秒
		std::this_thread::sleep_for(dura); //休息一定的时长


		if (!msgRecvQueue.empty()) {
			//消息不为空
			command = msgRecvQueue.front(); //返回第一个元素，但不检查元素是否存在；
			msgRecvQueue.pop_front(); //移除第一个元素，但不返回
			return true;
		}
		return false;
	}

	//把数据从消息队列中取出的线程
	void outMsgRecvQueue(){
		int command = 0;
		for (int i = 0; i < 10000; ++i)
		{
			bool result = outMsgLULProc(command);
			if (result == true){
				cout << "outMsgRecvQueue()执行，取出一个元素 " << command << endl;
				//可以考虑进行命令（数据）处理
				//.....
			}
			else{
				//消息队列为空
				cout << "outMsgRecvQueue() 执行，但目前消息队列中为空" << i << endl;
			}
		}
		cout << "end" << endl;
	}

private:

	std::list<int> msgRecvQueue; //容器， 专门用于代表玩家给咱们发生过来的命令
	std::mutex my_mutex; //创建了一个互斥量
	std::mutex my_mutex1;
	std::mutex my_mutex2;
};


int main()
{
	A myobja;
	std::thread myOutMsgObj(&A::outMsgRecvQueue, &myobja); //第二个参数是引用，才能保证线程里 用的是同一个对象。
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
	myOutMsgObj.join();
	myInMsgObj.join();


	//一：unique_lock 取代 lock_guard
	//unique_lock是个类模板，工作中，一般lock_guard(推荐使用)；lock_guard取代了mutex的lock()和unlock();
	//unique_lock比lock_guard灵活很多，效率上差一点，内存占用多一点。

	//二：unique_lock 的第二个参数
	//lock_guard可以带第二个参数：
	//std::lock_guard<std::mutex> abguard1(my_mutex1, std::adopt_lock); //adopt_lock标记作用；
	
	//(2.1) std::adopt_lock： 表示这个互斥量已经被lock了（你必须要把互斥量提前lock了，否则会报异常）
	//std::adopt_lock 标记的效果就是 “假设调用方 线程已经拥有了互斥的所有权（已经lock（）成功了）；”
	//通知lock_guard不需要再构造函数中lock这个互斥量了；
	//unique_lock也可以带std::adopt_lock标记，含义相同，就是不希望再unique_lock()的构造函数中lock这个mutex。
	//用这个adopt_lock的前提是，你需要自己先把mutex先lock上；

	//(2.2) std::try_to_lock
	//我们会尝试用mutex的lock去锁定这个mutex,但如果没有锁定成功，我们也会立即返回，并不会阻塞在那里；
	//用这个try_to_lock的前提是你自己不能先去lock。

	//(2.3) std::defer_lock
	//用这个defer_lock的前提是 你不能自己先lock,否则会报异常。
	//defer_lock的意思 就是 并没有给mutex加锁： 初始化了一个没有加锁的mutex。
	//我们借着defer_lock的话题，来介绍一些unique_lock 的重要成员函数

	//三：unique_lock 的成员函数
		//(3.1) lock()  加锁 
		//(3.2) unlock() 解锁
		//(3.3) try_lock() 尝试给互斥量加锁，如果拿不到锁，则返回false,如果拿到了锁，返回true, 这个函数不阻塞的；
		//(3.4) release()，返回它所管理的mutex对象指针，并释放所有权，也就是说，这个unique_lock和mutext不再有关系。
		//严格区分unlock（）和release（）的区别，不要混淆。
		//如果原来mutex对象处于加锁状态，你有责任接管过来并负责解锁。（release返回的是原始mutext）
		//为什么有时候需要unlock（），因为你lock锁住的代码段越少，执行越快，整个程序运行效率越高。
		//有人也把锁头锁住的代码多少 称为锁的粒度，粒度一般用粗细来描述；
		//a）锁住的代码少，这个粒度叫细。执行效率高；
		//b）锁住的代码多，粒度叫粗，那执行效率就低；
		//要学会尽量选择合适粒度的代码进行保护，粒度太细，可能漏掉共享数据的保护，粒度太粗，影响效率。
		//选择合适的粒度，是高级程序员的能力和实力的体现；


	//四：unique_lock 所有权的传递
	//std::unique_lock<std::mutex> sbguard1(my_mutex1); //所有权概念
	//sbguard1拥有my_mutex1的所有权
	//sbguard1可以把自己对mutex(my_mutex1)的所有权转移给其他的unique_lock对象；
	//所以，unique_lock对象这个mutex的所有权是属于 可以转移，但是不能复制。 

	//a)std::move
	//b)return std::unique_lock<std::mutex>

	system("pause");
}

