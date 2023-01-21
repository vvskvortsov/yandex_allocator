cmake_minimum_required(VERSION 3.5)

project(yandex_allocator LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(cpputest)



add_executable(yandex_allocator pool_allocator.c pool_allocator.h main.cpp pool_allocator_cpp.h pool_allocator_config.h test.cpp port/port_lock.cpp)


target_include_directories(yandex_allocator  PRIVATE  ${CppUTest_INCLUDE_DIRS})
target_link_libraries(yandex_allocator  PRIVATE  ${CppUTest_LIBRARIES})
