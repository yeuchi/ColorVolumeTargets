



class IMAGE
{
public:

	IMAGE();
	~IMAGE();

	
public:

	long Read	( FILE *, IFD *, BYTE * );
	long Write	( FILE *, IFD *, BYTE * );

public:

	BYTE	*pImgBuf;					// image buffer pointer
	long	lImgLen;						// image length

	IMAGE	*n_img;						// next image class's address

};