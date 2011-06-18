// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef MainFrm_h
#define MainFrm_h

class CGeneralView;
class CPLifeDoc;

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Operations
public:
	void SaveWindowPosition();
	void LoadWindowPosition();
	BOOL CheckActions();
	afx_msg void OnViewFullscreen();
	afx_msg void OnViewBestfitwindow();
	void ChangeCursor(int nCursor);
	void ShowSplash(BOOL bForce);
	void StopTimer(bool bStop);

	void DisplayBrowser(CString sFileName);
	void RegisterOffline();

	void UpdateStatusBar();

	CGeneralView* GetGeneralView();
	CPLifeDoc* GetPLifeDoc();

	bool StatusBarState(bool bSave);
	void ViewBestfitwindow(bool bCenter);



	// Attributes
public:

	static CMainFrame* s_pMainFrame;

	CStatusBar  m_wndStatusBar;

	BOOL    m_bFullScreen;
	CMenu*  m_pMenu;
	BOOL    m_bViewClientEdge;
	BOOL    m_bFrameClientEdge;
	int     m_viewOption;
	int     m_actions;
	int     m_cursor;
	HCURSOR m_hCursor;
	BOOL    m_bMouseMoved;
	int     m_splashInterval;
	int     m_splashTimer;
	bool    m_bBlocked;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnUpdateBiotsStatus(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBiotsFeed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBiotsEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBiotsTerminate(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnBiotsEdit();
	afx_msg void OnBiotsFeed();
	afx_msg void OnBiotsStatus();
	afx_msg void OnInitialUpdate();
	afx_msg void OnBiotsTerminate();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStepTimer();
	afx_msg void OnBiotsMagnify();
	afx_msg void OnUpdateBiotsMagnify(CCmdUI* pCmdUI);
	afx_msg void OnBiotsRelocate();
	afx_msg void OnUpdateBiotsRelocate(CCmdUI* pCmdUI);
	afx_msg void OnBiotsCure();
	afx_msg void OnBiotsSave();
	afx_msg void OnBiotsLoad();
	afx_msg void OnUpdateBiotsLoad(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBiotsSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBiotsCure(CCmdUI* pCmdUI);
	afx_msg void OnAppAbout();
	afx_msg void OnUpdateAppAbout(CCmdUI* pCmdUI);
	afx_msg void OnRegister();
	afx_msg void OnUpdateRegister(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnViewBar();
	afx_msg void OnUpdateViewBar(CCmdUI* pCmdUI);
	afx_msg void OnEnvironmentStats();
	afx_msg void OnUpdateEnvironmentStats(CCmdUI* pCmdUI);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



inline CMainFrame& AfxMainFrame()
{
	return (CMainFrame&) *AfxGetMainWnd();
}


/////////////////////////////////////////////////////////////////////////////
// CMainTimer - a simple class to turn off the timer while a dialog is up
// or for some other activity.  The destructor will start the timer.
//
//
class CMainTimer
{
public:
	CMainTimer() : m_mainFrame(AfxMainFrame()) { Stop(); }
	virtual ~CMainTimer() { Start(); }
	
	void Stop()  { m_mainFrame.StopTimer(true); }
	void Start() { m_mainFrame.StopTimer(false); }

private:
	CMainFrame& m_mainFrame;
};

#endif
