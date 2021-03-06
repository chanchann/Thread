/*
## 互斥体对象我们可以使用两种方式来初始化：

1.
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

2.
如果互斥量是动态分配的或者需要给互斥量设置属性，则需要使用 
int pthread_mutex_init(pthread_mutex_t* restrict mutex, 
                       const pthread_mutexattr_t* restrict attr);
    
pthread_mutex_t mymutex;
pthread_mutex_init(&mutex, NULL);

## pthread_mutex_destroy 函数来销毁

使用 PTHREAD_MUTEX_INITIALIZER 初始化的互斥量无须销毁；

不要去销毁一个已经加锁或正在被条件变量使用的互斥体对象，

如果我们遵循正确的使用 mutex 的规范，
如创建 mutex 对象后再对其加锁，加锁后才对其进行解锁操作，解锁后才做销毁操作，
那么编码时一般不用考虑 pthread_mutex_init/pthread_mutex_destroy/pthread_mutex_lock/pthread_mutex_unlock 等函数的返回值。


以下代码段演示了尝试销毁一个被锁定的 mute 对象：
*/

// g++ -g -o test 08_mutex.c -pthread
#include <pthread.h>
#include <stdio.h>
#include <errno.h>

int main() {
    pthread_mutex_t mymutex;
    pthread_mutex_init(&mymutex, NULL);
    int ret = pthread_mutex_lock(&mymutex);

    //尝试对被锁定的mutex对象进行销毁
    ret = pthread_mutex_destroy(&mymutex);
    if (ret != 0) {
        if (ret == EBUSY)
            printf("EBUSY\n");
        printf("Failed to destroy mutex.\n");
    }

    ret = pthread_mutex_unlock(&mymutex);
    //尝试对已经解锁的mutex对象进行销毁
    ret = pthread_mutex_destroy(&mymutex);
    if (ret == 0)
        printf("Succeeded to destroy mutex.\n");

    return 0;
}

/*
EBUSY
Failed to destroy mutex.
Succeeded to destroy mutex.
*/
