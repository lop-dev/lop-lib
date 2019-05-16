
cmake_minimum_required(VERSION 2.8.10)

project(mongoose)
set(description "mongoose C client")

SET(LIBRARY_OUTPUT_PATH "../lib/cos-x86-64")
SET(CMAKE_CXX_FLAGS_RELEASE, "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -Wno-deprecated -finput-charset=GB2312 -fexec-charset=UTF-8 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG, "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -Wall -Wno-deprecated -finput-charset=GB2312 -fexec-charset=UTF-8 -std=c++11")
SET(CMAKE_DEBUG_POSTFIX "_d")


# general options
include_directories(include ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
if(UNIX)
  add_compile_options(-Wall -fPIC)
elseif(WIN32)
  add_compile_options(/W3)
endif()

# mongoose library
set(mongoose_sources mongoose.c)

add_library(mongoose STATIC ${mongoose_sources})

