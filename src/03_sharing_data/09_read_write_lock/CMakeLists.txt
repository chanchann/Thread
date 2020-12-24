
cmake_minimum_required(VERSION 2.8)
project(rwlock)
 
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
 
find_package(Boost REQUIRED COMPONENTS thread)

if(NOT Boost_FOUND)
    message("Not found Boost")
endif()
 
include_directories(${Boost_INCLUDE_DIRS})
message("${Boost_INCLUDE_DIRS}")
message("${Boost_LIBRARIES}")
 
add_executable(rwlock 05.cc)
target_link_libraries(rwlock ${Boost_LIBRARIES})