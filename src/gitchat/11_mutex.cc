/*
PTHREAD_MUTEX_ERRORCHECK（检错锁）

如果一个线程使用 pthread_mutex_lock 对已经加锁的互斥体对象再次加锁，pthread_mutex_lock 会返回 EDEADLK。


*/

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() {
    pthread_mutex_t mymutex;
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK); // **
    pthread_mutex_init(&mymutex, &mutex_attr);

    int ret = pthread_mutex_lock(&mymutex);
    printf("ret = %d\n", ret);

    ret = pthread_mutex_lock(&mymutex);
    printf("ret = %d\n", ret);
    if (ret == EDEADLK) {
        printf("EDEADLK\n");
    }

    pthread_mutex_destroy(&mymutex);
    pthread_mutexattr_destroy(&mutex_attr);

    return 0;
}
