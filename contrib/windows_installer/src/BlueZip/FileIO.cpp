#include "BlueHead.h"

File FileOpenRead(LPCTSTR s)
{
	return CreateFile(s, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
}

File FileOpenWrite(LPCTSTR s)
{
	return CreateFile(s, GENERIC_WRITE,
		FILE_SHARE_READ, NULL, CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL, NULL);
}

void FileRead(File f, void* buf, DWORD size)
{
	DWORD d;
	ReadFile(f, buf, size, &d, NULL);
	Assert(size == d);
}

int FileReadBuf(File f, void* buf, DWORD size)
{
	DWORD d;
	ReadFile(f, buf, size, &d, NULL);
	return d;
}

void FileWrite(File f, const void* buf, DWORD size)
{
	DWORD d;
	WriteFile(f, buf, size, &d, NULL);
	Assert(size == d);
}


bool FileValid(File f)
{
	return (f != File_Invalid);
}

void FileClose(File f)
{
#ifdef _DEBUG
	if (!CloseHandle(f)) _asm int 3;
#else
	CloseHandle(f);
#endif
}


bool FileReplace(LPCTSTR To, LPCTSTR From)
{
	//First remove the existing file
	DeleteFile(To);
	//Then copy it accross
	return (MoveFile(From, To) != FALSE);
}

File FileOpenTemp(LPTSTR s)
{
	char Buffer[MAX_PATH];

	if ((GetTempPath(MAX_PATH, Buffer) == 0) ||
		(GetTempFileName(Buffer, "ZIP", 0, s) == 0))
		return File_Invalid;

	return CreateFile(s, GENERIC_WRITE,
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, //Create Always, not Create New
		FILE_ATTRIBUTE_NORMAL, NULL);;
}


//return TRUE for success, FALSE for failure
//fseek returns 0 for success

void inline _p_Seek(File f, long offset, DWORD origin)
{
#ifdef _DEBUG
	if (SetFilePointer(f, offset, NULL, origin) == 0xFFFFFFFF) _asm int 3;
#else
	SetFilePointer(f, offset, NULL, origin);
#endif
}

void SeekBeg(File f, long offset)
{_p_Seek(f, offset, FILE_BEGIN);}

void SeekCur(File f, long offset)
{_p_Seek(f, offset, FILE_CURRENT);}

void SeekEnd(File f, long offset)
{_p_Seek(f, offset, FILE_END);}


int FilePos(File f)
{
	DWORD d = SetFilePointer(f, 0, NULL, FILE_CURRENT);
	Assert(d != 0xFFFFFFFF);
	return d;
}

int FileLen(File f)
{
	return GetFileSize(f, NULL);
}

void FileGetDate(File f, datCentral* data)
{
	FILETIME ft;
#ifdef _DEBUG
	if (!GetFileTime(f, NULL, NULL, &ft)) _asm int 3;
	if (!FileTimeToDosDateTime(&ft, &data->Date, &data->Time)) _asm int 3;
#else
	GetFileTime(f, &ft, NULL, NULL);
	FileTimeToDosDateTime(&ft, &data->Date, &data->Time);
#endif
}

#define A_RONLY    0x01
#define A_HIDDEN   0x02
#define A_SYSTEM   0x04
#define A_LABEL    0x08
#define A_DIR      0x10
#define A_ARCHIVE  0x20

u32 FileGetAttrib(File f, LPCTSTR FileName)
{
	DWORD Attr = GetFileAttributes(FileName);
	Assert(Attr != 0xFFFFFFFF);

	return(
		(Attr & FILE_ATTRIBUTE_READONLY  ? A_RONLY   :0) |
		(Attr & FILE_ATTRIBUTE_HIDDEN    ? A_HIDDEN  :0) |
		(Attr & FILE_ATTRIBUTE_SYSTEM    ? A_SYSTEM  :0) |
		(Attr & FILE_ATTRIBUTE_DIRECTORY ? A_DIR     :0) |
		(Attr & FILE_ATTRIBUTE_ARCHIVE   ? A_ARCHIVE :0));
}

