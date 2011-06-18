#if !defined(AFX_WorldSize_H__92E49E62_00A0_11D3_8FD9_DF2C58812444__INCLUDED_)
#define AFX_WorldSize_H__92E49E62_00A0_11D3_8FD9_DF2C58812444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorldSize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorldSize dialog
#include "settings.h"

class CWorldSize : public CPropertyPage
{
	DECLARE_DYNCREATE(CWorldSize)

// Construction
public:
	CWorldSize();
	~CWorldSize();

// Dialog Data
	//{{AFX_DATA(CWorldSize)
	enum { IDD = IDD_WORLD };
	CEdit	m_widthCtrl;
	CEdit	m_heightCtrl;
	CStatic	m_widthText;
	CStatic	m_heightText;
	int		m_nWidth;
	int		m_nHeight;
	int		m_nSizeChoice;
	CButton	m_sizeCtrl;
	//}}AFX_DATA

	void EnableControls(BOOL bEnable);
	void Exchange(CSettings& s, bool bSave);

// Overrides
	//{{AFX_VIRTUAL(CWorldSize)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWorldSize)
	afx_msg void OnEntireScreen();
	afx_msg void OnEntireWindow();
	afx_msg void OnCustom();
	afx_msg void OnFitWindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WorldSize_H__92E49E62_00A0_11D3_8FD9_DF2C58812444__INCLUDED_)
