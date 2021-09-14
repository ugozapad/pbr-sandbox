file(GLOB APP_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/app/*.*"
)
source_group("app" FILES ${APP_SRC})

file(GLOB RENDER_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/render/*.*"
)
source_group("render" FILES ${RENDER_SRC})

add_executable(pbr-sandbox ${APP_SRC} ${RENDER_SRC} "${CMAKE_CURRENT_SOURCE_DIR}/render/glad/src/glad.c")
target_include_directories(pbr-sandbox PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(pbr-sandbox PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/render/glad/include")
target_include_directories(pbr-sandbox PRIVATE "${CMAKE_SOURCE_DIR}/libs/glm")
target_include_directories(pbr-sandbox PRIVATE "${CMAKE_SOURCE_DIR}/libs/stb")
target_include_directories(pbr-sandbox PRIVATE "${CMAKE_SOURCE_DIR}/libs/tinygltf")
target_link_libraries(pbr-sandbox glfw spdlog)