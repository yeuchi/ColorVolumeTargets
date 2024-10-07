#include <stdafx.h>
#include "error.h"
#include "fields.h"
#include "direntry.h"
#include "ifd.h"
#include "image.h"

IMAGE::IMAGE ()
{
	pImgBuf  = NULL;
	lImgLen	= 0;
	n_img		= NULL;
}


IMAGE::~IMAGE ()
{
}


/////////////////////////////////////////////////////////////
//PUBLIC  READ///////////////////////////////////////////////

long IMAGE::Read ( FILE *in_stream, IFD *ifd, BYTE *pImg )
{
	long	lByteRead  = 0;
	long	lByteCount = 0;

	int	iCntdex;
	int	iOffdex;

	long	*pCntdex;
	long	*pOffdex;
	
	long	nNumStrp;


/* FIND TAGS:				StripByteCounts & StripOffset
 * FIND # OF STRIPS
 */
	iCntdex	= ifd->findTAG ( STRIPBYTECOUNTS );
	iOffdex	= ifd->findTAG ( STRIPOFFSETS );

	ifd->entry = (DirEntry *) ifd->de[iOffdex];
	nNumStrp = ifd->entry->lCount;

	if ( iCntdex < 0 ) 
		return iCntdex;

	if ( iOffdex < 0 ) 
		return iOffdex;


/* POINT TO DATA
 */
	ifd->entry = (DirEntry *) ifd->de[iCntdex];

	if ( ifd->entry->lCount > 1 )
		pCntdex = ( long * ) ifd->entry->Buf;

	else
		pCntdex = ( long * ) &ifd->entry->lValOff;


	ifd->entry = (DirEntry *) ifd->de[iOffdex];

	if ( ifd->entry->lCount > 1 )
		pOffdex = ( long * ) ifd->entry->Buf;
	else
		pOffdex = ( long * ) &ifd->entry->lValOff;


/* ALLOCATE BUFFER
 */	
	for ( int a = 0; a < nNumStrp; a ++ )
		lByteCount += pCntdex[a];
	
	if ( !pImg )
	{
		pImgBuf = new BYTE [ lByteCount ];
		pImg = pImgBuf;
	}


/* READ IMAGE
*/
	int b = 0;

	
	while ( ( b < nNumStrp ) && ( lByteRead + pCntdex[b] <= lByteCount ) )
	{
		fseek ( in_stream, pOffdex[b], SEEK_SET ); 
		lByteRead += fread ( ( pImg + lByteRead ), sizeof (BYTE), pCntdex[b], in_stream );
		b ++;
	}
	lImgLen = lByteRead;
	return lByteRead;
}


long IMAGE::Write ( FILE *out_stream, IFD *ifd, BYTE * pImg )
{
	long	lByteWrote  = 0;
	long	lByteCount = 0;

	int	iCntdex;
	int	iOffdex;

	long	*pCntdex;
	long	*pOffdex;
	
	long	nNumStrp;

	if ( ! pImgBuf )
		return MISSINIMAGE;


/* FIND TAGS:				StripByteCounts & StripOffset
 * FIND # OF STRIPS
 */
	iCntdex	= ifd->findTAG ( STRIPBYTECOUNTS );
	iOffdex	= ifd->findTAG ( STRIPOFFSETS );
	
	ifd->entry = (DirEntry *) ifd->de[iOffdex];
	nNumStrp = ifd->entry->lCount;

	if ( iCntdex < 0 ) 
		return iCntdex;

	if ( iOffdex < 0 ) 
		return iOffdex;


/* POINT TO DATA
 */
	ifd->entry = (DirEntry *) ifd->de[iCntdex];

	if ( ifd->entry->lCount > 1 )
		pCntdex = ( long * ) ifd->entry->Buf;

	else
		pCntdex = ( long * ) &ifd->entry->lValOff;


	ifd->entry = (DirEntry *) ifd->de[iOffdex];

	if ( ifd->entry->lCount > 1 )
		pOffdex = ( long * ) ifd->entry->Buf;
	else
		pOffdex = ( long * ) &ifd->entry->lValOff;


/* CHECK BUFFER SIZE
*/	
	long *ptr = pCntdex;
	for ( int a = 0; a < nNumStrp; a ++ )
		lByteCount += *ptr ++;
	
	if ( lByteCount != lImgLen )
		return BADIMAGESIZE;


/* WRITE IMAGE
*/
	int b = 0;

	if ( !pImg )
		pImg = pImgBuf;

	while ( ( b < nNumStrp ) && ( lByteWrote + pCntdex[b] <= lByteCount ) )
	{
		fseek ( out_stream, pOffdex[b], SEEK_SET ); 
		lByteWrote += fwrite( ( pImg + lByteWrote ), sizeof (BYTE), pCntdex[b], out_stream );
		b ++;
	}
	return lByteWrote;
}


