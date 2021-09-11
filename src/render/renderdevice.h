#pragma once

#include <stdint.h>

class RenderDevice
{
public:
	enum ClearType
	{
		CLEAR_COLOR = 1 << 0,
		CLEAR_DEPTH = 1 << 1,
		CLEAR_STENCIL = 1 << 2
	};

public:
	static RenderDevice* get_instance();
public:
	void clear_color(float r, float g, float b, float a);
	void clear(uint32_t flag);
};