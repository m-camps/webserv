cmake_minimum_required(VERSION 3.23)
project(webserv)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED(true))

set(INC_HEADERS ./inc)
set(EXCHANGE_HEADERS ./src/Exchange)

include_directories(${INC_HEADERS})
include_directories(${EXCHANGE_HEADERS})

set(SOURCES
${INC_HEADERS}/Constants.hpp
${INC_HEADERS}/Location.hpp
${INC_HEADERS}/Network.hpp
${INC_HEADERS}/Parse.hpp
${INC_HEADERS}/Server.hpp
${INC_HEADERS}/Utilities.hpp
${INC_HEADERS}/Generator.hpp
${EXCHANGE_HEADERS}/Exchange.hpp
${EXCHANGE_HEADERS}/Respond.hpp
${EXCHANGE_HEADERS}/Request.hpp

src/Location.cpp
src/main.cpp
src/Network.cpp
src/Parse.cpp
src/Server.cpp
src/Exchange/Exchange.cpp
src/Exchange/Request.cpp
src/Exchange/Respond.cpp
src/Utilities/defaultStatusPage.cpp
src/Utilities/deleteFile.cpp
src/Utilities/findCharLocation.cpp
src/Utilities/readFile.cpp
src/Generator.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
