#include "render/renderdevice.h"
#include "render/vertexbuffer.h"

#include "glad/glad.h"

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

VertexBuffer* RenderDevice::create_vertex_buffer(void* data, size_t size, BufferAccess access)
{
	return new VertexBuffer(data, size, access);
}

void RenderDevice::delete_vertex_buffer(VertexBuffer* buffer)
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
