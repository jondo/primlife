// PrimCmdLine.h
//

#ifndef PrimCmdLine_h
#define PrimCmdLine_h


//CRC////////////////////////////////////////////////////////////////////////
//
// CPrimCmdLine
//
// Collects command line parameters during initialization and controls
// how they are interpreted by the MFC framework
//
// Responsibilities:
//	- Determine what options have been set
//
// Collaborators:
//		None
//
// Usage Notes:
//
class CPrimCmdLine : public CCommandLineInfo
{
public:
	CPrimCmdLine();
	~CPrimCmdLine();

	virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast);

public:
	enum {
		SHOW_CHANGE_PASSWORD_DIALOG,
		SHOW_SETTINGS_DIALOG,
		SHOW_SMALL_WINDOW,
		SHOW_SAVER_WINDOW,
		SHOW_NORMAL_WINDOW,
		SHOW_ERROR
	};

	int  m_viewOption;
	HWND m_hWnd;
};

CPrimCmdLine& AfxCommandLine();

#endif