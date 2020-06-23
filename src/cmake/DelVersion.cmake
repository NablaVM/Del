#
#   Create a version string
#
set(DEL_VERSION_MAJOR 0)
set(DEL_VERSION_MINOR 1)
set(DEL_VERSION_PATCH 0)

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
    set(DEL_BUILD_TYPE "Release")
else()
    set(DEL_BUILD_TYPE "Debug")
endif()

set(DEL_VERSION_INFO "${DEL_VERSION_MAJOR}.${DEL_VERSION_MINOR}.${DEL_VERSION_PATCH}-${DEL_BUILD_TYPE}")

#
#   Add definitions so application can get the information
#
add_definitions(-DDEL_VERSION_INFO="${DEL_VERSION_INFO}")