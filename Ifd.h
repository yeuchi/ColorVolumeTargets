




class IFD
{
public:
	
	IFD();
	~IFD();

public:
	
	long	Read  ( FILE *, long );	
	long	Write ( FILE *, long );

	int	findTAG ( int );

private:

	void deleteAll ();

public:
	
	short			nNumDE;				// Number of Directory Entry
	DirEntry		*entry;
	long			**de;					// Directory Entry pointer
	long			nNextIFD;			// Offset to next Image File Directory

	IFD			*n_ifd;				// next IFD class's address


public:

	long			bumper;				// total length of all extra data 
	long			lByteRead;
	long			lByteWrote;
};