/*
对于互斥体的加锁和解锁操作我们一般使用以下三个函数：

int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_trylock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);

设置互斥体对象的属性需要创建一个 pthread_mutexattr_t 类型的对象，

int pthread_mutexattr_init(pthread_mutexattr_t* attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t* attr);

设置或获取想要的属性类型：

int pthread_mutexattr_settype(pthread_mutexattr_t* attr, int type);
int pthread_mutexattr_gettype(const pthread_mutexattr_t* restrict attr, int* restrict type);

mutex 锁的类型

PTHREAD_MUTEX_NORMAL（普通锁）

*/

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

pthread_mutex_t mymutex;
int resourceNo = 0;

void*  worker_thread(void* param) {
    pthread_t threadID = pthread_self();

    printf("thread start, ThreadID: %lu\n", threadID);

    while (true) {
        pthread_mutex_lock(&mymutex);

        printf("Mutex lock, resourceNo: %d, ThreadID: %lu\n", resourceNo, threadID);
        resourceNo++;

        printf("Mutex unlock, resourceNo: %d, ThreadID: %lu\n", resourceNo, threadID);

        pthread_mutex_unlock(&mymutex);

        //休眠1秒
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_init(&mymutex, &mutex_attr);

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