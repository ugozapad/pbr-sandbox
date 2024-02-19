#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>
#include "index_buffer_gl.h"

IndexBuffer_GL::IndexBuffer_GL(const void* data, const int size, const bool dynamicAccess) :
	m_Handle(-1),
	m_Size(size),
	m_IsDynamicAccess(dynamicAccess)
{
	Init(data, size, dynamicAccess);
}

IndexBuffer_GL::~IndexBuffer_GL()
{
	if (m_Handle >= 0)
		Destroy();
}

void IndexBuffer_GL::Init(const void* data, const int size, const bool dynamicAccess)
{
	// Clear GL errors
	glGetError();

	glGenBuffers(1, &m_Handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, dynamicAccess ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	GLenum error = glGetError();
	if (error == GL_OUT_OF_MEMORY)
	{
		printf("IndexBuffer::Init: Out of memory. Couldn't allocate buffer with %i size\n", size);
	}
}

void IndexBuffer_GL::Destroy()
{
	glDeleteBuffers(1, &m_Handle);
}

void IndexBuffer_GL::UpdateData(const void* data, const int size)
{
	SDL_assert(m_IsDynamicAccess && "Trying to update static buffer.");

	// Clear GL errors
	glGetError();

	// Bind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);

	GLenum error = glGetError();
	if (error == GL_OUT_OF_MEMORY)
	{
		printf("IndexBuffer::UpdateData: Out of memory. Couldn't allocate buffer with %i size\n", size);
	}

	m_Size = size;
}

void IndexBuffer_GL::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
}

GLuint IndexBuffer_GL::GetHandle() const
{
	return m_Handle;
}

int IndexBuffer_GL::GetSize() const
{
	return m_Size;
}
