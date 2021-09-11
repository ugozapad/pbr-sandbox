#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "render/render_common.h"

class RenderDevice;

class VertexBuffer : public IGPUBuffer
{
	friend class RenderDevice;
private:
	VertexBuffer(void* data, size_t size, BufferAccess access);
	~VertexBuffer();

public:
	void* map(BufferMapping mapping) override;
	void  unmap() override;

private:
	uint32_t m_buffer;
};

#endif // VERTEX_BUFFER_H
