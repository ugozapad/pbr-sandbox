#ifndef INDEX_BUFFER_GL_H
#define INDEX_BUFFER_GL_H

#include "renderer/renderinterface_gl/renderinterface_gl.h"

class IndexBuffer_GL : public IndexBuffer
{
public:
	IndexBuffer_GL(const void* data, const int size, const bool dynamicAccess);
	~IndexBuffer_GL();

	void Init(const void* data, const int size, const bool dynamicAccess);
	void Destroy();

	void UpdateData(const void* data, const int size);

	void Bind() const;

	GLuint GetHandle() const;
	int GetSize() const;

private:
	GLuint m_Handle;
	int m_Size;
	bool m_IsDynamicAccess;

};

inline const IndexBuffer_GL* gl_upcast(const IndexBuffer* buf)
{
	return (const IndexBuffer_GL*)buf;
}

#endif // !INDEX_BUFFER_H
