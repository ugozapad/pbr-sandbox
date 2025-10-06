#ifndef RENDERINTERFACE_H
#define RENDERINTERFACE_H

#include <glm/glm.hpp>

#define RENDERER_GL

struct SDL_Window;

// texture format
enum PixelFormat
{
	PF_UNKNOWN,
	PF_R8G8B8,
	PF_R8G8B8A8,
	PF_R8G8B8F,
	PF_R8G8B8A8F,

	// Depth formats
	PF_DEPTH32F
};

enum PrimitiveType
{
	PT_POINTS,
	PT_LINES,
	PT_TRIANGLES,
	PT_MAX
};

enum BufferType
{
	BT_VERTEX,
	BT_INDEX
};

struct ShaderDescriptor
{
	const void* vertexData;
	const int vertexDataSize;
	const void* pixelData;
	const int pixelDataSize;

	ShaderDescriptor(const void* _vertexData,
		const int _vertexDataSize,
		const void* _pixelData,
		const int _pixelDataSize) :
		vertexData(_vertexData),
		vertexDataSize(_vertexDataSize),
		pixelData(_pixelData),
		pixelDataSize(_pixelDataSize)
	{
	}
};

struct SurfaceDescriptor
{
	int width;
	int height;
	PixelFormat format;
};

#ifdef RENDERER_GL
#include <SDL.h>
#include <SDL_opengl.h>

#include "renderer/gl/buffer_gl.h"
#include "renderer/gl/texture2d_gl.h"
#include "renderer/gl/shader_program_gl.h"
#endif

#endif // !RENDERINTERFACE_H
