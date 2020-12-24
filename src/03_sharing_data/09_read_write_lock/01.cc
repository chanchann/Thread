/*
有些数据（比如缓存中存放的DNS入口表）需要经常访问但更新频率很低，如果用std::mutex保护数据有些过度（大量读的操作也会因锁而影响性能），
这就需要用上读写锁（reader-writer mutex），它允许多个线程并发读但仅一个线程写

C++17提供了std::shared_mutex和std::shared_timed_mutex（C++14），
后者比前者提供了更多操作，但前者性能更高。C++11没有提供读写锁，为此可使用boost::shared_mutex
读写锁并不是万能的，其性能与处理器数量及读写线程的负载有关

C++14提供了std::shared_lock，用法和std::unique_lock相同，
此外std::shared_lock还允许多线程同时获取共享锁，因此一般用std::shared_lock锁定读，std::unique_lock锁定写


class A {
private:
    mutable std::shared_mutex m;
    int n = 0;
public:
    int read()
    {
        std::shared_lock<std::shared_mutex> l(m);
        return n;
    }
    void write()
    {
        std::unique_lock<std::shared_mutex> l(m);
        ++n;
    }
};

*/

