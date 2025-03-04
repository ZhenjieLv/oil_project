cmake_minimum_required(VERSION 3.10)

project(iot_project)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lstdc++")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -DNDEBUG")

set(BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${BUILD_DIR}/lib)

option(IS_DEBUG "debug or release" ON)
if(${IS_DEBUG})
    set(CMAKE_BUILD_TYPE "Debug")
else()
    set(CMAKE_BUILD_TYPE "Release")
endif()

include_directories(${BUILD_DIR}/include)

execute_process(COMMAND mysql_config --cflags
    OUTPUT_VARIABLE MYSQL_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND mysql_config --libs
    OUTPUT_VARIABLE MYSQL_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

set(LIB_CMAKE_PATH /usr/lib/aarch64-linux-gnu/cmake)
set(BUILD_ITEMS "" CACHE INTERNAL "all items to build")

# find all package
set(jsoncpp_DIR ${LIB_CMAKE_PATH}/jsoncpp)
find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
include_directories(${JSON_INC_PATH})

set(spdlog_DIR ${LIB_CMAKE_PATH}/spdlog/)
find_package(spdlog REQUIRED)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

#set(Qt5_DIR ${LIB_CMAKE_PATH}/Qt5)
#find_package(Qt5 REQUIRED COMPONENTS Core Widgets WebEngineWidgets WebChannel)

add_subdirectory(util)
add_subdirectory(database)
add_subdirectory(kafka)
if(${IS_DEBUG})
   add_subdirectory(test)
endif()
add_subdirectory(backend)
add_subdirectory(frontend)
file(GLOB allCopyFiles  "./edge/scripts/*.py")
file(COPY ${allCopyFiles} DESTINATION ${BUILD_DIR}/bin/scripts)

    
if(${IS_DEBUG})
    message(STATUS "编译调试版本")
else()
    message(STATUS "编译发布版本")
endif()

message(STATUS "目前准备编译前后端程序，将会在./build/bin中输出：${BUILD_ITEMS}")

file(MAKE_DIRECTORY ${BUILD_DIR}/bin/logs)
