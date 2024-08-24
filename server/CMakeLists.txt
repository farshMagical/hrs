cmake_minimum_required(VERSION 3.22)

set(TARGET hrs_server) 

set(CMAKE_BUILD_TYPE "Debug")

add_executable(${TARGET} hrs_server.cpp)
target_link_libraries(${TARGET} PRIVATE fmt::fmt Boost::beast)
