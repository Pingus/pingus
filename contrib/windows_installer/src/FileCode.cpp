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
	if (File[i-1] == '\\')
		File[i-1] = 0;
}

bool ParentFolder(char* File)
{
	char* s = strrchr(File, '\\');
	if (s == NULL)
		return false;

	s[0] = 0;
	return true;
}

void UnparentFolder(char* File)
{
	File[strlen(File)] = '\\';
}


bool EnsureFolder(char* File)
{
	if (ExistsDir(File))
		return true;

	if (ParentFolder(File))
	{
		bool Res = EnsureFolder(File);
		UnparentFolder(File);
		if (!Res) return false;
	}

	return (CreateDirectory(File, NULL) != 0);
}

void FileSize(__int64 Size, char* Buffer)
{
	//set the number of bytes as a Windows standard file count
	const TCHAR PreFix[] = "KMGTP";
	//make sure to 3 sf

	if (Size < 1000)
	{
		itoa((int) Size, Buffer, 10);
		strcat(Buffer, " bytes");
	}
	else
	{
		int i, j = 1024;
		for (i = 0; Size > j * 999; i++)
			j *= 1024;

		itoa((int) (Size / (__int64) j), Buffer, 10);
		int k = strlen(Buffer);
		if (k != 3)
		{
			Buffer[k] = '.';
			j = ((int) (Size % j) * 1000) / j;
			int l = 100;
			for (k++; k != 4; k++)
			{
				Buffer[k] = (j / l) + '0';
				j %= l;
				l /= 10;
			}
		}
		Buffer[k + 0] = PreFix[i];
		Buffer[k + 1] = 'B';
		Buffer[k + 2] = 0;
	}
}
