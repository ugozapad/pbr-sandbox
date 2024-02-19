#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>
#include "vertex_buffer_gl.h"

VertexBuffer_GL::VertexBuffer_GL(const void* data, int size, const int stride, const bool dynamicAccess) :
	m_Handle(-1),
	m_Size(size),
	m_Stride(stride),
	m_IsDynamicAccess(dynamicAccess)
{
	Init(data, size, stride, dynamicAccess);
}

VertexBuffer_GL::~VertexBuffer_GL()
{
	if (m_Handle >= 0)
		Destroy();
}

void VertexBuffer_GL::Init(const void* data, int size, const int stride, const bool dynamicAccess)
{
	// Clear GL errors
	glGetError();

	glGenBuffers(1, &m_Handle);
	glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
	glBufferData(GL_ARRAY_BUFFER, size, data, dynamicAccess ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	GLenum error = glGetError();
	if (error == GL_OUT_OF_MEMORY)
	{
		printf("VertexBuffer::Init: Out of memory. Couldn't allocate buffer with %i size\n", size);
	}
}

void VertexBuffer_GL::Destroy()
{
	glDeleteBuffers(1, &m_Handle);
}

void VertexBuffer_GL::UpdateData(const void* data, int size)
{
	SDL_assert(m_IsDynamicAccess && "Trying to update static buffer.");

	// Clear GL errors
	glGetError();

	// Bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

	GLenum error = glGetError();
	if (error == GL_OUT_OF_MEMORY)
	{
		printf("VertexBuffer::UpdateData: Out of memory. Couldn't allocate buffer with %u size\n", (unsigned)size);
	}

	m_Size = size;
}

void VertexBuffer_GL::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
}
