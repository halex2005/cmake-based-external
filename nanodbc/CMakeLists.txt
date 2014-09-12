CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

if(NANODBC_USE_CPP11)
    message(STATUS "C++11 support: Enabled")
    add_definitions(-DNANODBC_USE_CPP11 --std=c++11)
else()
    message(STATUS "C++11 support: Disabled")
    add_definitions(--std=c++03)
endif()

if(NANODBC_USE_UNICODE)
    message(STATUS "Unicode support: Enabled")
    add_definitions(-DNANODBC_USE_UNICODE)
else()
    message(STATUS "Unicode support: Disabled")
endif()

set (SOURCES
    nanodbc.h
    nanodbc.cpp
    utf8/utf8.h
    utf8/utf8/checked.h
    utf8/utf8/core.h
    utf8/utf8/unchecked.h
)

add_library (nanodbc ${SOURCES})
target_link_libraries(nanodbc odbc)

########################################
## executable example
########################################
add_executable(nanodbc-example example.cpp)
add_dependencies(nanodbc-example nanodbc)
target_link_libraries(nanodbc-example nanodbc)