cmake_minimum_required(VERSION 3.24)
project(Laboratory)

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 3 REQUIRED)

#L1
add_executable(Circles1
        L1/main.cpp)

#L2
add_executable(Circles
        L2/T1/main.cpp)

add_executable(MyString
        L2/T2/main.cpp
        L2/T2/my_string.cpp
        L2/T2/my_string.h)

add_executable(MyStringTest
        L2/T2/test.cpp
        L2/T2/my_string.h
        L2/T2/my_string.cpp)

target_link_libraries(MyStringTest PRIVATE Catch2::Catch2WithMain)

#L3
add_executable(Inheritance
        L3/main.cpp
        L3/car.h
        L3/car.cpp)

#L4
add_executable(MyList
        L4/main.cpp
        L4/list.h
        L2/T2/my_string.h
        L2/T2/my_string.cpp)

target_include_directories(MyList PRIVATE L2/T2)

add_executable(MyListTest
        L4/test.cpp
        L4/list.h)

target_link_libraries(MyListTest PRIVATE Catch2::Catch2WithMain)

#L5
add_executable(Exceptions
        L5/main.cpp
        L4/list.h
        L5/exception.cpp
        L5/exception.h)

target_include_directories(Exceptions PRIVATE L4)

#L6
add_executable(Files
        L4/list.h
        L6/System/File.cpp
        L6/System/File.h
        L6/System/Text.cpp
        L6/System/Text.h
        L6/System/Binary.cpp
        L6/System/Binary.h
        L6/Datagrams/Message.cpp
        L6/Datagrams/Message.h
        L6/Datagrams/History.cpp
        L6/Datagrams/History.h
        L6/main.cpp)

target_include_directories(Files PRIVATE L4 L6/System L6/Datagrams)

#L7
add_executable(Algorithms
        L4/list.h
        L7/algorithms.h
        L7/main.cpp)

target_include_directories(Algorithms PRIVATE L4)

#L8
add_executable(STL
        L8/main.cpp)