/*
一个线程加锁，其他线程再次加锁的效果：

gdb test
r
^C
bt
inf threads   --> " __lll_lock_wait () at

如果互斥体的属性是 PTHREAD_MUTEX_ERRORCHECK，当前线程重复调用 pthread_mutex_lock 会直接返回 EDEADLOCK，其他线程如果对这个互斥体再次调用 pthread_mutex_lock 会阻塞在该函数的调用处。
*/

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

pthread_mutex_t mymutex;

void* worker_thread(void* param) {
    pthread_t threadID = pthread_self();

    printf("thread start, ThreadID: %lu\n", threadID);

    while (true) {
        int ret = pthread_mutex_lock(&mymutex);
        if (ret == EDEADLK) {
            printf("EDEADLK, ThreadID: %lu\n", threadID);
        } 
        else
            printf("ret = %d, ThreadID: %lu\n", ret, threadID);
        //休眠1秒
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&mymutex, &mutex_attr);

    int ret = pthread_mutex_lock(&mymutex);
    printf("ret = %d\n", ret);

    //创建5个工作线程
    pthread_t threadID[5];
    for (int i = 0; i < 5; ++i) {
        pthread_create(&threadID[i], NULL, worker_thread, NULL);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_join(threadID[i], NULL);
    }

    pthread_mutex_destroy(&mymutex);
    pthread_mutexattr_destroy(&mutex_attr);

    return 0;
}

