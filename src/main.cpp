#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <SDL.h>

#include "core/filemanager.h"
#include "renderer/render.h"
#include "renderer/shadermanager.h"
#include "renderer/gl/render_gl.h"

#include "inputmanager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

class SimpleShader
{
public:
	SimpleShader();
	~SimpleShader();

	void Bind(const glm::mat4& modelViewProj);

private:
	ShaderProgram* m_shaderProgram;
	int32_t m_modelViewProjectionUniform;
};

SimpleShader::SimpleShader() :
	m_shaderProgram(nullptr),
	m_modelViewProjectionUniform(0)
{
	m_shaderProgram = ShaderManager::Create("data/shaders/test.vs", "data/shaders/test.ps");

	m_modelViewProjectionUniform = m_shaderProgram->GetUniformLocation("u_modelViewProjection");
	SDL_assert(m_modelViewProjectionUniform != -1 && "Uniform u_modelViewProjection not found");
}

SimpleShader::~SimpleShader()
{
	if (m_shaderProgram)
	{
		delete m_shaderProgram;
		m_shaderProgram = nullptr;
	}
}

void SimpleShader::Bind(const glm::mat4& modelViewProj)
{
	g_render->SetShader(m_shaderProgram);
	m_shaderProgram->SetMatrix4(m_modelViewProjectionUniform, modelViewProj);
}

static SimpleShader* g_simpleShader = nullptr;

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING ^ SDL_INIT_SENSOR) != 0)
	{
		printf("Failed to initialize SDL2. Error code: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

bool Sys_OnSDLEvent(SDL_Event& event)
{
	bool exitRequested = false;

	switch (event.type)
	{
	case SDL_QUIT:
		exitRequested = true;
		break;

		// Input stuff
	case SDL_KEYDOWN:
		g_inputManager->OnKeyboardAction(event.key.keysym.sym, true);
		break;
	case SDL_KEYUP:
		g_inputManager->OnKeyboardAction(event.key.keysym.sym, false);
		break;
	case SDL_MOUSEMOTION:
		g_inputManager->OnMousePosAction(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y));
		break;
	}

	return exitRequested;
}

int main(int argc, char* argv[])
{
	if (!InitSDL())
		return -1;

	// Initialize OpenGL context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	// Create window
	SDL_Window* renderWindow = SDL_CreateWindow("PBR Sandbox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
	if (!renderWindow)
	{
		printf("Failed to create render window. Error core: %s\n", SDL_GetError());
		return -1;
	}

	g_render->Init(renderWindow);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	Buffer* vertexBuffer = g_render->CreateBuffer(BT_VERTEX, vertices, sizeof(vertices), sizeof(float) * 3, false);

	SimpleShader* shader = new SimpleShader();

	int width = 0, height = 0;
	SDL_GetWindowSize(renderWindow, &width, &height);

	float aspectRatio = (float)width / (float)height;

	glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspectRatio, 0.1f, 100.0f);

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	Uint32 startTicks = 0, endTicks = 0;

	bool exitRequested = false;
	while (!exitRequested)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			exitRequested = Sys_OnSDLEvent(event);
		}

		startTicks = SDL_GetTicks();
		float deltaTime = (startTicks - endTicks) / 100.0f;
		endTicks = startTicks;

		static float angle = 0.0f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0));

		angle += deltaTime * 25.0f;

		glm::mat4 modelViewProjection = projection * view * model;

		g_inputManager->update();

		g_render->BeginFrame();
		g_render->EndFrame();
	
		g_render->SetVertexBuffer(vertexBuffer);

		shader->Bind(modelViewProjection);

		g_render->DrawArrays(PT_TRIANGLES, 0, 3);

		g_render->Present();
	}

	delete shader;
	delete vertexBuffer;

	g_render->Shutdown();

	if (renderWindow)
	{
		SDL_DestroyWindow(renderWindow);
		renderWindow = nullptr;
	}

	SDL_Quit();

	return 0;
}
