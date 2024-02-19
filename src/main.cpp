#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <SDL.h>

#include "core/filemanager.h"
#include "renderer/renderinterface.h"

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
			}
		}

		renderInterface->BeginFrame();
		renderInterface->EndFrame();

		renderInterface->SetVertexBuffer(vertexBuffer);
		renderInterface->SetShader(shader);
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
