/* ------------------------------------------------------------------------
 *	SimpleTIFF class members
 *
 *	Use this class to read write tif 6.0 version files.  This is a 
 * simplification layer over TIFF class.... For general read & write
 * of RGB & CMYK non-compressed im_dMgnes... this is just fine.
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
#include "simpleTiff.h"


SimpleTIFF::SimpleTIFF()
{
	lWidth	= 0;
	lLength	= 0;

	lBpp		= 0;
	lSpp		= 0;

	lXdpi		= 0;
	lYdpi		= 0;

	tif	= NULL;
}


SimpleTIFF::~SimpleTIFF()
{
	deleteAll ();
}


void SimpleTIFF::deleteAll()
{
	if ( tif )
		delete tif;
}


/* -------------------------------------------------------------------
 *	Read
 */
int SimpleTIFF::Read( char *strFile, BYTE **pImg, long *wid, long *len, 
							 int *bpp, int *spp, int *clr, long *dpi )
{
	deleteAll ();
	int	iErr;
	long	lSize;



/* READ Im_dMgnE
*/
	BYTE *ptr = *pImg;

	tif = new TIFF();
	iErr = tif->Read ( strFile );
	
	if ( iErr < 0 )
		return iErr;


/* GET Im_dMgnE PTR
*/
	iErr = tif->GetImagePtr ( 0, &ptr, &lSize );
	
	if ( iErr < 0 )
		return iErr;

	*pImg = ptr;



/* GET TAG INFO
*/
	iErr = GetImageInfo ();

	if ( iErr < 0 )
		return iErr;


/* Im_dMgnE INFO
*/
	*wid = lWidth;
	*len = lLength;
	*bpp = lBpp;
	*spp = lSpp;
	*clr = lColor;
	*dpi = lXdpi;
	
	return iErr;
}


/* -------------------------------------------------------------------
 *	Write
 */
long SimpleTIFF::Write( char *strFile, BYTE *pImg, long wid, long len, 
							   int bpp, int spp, int clr, long dpi )
{
	int iErr;
	BYTE *im_dMgne = NULL;

	lWidth = wid;
	lLength	= len;
	lBpp		= bpp;
	lSpp		= spp;
	lColor	= clr;
	lXdpi		= dpi;
	lYdpi		= dpi;


/* INIT TIFF
*/
	tif = new TIFF();
	
	iErr = tif->PutNumIFD	( 1 );									// Number of Im_dMgne File Directory
	
	if ( iErr < 0 )
		return iErr;

	iErr = tif->PutNumDE		( 0, NUMOFDE );						// Number of Directory Entry
	
	if ( iErr < 0 )
		return iErr;
	
	iErr = tif->PutImagePtr	( 0, pImg, len * wid * spp );	// Number of im_dMgnes
	
	if ( iErr < 0 )
		return iErr;


/* FILL TAGs
*/
	iErr = PutImageInfo ();

	if ( iErr < 0 )
		return iErr;


/* WRITE TIFF FILE
*/
	iErr = tif->Write ( strFile );

	if ( iErr == OKAY )
		return tif->img->lImgLen;

	return iErr;
}


/* -------------------------------------------------------------------
 *	GetIm_dMgneInfo
 */
int SimpleTIFF::GetImageInfo ( )
{
	int iErr;
	short type;
	long	count, offset, value;

			/*
			 *
			 *
			 *		There must not be compression...
			 *		and plane configuraiton must be chuncky...
			 *
			 *
			 */

	// compression		259
	iErr = tif->GetTAGinfo ( 0, COMPRESSION, &type, &count, &value );			
		
	if ( iErr < 0 )
		return iErr;

	if ( value != NO_COMPRESSION )
		return BADCOMPRESSION;


	// PhotometricInterpretation 262
	
	iErr = tif->GetTAGinfo ( 0, PHOTOMETRICINTERPRETATION, &type, &count, &lColor );		
		
	if ( iErr < 0 )
		return iErr;

	if ( ( lColor != RGB_CLR ) && ( lColor != CMYK_CLR ) )
		return BAD_COLOR_SPACE;



	// PlanarConfiguraiton 284
	iErr = tif->GetTAGinfo ( 0, PLANARCONFIGURATION, &type, &count, &value );			
		
	if ( iErr < 0 )
		return iErr;

	if ( value != CHUNCKY )
		return BADPLANARCONFIG;

			/*
			 *
			 *
			 *		Everything else is trivial...
			 *
			 *		All up to the user to figure out 
			 *
			 *
			 */


	// SamplePerPixel	277
	iErr = tif->GetTAGinfo ( 0, SAMPLESPERPIXEL, &type, &count, &lSpp );		
		
	if ( iErr < 0 )
		return iErr;


	// im_dMgne width		256
	iErr = tif->GetTAGinfo ( 0, IMAGEWIDTH, &type, &count, &lWidth );		
		
	if ( iErr < 0 )
		return iErr;
		

	// im_dMgne length	257
	iErr = tif->GetTAGinfo ( 0, IMAGELENGTH, &type, &count, &lLength );		
		
	if ( iErr < 0 )
		return iErr;
		

	// BitsPerSample	258
	short *pBpp;
		
	iErr = tif->GetTAGinfo ( 0, BITSPERSAMPLE, &type, &count, &offset );
	
	if ( iErr < 0 )
		return iErr;

	pBpp = ( short * ) tif->GetTAGdata ( 0, BITSPERSAMPLE );

	for ( int a = 0; a < count-1; a ++ )
	{
		if ( pBpp[a] != pBpp[a+1] )
			return UNEQUAL_BPP;
	}
	lBpp = pBpp[0];


	// x resolution	282
	long *xres;
	iErr = tif->GetTAGinfo ( 0, XRESOLUTION, &type, &count, &offset );			
	
	if ( iErr < 0 )
		return iErr;

	xres  = ( long * ) tif->GetTAGdata ( 0, XRESOLUTION );
	lXdpi = xres[0] / xres[1];


	// y resolution	283
	long *yres;
	iErr = tif->GetTAGinfo ( 0, YRESOLUTION, &type, &count, &offset );			
	
	if ( iErr < 0 )
		return iErr;

	yres  = ( long * ) tif->GetTAGdata ( 0, YRESOLUTION );
	lYdpi = yres[0] / yres[1];

	
	return OKAY;
}


/* -------------------------------------------------------------------
 *	PutIm_dMgneInfo
 */
int SimpleTIFF::PutImageInfo ()
{
	int   iErr;
	long	lOffset = DIR_ENTRY_LEN * NUMOFDE + HEADER_LEN + LONG;


	// New subfile type		254
	iErr = tif->PutTAGinfo ( 0, NEWSUBFILETYPE, typeEncode ( LONG ), 1, 0, NULL );	

	if ( iErr < 0 )
		return iErr;


	// im_dMgne width				256
	iErr = tif->PutTAGinfo ( 0, IMAGEWIDTH, typeEncode ( SHORT ), 1, lWidth, NULL );	

	if ( iErr < 0 )
		return iErr;

	// im_dMgne length			257	
	iErr = tif->PutTAGinfo ( 0, IMAGELENGTH, typeEncode ( SHORT ), 1, lLength, NULL );	

	if ( iErr < 0 )
		return iErr;


		/*
		 *
		 *		Offset is not necessary when there is extra
		 *		data to be written.  IFDclass will determine
		 *		the appropriate location to write the data.
		 *
		 *
		 */

	// BitsPerPixel			258
	short *bpp = new short[lSpp];

	for (int a = 0; a < lSpp; a ++ )
		bpp[a] = (short)lBpp;


	iErr = tif->PutTAGinfo ( 0, BITSPERSAMPLE, typeEncode ( SHORT ), lSpp, 0, bpp );	
	lOffset += SHORT * lSpp;

	if ( iErr < 0 )
		return iErr;


	// compression				259
	iErr = tif->PutTAGinfo ( 0, COMPRESSION, typeEncode ( SHORT ), 1, NO_COMPRESSION, NULL );	

	if ( iErr < 0 )
		return iErr;


	// photometricinterpretation	262
	int m_iClr;

	switch ( lSpp )
	{
	case 3:
		m_iClr = RGB_CLR;
		break;

	case 4:
		m_iClr = CMYK_CLR;
		break;

	default:
		return BAD_SPP;
	}

	iErr = tif->PutTAGinfo ( 0, PHOTOMETRICINTERPRETATION, typeEncode ( SHORT ), 1, m_iClr, NULL );	

	if ( iErr < 0 )
		return iErr;


	// StripOffsets			273			
	iErr = tif->PutTAGinfo ( 0, STRIPOFFSETS, typeEncode ( LONG ), 1, lOffset + 500, NULL );	

	if ( iErr < 0 )
		return iErr;


	// samples per pixel		277
	iErr = tif->PutTAGinfo ( 0, SAMPLESPERPIXEL, typeEncode ( SHORT ), 1, lSpp, NULL );	

	if ( iErr < 0 )
		return iErr;


	// rows per strip			278
	iErr = tif->PutTAGinfo ( 0, ROWSPERSTRIP, typeEncode ( SHORT ), 1, lLength, NULL );	

	if ( iErr < 0 )
		return iErr;


	// strip byte count		279
	long lSize = lWidth * lLength * lSpp;
	iErr = tif->PutTAGinfo ( 0, STRIPBYTECOUNTS, typeEncode ( LONG ), 1, lSize, NULL );	

	if ( iErr < 0 )
		return iErr;


	// x resolution			282
	long *xres = new long[2];
	xres[0] = lXdpi * 10000;
	xres[1] = 10000;

	iErr = tif->PutTAGinfo ( 0, XRESOLUTION, typeEncode ( RATIONAL ), 1, 0, xres );	
	lOffset += RATIONAL;
	delete xres;

	if ( iErr < 0 )
		return iErr;


	// y resolution			283 
	long *yres = new long[2];
	yres[0] = lYdpi * 10000;
	yres[1] = 10000;

	iErr = tif->PutTAGinfo ( 0, YRESOLUTION, typeEncode ( RATIONAL ), 1, 0, yres );	
	lOffset += RATIONAL;
	delete yres;


	// PlanarConfiguration	284
	iErr = tif->PutTAGinfo ( 0, PLANARCONFIGURATION, typeEncode ( SHORT ), 1, CHUNCKY, NULL );	

	if ( iErr < 0 )
		return iErr;


	// resolution unit		296
	iErr = tif->PutTAGinfo ( 0, RESOLUTIONUNIT, typeEncode ( SHORT ), 1, INCH, NULL );	

	if ( iErr < 0 )
		return iErr;


	return OKAY;
}


/* --------------------------------------------------------------
 * typeEncode
 */
int SimpleTIFF::typeEncode ( int iByteLen )
{
	switch ( iByteLen )
	{
	case ASCII:
		return 1;

	case SHORT:
		return 3;

	case LONG:
		return 4;

	case RATIONAL:
		return 5;
	}
	return BADENTRYTYPE;
}