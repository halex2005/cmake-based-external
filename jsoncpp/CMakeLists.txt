CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (SOURCES jsoncpp.cpp)

add_library(jsoncpp ${SOURCES})
