#ifndef RENDERINTERFACE_GL_H
#define RENDERINTERFACE_GL_H

#include "renderer/renderinterface.h"

#include <SDL.h>
#include <SDL_opengl.h>

class RenderInterface_GL : public RenderInterface
{
public:
	RenderInterface_GL();
	~RenderInterface_GL();

	void Init(SDL_Window* window) override;
	void Shutdown() override;

	void BeginFrame() override;
	void EndFrame() override;

	void EnableVSync(bool value) override;
	void Present() override;

	Texture2D* CreateTexture2D(int width, int height, void* data, PixelFormat pixelFormat, bool generateMips = false) override;
	void SetTexture2D(Texture2D* texture, int slot) override;

	VertexBuffer* CreateVertexBuffer(const void* data, int size, int stride, bool dynamic) override;
	IndexBuffer* CreateIndexBuffer(const void* data, int size, bool dynamic) override;
	void SetVertexBuffer(VertexBuffer* buffer) override;
	void SetIndexBuffer(IndexBuffer* buffer) override;

	ShaderProgram* CreateShader(const ShaderDescriptor& shaderDesc) override;
	void SetShader(ShaderProgram* shader) override;

	void DrawArrays(PrimitiveType mode, int first, int count) override;

private:
	SDL_Window* m_window;
	SDL_GLContext m_gl_context;

	GLuint m_vao;
};

#endif // !RENDERINTERFACE_GL_H
