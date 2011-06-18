// HelpPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "Primordial Life.h"

#include "MainFrm.h"
#include "HelpPropertySheet.h"

// The magical help button ID (found through Spy)
#define IDB_HELP    9


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Derive a class CMyPropertySheet from CPropertySheet.
// Include the file afxpriv.h to obtain the id for
// WM_COMMANDHELP in the implementation file for CMyPropertySheet



////////////////////////////////////////////////////////////////////////////
// CHelpPropertySheet

IMPLEMENT_DYNAMIC(CHelpPropertySheet, CPropertySheet)

CHelpPropertySheet::CHelpPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
		m_sCommandContext = _T("PrimLife.htm");
}

CHelpPropertySheet::CHelpPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
		m_sCommandContext = _T("PrimLife.htm");;
}

CHelpPropertySheet::~CHelpPropertySheet()
{
}

BEGIN_MESSAGE_MAP(CHelpPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CHelpPropertySheet)

		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CHelpPropertySheet::SetHelpButtonContextId(CString sHelp)
{
	m_sCommandContext = sHelp;
}

BOOL CHelpPropertySheet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// If the user clicks the help button we will find our help file and call htmlhelp directly here
	if(wParam == IDHELP)
	{
		AfxMainFrame().DisplayBrowser(m_sCommandContext);
		return TRUE;
	}
	else
		return CPropertySheet::OnCommand(wParam, lParam);
}


/////////////////////////////////////////////////////////////////////////////
// CHelpPropertySheet message handlers
