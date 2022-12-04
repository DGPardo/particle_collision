cmake_minimum_required(VERSION 3.22.0)
project(multi_threading VERSION 1.0)


################################################################################
# Compile time options

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic -Wmisleading-indentation")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
   message("BUILDING DEBUG MODE")
   set(CMAKE_CXX_FLAGS
       "${CMAKE_CXX_FLAGS} \
       -g \
       -fsanitize=address \
       -fsanitize=undefined \
       -fno-sanitize-recover=all \
       -fsanitize=float-divide-by-zero \
       -fsanitize=float-cast-overflow \
       -fno-sanitize=null \
       -fno-sanitize=alignment")
else()
   message("BUILDING RELEASE MODE")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()


################################################################################
# Dependencies

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                  "${CMAKE_BINARY_DIR}/conan.cmake")
endif()


include(${CMAKE_BINARY_DIR}/conan.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


################################################################################
# Global Definitions
add_compile_definitions(scalar_t=float)

# add_compile_definitions does not handle spaces
set(LABEL_T_STR "unsigned long long int")
add_compile_definitions(label_t=${LABEL_T_STR})


################################################################################
# Unit testing
enable_testing()


################################################################################
# Compilation Directories
add_subdirectory(src/)
add_subdirectory(apps/)

