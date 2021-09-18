#pragma once

#include <memory>
#include <map>
#include <string>

#include "render/texture2d.h"

class ResourceManager
{
public:
	static ResourceManager& getInstance()
	{
		static ResourceManager instance;
		return instance;
	}
public:
	template <typename T>
	std::shared_ptr<T> createResource(const char* filename);

private:
	std::map<std::string, std::shared_ptr<Texture2D>> m_textures;
};