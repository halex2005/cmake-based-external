project(pugixml)

cmake_minimum_required(VERSION 2.6)
set(PUGIXML_BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared instead of static library")

set(HEADERS pugixml.hpp pugiconfig.hpp)
set(SOURCES ${HEADERS} pugixml.cpp)

if(PUGIXML_BUILD_SHARED_LIBS)
    add_library(pugixml SHARED ${SOURCES})
else()
    add_library(pugixml STATIC ${SOURCES})
endif()

set_target_properties(pugixml PROPERTIES VERSION 1.4 SOVERSION 1)
