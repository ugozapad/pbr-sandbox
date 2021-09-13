#pragma once

#include <memory>

#include "render/texture2d.h"

class ResourceManager
{
public:
	static ResourceManager& get_instance()
	{
		static ResourceManager instance;
		return instance;
	}
public:
	template <typename T>
	std::shared_ptr<T> create_resource(const char* filename);
};