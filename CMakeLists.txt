cmake_minimum_required(VERSION 2.8.9)
project(homeAutomator)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

find_package (Threads)

#Bring the headers, such as Student.h into the project
include_directories(include)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cpp")

add_executable(homeAutomator ${SOURCES})
target_link_libraries (main ${CMAKE_THREAD_LIBS_INIT})
