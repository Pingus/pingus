#include "Header.h"

bool Exists(char* File)
{
	return (GetFileAttributes(File) != 0xffffffff);
}

bool ExistsDir(char* File)
{
	DWORD Attr = GetFileAttributes(File);
	return ((Attr != 0xffffffff) && (Attr & FILE_ATTRIBUTE_DIRECTORY));
}

bool CanReadWrite(char* File)
{
	HANDLE hFile = CreateFile(File, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE) return false;
	CloseHandle(hFile);
	return true;
}

void NormalPath(char* File)
{
	for (int i = 0; File[i] != 0; i++)
	{
		if (File[i] == '/')
			File[i] = '\\';
	}
}

// The first file passed must end in a \\ character
bool CreateFolder(char* File)
{
	char* s = strrchr(File, '\\');
	if (s == NULL)
		return false;

	int BufChar = s[0];
	s[0] = 0;
	bool Success = false;
	if (!CreateDirectory(File, NULL))
	{
		if (CreateFolder(File) && CreateDirectory(File, NULL))
			Success = true;
	}
	else
		Success = true;

	s[0] = BufChar;
	return Success;
}
