#pragma once

// HelpPropertySheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHelpPropertySheet

class CHelpPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CHelpPropertySheet)

// Construction
public:
	CHelpPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CHelpPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CString m_sCommandContext;	// The current help context id (set from each page on their respective OnSetActive()

	void	SetHelpButtonContextId(CString sHelp);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpPropertySheet)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHelpPropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHelpPropertySheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

