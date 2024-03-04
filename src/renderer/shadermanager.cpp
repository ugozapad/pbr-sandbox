#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <SDL.h>

#include "core/filemanager.h"
#include "renderer/renderinterface.h"
#include "renderer/shadermanager.h"

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

	RenderInterface* renderInterface = RenderFacade::GetRenderInterface();
	return renderInterface->CreateShader(shaderDesc);
}
