/* ------------------------------------------------------------------------
 *	TIFF class members
 *
 *	Use this class to read write tif 6.0 version files.  Sub-classes of
 * TIFF are IFD, IMAGE and DIR_ENTRY.
 *
 * ------------------------------------------------------------------------
 */


#include <stdafx.h>
#include "error.h"
#include "fields.h"
#include "direntry.h"
#include "ifd.h"
#include "Image.h"
#include "tiff.h"



TIFF::TIFF()
{
	lReadPos  = 0;
	lWritePos = 0;
	nNumIFD	 = 0;	

	ifd = NULL;
	img = NULL;


/* TIFF header
*/
	hdr.Type[0] = 'I';
	hdr.Type[1] = 'I';
	hdr.version = 42;
	hdr.fstIFD	= 8;
	hdr.reserve = 0;
}


TIFF::~TIFF()
{
	deleteAll ();
	nNumIFD = 0;
}


void TIFF::deleteAll ()
{
	IFD *x_ifd, *xx_ifd;


	if ( ifd )
	{
		if ( ifd->n_ifd )
		{
			x_ifd = ifd->n_ifd;
		
			while ( x_ifd->n_ifd )
			{
				xx_ifd = x_ifd->n_ifd;
				delete x_ifd;

				if ( xx_ifd->n_ifd )
				{
					x_ifd = xx_ifd->n_ifd;
					delete xx_ifd;
				}
			}
		}
		delete ifd;
	}


	IMAGE *x_img, *xx_img;

	if ( img )
	{
		if ( img->n_img )
		{
			x_img = img->n_img;
		
			while ( x_img->n_img )
			{
				xx_img = x_img->n_img;
				delete x_img;

				if ( xx_img->n_img )
				{
					x_img = xx_img->n_img;
					delete xx_img;
				}
			}
		}
		delete img;
	}
}


/////////////////////////////////////////////////////////////
//PUBLIC READ////////////////////////////////////////////////


/* --------------------------------------------------------------
 * Read
 */
int TIFF::Read ( char *strFile )
{
	int	a = 0;
	int	iErr;
	long	lOffset;
	

	deleteAll ();

/* OPEN FILE
*/
	if ( in_stream = fopen ( strFile, "r+b" ) )
	{


/* READ HEADER
*/
		iErr = readTIFFhdr ();
		
		if ( iErr < 0 )
		{
			fclose ( in_stream );
			return iErr;
		}



/* READ 0th IFD	TIFF file must have 1 IFD & Image.  So, I am
						loading this minimum... any additional IFD &
						Images are loaded in the while loop.
*/	
		ifd = new IFD ();
		IFD	*x_ifd = ifd;
		iErr = x_ifd->Read ( in_stream, hdr.fstIFD );

		if ( iErr < 0 )
		{
			fclose ( in_stream );
			return iErr;
		}
		nNumIFD = 1;


/* READ 0th IMAGE
*/
		img = new IMAGE ();
		IMAGE	*x_img = img;
		iErr = x_img->Read ( in_stream, x_ifd, NULL );

		if ( iErr < 0 )
		{
			fclose ( in_stream );
			return iErr;
		}


/* LOOP Nth TIME:  TIFF file may contain Nth number of IFD & Images
						 So, I need to repeat the load process until all 
						 images, planes and IFDs are uploaded.
*/
		while ( x_ifd->nNextIFD )							
		{


/* READ Nth IFD 
*/
			lOffset			= x_ifd->nNextIFD;
			x_ifd->n_ifd	= new IFD ();
			x_ifd				= x_ifd->n_ifd;

			iErr = x_ifd->Read ( in_stream, lOffset );

			if ( iErr < 0 )
			{
				fclose ( in_stream );
				return iErr;
			}
			nNumIFD ++;


/* READ Nth IMAGE
*/
			x_img->n_img	= new IMAGE ();
			x_img				= x_img->n_img;

			iErr = img->Read ( in_stream, x_ifd, NULL );
					
			if ( iErr < 0 )
			{
				fclose ( in_stream );
				return iErr;
			}
		}	
		fclose ( in_stream );
		return OKAY;
	}
	return BADFILEOPEN;	
}


/* --------------------------------------------------------------
 * GetNumIFD:	Number of Image File Directory
 */
int TIFF::GetNumIFD ()
{	
	return nNumIFD;
}


/* --------------------------------------------------------------
 * GetNumDE	Number of Directory Entries in xth IFD
 */
int TIFF::GetNumDE ( int iIFD )
{
	if ( iIFD >= nNumIFD )
		return NOSUCHIFD;


/* LOCATE IFD
*/
	int a			= 0;
	IFD *x_ifd	= ifd;
	IFD *xx_ifd = ifd;

	while ( a < iIFD )
	{
		x_ifd	 = xx_ifd->n_ifd;			
		xx_ifd = x_ifd->n_ifd;
		a ++;
	}

	return x_ifd->nNumDE;
}


/* --------------------------------------------------------------
 * GetTIFFimg
 */
int TIFF::GetImagePtr ( int iIFD, BYTE **pImg, long *pLen )
{
	if ( iIFD >= nNumIFD )
		return NOSUCHIMAGE;


/* LOCATE IMAGE
*/
	int a		= 0;
	IMAGE	*x_img	= img;
	IMAGE *xx_img	= img;

	while ( a < iIFD )
	{
		x_img  = xx_img->n_img;
		xx_img = x_img->n_img;
		a ++;
	}


/* FOUND IT!
*/
	*pImg = x_img->pImgBuf;
	*pLen = x_img->lImgLen;

	return OKAY;
}


void* TIFF::GetTAGdata ( int iIFD, short nTAG )
{
	if ( iIFD >= nNumIFD )
		return NULL;


/* LOCATE IFD
*/
	int a			= 0;
	IFD *x_ifd	= ifd;
	IFD *xx_ifd = ifd;

	while ( a < iIFD )
	{
		x_ifd	 = xx_ifd->n_ifd;			
		xx_ifd = x_ifd->n_ifd;
		a ++;
	}


/* LOCATE DIRECTORY ENTRY
*/
	a = 0;
	while ( a < x_ifd->nNumDE )
	{
		x_ifd->entry = (DirEntry *)x_ifd->de[a];

		if ( x_ifd->entry->nTAG == nTAG )
			return ( x_ifd->entry->Buf );
	
		a ++;
	}
	return NULL;
}

/* --------------------------------------------------------------
 * GetTAGinfo
 */
int TIFF::GetTAGinfo ( int iIFD, short nTAG, short *pType, 
							  long *pCount, long *pValOff )
{
	if ( iIFD >= nNumIFD )
		return NOSUCHIFD;


/* LOCATE IFD
*/
	int a			= 0;
	IFD *x_ifd	= ifd;
	IFD *xx_ifd = ifd;

	while ( a < iIFD )
	{
		x_ifd	 = xx_ifd->n_ifd;			
		xx_ifd = x_ifd->n_ifd;
		a ++;
	}


/* LOCATE DIRECTORY ENTRY
*/
	a = 0;
	while ( a < x_ifd->nNumDE )
	{
		x_ifd->entry = (DirEntry *) x_ifd->de[a];

		if ( x_ifd->entry->nTAG == nTAG )
		{
			*pType	= x_ifd->entry->nType;
			*pCount	= x_ifd->entry->lCount;
			*pValOff = x_ifd->entry->lValOff;

			return OKAY;
		}
		a ++;
	}
	return NOSUCHTAG;
}


/////////////////////////////////////////////////////////////
//PUBLIC WRITE///////////////////////////////////////////////


/* --------------------------------------------------------------
 * Write
 */
long TIFF::Write ( char *strFile  )
{
	int iErr;

	if ( ( ! ifd ) || ( ! img ) || ( ! nNumIFD ) )
		return NOIFD;

	if ( out_stream = fopen ( strFile, "w+b" ) )
	{
		writeTIFFhdr ();
		lWritePos = 8;

		iErr = ifd->Write ( out_stream, lWritePos );
			
		if ( iErr < 0 )
			return iErr;

		iErr = img->Write ( out_stream, ifd, NULL );

		if ( iErr < 0 )
			return iErr;

		fclose ( out_stream );
		return img->lImgLen;
	}
	return BADFILEOPEN;
}


/* --------------------------------------------------------------
 * PutNumIFD
 */
int TIFF::PutNumIFD ( int iNumIFD )
{
	int a			 = 0;
	IFD   *x_ifd = ifd;
	IMAGE *x_img = img;

	if ( ( nNumIFD ) || ( ifd ) || ( img ) )
		return ALREADYALLOCATED;


/* ALLOCATE IFDs
*/
	ifd = new IFD;
	img = new IMAGE;
	nNumIFD = 1;

	while ( a < iNumIFD - 1 )
	{
		x_ifd->n_ifd = new IFD;
		x_ifd = x_ifd->n_ifd;

		x_img->n_img = new IMAGE;
		x_img = x_img->n_img;


		nNumIFD ++;
		a ++;
	}
	return OKAY;
}


/* --------------------------------------------------------------
 * initDEs
 */
int TIFF::PutNumDE ( int iIFD, int iNumDE )
{
	if ( iIFD >= nNumIFD )
		return NOSUCHIFD;


/* LOCATE IFD
*/
	int a			= 0;
	IFD *x_ifd	= ifd;
	IFD *xx_ifd = ifd;

	while ( a < iIFD )
	{
		x_ifd	 = xx_ifd->n_ifd;			
		xx_ifd = x_ifd->n_ifd;
		a ++;
	}


/* ALLOCATE DIRECTORY ENTRIES
*/
	if ( ( x_ifd->de ) || ( x_ifd->nNumDE ) )
		return ALREADYALLOCATED;


	x_ifd->nNumDE = iNumDE;
	x_ifd->de	  = new long *[iNumDE];
	
	for ( a = 0; a < iNumDE; a ++ )
	{
		x_ifd->entry = new DirEntry ();
		x_ifd->de[a] = (long *) x_ifd->entry;
	}
	return OKAY;
}


/* --------------------------------------------------------------
 * initImages
 */
int TIFF::PutImagePtr ( int iIFD, BYTE *pImg, long pLen )
{
	if ( iIFD >= nNumIFD )
		return NOSUCHIFD;


/* LOCATE IMAGE
*/
	int a		= 0;
	IMAGE	*x_img	= img;
	IMAGE *xx_img	= img;

	while ( a < iIFD )
	{
		x_img  = xx_img->n_img;
		xx_img = x_img->n_img;
		a ++;
	}

	if ( x_img->pImgBuf )
		return ALREADYALLOCATED;

	x_img->lImgLen = pLen;
	x_img->pImgBuf = pImg;

	return OKAY;
}


/* --------------------------------------------------------------
 * PutTAGinfo
 */
int TIFF::PutTAGinfo ( int iIFD, short nTAG, short nType, 
							  long lCount, long lValOff, void *Buf )
{
	int a			= 0;
	IFD *x_ifd  = ifd;
	IFD *xx_ifd = ifd;


/* LOCATE IFD
*/
	while ( a < iIFD )
	{
		if ( ! xx_ifd->n_ifd )
			return NOSUCHIFD;
	
		x_ifd  = xx_ifd->n_ifd;
		xx_ifd = x_ifd->n_ifd;
		a ++;
	}


/* LOCATE DIRECTORY ENTRY
*/
	while ( a < x_ifd->nNumDE )
	{
		x_ifd->entry = (DirEntry *) x_ifd->de[a];

		if ( ! x_ifd->entry->nTAG )
		{
			x_ifd->entry->nTAG	 = nTAG;
			x_ifd->entry->nType   = nType;
			x_ifd->entry->lCount  = lCount;
			x_ifd->entry->lValOff = lValOff;

			long lSize = x_ifd->entry->ByteCount() * lCount;
			if ( lSize > LONG )
			{
				if ( Buf )
				{
					x_ifd->entry->Buf = new BYTE [ lSize ]; 
					memcpy ( x_ifd->entry->Buf, Buf, lSize);
				}
				else
					return MISSINDATABUF;
			}
			return OKAY;
		}
		a ++;
	}
	return NOROOMFORTAG;
}


/////////////////////////////////////////////////////////////
//PRIVATE READ///////////////////////////////////////////////


/* --------------------------------------------------------------
 * readTIFFhdr
 */
int TIFF::readTIFFhdr ()
{
	lReadPos = fread((void *)&hdr, sizeof (char), HEADER_LEN, in_stream);

	if (	( hdr.Type[0] != 'I' ) || ( hdr.Type[1] != 'I' )	)	
		return BADBYTEORDER;										

	if ( hdr.version != 42 )
		return BADTIFFVERSION;

	if ( hdr.reserve != 0 )
		return BADTIFFHEADER;


	return OKAY;
}


/////////////////////////////////////////////////////////////
//PRIVATE WRITE//////////////////////////////////////////////


/* --------------------------------------------------------------
 * writeTIFFhdr
 */
int TIFF::writeTIFFhdr ()
{
	lWritePos = fwrite((void *)&hdr, sizeof (char), 8, out_stream);
	return OKAY;
}

