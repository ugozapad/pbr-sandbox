#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "render_common.h"

class IndexBuffer : public IGPUBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void bind();
private:

};

#endif // INDEX_BUFFER_H
