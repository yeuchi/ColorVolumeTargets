
#define HEADER_LEN						8
#define DIR_ENTRY_LEN					12

/* TYPE
*/
#define ASCII								1
#define SHORT								2
#define LONG								4
#define RATIONAL							8



class DirEntry
{
public:
	
	DirEntry ();
	~DirEntry ();

public:

	long	Read			( FILE *, long );
	long	Write			( FILE *, long );
	int	ByteCount	();

private:

	long	readData		( FILE *, long );
	
	long	writeData	( FILE *, long );

public:

	short nTAG;							// Identifier, see #define above
	short nType;						// Type of Data in directory entry
	long	lCount;						// number of data entry
	long	lValOff;						// data entry or offset to data in file


public:

	void *Buf;							// data entry when data > sizeof(long)
};