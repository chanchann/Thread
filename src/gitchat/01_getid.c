/*
三种获取线程id方式
1. 
pthread_t tid;
pthread_create(&tid, NULL, thread_proc, NULL);

2. 
pthread_t tid = pthread_self();
方法一和方法二获取的线程 ID 结果是一样的，这是一个 pthread_t，输出时本质上是一块内存空间地址，
由于不同的进程可能有同样地址的内存块，因此方法一和方法二获取的线程 ID 可能不是全系统唯一的，一般是一个很大的数字（内存地址）。

3. 
int tid = syscall(SYS_gettid);

而方法三获取的线程 ID 是系统范围内全局唯一的，一般是一个不会太大的整数，
这个数字也是就是所谓的 LWP （Light Weight Process，轻量级进程，早期的 Linux 系统的线程是通过进程来实现的，这种线程被称为轻量级线程）的 ID。

*/

#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void* thread_proc(void* arg) {
    pthread_t* tid1 = (pthread_t*)arg;
    pthread_t tid2 = pthread_self();
    int tid3 = syscall(SYS_gettid);


    while(1) {
        printf("tid1: %ld, tid2: %ld, tid3: %d\n", *tid1, tid2, tid3);
        sleep(2);
    }
}

int main() {    
    pthread_t tid;
    pthread_create(&tid, NULL, thread_proc, &tid);

    pthread_join(tid, NULL);

    return 0;
}