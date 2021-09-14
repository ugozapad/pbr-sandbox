#include "app/resource_manager.h"

#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

template <>
std::shared_ptr<Texture2D> ResourceManager::create_resource(const char* filename)
{
	int width = 0, height = 0, channels = 0;
	uint8_t* data = stbi_load(filename, &width, &height, &channels, 0);

	std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
	texture->init_from_memory(data, width, height, (channels == 3) ? ImageFormat::FMT_RGB : ImageFormat::FMT_RGBA);

	stbi_image_free(data);

	spdlog::info("ResourceManager: loaded texture 2d {}", filename);

	return texture;
}
