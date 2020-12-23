/**
 * Thread.h
 */

#include <pthread.h>
#include <unistd.h>
typedef pthread_t THREAD_HANDLE ;
/**定义了一个线程对象
*/
class  CThread {
public:
    /**构造函数
    */
    CThread();

    /**析构函数
    */
    virtual ~CThread();

    /**创建一个线程
    * @return true:创建成功 false:创建失败
    */
    virtual bool Create();

    /**获得本线程对象存储的线程句柄
    * @return 本线程对象存储的线程句柄线程句柄
    */
    THREAD_HANDLE GetHandle();

    /**线程睡眠 seconds 秒
    * @param seconds 睡眠秒数
    */
    void OSSleep(int nSeconds);

    void SleepMs(int nMilliseconds);

    bool Join();

    bool IsCurrentThread();

    void ExitThread();

private:    
    static void* _ThreadEntry(void* pParam);


    /**虚函数，子类可做一些实例化工作
    * @return true:创建成功 false:创建失败
    */
    virtual bool InitInstance();

    /**虚函数，子类清楚实例
    */
    virtual void ExitInstance();

    /**线程开始运行，纯虚函数，子类必须继承实现
    */
    virtual void Run() = 0;

private:
    THREAD_HANDLE  m_hThread;  /**< 线程句柄 */
    unsigned int   m_IDThread;

};

