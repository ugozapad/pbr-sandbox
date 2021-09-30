#include "render/constantbuffer.h"

#include "render/glad/include/glad/glad.h"

ConstantBuffer::ConstantBuffer(void* dataBuffer, size_t size)
{
	glCreateBuffers(1, &m_buffer);
	glNamedBufferData(m_buffer, size, dataBuffer, GL_DYNAMIC_DRAW);
}

ConstantBuffer::~ConstantBuffer()
{
	glDeleteBuffers(1, &m_buffer);
}

void ConstantBuffer::updateSubresourceData(void* data, size_t size)
{
	glNamedBufferSubData(m_buffer, 0, size, data);
}
