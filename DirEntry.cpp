#include <stdafx.h>
#include "error.h"
#include "fields.h"
#include "direntry.h"


DirEntry::DirEntry ()
{
	Buf		= NULL;

	nTAG		= 0;
	nType		= 0;
	lCount	= 0;
	lValOff	= 0;
}


DirEntry::~DirEntry ()
{
	if ( Buf )
		delete Buf;
}


///////////////////////////////////////////////////////////////////
//PUBLIC///////////////////////////////////////////////////////////

long DirEntry::Read ( FILE *in_stream, long lOffset )
{
	int	iCnt;
	long	lByteRead = 0;



/* DIRECTORY ENTRY
*/
	fseek ( in_stream, lOffset, SEEK_SET );

	iCnt = fread ( &nTAG,		SHORT, 1, in_stream );
	lByteRead += iCnt * SHORT;
	iCnt = fread ( &nType,		SHORT, 1, in_stream );
	lByteRead += iCnt * SHORT;
	iCnt = fread ( &lCount,		LONG,	1, in_stream );
	lByteRead += iCnt * LONG;
	iCnt = fread ( &lValOff,	LONG, 1, in_stream );
	lByteRead += iCnt * LONG;

	if ( lByteRead != DIR_ENTRY_LEN )
		return BADTAGREAD;



/* ADDITIONAL DATA
*/
	long lNumByte = ByteCount() * lCount;

	if ( lNumByte > LONG )
		lByteRead += readData ( in_stream, lNumByte );

	else if ( lNumByte < 0 )
		return BADTAGREAD;

	return lByteRead;
}


/* --------------------------------------------------------------
 * Write
 */
long DirEntry::Write ( FILE *out_stream, long lOffset )
{
	int	iCnt;
	long	lByteWrote = 0;



/* WRITE ENTRY
*/
	fseek ( out_stream, lOffset, SEEK_SET );

	iCnt = fwrite ( &nTAG,		SHORT, 1, out_stream );
	lByteWrote += iCnt * SHORT;
	iCnt = fwrite ( &nType,		SHORT, 1, out_stream );
	lByteWrote += iCnt * SHORT;
	iCnt = fwrite ( &lCount,	LONG,	1, out_stream );
	lByteWrote += iCnt * LONG;
	iCnt = fwrite ( &lValOff,	LONG, 1, out_stream );
	lByteWrote += iCnt * LONG;

	if ( lByteWrote != DIR_ENTRY_LEN )
		return BADWRITE;


	
/* WRITE DATA
*/
	long lNumByte = ByteCount() * lCount;

	if ( lNumByte > LONG )
	{
		if ( !Buf )
			return NODATATOWRITE;

		lByteWrote += writeData ( out_stream, lNumByte );
	}
	else if ( lNumByte < 0 )
		return BADTAGREAD;

	return lByteWrote;
}

///////////////////////////////////////////////////////////////////
//PRIVATE//////////////////////////////////////////////////////////


long DirEntry::readData ( FILE *in_stream, long lLen )
{
	fseek ( in_stream, lValOff, SEEK_SET );

	Buf = new BYTE [ lLen ];
	return ( fread ( Buf, 1, lLen, in_stream ) );
}


/* --------------------------------------------------------------
 * writeData	- writes data
 */
long DirEntry::writeData ( FILE *out_stream, long lLen )
{
	fseek ( out_stream, lValOff, SEEK_SET );
	return ( fwrite ( Buf, 1, lLen, out_stream ) );
}


/* --------------------------------------------------------------
 * ByteCount
 */
int DirEntry::ByteCount ()
{

	switch( nType )// Code		Type			Remarks
	{				
	case 1:			//	01H		byte			8 bit byte		
	case 2:			//	02H		ascii			8 bit ascii code
	case 6:			//	06H		sbyte			8 bit signed integer
	case 7:			//	07H		undefined	8 bit contain anything
		return ASCII;

	case 3:			//	03H		short			16 bit unsigned integer
	case 8:			//	08H		sshort		16 bit signed integer
		return SHORT;

	case  4:			//	04H		long			32 bit unsigned integer
	case 11:			//	0BH		float			4 byte single percision
		return LONG;

	case 5:			//	05H		rational		2 long numbers									
						//				1st long		= number of a fraction
						//				2nd long		= denominator

	case 9:			//	09H		slong			32 bit signed integer
	case 10:			//	0AH		rational		2 slong numbers
						//				1st slong	= numerator of a fraction
						//				2nd slong	= denominator

	case 12:			//	0CH		doublet		8 byte double precision  
						//								  IEEE floating point
		return RATIONAL;
	}
	return BADDATATYPE;
}