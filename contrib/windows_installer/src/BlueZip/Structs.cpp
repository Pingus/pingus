#include "BlueHead.h"

#define Set16(pos, res) \
	*((u16*) (&data[pos])) = res

#define Set32(pos, res) \
	*((u32*) (&data[pos])) = res

#define Get16(pos) \
	*((u16*) (&data[pos]))

#define Get32(pos) \
	*((u32*) (&data[pos]))



void datCentral::ReadLocal(File f)
{
	char data[ldatLocal]; //26
	FileRead(f, data, ldatLocal);

	//16
	VerNeed =	Get16(0);
	Flags =		Get16(2);
	CompMode =	Get16(4);
	Time =		Get16(6);
	Date =		Get16(8);
	//32
	CRC =		Get32(10);
	CompSize =	Get32(14);
	OrigSize =	Get32(18);
	//16
	lFileName =	Get16(22);
	lExtra =	Get16(24);
}

void datCentral::WriteLocal(File f)
{
	char data[ldatLocal]; //26

	//16
	Set16(0, VerNeed);
	Set16(2, Flags);
	Set16(4, CompMode);
	Set16(6, Time);
	Set16(8, Date);
	//32
	Set32(10, CRC);
	Set32(14, CompSize);
	Set32(18, OrigSize);
	//16
	Set16(22, lFileName);
	Set16(24, lExtra);

	FileWrite(f, data, ldatLocal);
}

void datCentral::Read(File f)
{
	char data[ldatCentral]; //42
	FileRead(f, data, ldatCentral);

	//16
	VerMake =	Get16(0);
	VerNeed =	Get16(2);
	Flags =		Get16(4);
	CompMode =	Get16(6);
	Time =		Get16(8);
	Date =		Get16(10);
	//32
	CRC =		Get32(12);
	CompSize =	Get32(16);
	OrigSize =	Get32(20);
	//16
	lFileName = Get16(24);
	lExtra =	Get16(26);
	lComment =	Get16(28);
	Disk =		Get16(30);
	IntAttr =	Get16(32);
	//32
	ExtAttr =	Get32(34);
	Offset =	Get32(38);
}

void datCentral::Write(File f)
{
	char data[ldatCentral]; //42

	//16
	Set16(0, VerMake);
	Set16(2, VerNeed);
	Set16(4, Flags);
	Set16(6, CompMode);
	Set16(8, Time);
	Set16(10, Date);
	//32
	Set32(12, CRC);
	Set32(16, CompSize);
	Set32(20, OrigSize);
	//16
	Set16(24, lFileName);
	Set16(26, lExtra);
	Set16(28, lComment);
	Set16(30, Disk);
	Set16(32, IntAttr);
	//32
	Set32(34, ExtAttr);
	Set32(38, Offset);

	FileWrite(f, data, ldatCentral);
}

void datEnd::Read(File f)
{
	char data[ldatEnd]; //18
	FileRead(f, data, ldatEnd);

	//16
	DiskNo =	Get16(0);
	DiskOne =	Get16(2);
	DiskNum =	Get16(4);
	Count =		Get16(6);
	//32
	Size =		Get32(8);
	Offset =	Get32(12);
	//16
	lComment =	Get16(16);
}

void datEnd::Write(File f)
{
	char data[ldatEnd]; //18

	//16
	Set16(0, DiskNo);
	Set16(2, DiskOne);
	Set16(4, DiskNum);
	Set16(6, Count);
	//32
	Set32(8, Size);
	Set32(12, Offset);
	//16
	Set16(16, lComment);

	FileWrite(f, data, ldatEnd);
}

