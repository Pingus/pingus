#include "BlueHead.h"

bool BlueZip::Read()
{
	File f = FileOpenRead(FileName);				//ZIP file

	if (!FileValid(f))
	{
		ErrMsg("Failed to open the ZIP file");
		return false;
	}

    bool Res = ScanZip(f);
    FileClose(f);
	return Res;
}


bool BlueZip::ScanZip(File f)
{
	int i, n;				//Global enumeration variable
	long sig;				//signature variable

	//TASK 1: Find the last occurance of 'sigEnd' in the file
	//Use a 4096 character buffer for decent sized transfers
	const int BlockSize = 4096;

	char* EndBuffer = new char[BlockSize + 3]; //do not overflow
	int fLen = FileLen(f);
	EndBuffer[0] = 0; //ensure you do not match already present static

	do
	{
		//Copy over the trailing information
		EndBuffer[BlockSize+0] = EndBuffer[0];
		EndBuffer[BlockSize+1] = EndBuffer[2];
		EndBuffer[BlockSize+2] = EndBuffer[1];

		if (fLen >= BlockSize)
		{
			n = 0;
			fLen -= BlockSize;
			SeekBeg(f, fLen);
			FileRead(f, EndBuffer, BlockSize);
		}
		else
		{
			n = BlockSize - fLen;
			SeekBeg(f, 0);
			FileRead(f, &EndBuffer[n], fLen);
			fLen = 0;
		}
		
		//Look for the end signature
		for (i = BlockSize; i >= n; i--)
		{
			if (stream32(EndBuffer, i) == sigEnd)
			{
				//Use n as a temp variable
				n = fLen + (i-n) + 4;
				fLen = -1;
				SeekBeg(f, n); //Skip over the signature
				break; //for
			}
		}
	}
	while (fLen > 0);
	delete[] EndBuffer;
	//warn: garbage at the end of the file ignored

	if (fLen == 0)
	{
		ErrMsg("Failed to find the end signature");
		return false;
	}

	//END TASK 1

	//TASK 2: Read in the end header
	ReadEnd(f);

	//Check for mutliple disks
	if (data.DiskNum != data.Count)
	{
		ErrMsg("Does not support multiple disk archives");
		return false;
	}
	//END TASK 2

	//TASK 3: Read central header
	//Goto the central header
	//This is directly before the footer
	n -= data.Size + 4;
    SeekBeg(f, n);
	FilePosDelta = n - data.Offset;

#ifdef _DEBUG
	i = 0; //do a manual count of the files
#endif

	FileRead(f, &sig, 4);
	zList** next = &Files;

    while (sig == sigCentral)
	{
		zList* z = new zList;

		// Insert into list
		*next = z;
		next = &z->next;

		z->ReadCentral(f);

#ifdef _DEBUG //Maintain a count of the files
		i++;
#endif

		// Read next signature
		FileRead(f, &sig, 4);
	}
	//END TASK 3

	AssertD(data.Count == i); //Unexpected number of items

	//Neil alteration
	//DO NOT READ IN THE LOCAL HEADERS, assume they are correct
	//and do not contain any different information from the
	//central ones

	//Results: Speed up, smaller size
	//         Less code for me to write
	//         Does not detect badly encoded ZIP files (but does maintain them)


    return true;
}
