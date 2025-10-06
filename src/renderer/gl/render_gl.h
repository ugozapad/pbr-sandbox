#ifndef RENDERINTERFACE_GL_H
#define RENDERINTERFACE_GL_H

#include "renderer/render.h"

class Render_GL
{
public:
	Render_GL();
	~Render_GL();

	void Init(SDL_Window* window);
	void Shutdown();

	void BeginFrame();
	void EndFrame();

	void EnableVSync(bool value);
	void Present();

	Texture2D* CreateTexture2D(int width, int height, void* data, PixelFormat pixelFormat, bool generateMips = false);
	void SetTexture2D(Texture2D* texture, int slot);

	Buffer* CreateBuffer(BufferType type, const void* data, int size, int stride, bool dynamic);
	void SetVertexBuffer(Buffer* buffer);
	void SetIndexBuffer(Buffer* buffer);

	ShaderProgram* CreateShader(const ShaderDescriptor& shaderDesc);
	void SetShader(ShaderProgram* shader);

	void DrawArrays(PrimitiveType mode, int first, int count);

private:
	SDL_Window* m_window;
	SDL_GLContext m_gl_context;

	GLuint m_vao;
};

typedef Render_GL Render;

extern Render_GL* g_render;

#endif // !RENDERINTERFACE_GL_H
