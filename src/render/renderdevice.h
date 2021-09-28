#pragma once

#include "render/render_common.h"

class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;

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
	static RenderDevice* getInstance();
public:
	void clearColor(float r, float g, float b, float a);
	void clear(uint32_t flag);

	// drawing
	void drawArrays(PrimMode mode, int first, size_t count);
	void drawElements(PrimMode mode, int count);

	// buffer creation
	VertexBuffer* createVertexBuffer(void* data, size_t size, BufferAccess access);
	void deleteVertexBuffer(VertexBuffer* buffer);

	IndexBuffer* createIndexBuffer(void* data, size_t size, BufferAccess access);
	void deleteIndexBuffer(IndexBuffer* buffer);

	ConstantBuffer* createConstantBuffer(void* data, size_t size, BufferAccess access);
	void deleteConstantBuffer(ConstantBuffer* buffer);

	// set
	void setVertexBuffer(VertexBuffer* buffer);
	void setIndexBuffer(IndexBuffer* buffer);
	void setConstantBuffer(int slot, ConstantBuffer* buffer);
};