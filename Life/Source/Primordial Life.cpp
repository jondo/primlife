// Primordial Life.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Primordial Life.h"

#include "MainFrm.h"
#include "PLifeDoc.h"
#include "PLifeView.h"
#include "PrimCmdLine.h"
#include "SaverView.h"
#include "SmallView.h"

#include "evolve.h"
#include "environ.h"
#include "KeyRegistry.h"
#include "AboutWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class Biot;

/////////////////////////////////////////////////////////////////////////////
// CPLifeApp
//
BEGIN_MESSAGE_MAP(CPLifeApp, CWinApp)
	//{{AFX_MSG_MAP(CPLifeApp)
//	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLifeApp construction
//
CPLifeApp::CPLifeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPLifeApp object

CPLifeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPLifeApp initialization

BOOL CPLifeApp::InitInstance()
{
	AfxEnableControlContainer();

	m_sMainName = AfxRegisterWndClass( 
								CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW,
 								0, (HBRUSH) ::GetStockObject(BLACK_BRUSH),
								LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME))); 

	if (m_sMainName.IsEmpty())
	{
		AfxMessageBox("Unable to register window class.");
		return FALSE;
	}

	m_sClassName = AfxRegisterWndClass( 
								CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW,
								0, (HBRUSH) ::GetStockObject(BLACK_BRUSH),
								LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME))); 

	if (m_sClassName.IsEmpty())
	{
		AfxMessageBox("Unable to register window class.");
		return FALSE;
	}

	// Establish Sounds Events and default sound locations
	CString sPath;
	if (!GetWindowsDirectory(sPath.GetBuffer(MAX_PATH), MAX_PATH))
		return FALSE;

	sPath.ReleaseBuffer();

	if (sPath[sPath.GetLength() - 1] != '\\')
		sPath += '\\';

	sPath += "Media\\Primordial Life\\";
	
	CSoundRegistry sound("PL", "Primordial Life");

	sound.SoundEvent("PL.TooOld",       "Too Old",   sPath);
	sound.SoundEvent("PL.NoEnergy",     "No Energy", sPath);
	sound.SoundEvent("PL.Eaten",        "Eaten",     sPath);
	sound.SoundEvent("PL.Birth",        "Birth",     sPath);
	sound.SoundEvent("PL.Mate",         "Mate",      sPath);
	sound.SoundEvent("PL.Start",        "Start Simulation", sPath);
	sound.SoundEvent("PL.Extinction",   "Extinction", sPath);
	sound.SoundEvent("PL.Terminate",    "Terminate", sPath);
	sound.SoundEvent("PL.Feed",         "Feed",      sPath);
	sound.SoundEvent("PL.Edit",         "Edit",      sPath);
	sound.SoundEvent("PL.Status",       "Status",    sPath);
	

		// If we can't open, assume new installation and copy registration key
	if (!m_userRegistry.OpenKey("Software\\Ultravast\\Primordial Life\\4", HKEY_CURRENT_USER))
	{
		CKeyRegistry registry;

		if (!m_userRegistry.CreateKey("Software\\Ultravast\\Primordial Life\\4", HKEY_CURRENT_USER))
			return FALSE;

		if (registry.OpenKey("Software\\Primordial Life\\Window\\3.1", HKEY_CURRENT_USER))
		{
			m_userRegistry.SetName(registry.GetName());
		}
	}
		

	// Start up our registries
	if (!m_comRegistry.CreateKey("Software\\Ultravast\\Primordial Life\\4", HKEY_LOCAL_MACHINE))
		return FALSE;

	SetRegistryKey(_T("Ultravast"));

	// Some internals to adjust the INI key to our liking
	BOOL bEnable = AfxEnableMemoryTracking(FALSE);
	free((void*)m_pszProfileName);
	m_pszProfileName = _tcsdup(_T("Primordial Life\\4"));
	AfxEnableMemoryTracking(bEnable);

	if (m_comRegistry.GetString(_T("path"), sPath))
	{
		sPath += _T("\\Simulations");
		::SetCurrentDirectory(sPath);
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

 	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Parse command line for standard shell commands, DDE, file open
	ParseCommandLine(m_cmd);
	m_bFirst = FALSE;

	sPath.ReleaseBuffer(::GetModuleFileName(m_hInstance, sPath.GetBuffer(MAX_PATH), MAX_PATH));
	if (sPath.GetLength() > 4 &&
		sPath.Right(4).CompareNoCase(_T(".exe")) == 0)
		m_cmd.m_viewOption = CPrimCmdLine::SHOW_NORMAL_WINDOW;

	// We should have a view decided at this point
	if (m_cmd.m_viewOption == CPrimCmdLine::SHOW_ERROR)
		return FALSE;

	CSingleDocTemplate* pDocTemplate;
	switch(m_cmd.m_viewOption)
	{
		case CPrimCmdLine::SHOW_CHANGE_PASSWORD_DIALOG:
		{
			AfxChangePassword(m_cmd.m_hWnd);
			return FALSE;
		}
		break;

		case CPrimCmdLine::SHOW_NORMAL_WINDOW:
			// Register the application's document templates.  Document templates
			//  serve as the connection between documents, frame windows and views.
			pDocTemplate = new CSingleDocTemplate(
				IDR_MAINFRAME,
				RUNTIME_CLASS(CPLifeDoc),
				RUNTIME_CLASS(CMainFrame),       // main SDI frame window
				RUNTIME_CLASS(CPLifeView));
			AddDocTemplate(pDocTemplate);

			// Enable DDE Execute open
			EnableShellOpen();
			RegisterShellFileTypes(TRUE);
			break;

		case CPrimCmdLine::SHOW_SETTINGS_DIALOG:
		case CPrimCmdLine::SHOW_SAVER_WINDOW:
			// Register the application's document templates.  Document templates
			//  serve as the connection between documents, frame windows and views.
			pDocTemplate = new CSingleDocTemplate(
				IDR_MAINFRAME,
				RUNTIME_CLASS(CPLifeDoc),
				RUNTIME_CLASS(CMainFrame),       // main SDI frame window
				RUNTIME_CLASS(CSaverView));
			AddDocTemplate(pDocTemplate);
			m_cmd.m_strFileName.Empty();
			break;

		case CPrimCmdLine::SHOW_SMALL_WINDOW:
			pDocTemplate = new CSingleDocTemplate(
				IDR_MAINFRAME,
				RUNTIME_CLASS(CPLifeDoc),
				RUNTIME_CLASS(CMainFrame),       // main SDI frame window
				RUNTIME_CLASS(CSmallView));
			AddDocTemplate(pDocTemplate);
			m_cmd.m_strFileName.Empty();
			break;

		default:
			return FALSE;
	}


	if (m_cmd.m_viewOption == CPrimCmdLine::SHOW_SETTINGS_DIALOG)
	{
		ShowSettingsDialog();
		return FALSE;
	}

	m_bFirst = m_cmd.m_strFileName.IsEmpty();


	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(m_cmd))
		return FALSE;

	// Enable drag/drop open
	if (IsNormal())
 		m_pMainWnd->DragAcceptFiles();

	if (IsSaver())
		::SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, 0, 0);

	m_pMainWnd->PostMessage(WM_COMMAND, ID_INITIAL_UPDATE, 0);

	return TRUE;
}


int CPLifeApp::ExitInstance() 
{
	if (IsSaver())
		::SystemParametersInfo(SPI_SCREENSAVERRUNNING, FALSE, 0, 0);
	
	return CWinApp::ExitInstance();
}


void CPLifeApp::ShowSettingsDialog()
{
	Environment temp;

	temp.GetDefaultSettings();

	CreateDialogParam(AfxGetApp()->m_hInstance, 
		MAKEINTRESOURCE(DLG_SCRNSAVECONFIGURE),
		NULL, (DLGPROC) ConfigureDialog, 
		(LPARAM)&temp); 

	temp.SetDefaultSettings();
}


// App command to run the dialog
//void CPLifeApp::OnAppAbout()
//{
//	CAboutDlg aboutDlg;
//	aboutDlg.DoModal();
//}

/////////////////////////////////////////////////////////////////////////////
// CPLifeApp commands
/////////////////////////////////////////////////////////////
// Global Afx Functions
//
// Determines if we are running NT
//
bool AfxIsNT(void)
{
	OSVERSIONINFO version;
	ZeroMemory(&version, sizeof(OSVERSIONINFO));
	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&version);
	return (version.dwPlatformId == VER_PLATFORM_WIN32_NT);
}


/////////////////////////////////////////////////////////////
// AfxVerifyPassword
//
// Pops up a dialog if needed to verify the windows password
//
bool AfxVerifyPassword(HWND hWnd)
{
	bool bPassed = true;
	if (AfxIsNT())
		return bPassed;

	HINSTANCE hpwdcpl = ::LoadLibrary(_T("PASSWORD.CPL"));
	if (hpwdcpl)
	{
		typedef BOOL (WINAPI *VERIFYSCREENSAVEPWD) (HWND hWnd);

		VERIFYSCREENSAVEPWD VerifyScreenSavePwd =
			(VERIFYSCREENSAVEPWD) GetProcAddress(hpwdcpl, _T("VerifyScreenSavePwd"));

		if (VerifyScreenSavePwd != NULL)
			bPassed = (VerifyScreenSavePwd(hWnd) != FALSE);
		
		::FreeLibrary(hpwdcpl);
	}
	return bPassed;
}


/////////////////////////////////////////////////////////////
// AfxChangePassword
//
// Pops up a dialog if needed to change the windows password
//
void AfxChangePassword(HWND hWnd)
{
	if (AfxIsNT())
		return;

	HINSTANCE hmpr = ::LoadLibrary(_T("MPR.DLL"));
	if (hmpr)
	{
		typedef VOID (WINAPI *PWDCHANGEPASSWORD)
			(LPCSTR lpcRegkeyname, HWND hWnd, UINT uiReserved1, UINT uiReserved2);

		PWDCHANGEPASSWORD PwdChangePassword =
			(PWDCHANGEPASSWORD) GetProcAddress(hmpr, _T("PwdChangePasswordA"));

		if (PwdChangePassword != NULL)
			PwdChangePassword("SCRSAVE", hWnd, 0, 0);
		
		::FreeLibrary(hmpr);
	}
}


//////////////////////////////////////////////////////
// AfxStartHelp
//
//
void AfxStartHelp(HWND hWnd)
{
	char szHelp[MAX_PATH];

	szHelp[0] = '\0';
	AfxCommonRegistry().GetString("directory", szHelp, MAX_PATH);
	lstrcat(szHelp, "\\prim40.hlp");

	if (AfxGetPLife().GetView() == CPrimCmdLine::SHOW_SAVER_WINDOW)
		PostQuitMessage(0);

	WinHelp(hWnd, szHelp, HELP_CONTENTS, 0);
}


///////////////////////////////////////////////////////////
// AfxCommandLine
//
// Tells us if we are a screen saver, or small window or
// normal window
//
CPrimCmdLine& AfxCommandLine()
{
	return AfxGetPLife().m_cmd;
}


///////////////////////////////////////////////////////////
// AfxIsFirstNew
//
// Is the document the first document?
//
BOOL AfxIsFirstNew()
{
	if (AfxGetPLife().m_bFirst)
	{
		AfxGetPLife().m_bFirst = FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOL CPLifeApp::OnIdle(LONG lCount) 
{
   // In this example, as in most applications, you should let the
   // base class CWinApp::OnIdle complete its processing before you
   // attempt any additional idle loop processing.
   if (CWinApp::OnIdle(lCount))
	   return TRUE;   

   // The base class CWinApp::OnIdle reserves the lCount values 0 
   // and 1 for the framework's own idle processing.   If you wish to
   // share idle processing time at a peer level with the framework,
   // then replace the above if-statement with a straight call to
   // CWinApp::OnIdle; and then add a case statement for lCount value
   // 0 and/or 1. Study the base class implementation first to 
   // understand how your idle loop tasks will compete with the 
   // framework's idle loop processing.

    AfxMainFrame().GetGeneralView()->OnIdle();

	return TRUE;	// TODO: Add your specialized code here and/or call the base class
}


void CPLifeApp::WinHelp(DWORD dwData, UINT nCmd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWinApp::WinHelp(dwData, nCmd);
}
