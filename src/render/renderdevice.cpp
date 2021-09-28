#include "render/renderdevice.h"
#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"
#include "render/constantbuffer.h"

#include "glad/glad.h"

#include <stdio.h>

#include <spdlog/spdlog.h>

RenderDevice* RenderDevice::getInstance()
{
	static RenderDevice s_render_device;
	return &s_render_device;
}

void RenderDevice::clearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void RenderDevice::clear(uint32_t flag)
{
	GLenum clear_flags = 0;
	if (flag & CLEAR_COLOR)
		clear_flags |= GL_COLOR_BUFFER_BIT;
	if (flag & CLEAR_DEPTH)
		clear_flags |= GL_DEPTH_BUFFER_BIT;
	if (flag & CLEAR_STENCIL)
		clear_flags |= GL_STENCIL_BUFFER_BIT;

	glClear(clear_flags);
}

void RenderDevice::drawArrays(PrimMode mode, int first, size_t count)
{
	GLenum gl_mode;

	switch (mode)
	{
	case PM_TRIANGLES:
		gl_mode = GL_TRIANGLES;
		break;
	case PM_TRIANGLE_STRIP:
		gl_mode = GL_TRIANGLE_STRIP;
		break;
	default:
		spdlog::error("RenderDevice::draw_arrays: unknowed primitive mode");
		break;
	}

	glDrawArrays(gl_mode, first, count);
}

void RenderDevice::drawElements(PrimMode mode, int count)
{
	GLenum gl_mode;

	switch (mode)
	{
	case PM_TRIANGLES:
		gl_mode = GL_TRIANGLES;
		break;
	case PM_TRIANGLE_STRIP:
		gl_mode = GL_TRIANGLE_STRIP;
		break;
	default:
		spdlog::error("RenderDevice::drawElements: unknowed primitive mode");
		break;
	}

	glDrawElements(gl_mode, count, GL_UNSIGNED_INT, 0);
}

VertexBuffer* RenderDevice::createVertexBuffer(void* data, size_t size, BufferAccess access)
{
	return new VertexBuffer(data, size, access);
}

void RenderDevice::deleteVertexBuffer(VertexBuffer* buffer)
{
	if (buffer)
		delete buffer;
}

IndexBuffer* RenderDevice::createIndexBuffer(void* data, size_t size, BufferAccess access)
{
	return new IndexBuffer(data, size, access);
}

void RenderDevice::deleteIndexBuffer(IndexBuffer* buffer)
{
	if (buffer)
		delete buffer;
}

ConstantBuffer* RenderDevice::createConstantBuffer(void* data, size_t size, BufferAccess access)
{
	return new ConstantBuffer(data, size);
}

void RenderDevice::deleteConstantBuffer(ConstantBuffer* buffer)
{
	if (buffer)
		delete buffer;
}

void RenderDevice::setVertexBuffer(VertexBuffer* buffer)
{
	if (buffer)
		glBindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer);
	else
		glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderDevice::setIndexBuffer(IndexBuffer* buffer)
{
	if (buffer)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->m_buffer);
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderDevice::setConstantBuffer(int slot, ConstantBuffer* buffer)
{
	if (buffer)
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, buffer->m_buffer);
	else 
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, 0);
}
