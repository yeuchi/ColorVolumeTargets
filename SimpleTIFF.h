
#define NUMOFDE		14

class SimpleTIFF
{
public:
	
	SimpleTIFF();
	~SimpleTIFF();


public:

	int  Read  ( char *, BYTE **, long*, long*, int*, int*, int*, long* );
	long Write ( char *, BYTE *, long, long, int, int, int, long );

private:

	int	typeEncode	 ( int );
	int	GetImageInfo ();
	int	PutImageInfo ();

	void	deleteAll();

public:

	TIFF	*tif;


public:

	long	lWidth;
	long	lLength;
	
	long	lBpp;				// bi-level, gray scale
	long	lSpp;				// number of planes

	long	lXdpi;			// x-axis dots per inch
	long	lYdpi;			// y-axis dots per inch

	long	lColor;			// photometric Interpretation

	BYTE	*pImg;			// image pointer
};