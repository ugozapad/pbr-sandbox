set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

add_subdirectory(spdlog)

set(UVRE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(uvre)