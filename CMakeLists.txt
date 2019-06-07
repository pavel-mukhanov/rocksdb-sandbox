cmake_minimum_required(VERSION 3.12)
project(rocksdb_sandbox)

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

set(CMAKE_CXX_STANDARD 14)

add_executable(rocksdb_sandbox main.cpp column_family.cpp column_family.h utils.cpp utils.h)

TARGET_LINK_LIBRARIES(rocksdb_sandbox rocksdb)
