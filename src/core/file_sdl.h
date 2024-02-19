#ifndef FILE_SDL_H
#define FILE_SDL_H

#include <SDL.h>

#include "core/filecommon.h"

class FileSDL
{
public:
	FileSDL(const char* path, FileAccess access);
	~FileSDL();

	SDL_RWops* GetHandle() { return m_filehandle; }

	void Seek(SeekDir seekdir, long offset);
	long Tell();
	bool eof();

	size_t Read(void* buffer, size_t size);
	size_t write(void* buffer, size_t size);

	// helpers
	void readStringBuffer(char* buffer, size_t bufferSize);

private:
	SDL_RWops* m_filehandle;
	size_t m_size;
};

#endif // !FILE_SDL_H
