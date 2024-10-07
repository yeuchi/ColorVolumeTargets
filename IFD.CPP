#include <stdafx.h>
#include "error.h"
#include "fields.h"
#include "direntry.h"
#include "ifd.h"


IFD::IFD()
{
	nNumDE	= 0;
	nNextIFD	= 0;
	de			= NULL;
	entry		= NULL;
	n_ifd		= NULL;
}


IFD::~IFD()
{
	deleteAll ();
}


void IFD::deleteAll()
{
	if ( de )
	{
		for ( int a = 0; a < nNumDE; a ++ )
		{
			entry = (DirEntry * ) de[a];
			delete ( entry );
		}
		delete de;
		de = NULL;
	}
}


/////////////////////////////////////////////////////////////////
//PRIVATE////////////////////////////////////////////////////////

int IFD::findTAG ( int iTAG )
{
	int a = 0;

	while ( a < nNumDE )
	{
		entry = (DirEntry *) de[a];
		if ( entry->nTAG == iTAG )
			return a;

		a ++;
	}
	return UNKNOWNTAG;
}


/////////////////////////////////////////////////////////////////
//PUBLIC  READ///////////////////////////////////////////////////

long IFD::Read ( FILE *in_stream, long lOffset )
{
	int	iCnt, a	 = 0;
			lByteRead = 0;

	deleteAll ();


/* NUMBER OF ENTRIES
*/
	fseek ( in_stream, lOffset, SEEK_SET );

	iCnt = fread ( &nNumDE, SHORT, 1, in_stream );
	lByteRead += iCnt * SHORT;

	if ( ( nNumDE < 0 ) || ( nNumDE > 30 ) )
		return BAD_NUM_DE;



/* READ ENTRIES
*/
	de = new long* [ nNumDE ];
	
	lOffset += lByteRead;
	lByteRead = 0;

	while ( a < nNumDE )
	{
		entry = new DirEntry ();	
		iCnt  = entry->Read ( in_stream, lOffset );
		de[a] = (long *)entry;

		if ( iCnt < 0 )
			return iCnt;

		lByteRead += iCnt;
		lOffset += DIR_ENTRY_LEN;
		a ++;
	}


/* NEXT IFD?
*/
	fseek ( in_stream, lOffset, SEEK_SET );
	iCnt = fread ( &nNextIFD, LONG, 1, in_stream );
	lByteRead += iCnt * LONG;
	
	return a;
}


/////////////////////////////////////////////////////////////////
//PUBLIC  WRITE//////////////////////////////////////////////////


/* --------------------------------------------------------------
 * Write
 */
long IFD::Write ( FILE *out_stream, long lOffset )
{
	int	iCnt, a	  = 0;
			lByteWrote = 0;



/* WRITE NUM OF ENTRIES
*/
	if ( ! de )
		return NODIRENTRY;
	
	fseek ( out_stream, lOffset, SEEK_SET );

	iCnt = fwrite ( &nNumDE, SHORT, 1, out_stream );
	lByteWrote += iCnt * SHORT;
	
	lOffset   += lByteWrote;
	lByteWrote = 0;



/* WRITE ENTRIES
*/
	bumper = 0;

	while ( a < nNumDE )
	{
		entry = (DirEntry *)de[a];
		long lNumByte = entry->ByteCount() * entry->lCount;


		if ( lNumByte > LONG )						
		{														
			if ( ! entry->lValOff )		
			{
				entry->lValOff = ( nNumDE - a ) * DIR_ENTRY_LEN + LONG + bumper + lOffset;
				bumper += lNumByte;
			}
		}
		
		iCnt = entry->Write ( out_stream, lOffset );
		
		if ( iCnt < 0 )
			return iCnt;

		lByteWrote += iCnt;		
		lOffset += DIR_ENTRY_LEN;

		a ++;
	}


/* NEXT IFD?
*/
	iCnt = fwrite ( &nNextIFD, LONG, 1, out_stream );
	lByteWrote += iCnt * LONG;

	return a;
}