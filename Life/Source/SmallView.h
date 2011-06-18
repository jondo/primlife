// SmallView.h : header file
//
///////////////////////////////////////////////////////////////////
#include "GeneralView.h"

/////////////////////////////////////////////////////////////////////////////
// CSmallView view

class CSmallView : public CGeneralView
{
protected:
	CSmallView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSmallView)


// Attributes
public:

// Operations
public:
	void OnIdle();
	void OnDraw(CDC* pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmallView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSmallView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSmallView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
