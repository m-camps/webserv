cmake_minimum_required(VERSION 3.23)
project(custom-main-tests)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED(true))

target_include_directories(${PROJECT_NAME} PRIVATE ${YOUR_DIRECTORY}/inc)
include_directories(${YOUR_DIRECTORY}/inc)

include_directories(inc/webserv.h)

add_subdirectory(lib/Catch2-3.1.1)
add_executable(${PROJECT_NAME}
        src/ThrowTests.cpp
        ../src/Location.cpp
        ../src/Network.cpp
        ../src/Parse.cpp
        ../src/Server.cpp
        ../src/Exchange/Exchange.cpp
        ../src/Exchange/Request.cpp
        ../src/Exchange/Respond.cpp
        ../src/Utilities/defaultStatusPage.cpp
        ../src/Utilities/deleteFile.cpp
        ../src/Utilities/findCharLocation.cpp
        ../src/Utilities/readFile.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2)