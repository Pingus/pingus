#include "CompHead.h"

#ifndef NO_COMPRESSION

#ifdef _DEBUG
    extern u32 isize;           /* byte length of input file, for debug only */
#endif

File ifile;      /* file to compress */
File zfile;            /* output zip file */

void SetupDeftree(File In, File Out, int my_level)
{
	ifile = In;
	zfile = Out;
}


uint ReadBuf(char* buf, uint size)
{
	//IN assertion: size >= 2 (for end-of-line translation)

	//Do not do end of line translation, who cares (use TextPad!)
	const uint len = FileReadBuf(ifile, buf, size);
	if (len != 0)
		CRC((u8*) buf, len);

#ifdef _DEBUG
	isize += (u32) len;
#endif
	return len;
}

void flush_outbuf(char* buf, uint* size)
{
    if (*size != 0) {
        FileWrite(zfile, buf, *size);
    }
	*size = 0;
}



#endif //!NO_COMPRESSION