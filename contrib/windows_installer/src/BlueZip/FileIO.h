
//Use Windows (reliable) file IO routines
#define	File			HANDLE
#define File_Invalid	INVALID_HANDLE_VALUE

File FileOpenRead(LPCTSTR s);
File FileOpenWrite(LPCTSTR s);
File FileOpenTemp(LPTSTR s);

bool FileReplace(LPCTSTR To, LPCTSTR From);

void FileRead(File f, void* buf, DWORD size);
int FileReadBuf(File f, void* buf, DWORD size);
void FileWrite(File f, const void* buf, DWORD size);

bool FileValid(File f);
void FileClose(File f);

void SeekBeg(File f, long offset);
void SeekCur(File f, long offset);
void SeekEnd(File f, long offset);

int FilePos(File f);
int FileLen(File f);

void FileGetDate(File f, datCentral* data);
u32 FileGetAttrib(File f, LPCTSTR FileName);
