#include "BlueHead.h"

#ifndef NO_COMPRESSION

//Write the file back out, with the changes
bool BlueZip::Write(bool Store)
{
	//TODO: Do not use a TempFile, send straight to the output

	char TempFileName[MAX_PATH];
	File f;

	zList* z;
	zList** next = &Files; //where to insert the next zList

	int i, j; //global enumeration variables

	if ((Files == NULL) && (Pending == NULL))
	{
		ErrMsg("Blank ZIP files not allowed");
		return false;
	}

	//Always use a temporary file name (they may have the ZIP file on a floppy)
	f = FileOpenTemp(TempFileName);
	if (!FileValid(f))
	{
		ErrMsg(Failed to open the temporary file);
		return false;
	}

	if (Files != NULL)
	{
		File Orig = FileOpenRead(FileName);
		if (!FileValid(Orig))
		{
			ErrMsg("Failed to open the reading file");
			return false;
		}

		const int BlockSize = 4096;
		char* Buffer = new char[BlockSize];
		datCentral hLocal;
		for (z = Files; z != NULL; z = z->next)
		{
			if (!z->Delete)
			{
				//Remove any that have dropped out of the list
				*next = z;
				next = &z->next;

				//Perform a ZIP copy
				SeekBeg(Orig, z->data.Offset + z->FileDeltaPos);
				z->data.Offset = FilePos(f);

				u32 sig;
				FileRead(Orig, &sig, 4);
				Assert(sig == sigLocal);
				FileWrite(f, &sig, 4);

				hLocal.ReadLocal(Orig);
				hLocal.WriteLocal(f);

				i = hLocal.CompSize + hLocal.lFileName + hLocal.lExtra;
				while(i != 0)
				{
					j = min(i, BlockSize);
					FileRead(Orig, Buffer, j);
					FileWrite(f, Buffer, j);
					i -= j;
				}
			}
		}
		FileClose(Orig);
		delete[] Buffer;
	}


	while (Pending != NULL)
	{
		fList* fAdd = Pending;
		Pending = Pending->next;

		z = fAdd->ZipUp(f, Store);
		if (z == NULL)
		{
			ErrMsg("Failed to add the file");
		}
		else
		{
			*next = z;
			next = &z->next;
		}

		delete fAdd;

	}

	//Write out the central header
	data.Count = 0;
	data.Offset = FilePos(f);
	for (z = Files; z != NULL; z = z->next, data.Count++)
		z->WriteCentral(f);

	data.Size = FilePos(f) - data.Offset;
	WriteEnd(f);

	FileClose(f);

	//Using a temp file
	if (!FileReplace(FileName, TempFileName))
	{
		ErrMsg("Failed to copy the temporary file");
		return false;
	}

	return true;
}


#endif //!NO_COMPRESSION
