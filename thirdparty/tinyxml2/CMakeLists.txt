cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(tinyxml2)

add_library(tinyxml2 STATIC
	"${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2.h"
)

target_include_directories(tinyxml2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")