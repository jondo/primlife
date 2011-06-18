#if !defined(AFX_NETWORKDLG_H__69CC9701_4739_11D3_8FD9_0000E8CD5AAB__INCLUDED_)
#define AFX_NETWORKDLG_H__69CC9701_4739_11D3_8FD9_0000E8CD5AAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNetworkDlg dialog
#include "Settings.h"

class CNetworkDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CNetworkDlg)

// Construction
public:
	CNetworkDlg();
	~CNetworkDlg();

// Dialog Data
	//{{AFX_DATA(CNetworkDlg)
	enum { IDD = IDD_NETWORK };
	CButton	m_cSideEnable[CSettings::SIDES];
	CEdit	m_cSideAddress[CSettings::SIDES];
	CString	m_sSideAddress[CSettings::SIDES];
	BOOL	m_bSideEnable[CSettings::SIDES];
	//}}AFX_DATA

	void Exchange(CSettings& s, bool bSave);

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CNetworkDlg)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL



// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNetworkDlg)
	afx_msg void OnRightEnable();
	afx_msg void OnLeftEnable();
	afx_msg void OnTopEnable();
	afx_msg void OnBottomEnable();
	afx_msg void OnViewServers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKDLG_H__69CC9701_4739_11D3_8FD9_0000E8CD5AAB__INCLUDED_)
