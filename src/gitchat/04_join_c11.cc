/*
My version c++11 join demo
*/
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <sstream>


void fileThreadFunc(std::string filename) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss; // convert to string
    ss << std::put_time(std::localtime(&now), "%x %X");
    std::string time_str = ss.str();

    // if not exists, create, if exists, override
    std::ofstream fwrite(filename, std::ios::out | std::ios::trunc);
    if(!fwrite.is_open()) {
        std::thread::id thread_id = std::this_thread::get_id();
        ss << thread_id;
        std::string tid = ss.str();
        ss << "Thread " << tid << " failed to open file: " << filename << "\n";
        std::cout << ss.str() << "\n";
        return;
    }
    // write data
    fwrite << time_str;
    fwrite.close();
}

void fileRead(std::string filename) {
    std::string time_content;
    std::ifstream fread(filename, std::ios::in);
    if(!fread.is_open()) {
        std::cout << "Reading error" << std::endl;
        return;
    }
    while(fread.good()) {
        getline(fread, time_content);
        std::cout << time_content << std::endl;
    }
    fread.close();
}

int main(int argc, char* argv[]) {
    std::string filename;
    if(argc > 1) {
        filename = argv[1];
    } else{
        filename  = "timecpp11.txt";
    }
    std::thread fwrite(fileThreadFunc, filename);
    if(fwrite.joinable()) fwrite.join();
    fileRead(filename);
    return 0;
}