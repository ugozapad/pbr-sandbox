#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

#include "core/file_sdl.h"

typedef FileSDL File;

class FileManager
{
public:
	FileManager();
	~FileManager();

	void SetDefaultPath(const char* path);
	const char* getDefaultPath() { return m_defaultPath; }

	bool FileExist(const char* filename);

	File* OpenFile(const char* path, FileAccess access);
	void CloseFile(File*& file);

private:
	std::vector< std::string > m_files;
	const char* m_defaultPath;
};

extern FileManager* g_fileManager;

#endif // !FILEMANAGER_H
