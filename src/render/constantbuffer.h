#pragma once

#include "render/render_common.h"

class RenderDevice;

class ConstantBuffer
{
	friend class RenderDevice;

public:
	void updateSubresource(void* data, size_t size);

private:
	ConstantBuffer(void* dataBuffer, size_t size);
	~ConstantBuffer();

	uint32_t m_buffer;
};