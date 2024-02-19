#ifndef TEXTURE2D_GL_H
#define TEXTURE2D_GL_H

#include "renderer/renderinterface_gl/renderinterface_gl.h"

class Texture2D_GL : public Texture2D
{
public:
	Texture2D_GL();
	~Texture2D_GL();

	void CreateRaw(void* data, int width, int height, PixelFormat pf);
	void Destroy();

	void GenerateMipmaps();

	void Bind();

	uint32_t getHandle() { return m_handle; }
	PixelFormat GetPixelFormat() override { return m_pf; }

private:
	PixelFormat m_pf;
	int m_width;
	int m_height;
	int m_channels;
	uint32_t m_handle;
};

#endif // !TEXTURE2D_H
