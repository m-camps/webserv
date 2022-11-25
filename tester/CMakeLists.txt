cmake_minimum_required(VERSION 3.23)
project(catch2Tester)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED(true))

set(INC_HEADERS ../inc)

include_directories(${INC_HEADERS})
include_directories(${EXCHANGE_HEADERS})

add_subdirectory(lib/Catch2-3.1.1)

set(SOURCES
        ${INC_HEADERS}/Constants.hpp
        ${INC_HEADERS}/Location.hpp
        ${INC_HEADERS}/Network.hpp
        ${INC_HEADERS}/Parse.hpp
        ${INC_HEADERS}/Server.hpp
        ${INC_HEADERS}/Utilities.hpp
        ${INC_HEADERS}/Generator.hpp
        ${INC_HEADERS}/Cgi.hpp
        ${INC_HEADERS}/Client.hpp
        ${INC_HEADERS}/Exchange.hpp
        ${INC_HEADERS}/Respond.hpp
        ${INC_HEADERS}/Request.hpp

        ../src/Location.cpp
        ../src/Network.cpp
        ../src/Parse.cpp
        ../src/Server.cpp
        ../src/Generator.cpp
        ../src/Exchange/Exchange.cpp
        ../src/Exchange/Request.cpp
        ../src/Exchange/Respond.cpp
        ../src/Exchange/Methods/ResponsePost.cpp
        ../src/Exchange/Methods/ResponseGet.cpp
        ../src/Exchange/Methods/ResponseDelete.cpp
        ../src/Cgi.cpp
        ../src/Client.cpp
        ../src/Utilities/deleteFile.cpp
        ../src/Utilities/readFile.cpp
        ../src/Utilities/isDirectory.cpp
        ../src/Utilities/ft_stoi.cpp
        ../src/Utilities/splitLineWithStrtok.cpp
        src/ThrowTests.cpp
        src/ConfTest.cpp)

add_executable(${PROJECT_NAME}
        ${SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2)