cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(pbr-sandbox C CXX)
if(APPLE)
    enable_language(OBJC)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin") 

if (MSVC)
    # Always generate PDBs
    add_compile_options(/Zi)
    add_link_options(/DEBUG)

    add_compile_options(/W3) # Warning level
endif()

add_subdirectory(thirdparty)
add_subdirectory(src)
