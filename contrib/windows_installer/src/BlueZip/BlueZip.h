
class __declspec(dllexport) BlueZip
{
private:
	datEnd data;			// The end header
	char* Comment;			// Actual comment

	int FilePosDelta;		// The value added to begin seeks to compenstate for SFX headers

	LPTSTR FileName;

	//Used by ReadFile
	bool ScanZip(File f);
	void ReadEnd(File f);

	void Setup(LPCTSTR FileName);

#ifndef NO_COMPRESSION
	fList* Pending; // List of names to add
	void WriteEnd(File f);

	//fList** PendingNext; - Implement this to add in order
#endif

public:
	zList* Files;			// A point to the first file in the ZIP


	BlueZip(){};
	BlueZip(LPCTSTR FileName){Setup(FileName);}
	void SetZipFile(LPCTSTR FileName){Setup(FileName);}

	~BlueZip();

	bool Read(); //Load the file into memory

#ifndef NO_COMPRESSION
	bool Write(bool Store = false); //Write the file back out
	void AddFile(LPCTSTR FileName);
#endif

	bool GetFile(zList* z, LPCTSTR FileName);
};
