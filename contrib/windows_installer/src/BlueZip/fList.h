
#ifndef NO_COMPRESSION

class fList
{
public:
	char* FileName;
	fList* next;

	fList(LPCTSTR FileName, fList* prev);
	~fList();
	zList* ZipUp(File f, bool Store);
};

#endif //!NO_COMPRESSION

