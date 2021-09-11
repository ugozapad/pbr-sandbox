file(GLOB APP_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/app/*.*"
)

source_group("app" FILES ${APP_SRC})

add_executable(pbr-sandbox ${APP_SRC})

target_include_directories(pbr-sandbox PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")