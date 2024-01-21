cmake_minimum_required(VERSION 3.26)
project(IaDoCM)

set(CMAKE_CXX_STANDARD 23)

#L1
add_executable(L1 L1/main.cpp L1/PowerInfo.cpp L1/BatteryInfo.cpp L1/System.cpp)
target_link_libraries(L1 PRIVATE iphlpapi setupapi PowrProf)

#L4
find_package(OpenCV REQUIRED)
add_executable(L4 L4/main.cpp)
target_link_libraries(L4 PRIVATE ${OpenCV_LIBS} setupapi)

#L5
add_executable(L5 L5/main.cpp L5/Device.cpp)
target_link_libraries(L5 PRIVATE setupapi)

#L6
add_executable(L6 L6/main.cpp)
target_link_libraries(L6 PRIVATE ws2_32 bthprops)
set_property(TARGET L6 PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_compile_definitions(L6 PRIVATE LOG)