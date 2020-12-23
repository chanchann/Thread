/**
 * Thread.cpp
 */

/*
在线程函数中通过在创建线程时（调用 CreateThread 或 pthread_create 方法）时，
将当前对象的 this 指针作为线程的函数的唯一参数传入，
这样在线程函数中，可以通过线程函数参数得到对象的指针，通过这个指针就可以自由访问类的实例方法了。
这一技巧非常常用，它广泛地用于各类开源 C++ 项目或者实际的商业 C++ 项目中

*/
#include "Thread.h"


void* CThread::_ThreadEntry(void* pParam) {
    CThread *pThread = (CThread *)pParam;
    if(pThread->InitInstance()) {
        pThread->Run();
    }
    pThread->ExitInstance();
    return NULL;
}

CThread::CThread() {
    m_hThread = (THREAD_HANDLE)0;
    m_IDThread = 0;
}

CThread::~CThread() {}

bool CThread::Create() {
    if (m_hThread != (THREAD_HANDLE)0) {
        return true;
    }
    bool ret = true;
    ret = (::pthread_create(&m_hThread,NULL,&_ThreadEntry , this) == 0);
    return ret;
}

bool CThread::InitInstance() {
    return true;
}

void CThread::ExitInstance() {}

void CThread::OSSleep(int seconds) {
    ::sleep(seconds);
}

void CThread::SleepMs(int nMilliseconds) {
    ::usleep(nMilliseconds);
}

bool CThread::IsCurrentThread() {
    return ::pthread_self() == m_hThread;
}

bool CThread::Join() {    
    THREAD_HANDLE hThread = GetHandle();
    if(hThread == (THREAD_HANDLE)0) {
        return true;
    }
    return (pthread_join(hThread, NULL) == 0);

}

void CThread::ExitThread() {}


