/*
std::future是一个非常有用也很有意思的东西，简单说std::future提供了一种访问异步操作结果的机制。
从字面意思来理解，它表示未来，我觉得这个名字非常贴切，因为一个异步操作我们是不可能马上就获取操作结果的，只能在未来某个时候获取
但是我们可以以同步等待的方式来获取结果，可以通过查询future的状态（future_status）来获取异步操作的结果。
future_status有三种状态：

deferred：异步操作还没开始
ready：异步操作已经完成
timeout：异步操作超时

std::future_status status;
    do {
        status = future.wait_for(std::chrono::seconds(1));
        if (status == std::future_status::deferred) {
            std::cout << "deferred\n";
        } else if (status == std::future_status::timeout) {
            std::cout << "timeout\n";
        } else if (status == std::future_status::ready) {
            std::cout << "ready!\n";
        }
    } while (status != std::future_status::ready); 

获取future结果有三种方式：get、wait、wait_for，
其中get等待异步操作结束并返回结果，wait只是等待异步操作完成，没有返回值，wait_for是超时等待返回结果。
*/

