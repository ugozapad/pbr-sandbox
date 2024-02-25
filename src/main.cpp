#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <SDL.h>

#include "core/filemanager.h"
#include "renderer/renderinterface.h"

#include "inputmanager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

class ShaderManager
{
public:
	static ShaderProgram* Create(const char* vertexFilename, const char* pixelFilename);
};

ShaderProgram* ShaderManager::Create(const char* vertexFilename, const char* pixelFilename)
{
	std::string vertexContent;
	std::string pixelContent;

	// Read vertex shader

	File* f = g_fileManager->OpenFile(vertexFilename, FileAccess::Read);
	SDL_assert(f->GetHandle());

	f->Seek(SeekDir::End, 0);
	size_t fileLength = f->Tell();
	f->Seek(SeekDir::Begin, 0);

	vertexContent.resize(fileLength + 1);
	f->Read((void*)vertexContent.data(), fileLength);
	vertexContent[fileLength] = '\0';

	g_fileManager->CloseFile(f);

	// Read pixel shader
	f = g_fileManager->OpenFile(pixelFilename, FileAccess::Read);
	SDL_assert(f->GetHandle());

	f->Seek(SeekDir::End, 0);
	fileLength = f->Tell();
	f->Seek(SeekDir::Begin, 0);

	pixelContent.resize(fileLength + 1);
	f->Read((void*)pixelContent.data(), fileLength);
	pixelContent[fileLength] = '\0';

	g_fileManager->CloseFile(f);

	ShaderDescriptor shaderDesc(static_cast<const void*>(vertexContent.data()), vertexContent.length(),
		static_cast<const void*>(pixelContent.data()), pixelContent.length());

	RenderInterface* renderInterface = RenderFacade::GetInstance()->GetRenderInterface();
	return renderInterface->CreateShader(shaderDesc);
}

class SimpleShader
{
public:
	SimpleShader();
	~SimpleShader();

	void Bind(const glm::mat4& modelViewProj);

private:
	ShaderProgram* m_shaderProgram;
	uint32_t m_modelViewProjectionUniform;
};

SimpleShader::SimpleShader() :
	m_shaderProgram(nullptr),
	m_modelViewProjectionUniform(0)
{
	m_shaderProgram = ShaderManager::Create("data/test_mesh.vs", "data/test_mesh.ps");
	m_modelViewProjectionUniform = m_shaderProgram->GetUniformLocation("u_modelViewProjection");
}

SimpleShader::~SimpleShader()
{
}

void SimpleShader::Bind(const glm::mat4& modelViewProj)
{
	m_shaderProgram->SetMatrix4(m_modelViewProjectionUniform, modelViewProj);
}

static SimpleShader* g_simpleShader = nullptr;

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING ^ SDL_INIT_SENSOR) != 0)
	{
		printf("Failed to initialize SDL2. Error core: %s\n", SDL_GetError());
		return false;
	}

	return true;
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

	RenderInterface* renderInterface = RenderFacade::GetInstance()->GetRenderInterface();
	renderInterface->Init(renderWindow);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	VertexBuffer* vertexBuffer = renderInterface->CreateVertexBuffer(vertices, sizeof(vertices), sizeof(float) * 3, false);

	ShaderProgram* shader = ShaderManager::Create("data/test.vs", "data/test.ps");

	int width = 0, height = 0;
	SDL_GetWindowSize(renderWindow, &width, &height);

	float aspectRatio = (float)width / (float)height;

	glm::mat4 projection = glm::perspective(glm::radians(75.0f), aspectRatio, 0.1f, 100.0f);

	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 modelViewProjection = projection * view * model;

	uint32_t modelViewProjectionUniform = shader->GetUniformLocation("u_modelViewProjection");

	bool exitRequested = false;
	while (!exitRequested)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				exitRequested = true;
				break;

			// Input stuff
			case SDL_KEYDOWN:
				InputManager::instance->OnKeyboardAction(event.key.keysym.scancode, true);
				break;
			case SDL_KEYUP:
				InputManager::instance->OnKeyboardAction(event.key.keysym.scancode, false);
				break;
			case SDL_MOUSEMOTION:
				InputManager::instance->OnMousePosAction(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y));
				break;
			}
		}

		InputManager::instance->update();

		renderInterface->BeginFrame();
		renderInterface->EndFrame();
	
		renderInterface->SetVertexBuffer(vertexBuffer);

		renderInterface->SetShader(shader);
		shader->SetMatrix4(modelViewProjectionUniform, modelViewProjection);

		renderInterface->DrawArrays(PT_TRIANGLES, 0, 3);

		renderInterface->Present();
	}

	delete shader;
	delete vertexBuffer;

	renderInterface->Shutdown();

	if (renderWindow)
	{
		SDL_DestroyWindow(renderWindow);
		renderWindow = nullptr;
	}

	SDL_Quit();

	return 0;
}
