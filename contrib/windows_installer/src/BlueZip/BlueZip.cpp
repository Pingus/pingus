#include "BlueHead.h"

void BlueZip::Setup(LPCTSTR FileName)
{
	this->FileName = CopyString(FileName);

	//Initialise for a blank file name
	Files = NULL;
	Comment = NULL;
	data.lComment = 0;

#ifndef NO_COMPRESSION
	Pending = NULL;
#endif
}

BlueZip::~BlueZip()
{
	delete[] FileName;
	if (Comment) delete[] Comment;

	while (Files != NULL)
	{
		zList* z = Files;
		Files = Files->next;
		delete z;
	}

#ifndef NO_COMPRESSION
	while (Pending != NULL)
	{
		fList* f = Pending;
		Pending = Pending->next;
		delete f;
	}
#endif
}

#ifndef NO_COMPRESSION

void BlueZip::AddFile(LPCTSTR FileName)
{
	Pending = new fList(FileName, Pending);
}

void BlueZip::WriteEnd(File f)
{
	//Handle mutliple disks correctly (i.e. don't)
	data.DiskNo = 0;
	data.DiskOne = 0;
	data.DiskNum = data.Count;

	long l = sigEnd;
	FileWrite(f, &l, 4);
    data.Write(f);
	if (data.lComment)
		FileWrite(f, Comment, data.lComment);
}

#endif //!NO_COMPRESSION


void BlueZip::ReadEnd(File f)
{
    data.Read(f);

	if (data.lComment)
	{
		Comment = new char[data.lComment];
		FileRead(f, Comment, data.lComment);
	}
}

bool BlueZip::GetFile(zList* z, LPCTSTR FileName)
{
	//First open the ZIP file for reading
	File Orig = FileOpenRead(this->FileName);
	if (!FileValid(Orig))
	{
		ErrMsg("Failed to open the reading file");
		return false;
	}

	File Out = FileOpenWrite(FileName);
	if (!FileValid(Out))
	{
		ErrMsg("Failed to open the output file");
		return false;
	}

	//Perform CRC checks yourself
	InitCRC();
	SeekBeg(Orig, z->data.Offset + FilePosDelta);
	ReadStore(Orig, Out, z);

	if (z->data.CRC != GetCRC())
	{
		ErrMsg("Failed on the CRC");
		return false;
	}

	FileClose(Orig);
	FileClose(Out);
	return true;
}
