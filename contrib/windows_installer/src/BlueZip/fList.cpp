#include "BlueHead.h"

#ifndef NO_COMPRESSION

/* : Required by the deflate routines */
void SetupDeftree(File In, File Out, int my_level);
u32 filecompress(int* method);
u32 GetCRC();


fList::fList(LPCTSTR NewFile, fList* prev)
{
	FileName = CopyString(NewFile);
	//Replace \ with /
	int i;
	for (i = 0; FileName[i] != 0; i++)
	{
		if (FileName[i] == '\\') FileName[i] = '/';
	}
	next = prev;
}

fList::~fList()
{
	if (FileName != NULL)
		delete[] FileName;
}

zList* fList::ZipUp(File f, bool Store)
{
	zList* z = new zList;

	//Open the new file
	File In = FileOpenRead(FileName);
	if (!FileValid(In))
	{
		ErrMsg("Failed to open the file to compress in");
		return NULL;
	}

	z->data.ExtAttr = FileGetAttrib(In, FileName);

	//Copy over the filename
	char* s = strrchr(FileName, '/');
	if (s == NULL)
	{
		z->FileName = FileName;
	}
	else
	{
		z->FileName = CopyString(&s[1]);
		delete[] FileName;
	}
	FileName = NULL;
	z->data.lFileName = strlen(z->FileName);

	//Get some information on the filename
	//Not implemented properly yet


	//PK[UN]ZIP 2.0 made these files and will be able to extract them
	z->data.VerMake = 20;
	z->data.Disk = 0;
	z->data.Offset = FilePos(f);
	z->data.Disk = 0;

	z->data.IntAttr = 0; //BINARY - like I care if it's text?
	FileGetDate(In, &z->data);


	z->data.lExtra = 0;
	z->data.lComment = 0;
	z->data.OrigSize = FileLen(In);


	//Info-Zip would write out the local header here
	SeekCur(f, 4 + ldatLocal + z->data.lFileName);

	//Just store it
	InitCRC();
	if (Store) //Just store it
		WriteStore(In, f, z);
	else
	{
/* CRC CHECK
		u8* Buffer = new u8[z->data.CompSize];
		SeekBeg(In, 0);
		FileRead(In, Buffer, z->data.CompSize);
		FileWrite(f, Buffer, z->data.CompSize);
		z->data.CRC = CRC(0, Buffer, z->data.CompSize);
		delete[] Buffer;

*/
		int Method = compDeflate;
		SetupDeftree(In, f, 9);
		z->data.CompSize = filecompress(&Method);
		z->data.CompMode = Method;
	}

	z->data.CRC = GetCRC();
	z->data.VerNeed = (z->data.CompMode == compDeflate ? 20 : 10);
	//Version needed is 20, because using compress, 10 for just store

	//WinZIP compatible hacks
	z->data.Flags = 2;

	FileClose(In);

	int LocalFileEnd = FilePos(f);
	SeekBeg(f, z->data.Offset);
	z->WriteLocal(f);

	SeekBeg(f, LocalFileEnd);
	return z;
}

#endif //!NO_COMPRESSION
