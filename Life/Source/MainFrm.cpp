// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Primordial Life.h"

#include "MainFrm.h"
#include "PLifeDoc.h"
#include "AboutWnd.h"
#include "GeneralView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_name;
	CString	m_key;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnRegisterOffline();
	afx_msg void OnRegisterOnline();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_name = _T("");
	m_key = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_FULL_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 32);
	DDX_Text(pDX, IDC_KEY, m_key);
	DDV_MaxChars(pDX, m_key, 16);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_REGISTER_OFFLINE, OnRegisterOffline)
	ON_BN_CLICKED(IDC_REGISTER_ONLINE, OnRegisterOnline)
	ON_BN_CLICKED(ID_APP_ABOUT, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CAboutDlg::OnRegisterOffline() 
{
	AfxMainFrame().RegisterOffline();
}

void CAboutDlg::OnRegisterOnline() 
{
	AfxMainFrame().DisplayBrowser(_T("online.htm"));
}

void CAboutDlg::OnHelp() 
{
	AfxMainFrame().DisplayBrowser(_T("register.htm"));
}


class CMainFrame* CMainFrame::s_pMainFrame = NULL;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
static UINT indicators[] =
{
	IDS_DAY, 
	IDS_EMPTY,
	IDS_POPULATION,
	IDS_EMPTY,
	IDS_EXTINCTIONS,
	IDS_EMPTY
};

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	ON_UPDATE_COMMAND_UI(ID_BIOTS_STATUS, OnUpdateBiotsStatus)
	ON_UPDATE_COMMAND_UI(ID_BIOTS_FEED, OnUpdateBiotsFeed)
	ON_UPDATE_COMMAND_UI(ID_BIOTS_EDIT, OnUpdateBiotsEdit)
	ON_UPDATE_COMMAND_UI(ID_BIOTS_TERMINATE, OnUpdateBiotsTerminate)
	ON_WM_SETCURSOR()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_BIOTS_EDIT, OnBiotsEdit)
	ON_COMMAND(ID_BIOTS_FEED, OnBiotsFeed)
	ON_COMMAND(ID_BIOTS_STATUS, OnBiotsStatus)
	ON_COMMAND(ID_INITIAL_UPDATE, OnInitialUpdate)
	ON_COMMAND(ID_BIOTS_TERMINATE, OnBiotsTerminate)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_STEP_TIMER, OnStepTimer)
	ON_COMMAND(ID_BIOTS_MAGNIFY, OnBiotsMagnify)
	ON_UPDATE_COMMAND_UI(ID_BIOTS_MAGNIFY, OnUpdateBiotsMagnify)
	ON_COMMAND(ID_BIOTS_RELOCATE, OnBiotsRelocate)
	ON_UPDATE_COMMAND_UI(ID_BIOTS_RELOCATE, OnUpdateBiotsRelocate)
	ON_COMMAND(ID_BIOTS_CURE, OnBiotsCure)
	ON_COMMAND(ID_BIOTS_SAVE, OnBiotsSave)
	ON_COMMAND(ID_BIOTS_LOAD, OnBiotsLoad)
	ON_UPDATE_COMMAND_UI(ID_BIOTS_LOAD, OnUpdateBiotsLoad)
	ON_UPDATE_COMMAND_UI(ID_BIOTS_SAVE, OnUpdateBiotsSave)
	ON_UPDATE_COMMAND_UI(ID_BIOTS_CURE, OnUpdateBiotsCure)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_UPDATE_COMMAND_UI(ID_APP_ABOUT, OnUpdateAppAbout)
	ON_COMMAND(ID_REGISTER, OnRegister)
	ON_UPDATE_COMMAND_UI(ID_REGISTER, OnUpdateRegister)
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_BAR, OnViewBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BAR, OnUpdateViewBar)
	ON_COMMAND(ID_ENVIRONMENT_STATS, OnEnvironmentStats)
	ON_UPDATE_COMMAND_UI(ID_ENVIRONMENT_STATS, OnUpdateEnvironmentStats)
	ON_COMMAND(ID_VIEW_BESTFITWINDOW, OnViewBestfitwindow)
	ON_WM_ACTIVATEAPP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics
//
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
//
CMainFrame::CMainFrame()
{
	m_bFullScreen = FALSE;
	m_pMenu = NULL;
	m_bViewClientEdge  = FALSE;
	m_bFrameClientEdge = FALSE;
	m_viewOption = CPrimCmdLine::SHOW_ERROR;

	m_cursor = IDC_MAGNIFY;
	m_hCursor = AfxGetApp()->LoadCursor(m_cursor);
	m_bMouseMoved = FALSE;
	m_splashInterval = 210;
	m_splashTimer    = 0;
	m_bBlocked = false;

	s_pMainFrame = this;
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// Don't let the screen saver run if we are
	if (nID == SC_SCREENSAVE)
	{
		if (GetGeneralView()->m_bStarted &&
			AfxGetPLife().IsNormal())
			return;

		if (AfxGetPLife().IsSaver())
			return;
	}

	if (nID == SC_CLOSE && AfxGetPLife().IsSaver())
		return;

	CFrameWnd::OnSysCommand(nID, lParam);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	CRect rect;

	m_viewOption = AfxGetPLife().GetView();

	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	switch (m_viewOption)
	{
	case CPrimCmdLine::SHOW_SMALL_WINDOW:
	{
		CWnd* pParent = CWnd::FromHandle(AfxGetPLife().m_cmd.m_hWnd);
		if (!pParent)
			return FALSE;

		pParent->GetClientRect(&rect);
		cs.hwndParent = pParent->GetSafeHwnd();
		cs.cy = rect.bottom - rect.top;
		cs.cx = rect.right - rect.left;
		cs.x  = rect.left;
		cs.y  = rect.top;
		cs.style      = WS_CHILD | WS_VISIBLE;
		cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
		cs.hMenu      = NULL;
		cs.lpszClass = AfxGetPLife().m_sMainName;
		break;
	}

	case CPrimCmdLine::SHOW_SAVER_WINDOW:
		cs.cy = ::GetSystemMetrics(SM_CYSCREEN);
		cs.cx = ::GetSystemMetrics(SM_CXSCREEN);
		cs.hMenu = NULL;
		cs.x = 0;
		cs.y = 0;
		cs.style = WS_VISIBLE | WS_POPUP;
		cs.dwExStyle = WS_EX_TOPMOST;
		cs.lpszClass = AfxGetPLife().m_sMainName;
		break;

	case CPrimCmdLine::SHOW_NORMAL_WINDOW:
//		cs.style |= WS_VISIBLE;
		cs.lpszClass = AfxGetPLife().m_sMainName;
		break;

	default:
		break;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
//
void CMainFrame::OnViewFullscreen() 
{
	static bool bWasMaximum = false;
	m_bFullScreen = !m_bFullScreen;

	if (m_bFullScreen)
	{
		CRect rect(0, 0,::GetSystemMetrics(SM_CXSCREEN) + 
						::GetSystemMetrics(SM_CXVSCROLL),
						::GetSystemMetrics(SM_CYSCREEN) +
						::GetSystemMetrics(SM_CYHSCROLL));

		StatusBarState(true);

		CFrameWnd::ShowControlBar(&m_wndStatusBar, FALSE, FALSE);

		bWasMaximum = ((GetStyle() & WS_MAXIMIZE) == WS_MAXIMIZE);

		m_bFrameClientEdge = ((GetExStyle() & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE);
		if (m_bFrameClientEdge)
			VERIFY(ModifyStyleEx(WS_EX_CLIENTEDGE, 0, 0));

		m_bViewClientEdge = ((GetActiveView()->GetExStyle() & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE);
		if (m_bViewClientEdge)
			VERIFY(GetActiveView()->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, 0));

		::SetMenu(m_hWnd, NULL);

		VERIFY(ModifyStyle( WS_OVERLAPPEDWINDOW, 0, 0 ));

		ShowWindow(SW_SHOWMAXIMIZED);

		MoveWindow(rect);

	}
	else
	{
		if (StatusBarState(false))
		{
			CFrameWnd::ShowControlBar(&m_wndStatusBar, TRUE, FALSE);
			UpdateStatusBar();
		}

		if (m_bFrameClientEdge)
	   		VERIFY(ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0));

		if (m_bViewClientEdge)
			VERIFY(GetActiveView()->ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0));

		::SetMenu(m_hWnd, m_hMenuDefault);

		VERIFY(ModifyStyle( 0, WS_OVERLAPPEDWINDOW, 0 ));

		if (bWasMaximum)
		{
			CRect screenRect;
			::SystemParametersInfo(SPI_GETWORKAREA, 0, (RECT*) screenRect, 0);
			MoveWindow(screenRect);
		}
		else
			ShowWindow(SW_RESTORE);
	}
}


///////////////////////////////////////////////////////
// OnViewBestfitwindow
//
// Resize the window optimally.  Doesn't quite work if
// the window has a doubled over menu
//
void CMainFrame::OnViewBestfitwindow() 
{
	ViewBestfitwindow(true);
}


///////////////////////////////////////////////////////
// ViewBestfitwindow
//
// Resize the window optimally.  Doesn't quite work if
// the window has a doubled over menu
//
void CMainFrame::ViewBestfitwindow(bool bCenter) 
{
	// We never put it in full screen automatically
	if (m_bFullScreen)
		OnViewFullscreen();

	bool bWasMaximized = ((GetStyle() & WS_MAXIMIZE) == WS_MAXIMIZE);

	CRect envRect;
	GetPLifeDoc()->GetEnvironmentSize(envRect);

	CRect currentRect;
	GetGeneralView()->GetClientRect(currentRect);

	DWORD dwStyle = GetGeneralView()->GetStyle();

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if (dwStyle & WS_VSCROLL)
	{
		// vert scrollbars will impact client area of this window
		currentRect.right += GetSystemMetrics(SM_CXVSCROLL);   // currently on - adjust now
	}

	if (dwStyle & WS_HSCROLL)
	{
		// horz scrollbars will impact client area of this window
		currentRect.bottom += GetSystemMetrics(SM_CYHSCROLL);   // currently on - adjust now
	}

	CRect winRect;
	GetWindowRect(winRect);

	winRect.top    += (envRect.top    - currentRect.top);
	winRect.bottom += (envRect.bottom - currentRect.bottom);
	winRect.left   += (envRect.left   - currentRect.left);
	winRect.right  += (envRect.right  - currentRect.right);

	CRect screenRect;

	::SystemParametersInfo(SPI_GETWORKAREA, 0, (RECT*) screenRect, 0);

	// If the width is too big, we need room for the scroll bar in the
	// other dimension
	if (winRect.Width() > screenRect.Width())
		winRect.bottom += GetSystemMetrics(SM_CYHSCROLL);

	if (winRect.Height() > screenRect.Height())
		winRect.right += GetSystemMetrics(SM_CXHSCROLL);

	// We need to check this twice
	if (winRect.Width() > screenRect.Width())
		winRect.bottom += GetSystemMetrics(SM_CYHSCROLL);

	if (winRect.Width() >= screenRect.Width() &&
		winRect.Height() >= screenRect.Height())
	{
		if (!bWasMaximized && bCenter)
			ShowWindow(SW_SHOWMAXIMIZED);
	}
	else
	{
		if (bWasMaximized)
			ShowWindow(SW_SHOWNORMAL);

		if (winRect.Width() > screenRect.Width())
		{
			winRect.left  = 0;
			winRect.right = screenRect.Width();
		}

		if (winRect.Height() > screenRect.Height())
		{
			winRect.top    = 0;
			winRect.bottom = screenRect.Height();
		}
		MoveWindow(winRect);
		if (bCenter)
			CenterWindow();
	}
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Set up a timer to set the cursor to NULL automatically
 	SetTimer(1, 5000, NULL);

	ShowSplash(TRUE);

	if (AfxGetPLife().IsSaver() || AfxGetPLife().IsSmall())
		::SetCursor(NULL);
	else
		::SetCursor(m_hCursor);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneStyle(0, SBPS_NOBORDERS);
	m_wndStatusBar.SetPaneInfo(1, IDS_EMPTY, SBPS_NORMAL,    50);
	m_wndStatusBar.SetPaneStyle(2, SBPS_NOBORDERS);
	m_wndStatusBar.SetPaneInfo(3, IDS_EMPTY, SBPS_NORMAL, 50);
	m_wndStatusBar.SetPaneStyle(4, SBPS_NOBORDERS);
	m_wndStatusBar.SetPaneInfo(5, IDS_EMPTY, SBPS_NORMAL, 50);
//	m_wndStatusBar.SetPaneInfo(3, IDS_EXTINCTIONS, SBPS_NORMAL, 100);
//	m_wndStatusBar.SetPaneInfo(4, IDS_EXTINCTIONS, SBPS_NORMAL, 100);

	if (!StatusBarState(false))
		m_wndStatusBar.ShowWindow(FALSE);

	return 0;
}


///////////////////////////////////////////////////////
// UpdateStatusBar
//
//
void CMainFrame::UpdateStatusBar()
{
	if (m_wndStatusBar.IsWindowVisible())
	{
		
		CString sString;
		CEnvStats& stats = GetGeneralView()->GetDocument()->m_env.m_stats;
		m_wndStatusBar.SetPaneText(1, stats.GetDaysStr());
		m_wndStatusBar.SetPaneText(3, stats.GetPopulationStr());
		m_wndStatusBar.SetPaneText(5, stats.GetExtinctionsStr());
	}
}


///////////////////////////////////////////////////////
// OnTimer
//
//
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if (m_bBlocked)
		return;

	// Time to null the cursor
	if (m_bMouseMoved == TRUE)
	{
		m_bMouseMoved = FALSE;
	}
	else
	{
		if (m_bFullScreen || AfxGetPLife().IsSaver())
			::SetCursor(NULL);
	}

	if (!AfxGetPLife().IsSmall())
	{
		m_splashTimer += 5;
		if (m_splashTimer > m_splashInterval && !IsIconic())
		{
			ShowSplash(FALSE);
			m_splashTimer  = 0;
			if (m_splashInterval > 30)
				m_splashInterval -= 5;
		}
	}
	CFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bMouseMoved)
	{
		m_bMouseMoved = TRUE;
		::SetCursor(m_hCursor);
	}

	CFrameWnd::OnMouseMove(nFlags, point);
}


void CMainFrame::SaveWindowPosition()
{
	WINDOWPLACEMENT window;
	CWinApp& app      = *AfxGetApp();
	LPCTSTR  pSection = "Placement";

	if (m_viewOption == CPrimCmdLine::SHOW_NORMAL_WINDOW)
	{
		if (GetWindowPlacement(&window))
		{
			app.WriteProfileInt(pSection, "IsValid", TRUE);
			app.WriteProfileInt(pSection, "length", window.length);
			app.WriteProfileInt(pSection, "flags", window.flags);
			app.WriteProfileInt(pSection, "showCmd", window.showCmd);
			app.WriteProfileInt(pSection, "ptMinPosition.x", window.ptMinPosition.x);	
			app.WriteProfileInt(pSection, "ptMinPosition.y", window.ptMinPosition.y);	
			app.WriteProfileInt(pSection, "ptMaxPosition.x", window.ptMaxPosition.x);	
			app.WriteProfileInt(pSection, "ptMaxPosition.y", window.ptMaxPosition.y);	
			app.WriteProfileInt(pSection, "rcNormalPosition.left", window.rcNormalPosition.left);
			app.WriteProfileInt(pSection, "rcNormalPosition.right", window.rcNormalPosition.right);
			app.WriteProfileInt(pSection, "rcNormalPosition.top", window.rcNormalPosition.top);
			app.WriteProfileInt(pSection, "rcNormalPosition.bottom", window.rcNormalPosition.bottom);
			app.WriteProfileInt(pSection, "IsFullScreen", m_bFullScreen);
		}
		else
		{
			app.WriteProfileInt(pSection, "IsValid", FALSE);
		}
	}
}


void CMainFrame::LoadWindowPosition()
{
	WINDOWPLACEMENT window;
	CWinApp& app      = *AfxGetApp();
	LPCTSTR  pSection = "Placement";

	if (m_viewOption == CPrimCmdLine::SHOW_NORMAL_WINDOW)
	{
		if (app.GetProfileInt(pSection, "IsValid", FALSE))
		{
			window.length  = app.GetProfileInt(pSection, "length",  0);
			window.flags   = app.GetProfileInt(pSection, "flags",   0);
			window.showCmd = app.GetProfileInt(pSection, "showCmd", 0);
			window.ptMinPosition.x = app.GetProfileInt(pSection, "ptMinPosition.x", 0);	
			window.ptMinPosition.y = app.GetProfileInt(pSection, "ptMinPosition.y", 0);	
			window.ptMaxPosition.x = app.GetProfileInt(pSection, "ptMaxPosition.x", 0);	
			window.ptMaxPosition.y = app.GetProfileInt(pSection, "ptMaxPosition.y", 0);	
			window.rcNormalPosition.left   = app.GetProfileInt(pSection, "rcNormalPosition.left",   0);
			window.rcNormalPosition.right  = app.GetProfileInt(pSection, "rcNormalPosition.right",  0);
			window.rcNormalPosition.top    = app.GetProfileInt(pSection, "rcNormalPosition.top",    0);
			window.rcNormalPosition.bottom = app.GetProfileInt(pSection, "rcNormalPosition.bottom", 0);

			m_bFullScreen = app.GetProfileInt(pSection, "IsFullScreen", m_bFullScreen);

			MoveWindow(&window.rcNormalPosition);

			AfxGetApp()->m_nCmdShow = window.showCmd;
			if (m_bFullScreen == TRUE)
			{
				m_bFullScreen = FALSE;
				PostMessage(WM_COMMAND, ID_VIEW_FULLSCREEN, 0);
			}
		}
	}
}

void CMainFrame::OnDestroy() 
{
	KillTimer(1);
	SaveWindowPosition();
	CFrameWnd::OnDestroy();
}


////////////////////////////////////////////////////////
// CheckActions
//
//
BOOL CMainFrame::CheckActions()
{
	m_actions++;
	if (!AfxKeyRegistry().IsGoodKey())
	{
		if (m_actions > 5)
		{
			AfxMessageBox("In the unregisted version, you may only use the menu a few times.");
	        return FALSE;
		}
	}
	return TRUE;
}


////////////////////////////////////////////////////////
// OnGetMinMaxInfo
//
// Full screen requires a larger 
//
//
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (m_bFullScreen)
	{
		lpMMI->ptMaxSize.x = ::GetSystemMetrics(SM_CXSCREEN) + 2 * ::GetSystemMetrics(SM_CXVSCROLL) + 100;
		lpMMI->ptMaxSize.y = ::GetSystemMetrics(SM_CYSCREEN) + 2 * ::GetSystemMetrics(SM_CYVSCROLL) + 100;
		lpMMI->ptMaxTrackSize = lpMMI->ptMaxSize;
	}
	else
	{
		CFrameWnd::OnGetMinMaxInfo(lpMMI);
	}
}


/////////////////////////////////////////////////////////////////////
// Cursor Support
//
BOOL CMainFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		if (::GetCursor() != m_hCursor)
			::SetCursor(m_hCursor);
	}
	return CFrameWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainFrame::ChangeCursor(int nCursor)
{
	if (m_cursor != nCursor)
	{
		m_cursor = nCursor;
		if (m_cursor != NULL)
			m_hCursor = AfxGetApp()->LoadCursor(m_cursor);
		else
			m_hCursor = NULL;

		::SetCursor(m_hCursor);
	}
}


////////////////////////////////////////////////////////////
// Biot Menu Support
//
// Update Menu Functions
//
void CMainFrame::OnUpdateBiotsStatus(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck((m_cursor == IDC_STATUS)?1:0);
	
}

void CMainFrame::OnUpdateBiotsFeed(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck((m_cursor == IDC_FEED)?1:0);
}

void CMainFrame::OnUpdateBiotsEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck((m_cursor == IDC_EDIT)?1:0);
}

void CMainFrame::OnUpdateBiotsTerminate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck((m_cursor == IDC_TERMINATE)?1:0);
}

void CMainFrame::OnUpdateBiotsRelocate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck((m_cursor == IDC_RELOCATE)?1:0);
}

void CMainFrame::OnUpdateBiotsMagnify(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck((m_cursor == IDC_MAGNIFY)?1:0);
}


void CMainFrame::OnUpdateBiotsLoad(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck((m_cursor == IDC_LOAD)?1:0);
}

void CMainFrame::OnUpdateBiotsSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck((m_cursor == IDC_SAVE)?1:0);
}

void CMainFrame::OnUpdateBiotsCure(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
	pCmdUI->SetCheck((m_cursor == IDC_CURE)?1:0);
}

void CMainFrame::OnUpdateEnvironmentStats(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnBiotsEdit() 
{
	ChangeCursor(IDC_EDIT);
}

void CMainFrame::OnBiotsFeed() 
{
	ChangeCursor(IDC_FEED);
}

void CMainFrame::OnBiotsStatus() 
{
	ChangeCursor(IDC_STATUS);
}

void CMainFrame::OnBiotsTerminate() 
{
	ChangeCursor(IDC_TERMINATE);
}

void CMainFrame::OnBiotsMagnify() 
{
	ChangeCursor(IDC_MAGNIFY);
}

void CMainFrame::OnBiotsRelocate() 
{
	ChangeCursor(IDC_RELOCATE);
}

void CMainFrame::OnBiotsCure() 
{
	ChangeCursor(IDC_CURE);
}

void CMainFrame::OnBiotsSave() 
{
	ChangeCursor(IDC_SAVE);
}

void CMainFrame::OnBiotsLoad() 
{
	ChangeCursor(IDC_LOAD);
}

void CMainFrame::OnEnvironmentStats() 
{
	GetGeneralView()->OpenEnvironmentStatistics();
}

void CMainFrame::OnStepTimer() 
{
	GetActiveView()->PostMessage(WM_COMMAND, ID_STEP_TIMER);
}

void CMainFrame::OnInitialUpdate()
{
	CKeyRegistry& rKey = AfxKeyRegistry();

	if (!rKey.IsValid())
		rKey.SetKey(0);

	GetGeneralView()->GetDocument()->OnInitialUpdate();
	UpdateStatusBar();
}

void CMainFrame::ShowSplash(BOOL bForce)
{
	CKeyRegistry& rKey = AfxKeyRegistry();

	CString sOwner;
	sOwner.Empty();

	bool bRegistered = rKey.IsGoodKey();

	if (!AfxGetPLife().IsSmall() && (bForce || !bRegistered))
	{
		DWORD time = 3000;

		if (bRegistered)
		{
			time = 2000;

			switch (GetTickCount() % 3)
			{
			case 0:
				sOwner = _T("This world is registered to:\r\n");
				break;
			case 1:
				sOwner = _T("This world belongs to:\r\n");
				break;
			case 2:
				sOwner = _T("Adopted by:\r\n");
				break;
			}

			sOwner += rKey.m_sName;
			sOwner += _T("\r\n");

			if (rKey.IsSiteKey())
				sOwner += _T("Site License");
			else
			{
				if (rKey.IsWorkAndHomeKey())
					sOwner += _T("Single User License");
				else
					sOwner += _T("Single Computer License");
			}
		}
		else
		{
			sOwner.Empty();
		}

		CSplashWnd::ShowSplashScreen(sOwner, this);

		Sleep(time);

		CSplashWnd::HideSplashScreen();
	}
}


void CMainFrame::OnAppAbout() 
{
	CMainTimer timer;

	DisplayBrowser(_T("help.htm"));
}


void CMainFrame::OnUpdateAppAbout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}


void CMainFrame::OnRegister() 
{
	CMainTimer timer;

	CAboutDlg aboutDlg;
	CKeyRegistry& rKey = AfxKeyRegistry();

	aboutDlg.m_name = rKey.GetName();
	aboutDlg.m_key = rKey.GetHexKey();
	if (aboutDlg.m_key == _T("00000000"))
		aboutDlg.m_key.Empty();

	if (aboutDlg.DoModal() == IDOK)
	{
		aboutDlg.m_name.TrimRight();
		aboutDlg.m_name.TrimLeft();
		rKey.SetName(aboutDlg.m_name);

		aboutDlg.m_key.TrimRight();
		aboutDlg.m_key.TrimLeft();
		rKey.SetKey(rKey.SetHexKey(aboutDlg.m_key));

		if (!rKey.IsGoodKey())
		{
			rKey.SetKey(0);
			AfxMessageBox(_T("Sorry, you have entered an invalid or expired registration key."));
		}
		else
		{
			AfxMessageBox(_T("Thank you for registering!\nAll features enabled."));
		}
	}
}


void CMainFrame::OnUpdateRegister(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();	
}

void  CMainFrame::StopTimer(bool bStop)
{
	if (bStop)
	{
		m_bBlocked = true;
		GetGeneralView()->m_bBlocked = true;
	}
	else
	{
		m_bBlocked = false;
		GetGeneralView()->m_bBlocked = false;
	}
}


void CMainFrame::DisplayBrowser(CString sFileName)
{
	CWaitCursor wait;

	CKeyRegistry& key = AfxCommonRegistry();
	
	CString sTemp;
	key.GetString(_T("path"), sTemp);
	sTemp += _T("\\help\\") + sFileName;

	DWORD nError = (DWORD) ShellExecute(GetSafeHwnd(), NULL, sTemp, NULL, NULL, SW_SHOWNORMAL);
	if (nError < 32)
	{
		CString sError;
		switch (nError)
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			sError  = _T("The following file is missing:\n");
			sError +=  sTemp;
			sError += _T("\nYou may need to re-install Primordial Life.");
			break;

		default:
			sError = _T("Unable to open your web browser.  Please make\nsure your web browser is installed properly.");
			break;
		}
		AfxMessageBox(sError);
	}
}


void CMainFrame::RegisterOffline()
{
	PROCESS_INFORMATION p;
	STARTUPINFO s;
	CString sDest;
	CString sDir;

	CKeyRegistry& key = AfxCommonRegistry();
	
	key.GetString(_T("path"), sDir);
	sDir += _T("\\register");
	sDest = sDir + _T("\\register.exe");

	ZeroMemory(&s, sizeof(s));
	s.wShowWindow = SW_MAXIMIZE;

	if (CreateProcess(
			NULL,	// pointer to name of executable module 
			(LPSTR)((LPCTSTR)sDest),	// pointer to command line string
			NULL,	// pointer to process security attributes 
			NULL,	// pointer to thread security attributes 
			TRUE,	// handle inheritance flag 
			0,	// creation flags 
			NULL,	// pointer to new environment block 
			(LPSTR)((LPCTSTR)sDir),	// pointer to current directory name 
			&s,	// pointer to STARTUPINFO 
			&p 	// pointer to PROCESS_INFORMATION  
		))
	{
		CloseHandle(p.hThread);
		CloseHandle(p.hProcess);

		if (AfxGetPLife().GetView() == CPrimCmdLine::SHOW_SAVER_WINDOW)
			PostQuitMessage(0);
	}
	else
	{
		CString sError;
		sError  = _T("Unable to find the registration program:\n") + sDest;
		sError += _T("\nTry re-installing Primordial Life, or\ncontact spofford@io.com for assistance.");
		AfxMessageBox(sError);
	}
}

void CMainFrame::OnClose() 
{
	if (AfxGetPLife().IsSaver())
		return;

	StatusBarState(true);

	CFrameWnd::OnClose();
}


bool CMainFrame::StatusBarState(bool bSave)
{
//	LPCTSTR szSection = _T("Placement");
	LPCTSTR szStatus  = _T("statusBar");

	if (bSave)
	{
		AfxKeyRegistry().SetValue(szStatus, (DWORD) m_wndStatusBar.IsWindowVisible());
		return true;
	}
	else
	{
		DWORD dwVisible = 1;
		AfxKeyRegistry().GetValue(szStatus, &dwVisible);
		return (dwVisible != 0 && !AfxGetPLife().IsSmall());
	}
}


CGeneralView* CMainFrame::GetGeneralView()
{
	return (CGeneralView*)(GetActiveView());
}

CPLifeDoc* CMainFrame::GetPLifeDoc()
{
	return (CPLifeDoc*)(GetActiveView()->GetDocument());
}

void CMainFrame::OnViewBar() 
{
	ShowControlBar(&m_wndStatusBar, !m_wndStatusBar.IsVisible(), FALSE);
	if ((GetStyle() & WS_MAXIMIZE) != WS_MAXIMIZE)
		ViewBestfitwindow(false);
	UpdateStatusBar();
}

void CMainFrame::OnUpdateViewBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndStatusBar.IsVisible());
	pCmdUI->Enable();
}


void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CFrameWnd::OnActivateApp(bActive, hTask);
	
	if (AfxGetPLife().IsNormal())
	{
		if (bActive)
			::SetPriorityClass(AfxGetInstanceHandle(), NORMAL_PRIORITY_CLASS);
		else
			::SetPriorityClass(AfxGetInstanceHandle(), IDLE_PRIORITY_CLASS);
	}
	else
	{
		::SetPriorityClass(AfxGetInstanceHandle(), IDLE_PRIORITY_CLASS);
	}
}
