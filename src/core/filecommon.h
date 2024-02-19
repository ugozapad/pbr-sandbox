#ifndef FILECOMMON_H
#define FILECOMMON_H

static const int kMaxPathLength = 260;

enum class FileAccess
{
	Read,
	Write
};

enum class SeekDir
{
	Begin,
	Current,
	End
};

#endif // !FILECOMMON_H
