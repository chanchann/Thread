#include <boost/thread.hpp>
typedef boost::shared_lock<boost::shared_mutex> readLock;
typedef boost::unique_lock<boost::shared_mutex> writeLock;
boost::shared_mutex						rwmutex;

void readOnly(int count) {
	while (--count > 0) {
		readLock  rdlock(rwmutex);
		std::cout << "read" << std::endl;
	}
}

void writeOnly(int count) {
	while (--count > 0) {
		writeLock  wtlock(rwmutex);
		std::cout << "write" << std::endl;
	}
}

int main() {
    int count = 100;
	boost::thread Reader1(readOnly, count);
    boost::thread Reader2(readOnly, count);
	boost::thread Writer(writeOnly, count);
	Reader1.join();
    Reader2.join();
	Writer.join();

	return 0;
}
