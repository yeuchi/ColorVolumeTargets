



/* TIFF Header -- Byte Order
*/
#define INTEL								100
#define MOTOROLA							101

struct	TIFFheader	
{
	char	Type[2];				// II = Intel :: MM = Motorola
	short	version;				// version number
	short	fstIFD;				// pointer to the first image file directory
	short reserve;				// to be set to NULL
};



class TIFF
{
public:

	TIFF();
	~TIFF();

public:	// READ

	int	Read  ( char * );

	int	GetNumIFD	 ( );
	int	GetNumDE		 ( int );
	void* GetTAGdata	 ( int, short );
	int	GetTAGinfo	 ( int, short, short*, long*, long* );
	int	GetImagePtr	 ( int, BYTE **, long * );


public:	// WRITE

	long Write ( char * );


	int PutNumIFD   ( int );
	int PutNumDE	 ( int, int );
	int PutImagePtr ( int, BYTE*, long );
	int PutTAGinfo  ( int, short, short, long, long, void* );
	


private:

	int  readTIFFhdr	();
	int  writeTIFFhdr	();
	void deleteAll		();


public:	// MUST HAVE THESE IN TIFF

	IFD			*ifd;		// 0th Image File Directory
	IMAGE			*img;		// 0th Image data
	TIFFheader	hdr;		// header


private:

	FILE *in_stream;		// input stream: file access
	FILE *out_stream;		// output stream file access

	short	nNumIFD;			// number of Image File Directory
	long	lReadPos;
	long	lWritePos;

};