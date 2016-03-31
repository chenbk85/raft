cmake_minimum_required (VERSION 2.6)                                                                                                
project (raft C CXX)

SET (raft_VERSION_MAJOR 1)
SET (raft_VERSION_MINOR 0)

if(NOT CMAKE_BUILD_TYPE)                                                                                                            
    set(CMAKE_BUILD_TYPE "Release")    
endif()

set(CXX_FLAGS
    -std=c++11
    -Wall
    -Wextra
    #-Werror
    #-Wconversion
    -Wno-unused-parameter
    #-Wold-style-cast
    -Wpointer-arith
    #-Wshadow
    -Wwrite-strings
    -march=native                                                                                                                   
    -rdynamic
    )

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}") 

set(CMAKE_CXX_COMPILER "g++")    
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED system filesystem)                                                                                                                    
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

# TODO: find grpc、find protoc-gen-grpc executable
# TODO: find leveldb

include_directories(${PROJECT_SOURCE_DIR}/src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(src)
