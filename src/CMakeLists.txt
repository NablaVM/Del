cmake_minimum_required(VERSION 3.8)

project(DelLang)

option(COMPILE_TESTS        "Execute compile-time tests"     ON)

include(${CMAKE_SOURCE_DIR}/cmake/FindCppuTest.cmake)

#-------------------------------------------------
#   Nabla Version Information
#-------------------------------------------------

include(${CMAKE_SOURCE_DIR}/cmake/DelVersion.cmake)

#-------------------------------------------------
#   Platform Information
#-------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register -Wno-unused-function")

find_package(libnabla REQUIRED)

find_package(Threads)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(${LIBNABLA_INCLUDE_DIRS})

#-------------------------------------------------
#   Compilers
#-------------------------------------------------

set(DEL_COMPILER_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/del
)

include(${DEL_COMPILER_DIR}/del.cmake)

include_directories(
    ${DEL_COMPILER_DIR}
    ${DEL_INCLUDE_DIRS}
)

#-------------------------------------------------
#   Executable
#-------------------------------------------------

add_executable(del
        main.cpp
        ${DEL_COMPILER_SOURCES}
)

target_link_libraries(del
    PRIVATE 
        ${LIBNABLA_LIBRARIES}
        Threads::Threads
)

#-------------------------------------------------
#   Executable
#-------------------------------------------------

add_custom_target(copy-del-examples ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/examples ${CMAKE_BINARY_DIR}/examples
)
