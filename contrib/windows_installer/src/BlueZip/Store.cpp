#include "BlueHead.h"
#include <stdio.h>
#include "inflate.h"

void FileDataCopy(File From, File To, int Len)
{
	//Optimisation possible, use static buffers from Deflate/Inflate
	const int BlockSize = 4096;
	u8 Buffer[BlockSize];

	int i;
	while(Len != 0)
	{
		i = min(Len, BlockSize);
		FileRead(From, Buffer, i);
		FileWrite(To, Buffer, i);

		CRC(Buffer, i); //Perform the

		Len -= i;
	}
}


void ReadStore(File In, File Out, zList* z)
{
	//Read from In - the zip file
	//In is at the start of the file (i.e. sigLocal)
	//Out is at the begining, currently the length is 0
	//z contains the table entry

	datCentral hLocal;
	u32 sig;
	FileRead(In, &sig, 4);
	Assert(sig == sigLocal);

	hLocal.ReadLocal(In);

	//Skip to the begining of the actual data
	SeekCur(In, hLocal.lFileName + hLocal.lExtra);

	//Check that WinZip would accept this as a file, that has been stored
	if (hLocal.CompMode == compStore)
	{
		Assert(hLocal.CompSize == hLocal.OrigSize);

		FileDataCopy(In, Out, hLocal.CompSize);
	}
	else
	{
		//use the Zip inflate routine
		//use the global G
		Neil_Init(Out, In);

		//If not 0 then a failure, I think
		inflate();
	}
}

void WriteStore(File In, File Out, zList* z)
{
	//Read from In - the binary file
	//In is at the begining
	//Out is the ZIP file, and is ready to receive data
	//z contains the table entry, including the correct OrigSize
	//must set CompMode, CompSize

	z->data.CompMode = compStore;
	z->data.CompSize = z->data.OrigSize;
	FileDataCopy(In, Out, z->data.OrigSize);
}
