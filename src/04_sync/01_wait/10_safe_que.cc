/*
用条件变量实现线程安全的queue
https://downdemo.gitbook.io/cpp-concurrency-in-action-2ed/3.-tong-bu-bing-fa-cao-zuo-synchronizing-concurrent-operation/yong-tiao-jian-bian-liang-shi-xian-xian-cheng-an-quan-de-queue
*/

#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class A {
    mutable std::mutex m; // 必须可变
    std::queue<T> q;
    std::condition_variable cv;
public:
    A() {}
    A(const A& rhs)
    {
        std::lock_guard<std::mutex> l(rhs.m);
        q = rhs.q;
    }

    void push(T x)
    {
        std::lock_guard<std::mutex> l(m);
        q.push(std::move(x));
        cv.notify_one();
    }

    void wait_and_pop(T& x)
    {
        std::unique_lock<std::mutex> l(m);
        cv.wait(l, [this] { return !q.empty(); });
        x = std::move(q.front());
        q.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> l(m);
        cv.wait(l, [this] { return !q.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(std::move(q.front())));
        q.pop();
        return res;
    }

    bool try_pop(T& x)
    {
        std::lock_guard<std::mutex> l(m);
        if (q.empty()) return false;
        x = std::move(q.front());
        q.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> l(m);
        if (q.empty()) return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(std::move(q.front())));
        q.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> l(m);
        // 其他线程可能有此对象（拷贝构造）所以要上锁
        return q.empty();
    }
};