file(GLOB_RECURSE ROOT_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(pbr-sandbox ${ROOT_SRC})
target_include_directories(pbr-sandbox PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(pbr-sandbox PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/glm")
target_include_directories(pbr-sandbox PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/stb")
target_include_directories(pbr-sandbox PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/SDL2-2.26.5")

target_link_libraries(pbr-sandbox glad tinyxml2 SDL2::SDL2 SDL2::SDL2main)
