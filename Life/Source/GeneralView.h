// GeneralView.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CGeneralView view
//
#pragma once

class CGeneralView : public CScrollView
{
public:
	CPLifeDoc* GetDocument();
	bool m_bBlocked;
	bool m_bStarted;


 	void OpenEnvironmentStatistics();

	virtual void OnIdle() = 0;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeneralView)
	protected:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

protected:
	CGeneralView(); 
	virtual ~CGeneralView();

	DECLARE_DYNCREATE(CGeneralView)

	//{{AFX_MSG(CGeneralView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


inline CPLifeDoc* CGeneralView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPLifeDoc)));
	return (CPLifeDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
