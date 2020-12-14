/*
虽然通过lock()与unlock()可以解决线程之间的资源竞争问题，但是这里也存在不足。

func()
{
    加锁()
    执行逻辑处理;    //如果该过程抛出异常导致程序退出了，就没法unlock
    解锁()
}

func()中再执行逻辑处理中程序因为某些原因退出了，此时就无法unlock()了，这样其他线程也就无法获取std::mutex，造成死锁现象
，其实在加锁之前可以通过trylock()尝试一下能不能加锁。实际开发中，通常也不会这样写代码，而是采用lock_guard来控制std::mutex。


*/

template <class _Mutex>
class lock_guard { 
public:
    using mutex_type = _Mutex;
​
    explicit lock_guard(_Mutex& _Mtx) : _MyMutex(_Mtx) { 
        _MyMutex.lock();     //构造函数加锁       
    }
​    // 第2个构造函数有两个参数，其中第二个参数类型为：std::adopt_lock_t。
    // 这个构造函数假定：当前线程已经上锁成功，所以不再调用lock()函数。
    lock_guard(_Mutex& _Mtx, adopt_lock_t) : _MyMutex(_Mtx) { }
​
    ~lock_guard() noexcept { 
        _MyMutex.unlock();   //析构函数解锁
    }
​
    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;
​
private:
    _Mutex& _MyMutex;
};