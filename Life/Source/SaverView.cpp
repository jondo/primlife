// SaverView.cpp : implementation file
//
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Primordial Life.h"
#include "PLifeDoc.h"
#include "SaverView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaverView

IMPLEMENT_DYNCREATE(CSaverView, CGeneralView)

CSaverView::CSaverView()
{
	m_ptLast = CPoint(-1,-1);
	m_bDialogUp = false;
}


CSaverView::~CSaverView()
{
}


BEGIN_MESSAGE_MAP(CSaverView, CGeneralView)
	//{{AFX_MSG_MAP(CSaverView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_SYSCOMMAND()
	ON_WM_SETCURSOR()
	ON_WM_NCACTIVATE()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSaverView diagnostics

#ifdef _DEBUG
void CSaverView::AssertValid() const
{
	CView::AssertValid();
}


void CSaverView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSaverView message handlers
void CSaverView::OnDestroy() 
{
	PostQuitMessage(0);
	CView::OnDestroy();
}


void CSaverView::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if ((nID == SC_SCREENSAVE) || (nID == SC_CLOSE))
		return;
	CView::OnSysCommand(nID, lParam);
}


BOOL CSaverView::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
//	SetCursor(NULL);
	return TRUE;
}


BOOL CSaverView::OnNcActivate(BOOL bActive) 
{
	if (!bActive)
		return FALSE;
	return CView::OnNcActivate(bActive);
}


void CSaverView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	if (nState == WA_INACTIVE && !m_bDialogUp)
		PostMessage(WM_CLOSE);
	CView::OnActivate(nState, pWndOther, bMinimized);
}


void CSaverView::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	if (!bActive && !m_bDialogUp)
		PostMessage(WM_CLOSE);
	CView::OnActivateApp(bActive, hTask);
}


void CSaverView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_ptLast == CPoint(-1,-1))
		m_ptLast = point;
	else if (m_ptLast != point && !m_bDialogUp)
		PostMessage(WM_CLOSE);

	CView::OnMouseMove(nFlags, point);
}


void CSaverView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_bDialogUp)
		PostMessage(WM_CLOSE);
	CView::OnLButtonDown(nFlags, point);
}


void CSaverView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_bDialogUp)
		PostMessage(WM_CLOSE);
	CView::OnMButtonDown(nFlags, point);
}


void CSaverView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_bDialogUp)
		PostMessage(WM_CLOSE);
	CView::OnRButtonDown(nFlags, point);
}


void CSaverView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
//	if (!m_bDialogUp)
		PostMessage(WM_CLOSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CSaverView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bDialogUp)
		PostMessage(WM_CLOSE);
	CView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CSaverView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.lpszClass = AfxGetPLife().m_sClassName;

	BOOL bResult = CView::PreCreateWindow(cs);

	cs.dwExStyle &= (~WS_EX_CLIENTEDGE);

	return bResult;
}


void CSaverView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CPLifeDoc* pDoc = GetDocument();
	ASSERT(pDoc);

	CSize sizeTotal(pDoc->Width(), pDoc->Height());

	SetScrollSizes(MM_TEXT, sizeTotal);
	m_bCenter = TRUE;
}


void CSaverView::OnIdle() 
{
	GetDocument()->m_env.Skip(this);
}

void CSaverView::OnClose() 
{
	m_bDialogUp = true;

	// TODO: Add your message handler code here and/or call default
	if (AfxVerifyPassword(GetSafeHwnd()))
		CGeneralView::OnClose();

//	m_bDialogUp = false;
}
