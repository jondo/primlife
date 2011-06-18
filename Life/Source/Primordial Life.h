// Primordial Life.h : main header file for the PRIMORDIAL LIFE application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "PrimCmdLine.h"
#include "KeyRegistry.h"

class Biot;

/////////////////////////////////////////////////////////////////////////////
// CPLifeApp:
// See Primordial Life.cpp for the implementation of this class
//
//
class CPLifeApp : public CWinApp
{
public:
	CPLifeApp();
	void ShowSettingsDialog();	
	int  GetView() { return m_cmd.m_viewOption; }
	bool IsSaver() { return (m_cmd.m_viewOption == CPrimCmdLine::SHOW_SAVER_WINDOW); }
	bool IsSmall() { return (m_cmd.m_viewOption == CPrimCmdLine::SHOW_SMALL_WINDOW); }
	bool IsNormal() { return (m_cmd.m_viewOption == CPrimCmdLine::SHOW_NORMAL_WINDOW); }

// Variables
public:
	CString      m_sClassName;
	CString      m_sMainName;
	CPrimCmdLine m_cmd;
	BOOL         m_bFirst;

	CKeyRegistry m_comRegistry;
	CKeyRegistry m_userRegistry;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLifeApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int ExitInstance();
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPLifeApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

bool AfxIsNT(void);
bool AfxVerifyPassword(HWND hWnd);
void AfxChangePassword(HWND hWnd);

void AfxStartHelp(HWND hWnd);

inline CPLifeApp& AfxGetPLife()
{
	return (CPLifeApp&)	*AfxGetApp();
}

inline CKeyRegistry& AfxKeyRegistry()
{
	return AfxGetPLife().m_userRegistry;
}

inline CKeyRegistry& AfxUserRegistry()
{
	return AfxGetPLife().m_userRegistry;
}

inline CKeyRegistry& AfxCommonRegistry()
{
	return AfxGetPLife().m_comRegistry;
}
/////////////////////////////////////////////////////////////////////////////
