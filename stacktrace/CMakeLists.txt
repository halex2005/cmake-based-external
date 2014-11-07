CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set (${SOURCES} call_stack.hpp stack_exception.hpp)
if (WIN32)
    list(APPEND SOURCES call_stack_msvc.cpp)
    list(APPEND SOURCES detail/StackWalker.cpp)
elseif(UNIX)
    list(APPEND SOURCES call_stack_gcc.cpp)
endif()

add_library(stacktrace ${SOURCES})
if (UNIX)
    target_link_libraries(stacktrace dl)
endif()