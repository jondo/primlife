//
// PLifeView.h : interface of the CPLifeView class
//
/////////////////////////////////////////////////////////////////////////////
#include "GeneralView.h"

class CPLifeView : public CGeneralView
{
protected: // create from serialization only
	CPLifeView();
	DECLARE_DYNCREATE(CPLifeView)

// Attributes
public:
	BOOL m_bFullScreen;
	int m_xScroll;
	int m_yScroll;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLifeView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

	void OnIdle();

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
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
