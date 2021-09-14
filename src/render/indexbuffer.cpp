#include "indexbuffer.h"
#include "glad/glad.h"

IndexBuffer::IndexBuffer(void* data, size_t size, BufferAccess access)
{
	glGenBuffers(1, &m_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, get_buffer_access(access));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_buffer);
}

void* IndexBuffer::map(BufferMapping mapping)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	void* ret = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, get_buffer_mapping(mapping));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return ret;
}

void IndexBuffer::unmap()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
