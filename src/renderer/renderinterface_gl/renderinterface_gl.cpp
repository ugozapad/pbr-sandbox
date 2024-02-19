#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>

#include "renderer/renderinterface_gl/renderinterface_gl.h"
#include "renderer/renderinterface_gl/vertex_buffer_gl.h"
#include "renderer/renderinterface_gl/index_buffer_gl.h"
#include "renderer/renderinterface_gl/shader_program_gl.h"
#include "renderer/renderinterface_gl/texture2d_gl.h"

void APIENTRY RenderDebugOutput(GLenum source, GLenum type, unsigned int id,
	GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	static char longBuffer[1024];
	sprintf(longBuffer, "%stype = 0x%x, severity = 0x%x, message = %s",
		(type == GL_DEBUG_TYPE_ERROR_ARB ? "** GL ERROR ** " : "GL Warning: "),
		type, severity, message);


	printf("%s\n", longBuffer);
}

RenderInterface_GL::RenderInterface_GL() :
	m_window(nullptr),
	m_gl_context(nullptr),
	m_vao(-1)
{
}

RenderInterface_GL::~RenderInterface_GL()
{
}

void RenderInterface_GL::Init(SDL_Window* window)
{
	printf("Initializing renderer backend ...\n");

	m_window = window;

	m_gl_context = SDL_GL_CreateContext(m_window);
	SDL_GL_MakeCurrent(m_window, m_gl_context);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	if (GLVersion.major == 0 || GLVersion.minor == 0)
	{
		printf("Failed to load OpenGL\n");
		SDL_TriggerBreakpoint(); // Fatal, no return
	}

	printf("%s\n", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	printf("%s\n", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	printf("OpenGL ver. %s\n", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

	if (glDebugMessageCallbackARB)
	{
#define GL_DEBUG_OUTPUT                   0x92E0

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glDebugMessageCallbackARB(RenderDebugOutput, NULL);
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void RenderInterface_GL::Shutdown()
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_vao);

	SDL_GL_MakeCurrent(nullptr, nullptr);
}

void RenderInterface_GL::BeginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void RenderInterface_GL::EndFrame()
{
}

void RenderInterface_GL::EnableVSync(bool value)
{
	SDL_GL_SetSwapInterval(value);
}

void RenderInterface_GL::Present()
{
	SDL_GL_SwapWindow(m_window);
}

Texture2D* RenderInterface_GL::CreateTexture2D(int width, int height, void* data, PixelFormat pixelFormat, bool generateMips)
{
	Texture2D_GL* texture = new Texture2D_GL();
	texture->CreateRaw(data, width, height, pixelFormat);

	if (generateMips)
		texture->GenerateMipmaps();

	return texture;
}

void RenderInterface_GL::SetTexture2D(Texture2D* texture, int slot)
{
	if (texture)
	{
		Texture2D_GL* texture_gl = (Texture2D_GL*)texture;
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture_gl->getHandle());
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

VertexBuffer* RenderInterface_GL::CreateVertexBuffer(const void* data, int size, int stride, bool dynamic)
{
	return new VertexBuffer_GL(data, size, stride, dynamic);
}

IndexBuffer* RenderInterface_GL::CreateIndexBuffer(const void* data, int size, bool dynamic)
{
	return new IndexBuffer_GL(data, size, dynamic);
}

void RenderInterface_GL::SetVertexBuffer(VertexBuffer* buffer)
{
	if (buffer)
	{
		gl_upcast(buffer)->Bind();

		// #TODO: Shader/Buffer input layout
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void RenderInterface_GL::SetIndexBuffer(IndexBuffer* buffer)
{
	if (buffer)
	{
		gl_upcast(buffer)->Bind();
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

ShaderProgram* RenderInterface_GL::CreateShader(const ShaderDescriptor& shaderDesc)
{
	return new ShaderProgram_GL(shaderDesc);
}

void RenderInterface_GL::SetShader(ShaderProgram* shader)
{
	if (shader)
	{
		ShaderProgram_GL* shaderGL = (ShaderProgram_GL*)shader;
		glUseProgram(shaderGL->GetHandle());
	}
	else
	{
		glUseProgram(0);
	}
}

void RenderInterface_GL::DrawArrays(PrimitiveType mode, int first, int count)
{
	static GLenum primitiveModes[PT_MAX] = {
		GL_POINTS,
		GL_LINES,
		GL_TRIANGLES
	};

	glDrawArrays(primitiveModes[mode], first, GLsizei(count));
}

// TODO: Make this more beautiful

static RenderInterface_GL s_RenderInterface_GL;

RenderInterface* RenderFacade::GetRenderInterface()
{
	return &s_RenderInterface_GL;
}