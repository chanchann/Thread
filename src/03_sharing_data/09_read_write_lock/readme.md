## 

https://blog.csdn.net/wilson1068/article/details/100766622

## Problems

1. cannot find -lboost_system

/sbin/ldconfig -p | grep boost_system | cut -d\> -f2

sudo ln -s  /usr/lib/x86_64-linux-gnu/libboost_system.so.1.58.0 /usr/lib/libboost_system.so

ls -la /usr/lib/ | grep boost
2. 

locate boost_thread

