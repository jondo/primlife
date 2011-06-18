// SmallView.cpp : implementation file
//
/////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Primordial Life.h"
#include "PLifeDoc.h"
#include "SmallView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmallView
//
IMPLEMENT_DYNCREATE(CSmallView, CGeneralView)

CSmallView::CSmallView()
{
}

CSmallView::~CSmallView()
{
}


BEGIN_MESSAGE_MAP(CSmallView, CGeneralView)
	//{{AFX_MSG_MAP(CSmallView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmallView diagnostics
//
#ifdef _DEBUG
void CSmallView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CSmallView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

#endif //_DEBUG

void CSmallView::OnDraw(CDC* pDC)
{
	CGeneralView::OnDraw(pDC);
	m_bStarted = true;
}


/////////////////////////////////////////////////////////////////////////////
// OnInitialUpdate
//
void CSmallView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CPLifeDoc* pDoc = GetDocument();
	ASSERT(pDoc);

	CSize sizeTotal(pDoc->Width(), pDoc->Height());

	SetScrollSizes(MM_TEXT, sizeTotal);
	m_bCenter = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CSmallView message handlers
//
BOOL CSmallView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.lpszClass = AfxGetPLife().m_sClassName;

	BOOL bResult = CScrollView::PreCreateWindow(cs);

	cs.dwExStyle &= (~WS_EX_CLIENTEDGE);

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// CSmallView message handlers
//
void CSmallView::OnIdle() 
{
	if (m_bStarted)
		GetDocument()->m_env.Skip(this);
//	TRACE("idle\n");
}
