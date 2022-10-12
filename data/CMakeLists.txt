cmake_minimum_required(VERSION 3.22)
project(../webserv)

include_directories(../inc/Parse.hpp)
include_directories(../inc/Server.hpp)
include_directories(../inc/Utilities.hpp)

include_directories(../src/Exchange/Exchange.hpp)
include_directories(../src/Exchange/Request.hpp)
include_directories(../src/Exchange/Respond.hpp)


set(CMAKE_CXX_STANDARD 98)

set(
        SOURCE_FILES

)

add_executable(../webserv
        ../src/*.cpp)