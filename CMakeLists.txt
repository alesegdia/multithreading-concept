cmake_minimum_required(VERSION 2.4)

project(multithreading)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g")

find_package (Threads)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

add_executable(multithreading ${SOURCES})
target_link_libraries(multithreading ${CMAKE_THREAD_LIBS_INIT})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)
