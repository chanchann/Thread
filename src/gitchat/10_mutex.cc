/*
一个线程如果对一个已经加锁的普通锁再次使用 pthread_mutex_lock 加锁，程序会阻塞在第二次调用 pthread_mutex_lock 

我们利用gdb查看

g++ -g -o test 10_mutex.cc -pthread
gdb test
r
^C
bt

在这种类型的情况， pthread_mutex_trylock 函数如果拿不到锁，不会阻塞，函数会立即返回，并返回 EBUSY 错误码。
*/

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() {
    pthread_mutex_t mymutex;
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
    pthread_mutex_init(&mymutex, &mutex_attr);

    int ret = pthread_mutex_lock(&mymutex);
    printf("ret = %d\n", ret);

    ret = pthread_mutex_lock(&mymutex);
    printf("ret = %d\n", ret);



    pthread_mutex_destroy(&mymutex);
    pthread_mutexattr_destroy(&mutex_attr);

    return 0;
}



