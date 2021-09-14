#include "render/renderdevice.h"
#include "render/vertexbuffer.h"
#include "render/indexbuffer.h"

#include "glad/glad.h"

#include <stdio.h>

RenderDevice* RenderDevice::get_instance()
{
	static RenderDevice s_render_device;
	return &s_render_device;
}

void RenderDevice::clear_color(float r, float g, float b, float a)
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

void RenderDevice::draw_arrays(PrimMode mode, int first, size_t count)
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
		printf("RenderDevice::draw_arrays: unknowed primitive mode\n");
		break;
	}

	glDrawArrays(gl_mode, first, count);
}

VertexBuffer* RenderDevice::create_vertex_buffer(void* data, size_t size, BufferAccess access)
{
	return new VertexBuffer(data, size, access);
}

void RenderDevice::delete_vertex_buffer(VertexBuffer* buffer)
{
	if (buffer)
		delete buffer;
}

IndexBuffer* RenderDevice::create_index_buffer(void* data, size_t size, BufferAccess access)
{
	return new IndexBuffer(data, size, access);
}

void RenderDevice::delete_index_buffer(IndexBuffer* buffer)
{
	if (buffer)
		delete buffer;
}

void RenderDevice::set_vertex_buffer(VertexBuffer* buffer)
{
	if (buffer)
		glBindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer);
	else
		glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderDevice::set_index_buffer(IndexBuffer* buffer)
{
	if (buffer)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->m_buffer);
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
