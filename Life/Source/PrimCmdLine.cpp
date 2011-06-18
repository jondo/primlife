// PrimCmdLine.cpp
//
//

#include "stdafx.h"
#include "PrimCmdLine.h"


// Constructor
CPrimCmdLine::CPrimCmdLine()
{
	m_viewOption = SHOW_SETTINGS_DIALOG;
	m_hWnd  = NULL;
}

// Destructor
CPrimCmdLine::~CPrimCmdLine()
{
}

// The work horse - interpret parameters as they are passed to use
//
// If we don't recognize the paremeters here, invoke the base class function
void CPrimCmdLine::ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast)
{
	// if pszParam is a '/' or '-' introduced flag
	if (bFlag)
	{
		if (lstrcmpiA(pszParam, "c") == 0)	
		{
			m_viewOption = SHOW_SETTINGS_DIALOG;
		}
		else if (lstrcmpiA(pszParam, "p") == 0 || lstrcmpiA(pszParam, "l") == 0)	
		{
			m_viewOption = SHOW_SMALL_WINDOW;
		}
		else if (lstrcmpiA(pszParam, "s") == 0)
		{
			m_viewOption = SHOW_SAVER_WINDOW;
		}
		else if (lstrcmpiA(pszParam, "w") == 0)
		{
			m_viewOption = SHOW_NORMAL_WINDOW;
		}
		else if (lstrcmpiA(pszParam, "a") == 0)
		{
			m_viewOption = SHOW_CHANGE_PASSWORD_DIALOG;
		}
		else
			CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
	}
	else
	{
		if (m_hWnd == NULL && IsWindow((HWND) atol(pszParam)) != 0)
		{
			m_hWnd = (HWND) atol(pszParam);
		}
		else
		{
			CCommandLineInfo::ParseParam(pszParam, bFlag, bLast);
		}
	}

	if (bLast)
	{
		// If we were given a file name, lets open it in a regular window
		if (!m_strFileName.IsEmpty())
			m_viewOption = SHOW_NORMAL_WINDOW;
	}
}
