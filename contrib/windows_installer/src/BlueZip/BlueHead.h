#define WIN32_MEAN_AND_LEAN
#include <windows.h>

#ifdef _DEBUG
# define Assert(a) if (!(a)) _asm int 3;
# define AssertD(a)if (!(a)) _asm int 3; //use if it has to create extra data to Assert on
#else
# define Assert(a) __assume(a); //Microsoft specific, speed up programs
# define AssertD(a)	//nothing
#endif

#if defined(_DEBUG) && defined(ERR_BREAK)
# define ErrMsg(a) _asm int 3;
#else
# define ErrMsg(a) //nothing
#endif

//to remove Compression define NO_COMPRESSION
#define NO_COMPRESSION

//Make good programming practice
#ifndef PLAY_NICELY
#define malloc		MALLOC_IS_OLD
#define free		FREE_IS_OLD
#define strdup		STRDUP_IS_OLD
#define BOOL		BOOL_IS_FOR_CRAP_C_COMPILERS_ONLY
#endif //!PLAY_NICELY

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned int uint;

// Macros for reading shorts and longs from an array of bytes
u16 inline stream16(char* b, int i)
{
	return *((u16*) &b[i]);
}

u32 inline stream32(char* b, int i)
{
	return *((u32*) &b[i]);
}

class datLocal;
class datCentral;
class datEnd;

#include "General.h"
#include "FileIO.h"
#include "Structs.h"


class BlueZip;
class zList;
class fList;

#include "BlueZip.h"
#include "zList.h"
#include "fList.h"

//From CRC
void CRC(const u8* buf, uint len);
void InitCRC();
u32 GetCRC();


//From Store.cpp
void ReadStore(File In, File Out, zList* z);
void WriteStore(File In, File Out, zList* z);
