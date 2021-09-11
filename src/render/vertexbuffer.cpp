#include "vertexbuffer.h"
#include "glad/glad.h"

uint32_t get_buffer_access(BufferAccess access)
{
	switch (access)
	{
	case BufferAccess::Static: return GL_STATIC_DRAW;
	case BufferAccess::Dynamic: return GL_DYNAMIC_DRAW;
	case BufferAccess::Stream: return GL_STREAM_DRAW;
	}

	return 0;
}

uint32_t get_buffer_mapping(BufferMapping mapping)
{
	switch (mapping)
	{
	case BufferMapping::READ_ONLY: return GL_READ_ONLY;
	case BufferMapping::WRITE_ONLY: return GL_WRITE_ONLY;
	case BufferMapping::READ_WRITE: return GL_READ_WRITE;
	}

	return 0;
}


VertexBuffer::VertexBuffer(void* data, size_t size, BufferAccess access)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ARRAY_BUFFER, size, data, get_buffer_access(access));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_buffer);
}

void* VertexBuffer::map(BufferMapping mapping)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	void* ret = glMapBuffer(GL_ARRAY_BUFFER, get_buffer_mapping(mapping));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return ret;
}

void VertexBuffer::unmap()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
