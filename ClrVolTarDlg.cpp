// ClrVolTarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClrVolTar.h"

#include "error.h"
#include "fields.h"
#include "direntry.h"
#include "ifd.h"
#include "image.h"
#include "tiff.h"
#include "simpletiff.h"
#include "target.h"

#include "ClrVolTarDlg.h"
#include ".\clrvoltardlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CClrVolTarDlg dialog



CClrVolTarDlg::CClrVolTarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClrVolTarDlg::IDD, pParent)
	, m_sFilePath(_T(""))
	, m_iClrDelta(5)
	, m_iNumSwtch(9)
	, m_iSwtchWid(50)
	, m_iSwtchLen(50)
	, m_lImgWid(0)
	, m_lImgLen(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClrVolTarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILEPATH, m_sFilePath);
	DDX_Text(pDX, IDC_SWATCH_DELTA, m_iClrDelta);
	DDX_Text(pDX, IDC_SWATCH_COUNT, m_iNumSwtch);
	DDX_Text(pDX, IDC_SWATCH_WID, m_iSwtchWid);
	DDX_Text(pDX, IDC_SWATCH_LEN, m_iSwtchLen);
	DDX_Text(pDX, IDC_IMG_WID, m_lImgWid);
	DDX_Text(pDX, IDC_IMG_LEN, m_lImgLen);
}

BEGIN_MESSAGE_MAP(CClrVolTarDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBnClickedButtonBrowse)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SWATCH_COUNT_SPIN, OnDeltaposSwatchCountSpin)
	ON_EN_CHANGE(IDC_SWATCH_COUNT, OnEnChangeSwatchCount)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SWATCH_DELTA_SPIN, OnDeltaposSwatchDeltaSpin)
	ON_EN_CHANGE(IDC_SWATCH_DELTA, OnEnChangeSwatchDelta)
	ON_EN_CHANGE(IDC_IMG_LEN, OnEnChangeImgLen)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SWATCH_WID_SPIN, OnDeltaposSwatchWidSpin)
	ON_EN_CHANGE(IDC_SWATCH_WID, OnEnChangeSwatchWid)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SWATCH_LEN_SPIN, OnDeltaposSwatchLenSpin)
	ON_EN_CHANGE(IDC_SWATCH_LEN, OnEnChangeSwatchLen)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDABOUT, OnBnClickedAbout)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SEP_MAX_INK_SPIN, OnDeltaposSepMaxInkSpin)
	ON_EN_CHANGE(IDC_SEP_MAX_INK, OnEnChangeSepMaxInk)
	ON_BN_CLICKED(IDC_BROWSE_SEP, OnBnClickedBrowseSep)
END_MESSAGE_MAP()


// CClrVolTarDlg message handlers

BOOL CClrVolTarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	OnInitData();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClrVolTarDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClrVolTarDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClrVolTarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////////////////
//User interface - On click //////////////////////////////////////////////////////////

// ============================================================================
// Description:	Create a dialog for user to choose where the files are going to
//				be written.
//
// Return:		void
void CClrVolTarDlg::OnBnClickedButtonBrowse()
// ============================================================================
{
	static char BASED_CODE szFilter[] = "TIFF Files (*.tif)|*.tif|All Files (*.*)|*.*||";
	CFileDialog	cFDlg( FALSE, "tif", "Target", OFN_HIDEREADONLY, szFilter, NULL  );
	if ( cFDlg.DoModal() == IDOK )
	{
		m_sFilePath = cFDlg.GetPathName();
	}
	UpdateData (FALSE);
}

// ============================================================================
// Description:
//
void CClrVolTarDlg::OnDeltaposSwatchCountSpin(NMHDR *pNMHDR, LRESULT *pResult)
// ============================================================================
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	if ( pNMUpDown->iDelta > 0 )	// down button pressed
	{
		if ( ( m_iNumSwtch % 2 ) && ( m_iNumSwtch > 2 ) )
			m_iNumSwtch -=2;
	}
	else									// up button pressed
	{
		m_iNumSwtch += 2;
	}
	UpdateImageDimension();
	UpdateData (FALSE);
}

void CClrVolTarDlg::OnEnChangeSwatchCount()
{
	if ( ! ( m_iNumSwtch % 2 ) )
		m_iNumSwtch ++;

	UpdateImageDimension();
	UpdateData (FALSE);
}

void CClrVolTarDlg::OnDeltaposSwatchDeltaSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	if ( pNMUpDown->iDelta > 0 )	// down button pressed
	{
		if ( m_iClrDelta > 0 )
			m_iClrDelta --;
	}
	else									// up button pressed
	{
		m_iClrDelta ++;
	}
	UpdateData (FALSE);
}

void CClrVolTarDlg::OnEnChangeSwatchDelta()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CClrVolTarDlg::OnEnChangeImgLen()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

// ============================================================================
// Description:
//
void CClrVolTarDlg::OnDeltaposSwatchWidSpin(NMHDR *pNMHDR, LRESULT *pResult)
// ============================================================================
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	if ( pNMUpDown->iDelta > 0 )	// down button pressed
	{
		if ( m_iSwtchWid > 1 )
			m_iSwtchWid --;
	}
	else									// up button pressed
	{
		m_iSwtchWid ++;
	}
	UpdateImageDimension();
	UpdateData (FALSE);
}

void CClrVolTarDlg::OnEnChangeSwatchWid()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

// ============================================================================
// Description:
//
void CClrVolTarDlg::OnDeltaposSwatchLenSpin(NMHDR *pNMHDR, LRESULT *pResult)
// ============================================================================
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	if ( pNMUpDown->iDelta > 0 )	// down button pressed
	{
		if ( m_iSwtchLen > 1 )
			m_iSwtchLen --;
	}
	else									// up button pressed
	{
		m_iSwtchLen ++;
	}
	UpdateImageDimension();
	UpdateData (FALSE);
}

void CClrVolTarDlg::OnEnChangeSwatchLen()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

// ============================================================================
// Description:	Create all the target im_dMgnes
//
// Return:		void
void CClrVolTarDlg::OnBnClickedOk()
// ============================================================================
{
	SimpleTIFF	*smp;
	Target		*tar;
	BYTE		*bImg;
	char		strPath[256];
	CString		str;

	int pos, scale, iErr;
	int iBpp = 8;
	int iSpp = 4;

	BeginWaitCursor ();

	GetColorInfo ();

	for ( int b = 0; b < m_iNumSwtch; b ++ )
	{
		smp = new SimpleTIFF ();
		tar = new Target ( m_iSwtchWid, m_iSwtchLen, m_iSwtchWid / 3, m_iSwtchLen / 3, m_iClrDelta, m_iColor, m_iKPlane );

		scale = (int)(100.0/(double)(m_iNumSwtch-1)*(double)b);
		m_iRc = tar->Create ( m_iNumSwtch, m_iNumSwtch, scale, scale, scale, scale );

		if ( m_iRc < OKAY )
			m_sRc = "Target Creation failed\n";

		bImg = tar->GetIm_dMgne ( &m_lImgWid, &m_lImgLen, &m_lImgSze );	

		if ( bImg )
		{
			pos = m_sFilePath.Find( '.' );
			sprintf ( strPath, "%d", scale );
			str = m_sFilePath;
			str.Insert (pos, strPath);

			iErr = smp->Write ( str.GetBuffer(0), bImg, m_lImgWid, m_lImgLen, iBpp, iSpp, CMYK_CLR, 100 );

			delete smp;
			delete tar;
			delete bImg;

			bImg = NULL;
		}
	}
	EndWaitCursor ( );
}

// ============================================================================
// Description:	Pop up an "about" dialog box
//
void CClrVolTarDlg::OnBnClickedAbout()
// ============================================================================

{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

// ============================================================================
// Description:
//
void CClrVolTarDlg::OnInitData()
// ============================================================================
{
	CheckRadioButton(IDC_KPLANE_CYAN, IDC_KPLANE_YEL, IDC_KPLANE_YEL );
	CheckRadioButton(IDC_COLOR_RGB, IDC_COLOR_CMYK, IDC_COLOR_CMYK );
	UpdateImageDimension();
	UpdateData (FALSE);
}

// ==================================================================
// Description:
//
void CClrVolTarDlg::OnError()
// ==================================================================
{
	char str[256];
	sprintf (str, "%s %d", m_sRc, m_iRc);
	AfxMessageBox (str, MB_OK, 0 );
}

// ============================================================================
// Description:
//
void CClrVolTarDlg::GetColorInfo()
// ============================================================================
{
	int iChecked;
	iChecked = GetCheckedRadioButton(IDC_KPLANE_CYAN, IDC_KPLANE_YEL); 

	switch ( iChecked )
	{
	case IDC_KPLANE_CYAN:
		m_iKPlane = PLANE_m_dCynN;
		break;

	case IDC_KPLANE_MAG:
		m_iKPlane = PLANE_m_dMgn;
		break;

	case IDC_KPLANE_YEL:
		m_iKPlane = PLANE_YEL;
		break;
	};
	
	iChecked = GetCheckedRadioButton(IDC_COLOR_RGB, IDC_COLOR_CMYK);

	switch ( iChecked )
	{
	case IDC_COLOR_RGB:
		m_iColor = COLOR_RGB;
		break;

	case IDC_COLOR_CMY:
		m_iColor = COLOR_CMY;
		break;

	case IDC_COLOR_CMYK:
		m_iColor = COLOR_CMYK;
		break;
	};
}

// ============================================================================
// Description:
//
void CClrVolTarDlg::UpdateImageDimension()
// ============================================================================
{
	m_lImgWid = ( m_iSwtchWid + m_iSwtchWid/3 ) * m_iNumSwtch;
	m_lImgWid = ( m_iSwtchLen + m_iSwtchLen/3 ) * m_iNumSwtch;
}

void CClrVolTarDlg::OnDeltaposSepMaxInkSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CClrVolTarDlg::OnEnChangeSepMaxInk()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CClrVolTarDlg::OnBnClickedBrowseSep()
{
	// TODO: Add your control notification handler code here
}
