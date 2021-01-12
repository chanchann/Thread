## 操作系统层面上关于多线程和多线程协作的接口原理

线程是操作系统的内核对象，当多线程编程时，如果线程数过多，就会导致频繁地上下文切换，这些对性能是一种额外的损耗。例如，在一些高并发的网络服务器编程中，使用一个线程服务一个 socket 连接是很不明智的，因此现在的主流做法是利用操作系统提供了基于事件模式的异步编程模型，用少量的线程来服务大量的网络连接和 IO。但是采用异步和基于事件的编程模型，让程序代码变得复杂，非常容易出错，也提高了排查错误的难度。

协程，是在应用层模拟的线程，它避免了上下文切换的额外损耗，同时又兼顾了多线程的优点，简化了高并发程序的复杂度。还是以上面的高并发网络服务器为例，可以为每一个 socket 连接使用一个协程来处理，在兼顾性能的情况下，代码也清晰。

## 排查 Linux 进程 CPU 使用率过高问题

pstack pid

top      -- 进程

top -H   -- 线程

## 线程 ID 的用途及原理

ps -efL | grep test

解决pstack不输出

1. shell脚本

2. sudo

## 如何等待线程结束

## TODO 多线程读写文件

## std::condition_variable(), unlock mutex before notfifying waiting thread(s)?

## What's the difference between notify_all() and notify_one() of std::condition_variable?

https://stackoverflow.com/questions/9015748/whats-the-difference-between-notify-all-and-notify-one-of-stdcondition-va

https://blog.csdn.net/brahmsjiang/article/details/102804756

## 虚假唤醒