#include "indexbuffer.h"
#include "glad/glad.h"

IndexBuffer::IndexBuffer(void* data, size_t size, BufferAccess access)
{
	glCreateBuffers(1, &m_buffer);
	glNamedBufferData(m_buffer, size, data, get_buffer_access(access));
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_buffer);
}

void* IndexBuffer::map(BufferMapping mapping)
{
	void* ret = glMapNamedBuffer(m_buffer, get_buffer_mapping(mapping));
	return ret;
}

void IndexBuffer::unmap()
{
	glUnmapNamedBuffer(m_buffer);
}
