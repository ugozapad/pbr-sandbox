#pragma once

#include "render/render_common.h"

class VertexBuffer;

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

	// buffer creation
	VertexBuffer* create_vertex_buffer(void* data, size_t size, BufferAccess access);
	void delete_vertex_buffer(VertexBuffer* buffer);
};