#include "BlueHead.h"

zList::zList()
{
	FileName = NULL;
	Extra = NULL;
	Comment = NULL;
	next = NULL;
	Delete = false;
}

zList::~zList()
{
	if (FileName) delete[] FileName;
	if (Extra) delete[] Extra;
	if (Comment) delete[] Comment;
}

void zList::WriteLocal(File f)
{
	long l = sigLocal;
	FileWrite(f, &l, 4);
    data.WriteLocal(f);

	FileWrite(f, FileName, data.lFileName);
	if (data.lExtra) FileWrite(f, Extra, data.lExtra);
	//No comment available, only in central header
}

void zList::WriteCentral(File f)
{
	long l = sigCentral;
	FileWrite(f, &l, 4);
	data.Write(f);

	FileWrite(f, FileName, data.lFileName);
	if (data.lExtra) FileWrite(f, Extra, data.lExtra);
	if (data.lComment) FileWrite(f, Comment, data.lComment);
}

void zList::ReadCentral(File f)
{
	data.Read(f);

	// Read file name, extra field and comment field
	// if (z->nam == 0) then 0 length name error

	FileName = new char[data.lFileName+1];
	FileRead(f, FileName, data.lFileName);
	FileName[data.lFileName] = '\0';	// terminate name

	if (data.lExtra)
	{
		Extra = new char[data.lExtra];
		FileRead(f, Extra, data.lExtra);
	}

	if (data.lComment)
	{
		Comment = new char[data.lComment];
		FileRead(f, Comment, data.lComment);
	}
}


