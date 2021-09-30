#pragma once

#include "render/render_common.h"

class Texture2D
{
public:
	static void unbind(int slot);

public:
	void init_from_memory(void* data, int width, int height, ImageFormat format);

	void bind(int slot);

private:
	uint32_t m_texture_handle;
};