// Structures for in-memory file information

class __declspec(dllexport) zList
{
public:
	datCentral data;

	char* FileName;		//File name in zip file
	char* Extra;		//Extra in central
	char* Comment;		//Comment (set only if com != 0)

	bool Delete;		//Should the file be removed on execute
	zList* next;

	zList();
	~zList();

	void WriteCentral(File f);
	void ReadCentral(File f);

	void WriteLocal(File f);

	int OriginalSize(){return data.OrigSize;}
	int CompressedSize(){return data.CompSize;}
};
