// GeneralView.cpp : implementation file
//
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Primordial Life.h"
#include "PLifeDoc.h"
#include "GeneralView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralView
//
IMPLEMENT_DYNAMIC(CGeneralView, CScrollView)

BEGIN_MESSAGE_MAP(CGeneralView, CScrollView)
	//{{AFX_MSG_MAP(CGeneralView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CGeneralView::CGeneralView()
{
	m_bBlocked = false;
	m_bStarted = false;
}

CGeneralView::~CGeneralView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CGeneralView drawing
//
void CGeneralView::OnDraw(CDC* pDC)
{
	CPLifeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rect(0, 0, pDoc->Width(), pDoc->Height());
	pDoc->m_env.Paint(pDC, rect);
}


void CGeneralView::OpenEnvironmentStatistics()
{
	GetDocument()->m_env.OpenEnvironmentStatistics(this);
}


void CGeneralView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/) 
{
	if (lHint == 1)
	{
		CSize sizeTotal(GetDocument()->Width(), GetDocument()->Height());

		SetScrollSizes(MM_TEXT, sizeTotal);

		m_bCenter = TRUE;
	}
}


