// ============================================================================
// Module:		Target.h/cpp
//
// Description:	Create color volume targets in raw format
//
// Author:		C.T. Yeung					cty
//
// Purpose:		1st implementation for Ilford im_dMgning of southern california
//				2nd implementation for Corel photo
//
// History:	
//	????		Creation										cty
// 24Sept06		Enhance for RGB im_dMgne generation				cty
// ============================================================================

// ============================================================================
// Constant
// ============================================================================
enum { COLOR_RGB=-1, COLOR_CMY, COLOR_CMYK };
enum { PLANE_m_dCynN, PLANE_m_dMgn, PLANE_YEL };

// ============================================================================
//	class definition
// ============================================================================
class Target
{

public:

	Target( int, int, int, int, double, int, int );
	~Target();


public:

	int		Create	( int, int, double, double, double, double );
	BYTE*	GetIm_dMgne ( long*, long*, long* );
	int		GetColor ( int, int, int, int, BYTE*, BYTE*, BYTE*, BYTE* );

private:

	int		BuiltColorTable ();
	int		BuiltImage	();
	int		DrawText( long, long, CString );
	void	Paint( long, long, int, int );

	int		iCreateStructures(CDC *, CBitmap *, HGLOBAL *, HGLOBAL *);			// create ddb im_dMgne
	DWORD	dwSetBmpHeader(LPBITMAPINFO, DWORD, DWORD, int);					// creates header for ddb
	int		WriteTxtDDB(CDC *);
	void	walkNwack(HGLOBAL *, long, long);									// merge the ddb and dib

public:

	int	m_iCelWid, m_iCelLen;													// swatch dimensions
	int	m_iBdrWid, m_iBdrLen;													// border dimensions

	int	m_iClr;		// Color space
	int m_iKPlane;

	double m_iDelta;															// color difference between swatch
	int	m_iNumCol, m_iNumRow;													// Number of Column & Row

	BYTE	*m_pClr;															// pointer to color array
	long	m_lClrSze;															// size of the color array

	BYTE	*m_pImg;																// im_dMgne pointer
	long	m_lImgWid, m_lImgLen, m_lImgSze;									// im_dMgne dimension

private:

	double	m_dCyn, m_dMgn, m_dYlw, m_dBlk;
	double	m_dCynStt, m_dCynCnt;
	double	m_dMgnStt, m_dMgnCnt;
	double	m_dYlwStt, m_dYlwCnt;


	LPBYTE	m_ImgPtr;					// pointer to background im_dMgne 
	CSize	m_txtSze;					// text raster block size

	char	*m_szTxtStr;
	CDC		m_dcMem;
	HFONT	m_hFont;
	DWORD	m_dwBytesPerRow;				// number of bytes per row		
};