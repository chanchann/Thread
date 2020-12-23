/*
std::this_thread::yield：　当前线程放弃执行，操作系统调度另一线程继续执行。
即当前线程将未使用完的“CPU时间片”让给其他线程使用，等其他线程使用完后再与其他线程一起竞争"CPU"。

这里测试stack在线程中不安全

*/

#include <iostream>
#include <stack>
#include <thread>
#include <vector>
#include <algorithm>

std::stack<int> st;
std::vector<int> v;

void add_data() {
	int loop_count = 30000;
	for (int i = 0; i < loop_count; ++i) {
		st.push(i);
	}
}

void show_data() {
	while (!st.empty()){    // 1
		int const value = st.top();    // 2
		st.pop();    // 3
		// do_something(value);
		// std::cout << value << std::endl;
		v.push_back(value);
	}
}

int main() {
	auto t1 = std::thread(add_data);
	t1.join();
	auto t2 = std::thread(show_data);
	auto t3 = std::thread(show_data);

	t2.join();
	t3.join();
	std::reverse(v.begin(), v.end());
	for(int i = 0; i < v.size(); i++) {
		if(i != v[i]) {
			std::cout << "wrong " << std::endl;
			break;
		}
	}
	return 0;
}