#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "render_common.h"

class RenderDevice;

class IndexBuffer : public IGPUBuffer
{
	friend class RenderDevice;
private:
	IndexBuffer(void* data, size_t size, BufferAccess access);
	~IndexBuffer();

public:
	void* map(BufferMapping mapping) override;
	void  unmap() override;

private:
	uint32_t m_buffer;
};

#endif // INDEX_BUFFER_H
