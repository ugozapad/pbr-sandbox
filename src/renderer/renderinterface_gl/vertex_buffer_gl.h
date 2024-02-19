#ifndef VERTEX_BUFFER_GL_H
#define VERTEX_BUFFER_GL_H

#include "renderer/renderinterface_gl/renderinterface_gl.h"

class VertexBuffer_GL : public VertexBuffer
{
public:
	VertexBuffer_GL(const void* data, int size, const int stride, const bool dynamicAccess);
	~VertexBuffer_GL();

	void Init(const void* data, int size, const int stride, const bool dynamicAccess);
	void Destroy();

	void UpdateData(const void* data, int size);

	void Bind() const;

	inline GLuint GetHandle() const { return m_Handle; }

	inline int GetSize() const { return m_Size; }

	inline int GetStride() const { return m_Stride; }

private:
	GLuint m_Handle;
	int m_Size;
	int m_Stride;
	bool m_IsDynamicAccess;

};

inline const VertexBuffer_GL* gl_upcast(const VertexBuffer* buf)
{
	return (const VertexBuffer_GL*)buf;
}

#endif // !VERTEX_BUFFER_H
