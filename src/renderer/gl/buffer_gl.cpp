#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>
#include "buffer_gl.h"

Buffer_GL::Buffer_GL(BufferType type, const void* data, int size, const int stride, const bool dynamicAccess) :
	m_Type(type),
	m_Target(-1),
	m_Handle(-1),
	m_Size(size),
	m_Stride(stride),
	m_IsDynamicAccess(dynamicAccess)
{
	Init(type, data, size, stride, dynamicAccess);
}

Buffer_GL::~Buffer_GL()
{
	if (m_Handle >= 0)
		Destroy();
}

void Buffer_GL::Init(BufferType type, const void* data, int size, const int stride, const bool dynamicAccess)
{
	// Clear GL errors
	glGetError();

	// select target
	if (m_Type == BT_VERTEX)
		m_Target = GL_ARRAY_BUFFER;
	else if (m_Type == BT_INDEX)
		m_Target = GL_ELEMENT_ARRAY_BUFFER;

	glGenBuffers(1, &m_Handle);
	glBindBuffer(m_Target, m_Handle);
	glBufferData(m_Target, size, data, dynamicAccess ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	GLenum error = glGetError();
	if (error == GL_OUT_OF_MEMORY)
	{
		printf("Buffer_GL::Init: Out of memory. Couldn't allocate buffer with %i size\n", size);
	}
}

void Buffer_GL::Destroy()
{
	glDeleteBuffers(1, &m_Handle);
}

void Buffer_GL::UpdateData(const void* data, int size)
{
	SDL_assert(m_IsDynamicAccess && "Trying to update static buffer.");

	// Clear GL errors
	glGetError();

	// Bind buffer
	glBindBuffer(m_Target, m_Handle);
	glBufferData(m_Target, size, data, GL_DYNAMIC_DRAW);

	GLenum error = glGetError();
	if (error == GL_OUT_OF_MEMORY)
	{
		printf("Buffer_GL::UpdateData: Out of memory. Couldn't allocate buffer with %u size\n", (unsigned)size);
	}

	m_Size = size;
}

void Buffer_GL::Bind() const
{
	glBindBuffer(m_Target, m_Handle);
}
