#ifndef BUFFER_GL_H
#define BUFFER_GL_H

#include "renderer/render.h"

class Buffer_GL
{
public:
	Buffer_GL(BufferType type, const void* data, int size, const int stride, const bool dynamicAccess);
	~Buffer_GL();

	void Init(BufferType type, const void* data, int size, const int stride, const bool dynamicAccess);
	void Destroy();

	void UpdateData(const void* data, int size);

	void Bind() const;

	inline GLuint GetHandle() const { return m_Handle; }
	inline int GetSize() const { return m_Size; }
	inline int GetStride() const { return m_Stride; }

private:
	BufferType m_Type;
	GLuint m_Target;
	GLuint m_Handle;
	int m_Size;
	int m_Stride;
	bool m_IsDynamicAccess;
};

typedef Buffer_GL Buffer;

#endif // !VERTEX_BUFFER_H
