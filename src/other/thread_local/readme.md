## thread_local

## What is the performance penalty of C++11 thread_local variables in GCC 4.8?

Unless you have some extern __thread variables, you could replace all __thread by thread_local without any loss of performance.

https://stackoverflow.com/questions/13106049/what-is-the-performance-penalty-of-c11-thread-local-variables-in-gcc-4-8

## 

http://cifangyiquan.net/programming/thread_local/