
//Compression modes
enum CompModes
{
	compStore = 0,
	compDeflate = 8,
};

//Signatures
enum Signature
{
	sigLocal = 0x04034b50,
	sigCentral = 0x02014b50,
	sigEnd = 0x06054b50,
	sigExtra = 0x08074b50,		//Local
};

//Use actual headers

//LOCHEAD 26, sizeof(datLocal) = 26
const int ldatLocal = 26;
/* Reference only: Use dat Central with some unused fields
class datLocal
{
public:
	u16 VerNeed;	//LOCVER - Version needed to extract
	u16 Flags;		//LOCFLG - encrypt, deflate flags
	u16 CompMode;	//LOCHOW - compression method
	u16 Time;		//LOCTIM - last modified file time, DOS format
	u16 Date;		//LOCDAT - last modified file date, DOS format
	u32 CRC;		//LOCCRC - uncompressed crc-32 for file
	u32 CompSize;	//LOCSIZ - compressed size in zip file
	u32 OrigSize;	//LOCLEN - uncompressed size
	u16 lFileName;	//LOCNAM - length of filename
	u16 lExtra;		//LOCEXT - length of extra field

	void Read(File f);
	void Write(File f);
};
*/

/* NEIL: Not currently required
struct datExtra
{
	u32 CRC;		//EXTCRC - uncompressed crc-32 for file
	u32 CompSize;	//EXTSIZ - compressed size in zip file
	u32 OrigSize;	//EXTLEN - uncompressed size
};*/

//CENHEAD 42, sizeof(datCentral) = 42
const int ldatCentral = 42;
class datCentral
{
public:
	u16 VerMake;	//CENVEM - version made by

	//SAME AS LOCAL
	u16 VerNeed;	//LOCVER - Version needed to extract
	u16 Flags;		//LOCFLG - encrypt, deflate flags
	u16 CompMode;	//LOCHOW - compression method
	u16 Time;		//LOCTIM - last modified file time, DOS format
	u16 Date;		//LOCDAT - last modified file date, DOS format
	u32 CRC;		//LOCCRC - uncompressed crc-32 for file
	u32 CompSize;	//LOCSIZ - compressed size in zip file
	u32 OrigSize;	//LOCLEN - uncompressed size
	u16 lFileName;	//LOCNAM - length of filename
	u16 lExtra;		//LOCEXT - length of extra field
	//END

	u16 lComment;	//CENCOM - file comment length
	u16 Disk;		//CENDSK - disk number start
	u16 IntAttr;	//CENATT - internal file attributes
	u32 ExtAttr;	//CENATX - external file attributes
	u32 Offset;		//CENOFF - relative offset of local header

	void Read(File f);
	void Write(File f);

	void ReadLocal(File f);
	void WriteLocal(File f);
};

//ENDHEAD 18, sizeof(datEnd) = 18
const int ldatEnd = 18;
class datEnd
{
public:
	u16 DiskNo;		//ENDDSK - number of this disk
	u16 DiskOne;	//ENDBEG - number of the starting disk
	u16 DiskNum;	//ENDSUB - entries on this disk
	u16 Count;		//ENDTOT - total number of entries
	u32 Size;		//ENDSIZ - size of entire central directory
	u32 Offset;		//ENDOFF - offset of central on starting disk
	u16 lComment;	//ENDCOM - length of zip file comment

	void Read(File f);
	void Write(File f);
};
