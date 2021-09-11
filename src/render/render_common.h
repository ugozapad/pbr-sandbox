#ifndef RENDER_COMMON_H
#define RENDER_COMMON_H

#include <stdint.h>

enum class BufferAccess
{
	Static,
	Dynamic,
	Stream
};

enum class ImageFormat
{
	FMT_RGB,
	FMT_RGBA,
	FMT_DEPTH24_STENCIL8,
	FMT_DEPTH32_STENCIL_8_F,
	FMT_SRGB,

	FMT_RGB32F,
	FMT_RGBA32F,

	FMT_RGB16F,
	FMT_RGBA16F,
};

struct RenderContext
{
	int width;
	int height;
	//glm::mat4 view;
	//glm::mat4 proj;
	//glm::mat4 model;
};

enum class BufferMapping
{
	READ_ONLY,
	WRITE_ONLY,
	READ_WRITE
};

struct IGPUBuffer
{
	virtual ~IGPUBuffer() {}

	virtual void* map(BufferMapping mapping) = 0;
	virtual void  unmap() = 0;
};

uint32_t get_buffer_access(BufferAccess access);
uint32_t get_buffer_mapping(BufferMapping mapping);

#endif // RENDER_COMMON_H
