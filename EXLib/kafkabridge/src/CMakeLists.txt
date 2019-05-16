# CMakeLists.txt for LightningServer

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT(kafkabridge)

#============================= check =========================================================
IF (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	MESSAGE(FATAL_ERROR "
			CMake generation for ${PROJECT_NAME} not allowed within the #source directory!
			Remove the CMakeCache.txt file and try again from another folder, e.g.:
			rm -rf CMakeFiles CMakeCache.txt
			mkdir cmake-make (or mkdir build)
			cd cmake-make (or cd build)
			cmake -G \"Unix Makefiles\" ../
			")
ENDIF (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)

SET(CMAKE_VERBOSE_MAKEFILE TRUE)

SET(CMAKE_USE_RELATIVE_PATHS TRUE)

IF (NOT CMAKE_BUILD_TYPE)
	MESSAGE(STATUS "set default as MINSIZEREL")
	SET(CMAKE_BUILD_TYPE MINSIZEREL)
	SET(CMAKE_CONFIGURARION_TYPES "MINSIZEREL")
ENDIF(NOT CMAKE_BUILD_TYPE)

#============================= sources ========================================================
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
INCLUDE_DIRECTORIES(
		./
		../../librdkafka/src
		)
#=============================== build =========================================================

#SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../lib/cos-x86-64")
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)
SET(LIBRARY_OUTPUT_PATH "../lib/cos-x86-64")

#IF (CMAKE_BUILD_TYPE MATCHES Debug)
#	MESSAGE(STATUS "${PROJECT_NAME} Debug ${CMAKE_BINARY_DIR}")
#	#SET(CMAKE_CONFIGURATION_TYPES "Debug") 
#ELSE (CMAKE_BUILD_TYPE MATCHES Debug)
#	MESSAGE(STATUS "${PROJECT_NAME} Release ${CMAKE_BINARY_DIR}")
#	SET(CMAKE_CONFIGURATION_TYPES "Release")
#	#SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "../lib/release")
#ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

ADD_DEFINITIONS(
		-DLIBRDKAFKA_STATICLIB
	       )


#SET(CMAKE_FIND_LIBRARY_SUFFIXES .so)

message(STATUS "source dir: ${PROJECT_SOURCE_DIR}")
SET(CMAKE_CXX_FLAGS_RELEASE, "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall -Wno-deprecated -finput-charset=GB2312 -fexec-charset=UTF-8 -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG, "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -Wall -Wno-deprecated -finput-charset=GB2312 -fexec-charset=UTF-8 -std=c++11")
set(CMAKE_DEBUG_POSTFIX "_d")
add_definitions(-D_LINUX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#IF (CMAKE_BUILD_TYPE MATCHES Debug)
#    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O0 -g -Wall " OUTPUT_NAME ${PROJECT_NAME})
#ELSE (CMAKE_BUILD_TYPE MATCHES Debug)
#    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-O2 -Wall " OUTPUT_NAME ${PROJECT_NAME})
#ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} C++11 support.")
else()
	message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


#message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${DIR_SRCS} )
#TARGET_LINK_LIBRARIES(${PROJECT_NAME}
#		${CMAKE_BINARY_DIR}/../thirdparts/librdkafka/src/librdkafka.a
#		z
#		stdc++
#		pthread
#		rt
#		dl
#		)


