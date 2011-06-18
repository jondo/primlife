//
// PrimView.h : interface of the CPLifeView class
//
/////////////////////////////////////////////////////////////////////////////

class CPrimView : public CScrollView
{
protected: // create from serialization only
	CPrimView();
	DECLARE_DYNCREATE(CPrimView)

// Attributes
public:
	CPLifeDoc& GetDocument();
	BOOL m_bStarted;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLifeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPLifeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPLifeView)
	afx_msg void OnSimulationStart();
	afx_msg void OnDestroy();
	afx_msg void OnUpdateSimulationStart(CCmdUI* pCmdUI);
	afx_msg void OnViewFullscreen();
	afx_msg void OnSimulationSettings();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnStepTimer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in PLifeView.cpp
inline CPLifeDoc& CPLifeView::GetDocument()
   { return *((CPLifeDoc*)m_pDocument); }
#endif

/////////////////////////////////////////////////////////////////////////////
