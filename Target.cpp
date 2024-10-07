// ========================================================================
//	 TITLE:			Target.cpp
//
//  DESCRIPTION:	generates a gray balance test target
//						
//	 VERSION:		1.0	( preliminary !!! Needs fine tuning )
//
//	 PLAN:			1) border on the left & top
//						2) write title, column & rows
//
//  DATE:			11October99		1st completion								cty
// ========================================================================

#include <stdafx.h>
#include "error.h"

#include "target.h"


/* ------------------------------------------------------------------------
 *	Target
 *
 *	iCw, iCl		- Cell width, cell length ( color patch dimension )
 * iBw, iBl		- Border width & Border length ( white space dimension )
 * iDt			- Delta, color difference between each patch
 */
Target::Target( int iCw		// [in] swatch width
			   , int iCl	// [in] swatch length
			   , int iBw	// [in] border width (between swatches)
			   , int iBl	// [in] border length (between swatches)
			   , double iDt	// [in] delta (color difference between swatches
			   , int iClr 	// [in] Color space RGB / CMY / CMYK
			   , int iK )	// [in] plane to be constant
{
	m_iClr	  = iClr;
	m_iKPlane = iK;
	m_iCelWid = iCw;
	m_iCelLen = iCl;
	m_iBdrWid = iBw;
	m_iBdrLen = iBl;

	m_iDelta  = iDt;

	m_pClr	= NULL;
	m_pImg	= NULL;
}


Target::~Target()
{
	if ( m_pClr )
		delete m_pClr;
}


///////////////////////////////////////////////////////////////////
//PUBLIC///////////////////////////////////////////////////////////


/* ------------------------------------------------------------------------
 *	GetIm_dMgne
 *
 *	If Create has been called with sucess... Then User may call 
 * GetIm_dMgne() to obtain im_dMgne and it's information.
 */
BYTE * Target::GetIm_dMgne ( long *wid, long *len, long *sze )
{
	if ( m_pImg )
	{
		*wid = m_lImgWid;
		*len = m_lImgLen;
		*sze = m_lImgSze;

		return m_pImg;
	}
	return NULL;
}


/* ------------------------------------------------------------------------
 *	GetColor
 *
 *	iNc, iNr		- Number of columns, number of rows (designate im_dMgne size)
 * iPc, iPr		- coordinates for the color patch we are interested in
 *
 * c,m,y,k		- color values for the center color tile ( to be replaced
 *					  with color value of interested color patch.
 */
int Target::GetColor ( int iNc, int iNr, int iPc, int iPr, 
							  BYTE *m_dCyn, BYTE *m_dMgn, BYTE *m_dYlw, BYTE *m_dBlk )
{
	int	iErr;

	m_iNumCol = iNc;
	m_iNumRow = iNr;

	// will color values work?
	if	(	( ( *m_dCyn + ( ( m_iNumCol / 2 ) * m_iDelta ) ) > 255 )	||
			( ( *m_dMgn + ( ( m_iNumCol / 2 ) * m_iDelta ) ) > 255 )	||
			( ( *m_dYlw + ( ( m_iNumCol / 2 ) * m_iDelta ) ) > 255 )		)

		return COLOR_DELTA_TOO_HIGH;

	if	(	( ( *m_dCyn - ( ( m_iNumCol / 2 ) * m_iDelta ) ) < 0 )	||
			( ( *m_dMgn - ( ( m_iNumCol / 2 ) * m_iDelta ) ) < 0 )	||
			( ( *m_dYlw - ( ( m_iNumCol / 2 ) * m_iDelta ) ) < 0 )		)

		return COLOR_DELTA_TOO_LOW;


	// Built color table
	iErr = BuiltColorTable();

	if ( iErr < 0 )
		return iErr;


	// Find the color value and return
	// ( length of a column ) * # of rows + column depth
	long b = ( m_iNumRow * 4 ) * iPc + ( iPr * 4 );	

	*m_dCyn = m_pClr[b];
	*m_dMgn = m_pClr[b+1];
	*m_dYlw = m_pClr[b+2];
	*m_dBlk = m_pClr[b+3];

	return OKAY;
}


/* ------------------------------------------------------------------------
 *	Create
 *
 *	Create a target tiff im_dMgne
 *
 */
int Target::Create( int iNc, int iNr, double cy, double ma, double ye, double bk )
{
	int	iErr;

	m_dCyn = cy;
	m_dMgn = ma;
	m_dYlw = ye;
	m_dBlk = bk;

	m_iNumCol = iNc;
	m_iNumRow = iNr;


	if ( !( m_iNumCol % 2 ) || !( m_iNumRow % 2 ) )
		return COL_ROW_NOT_ODD;


	// Built color table
	iErr = BuiltColorTable( );

	if ( iErr < 0 )
		return iErr;
	

	// Built Im_dMgne
	iErr = BuiltImage();

	if ( iErr < 0 )
		return iErr;

	return OKAY;
}


/* ------------------------------------------------------------------------
 *	BuiltColorTable
 *
 *	Base on center color value... built colors for im_dMgne
 */
int Target::BuiltColorTable()
{
				/*
				 *
				 * Yellow and Black are constants
				 * However, I have the option of 
				 * not put black on... 
				 *
				 */
	if ( m_iClr < COLOR_CMYK )
		m_dBlk = 0;									

				

	switch ( m_iKPlane )
	{
	case PLANE_m_dCynN:	// m_dCynn is constant

		m_dMgnStt = m_dMgn * 2.55 - ( (double) ( m_iNumRow - 1 ) / 2.0 * m_iDelta * 2.55 );
		m_dYlwStt = m_dYlw * 2.55 - ( (double) ( m_iNumCol - 1 ) / 2.0 * m_iDelta * 2.55 );

		m_dCynStt = m_dCynCnt = m_dCyn * 2.55;
		break;

	case PLANE_m_dMgn:	// m_dMgnenta is constant

		m_dYlwStt = m_dYlw * 2.55 - ( (double) ( m_iNumCol - 1 ) / 2.0 * m_iDelta * 2.55 );
		m_dCynStt = m_dCyn * 2.55 - ( (double) ( m_iNumRow - 1 ) / 2.0 * m_iDelta * 2.55 );

		m_dMgnStt = m_dMgnCnt = m_dMgn * 2.55;
		break;

	case PLANE_YEL:	// yellow is constant
	
		m_dCynStt = m_dCyn * 2.55 - ( (double) ( m_iNumRow - 1 ) / 2.0 * m_iDelta * 2.55 );
		m_dMgnStt = m_dMgn * 2.55 - ( (double) ( m_iNumCol - 1 ) / 2.0 * m_iDelta * 2.55 );
		
		m_dYlwStt = m_dYlwCnt = m_dYlw * 2.55;
		break;
	}

				/*
				 *
				 * I start the loop here for creating
				 * the color swatch ring around
				 *
				 */
	m_lClrSze = m_iNumCol * m_iNumRow * 4;
	m_pClr = new BYTE [ m_lClrSze ];

	int x, y, p = 0;

	m_dCynCnt = m_dCynStt;
	m_dMgnCnt = m_dMgnStt;
	m_dYlwCnt = m_dYlwStt;

	for ( y = 0; y < m_iNumRow; y ++ )
	{
		switch ( m_iKPlane )
		{
		case PLANE_m_dCynN:
			m_dYlwCnt = m_dYlwStt;
			break;

		case PLANE_m_dMgn:
			m_dCynCnt = m_dCynStt;
			break;

		case PLANE_YEL:
			m_dMgnCnt = m_dMgnStt;
			break;
		}

		for ( x = 0; x < m_iNumCol; x ++ )
		{
				/*
				 *
				 * Check for color boundaries here
				 * if we are out of bound... make it 
				 * a blank im_dMgne
				 *
				 */
			if (	( m_dCynCnt >= 0 ) && ( m_dCynCnt <= 255 )	&&
					( m_dMgnCnt >= 0 ) && ( m_dMgnCnt <= 255 )	&&
					( m_dYlwCnt >= 0 ) && ( m_dYlwCnt <= 255 )		)
			{
				m_pClr [p]		= ( BYTE ) m_dCynCnt;
				m_pClr [p+1]	= ( BYTE ) m_dMgnCnt;
				m_pClr [p+2]	= ( BYTE ) m_dYlwCnt;
				m_pClr [p+3]	= ( BYTE ) ( m_dBlk * 2.55 );
			}

			else
			{
				m_pClr [p]		= 0;
				m_pClr [p+1]	= 0;
				m_pClr [p+2]	= 0;
				m_pClr [p+3]	= 0;
			}			
			p  += 4;
			
			switch ( m_iKPlane )
			{
			case PLANE_m_dCynN:
				m_dYlwCnt += m_iDelta * 2.55;
				break;

			case PLANE_m_dMgn:
				m_dCynCnt += m_iDelta * 2.55;
				break;

			case PLANE_YEL:
				m_dMgnCnt += m_iDelta * 2.55;
				break;
			}
		}
		switch ( m_iKPlane )
		{
		case PLANE_m_dCynN:
			m_dMgnCnt += m_iDelta * 2.55;
			break;

		case PLANE_m_dMgn:
			m_dYlwCnt += m_iDelta * 2.55;
			break;

		case PLANE_YEL:
			m_dCynCnt += m_iDelta * 2.55;
			break;
		}
	}
	return OKAY;
}


/* ------------------------------------------------------------------------
 *	BuiltIm_dMgne
 *	
 */
int Target::BuiltImage()
{
	long	 a, x, y, p = 0;
	char	 txtClr;
	double *ptr;

			/*
			 *
			 *		Im_dMgne Dimension
			 *		( column goes vertical, rows are horizontal )
			 *		I need a minimun of 6 column to write the title banner
			 *
			 */

	if ( m_iNumCol > 6 )
		m_lImgWid = ( m_iNumCol + 1 ) * ( m_iCelWid + m_iBdrWid );

	else 
		m_lImgWid = 7 * ( m_iCelWid + m_iBdrWid );

	m_lImgLen = ( m_iNumRow + 2 ) * ( m_iCelLen + m_iBdrLen );
	m_lImgSze = m_lImgWid * m_lImgLen * 4;
	
	m_pImg = new BYTE [ m_lImgSze ];
	memset ( m_pImg, 0, m_lImgSze );
	
			/*
			 *
			 *		Create Im_dMgne
			 *		Once created... the user is responsible 
			 *		for the memory management
			 *
			 */
	long lX, lY;

	for ( y = 0; y < m_iNumRow; y ++ )
	{
		lY = ( y + 1 ) * ( m_iCelLen + m_iBdrLen );

		for ( x = 0; x < m_iNumCol; x ++ )
		{
			lX = x * ( m_iCelWid + m_iBdrWid );
			Paint ( lX, lY, x, y );
		} 
	}
			/*
			 *
			 *		Draw Title Banner
			 *
			 */
	m_szTxtStr = new char [50];

	sprintf ( m_szTxtStr, "Corel Photo ( C=%.0f M=%.0f Y=%.0f K=%.0f )", m_dCyn, m_dMgn, m_dYlw, m_dBlk);

	x = m_iBdrWid;
	y = m_iBdrLen;
	DrawText ( x, y, "Futura" );

	delete m_szTxtStr;

	switch ( m_iKPlane )
	{
	case PLANE_m_dCynN:
		txtClr = 'M';
		ptr = &m_dMgnStt;
		break;

	case PLANE_m_dMgn:
		txtClr = 'Y';
		ptr = &m_dYlwStt;
		break;

	case PLANE_YEL:
		txtClr = 'C';
		ptr = &m_dCynStt;
		break;
	}

			/*
			 *
			 *		Write the percentages on 
			 *		y border
			 *
			 */
	m_szTxtStr = new char [9];
	x = m_iNumCol * ( m_iCelWid + m_iBdrWid );
	
	for ( y = 0; y < m_iNumRow; y ++ )
	{
		lY = ( y + 1 ) * ( m_iCelLen + m_iBdrLen );
		a  = m_iNumCol * 4 * y;

		//	Write out the percentage
		sprintf ( m_szTxtStr, "%.1f %c", *ptr / 2.55, txtClr );
		*ptr += m_iDelta * 2.55;

		DrawText ( x, lY, "Arial" );
	}

			/*
			 *
			 *		Write the percentages on 
			 *		x border
			 *
			 */

	switch ( m_iKPlane )
	{
	case PLANE_m_dCynN:
		txtClr = 'Y';
		ptr = &m_dYlwStt;
		break;

	case PLANE_m_dMgn:
		txtClr = 'C';
		ptr = &m_dCynStt;
		break;

	case PLANE_YEL:
		txtClr = 'M';
		ptr = &m_dMgnStt;
		break;
	}
	
	y = ( m_iNumRow + 1 ) * ( m_iCelLen + m_iBdrLen );

	for ( x = 0; x < m_iNumCol; x ++ )
	{
		lX = x * ( m_iCelWid + m_iBdrWid );
		a  = x * 4 + 1;

		sprintf ( m_szTxtStr, "%.1f %c", *ptr / 2.55, txtClr );
		*ptr += m_iDelta * 2.55;

		DrawText ( lX, y, "Arial" );
	}
	delete m_szTxtStr;

	return OKAY;
}


/* ------------------------------------------------------------------------
 *	Paint
 *	
 *	Xstart, Ystart		- coordinates of where tha patch begins.
 */
void Target::Paint( long Xstart, long Ystart, int Xclr, int Yclr )
{
	long	Xend, Yend;
	long	j, k, o, p = 0;
	

			/*
			 *
			 *		Select Color from Table
			 *
			 */

	int	a = 0;
	a = ( Yclr * m_iNumCol * 4 ) + ( Xclr * 4 );


			/*
			 *
			 *		Paint the patch on im_dMgne buffer
			 *
			 */

	Xend  = Xstart + m_iCelWid;
	Yend	= Ystart + m_iCelLen;


	for ( k = Ystart; k < Yend; k ++ )
	{
		o = k * m_lImgWid * 4;

		for ( j = Xstart; j < Xend; j ++ )
		{
			p = o + ( j * 4 );

			m_pImg[p]	 = m_pClr[a];
			m_pImg[p+1] = m_pClr[a+1];
			m_pImg[p+2] = m_pClr[a+2];
			m_pImg[p+3] = m_pClr[a+3];
		}
	}
}


/* ------------------------------------------------------------------------
 *	DrawText
 *	
 *	Draw text info on to target im_dMgne.  Specifically, I want to draw 
 * percentage for row and column.... as well as a title
 */
int Target::DrawText ( long Xloca, long Yloca, CString font )
{
	int iErr			= OKAY;
	int iPxlDpth	= 1;								// ddb color depth
	int iSze;

		/*
		 *
		 *	Create font and write to 
		 * test dimension
		 *
		 */
							

	HGLOBAL	ghPxl	= 0;								// splace for im_dMgne pixels
	HGLOBAL	ghBMI	= 0;								// space for im_dMgne header

	if (m_dcMem.CreateCompatibleDC(NULL))			// display device context
	{
		m_dcMem.SaveDC();

		CBitmap		bmNew, *pbmOld;

		m_dcMem.SetMapMode(MM_TEXT);
		
			/*
			 *
			 * I want to make sure the text is outing
			 * to fit within 1 target swatch.. use the
			 * smaller dimension
			 *
			 */
		if ( m_iCelLen < m_iCelWid )
			iSze = m_iCelLen / 3;
		else
			iSze = m_iCelWid / 3;

		m_hFont = CreateFont( iSze, 0, 0, 0, 400, FALSE, 
									FALSE, 0, 0, 0, 0, 0, 0, font);

		m_dcMem.SelectObject(m_hFont);
		m_txtSze = m_dcMem.GetTextExtent(m_szTxtStr, lstrlen(m_szTxtStr));

		if (bmNew.CreateBitmap((int)m_txtSze.cx, (int)m_txtSze.cy, 1, iPxlDpth, NULL))
		{
			pbmOld = m_dcMem.SelectObject(&bmNew);
			if (WriteTxtDDB(&m_dcMem) == 0)				// bit blits text to DC
			{
				m_dcMem.SelectObject(pbmOld);
				m_dcMem.SetMapMode(MM_TEXT);

				// blend text onto background
				if (iCreateStructures(&m_dcMem, &bmNew, &ghBMI, &ghPxl) == 0)
					walkNwack(&ghPxl, Xloca, Yloca);			
				
				else
					iErr = BAD_CREATE_STRUCT;			// err in CreatStructure()
			}
			else
				iErr = BAD_TEXT_OUT;						// err in WriteTxtDDB()
		}
		else
			iErr = BAD_CREATE_BMP;						// err in CreateBitmap
		
		if (ghBMI)
			GlobalFree(ghBMI);

		if (ghPxl)
			GlobalFree(ghPxl);

		m_dcMem.RestoreDC(-1);
		m_dcMem.DeleteDC();
	
		return iErr;
	}
	return BAD_CREATE_DC;								// err in CreateCompatibleDC
}


// --------------------------------------------------------
//	MODULE:			iCreateStructures()
//
//	DESCRIPTION:	generate header, and space for text dib	
// --------------------------------------------------------
int Target::iCreateStructures(CDC *pDC, CBitmap *pbm, HGLOBAL *pghBMI, HGLOBAL *pghPxl)
{
	void FAR *		lpPxl	= 0;
	LPBITMAPINFO	lpbmi	= 0;
	BITMAP			bm;
	int ierr			= -1;
	int				iRc;
	
	pbm->GetObject(sizeof(BITMAP), &bm);
	const	int	iPaletteEntries = (bm.bmBitsPixel == 24) ? 0 : 1 << bm.bmBitsPixel;

	*pghBMI = GlobalAlloc(GHND, (DWORD)(sizeof(BITMAPINFO)
						  + iPaletteEntries * sizeof (RGBQUAD)));

	if (*pghBMI)
	{
		lpbmi = (BITMAPINFO FAR *) GlobalLock(*pghBMI);
		if (lpbmi)
		{
			dwSetBmpHeader(lpbmi, bm.bmWidth, bm.bmHeight, bm.bmBitsPixel);
			iRc = ::GetDIBits(pDC->m_hDC, (HBITMAP)pbm->m_hObject, 0, bm.bmHeight, 0, lpbmi, DIB_RGB_COLORS);

			if (iRc = bm.bmHeight)
			{
				*pghPxl = GlobalAlloc (GHND, lpbmi->bmiHeader.biSizeImage);

				if (*pghPxl)
				{
					lpPxl = (void FAR *)GlobalLock(*pghPxl);

					if (lpPxl)
					{
						iRc = ::GetDIBits(pDC->m_hDC, (HBITMAP)pbm->m_hObject, 0, bm.bmHeight, lpPxl, lpbmi, DIB_RGB_COLORS);
						GlobalUnlock(*pghBMI);
						GlobalUnlock(*pghPxl);

						if (iRc)
							ierr = 0;
					}
				}
			}
		}
	}
	return ierr;
}

// --------------------------------------------------------
//	MODULE:			dwSetBmpHeader()
//
//	DESCRIPTION:	fill text dib header
// --------------------------------------------------------
DWORD Target::dwSetBmpHeader(LPBITMAPINFO lpbmi, DWORD dwWid, DWORD dwLen, int iBpp)
{
	m_dwBytesPerRow = (int)(((dwWid *iBpp + 31) / 32) * 4);

	lpbmi->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	lpbmi->bmiHeader.biWidth			= dwWid;
	lpbmi->bmiHeader.biHeight			= dwLen;
	lpbmi->bmiHeader.biPlanes			= 1;
	lpbmi->bmiHeader.biBitCount			= (WORD)iBpp;
	lpbmi->bmiHeader.biCompression		= BI_RGB;
	lpbmi->bmiHeader.biSizeImage		= m_dwBytesPerRow * dwLen;
	lpbmi->bmiHeader.biXPelsPerMeter	= 0;
	lpbmi->bmiHeader.biYPelsPerMeter	= 0;
	lpbmi->bmiHeader.biClrUsed			= (iBpp == 24) ? 0 : 1 << iBpp;
	lpbmi->bmiHeader.biClrImportant		= 0;

	return (m_dwBytesPerRow);
}

// --------------------------------------------------------
//	MODULE:			WriteTxtDDB()
//
//	DESCRIPTION:	bit blits text to DC	
// --------------------------------------------------------
int Target::WriteTxtDDB(CDC *pDC)
{
	m_dcMem.SelectObject ( ::GetStockObject ( WHITE_BRUSH ) );
	m_dcMem.SelectObject ( ::GetStockObject ( WHITE_PEN ) );

	m_dcMem.Rectangle( 0, 0, ( int ) m_txtSze.cx, ( int ) m_txtSze.cy );
	m_dcMem.SetBkMode ( TRANSPARENT );

	if ( ::TextOut(pDC->m_hDC, 0, 0, m_szTxtStr, lstrlen(m_szTxtStr)) != 0 )
		return 0;
	else 
		return -2;
}

// --------------------------------------------------------
//	MODULE:			walkNwack()
//
//	DESCRIPTION:	merge text dib with background dib	
// --------------------------------------------------------
void Target::walkNwack( HGLOBAL *pghPxl, long Xstart, long Ystart )
{
	int  cnt;
	void FAR*	lpPxl	= 0;
	BYTE bin;
	BYTE *tptr;									// text dib pointer

	long j;

	lpPxl = ( void FAR * ) GlobalLock ( *pghPxl );
		
	if ( lpPxl )
	{
		tptr = ( unsigned char* )lpPxl;
		bin = 0x80;								// initial 1000 0000
		
		for ( int y = Ystart + m_txtSze.cy - 1; y >= Ystart; y -- )
		{
			j = ( y * m_lImgWid * 4 ) + ( Xstart * 4 );
			cnt = 0;

				/*
				 *
				 *	Dance is lead by the ddb, not tiff im_dMgne
				 *
				 */
			for ( int x = 0; x < (int) m_dwBytesPerRow; x ++ )
			{
				for ( int z = 0; z < 8; z ++ )
				{
					if ( cnt < m_txtSze.cx )
					{
						if ( ( bin & *tptr ) == 0 )// pixel is on? (masking here)
						{
							m_pImg[j]	 = 255;
							m_pImg[j+1] = 255;
							m_pImg[j+2] = 255;
							m_pImg[j+3] = 255;
						}
						j += 4;
						cnt ++;
					}

					if (bin > 0x01)				// > 0000 0001, 
						bin >>= 1;					// shift mask
					else						
					{
						bin = 0x80;					// reset to 1000 0000
						tptr ++;						// next 8 pixels
					}
				}
			}
		}	
	}
	GlobalUnlock(*pghPxl);
}