/*
https://my.oschina.net/SHIHUAMarryMe/blog/674661
在之前我们了解到std::future,但是通过class std::future获得的结果只能get()一次,第二次调用通常会抛出一个std::future_error。

但是当多个其他线程想处理另外一个线程的结果的时候，也就是需要多次get(),基于这个目的C++ standard library提供了 std::shared_future,于是我们就可以多次调用get(),拿到相同的结果，或者抛出(throw)同一个异常.

std::shared_future相对于std::future来说只是少了一个share()操作.别的操作基本上是与 std::future一致的.此外还有以下几点不同:

1, std::future是不能被拷贝的(但是需要注意的是std::fututre和std::shared_future一样都是支持move的).

2, std::shared_future可以多次调用get().

3, std::future可以通过std::future::shared()获得一个std::shared_future(当然这会导致std::future无效).

*/
#include <iostream>
#include <future>
#include <thread>
#include <exception>
#include <stdexcept>

int queryNumber() {
    std::cout << "enter a number: ";
    int number;
    std::cin >> number;

    if (!std::cin) {
        throw std::runtime_error(std::string("no number read!"));
    }

    return number;
}

void doSomething(const char& c, const std::shared_future<int>& f) {
    try {
      int number = f.get();

      for (int i = 0; i < number; ++i) {
          std::this_thread::sleep_for(std::chrono::duration<int, std::ratio<1, 1000>>(1));
          std::cout.put(c).flush();
      }

    } catch (const std::exception& e) {
      std::cerr << "EXCEPTION in thread" << std::this_thread::get_id() << ": " << e.what() << std::endl;
    }
}

int main() {
    try {
        std::shared_future<int> result = std::async(std::launch::async, queryNumber);
        
        std::future<void> result1 = std::async(std::launch::async, doSomething, '.', result); 
        // 注意我们在std::async()中传递的参数result其实是对result的拷贝.
        // 如果我们写成std::async(std::launch::async, '.', std::ref(result));这种情况下才是对result的引用.
        // 这样一来就是对一个std::shared_future多次调用get()了.于是就带来了风险.  --> // TODO : 为什么会带来风险??? 
        std::future<void> result2 = std::async(std::launch::async, doSomething, '+', result);
        std::future<void> result3 = std::async(std::launch::async, doSomething, '*', result);

        result1.get();
        result2.get();
        result3.get();

    } catch (const std::exception& e) {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }

    std::cout << "\ndone" << std::endl;

    return 0;
}
