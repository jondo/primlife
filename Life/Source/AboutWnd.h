// SplashWnd.h - Start-up "Splash Screen" Window
// $Revision$
//
//

#ifndef _SPLASH_SCRN_
#define _SPLASH_SCRN_


/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

class CSplashWnd : public CWnd
{
// Construction
protected:
	CSplashWnd();

// Operations
public:
	static void ShowSplashScreen(CString sOwner, CWnd* pParentWnd = NULL);
	static void IgnoreInput(BOOL ignore = TRUE);
	static void HideSplashScreen();
	static void HideSplashScreenAfter(UINT milliseconds = 0);
	static CWnd* GetSplashWnd();
//	static BOOL PreTranslateAppMessage(MSG* pMsg);

// Attributes:
private:
	CBitmap		m_bitmap;
	CString		m_text[6];
	CFont		m_font;
	CFont       m_font2;
	CRect		m_text_rect;
	CRect		m_text_rect2;
	COLORREF	m_background_color;
	COLORREF	m_text_color;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	~CSplashWnd();
	virtual void PostNcDestroy();

protected:
	static CSplashWnd*	s_pSplashWnd;
	static BOOL			s_bIgnoreInput;

	BOOL Create(CString sOwner, CWnd* pParentWnd = NULL);
	void Hide();

// Generated message map functions
protected:
	//{{AFX_MSG(CSplashWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif
