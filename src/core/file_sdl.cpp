#include <SDL.h>
#include "core/file_sdl.h"

static const char* s_stdioOpeningMode[] = { "r+b", "w+b" };
static int s_stdioSeekDir[] = { RW_SEEK_SET, RW_SEEK_CUR, RW_SEEK_END };

FileSDL::FileSDL(const char* path, FileAccess access)
{
	m_filehandle = SDL_RWFromFile(path, s_stdioOpeningMode[(int)access]);

	Seek(SeekDir::End, 0);
	m_size = Tell();
	Seek(SeekDir::Begin, 0);
}

FileSDL::~FileSDL()
{
	if (m_filehandle)
	{
		SDL_RWclose(m_filehandle);
		m_filehandle = nullptr;
	}
}

void FileSDL::Seek(SeekDir seekdir, long offset)
{
	SDL_RWseek(m_filehandle, offset, s_stdioSeekDir[(int)seekdir]);
}

long FileSDL::Tell()
{
	return long(SDL_RWtell(m_filehandle));
}

bool FileSDL::eof()
{
//	Core::error("FileSDL::eof: i dont give any shit how implement that :(");

	return Tell() == m_size;
}

size_t FileSDL::Read(void* buffer, size_t size)
{
	//return fread(buffer, 1, size, m_filehandle);
	return SDL_RWread(m_filehandle, buffer, 1, size);
}

size_t FileSDL::write(void* buffer, size_t size)
{
	//return fwrite(buffer, size, 1, m_filehandle);
	return SDL_RWwrite(m_filehandle, buffer, 1, size);
}

void FileSDL::readStringBuffer(char* buffer, size_t bufferSize)
{
	Seek(SeekDir::End, 0);
	size_t length = Tell();
	Seek(SeekDir::Begin, 0);

	SDL_assert(length <= bufferSize);

	Read(buffer, length);
	buffer[length] = '\0';
}
