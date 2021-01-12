/*
 * @Author: Shiyu Yi
 * @Github: https://github.com/chanchann
 */
/*
1. 为什么需要使用条件变量

2. 条件变量为什么要与互斥体对象结合

解锁和等待这两个步骤必须是同一个原子性的操作，以确保 cond_wait 唤醒之前不会有其他线程获得这个互斥体对象。



*/