
// ============================================================================
//	     (c) 2006 Corel Inc. All rights reserved.
// ============================================================================
//
// Module:			ClrVolTarDlg.h/cpp
//
// Description:		Dialog class for user interface
//
// Purpose:			Project designed to generate Color volume target im_dMgnes
//					for testing and im_dMgne analysis.
//
// Author:			C.T. Yeung		(cty)
//
// History:
// 20Sept06			started porting code from Ilford freebie project	cty
// ============================================================================

#pragma once


// ============================================================================
//	Class definition
// ============================================================================
// CClrVolTarDlg dialog
class CClrVolTarDlg : public CDialog
{
// Construction
public:
	CClrVolTarDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLRVOLTAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse();
	CString m_sFilePath;
	afx_msg void OnDeltaposSwatchCountSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeSwatchCount();
	afx_msg void OnDeltaposSwatchDeltaSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeSwatchDelta();
	afx_msg void OnEnChangeImgLen();
	afx_msg void OnDeltaposSwatchWidSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeSwatchWid();
	afx_msg void OnDeltaposSwatchLenSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeSwatchLen();
public:
	int m_iColor;
	int m_iKPlane;

	long m_lImgSze;
	int m_iRc;
	CString m_sRc;

	int m_iClrDelta;
	int m_iNumSwtch;
	int m_iSwtchWid;
	int m_iSwtchLen;
	long m_lImgWid;
	long m_lImgLen;

public:
	void OnInitData();
	void OnError();
	void GetColorInfo();
	void UpdateImageDimension();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnDeltaposSepMaxInkSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeSepMaxInk();
	afx_msg void OnBnClickedBrowseSep();
};
