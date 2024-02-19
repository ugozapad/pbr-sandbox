#include "core/filemanager.h"

static FileManager s_FileManager;
FileManager* g_fileManager = &s_FileManager;

FileManager::FileManager() :
	m_defaultPath(nullptr)
{
	//char currentDirectory[kMaxPathLength];
	//GetCurrentDirectoryA(kMaxPathLength, currentDirectory);
	//strcat(currentDirectory, "/");

	//resolveOsPath(currentDirectory);

	//m_defaultPath = strdup(currentDirectory);

	//char currentDirectory[kMaxPathLength];
	//GetCurrentDirectoryA(kMaxPathLength, currentDirectory);

	//resolveOsPath(currentDirectory);

	//addPath("root"			, nullptr	, currentDirectory	);
	//addPath("textures"		, "root"	, "/data/textures"	);
	//addPath("levels"		, "root"	, "/data/levels"	);
	//addPath("shaders"		, "root"	, "/data/shaders"	);
	//addPath("scripts"		, "root"	, "/data/scripts"	);
	//addPath("sounds"		, "root"	, "/data/sounds"	);
	//addPath("models"		, "root"	, "/data/models"	);
	//addPath("configs"		, "root"	, "/data/configs"	);

	//// logs
	//addPath("logs"			, "root"	, "/logs"			);
	//addPath("screenshots"	, "root"	, "/screenshots"	);

	//RecursiveSearch(m_defaultPath, m_files);
	//Logger::msg("total %i files", m_files.size());
}

FileManager::~FileManager()
{

}

void FileManager::SetDefaultPath(const char* path)
{
	m_defaultPath = path;
}

bool FileManager::FileExist(const char* filename)
{
	FILE* file = nullptr;
	if ((file = fopen(filename, "r")) != NULL)
	{
		fclose(file);
		return true;
	}

	return false;
}

File* FileManager::OpenFile(const char* path, FileAccess access)
{
	char pathBuffer[kMaxPathLength];
	if (m_defaultPath && !strstr(path, m_defaultPath))
		sprintf(pathBuffer, "%s%s", m_defaultPath, path);
	else
		strcpy(pathBuffer, path);

//	resolveOsPath(pathBuffer);

	return new File(pathBuffer, access);
}

void FileManager::CloseFile(File*& file)
{
	if (file)
	{
		delete file;
		file = nullptr;
	}
}
