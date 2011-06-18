// PLifeView.cpp : implementation of the CPLifeView class
//
//////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Primordial Life.h"

#include "PLifeDoc.h"
#include "PLifeView.h"
#include "MainFrm.h"
#include "evolve.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPLifeView

IMPLEMENT_DYNCREATE(CPLifeView, CGeneralView)

BEGIN_MESSAGE_MAP(CPLifeView, CGeneralView)
	//{{AFX_MSG_MAP(CPLifeView)
	ON_COMMAND(ID_SIMULATION_STARTSIMULATION, OnSimulationStart)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_SIMULATION_STARTSIMULATION, OnUpdateSimulationStart)
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_COMMAND(ID_SIMULATION_SETTINGS, OnSimulationSettings)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLifeView construction/destruction

CPLifeView::CPLifeView()
{
	m_yScroll  = 0;
	m_xScroll  = 0;
}

CPLifeView::~CPLifeView()
{
}

BOOL CPLifeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = AfxGetPLife().m_sClassName;

	return CScrollView::PreCreateWindow(cs);
}

void CPLifeView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CPLifeDoc* pDoc = GetDocument();
	ASSERT(pDoc);

	CSize sizeTotal(pDoc->Width(), pDoc->Height());

	SetScrollSizes(MM_TEXT, sizeTotal);

	m_bCenter = TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CPLifeView diagnostics
//
#ifdef _DEBUG
void CPLifeView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPLifeView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPLifeView message handlers
//
//
void CPLifeView::OnUpdateSimulationStart(CCmdUI* pCmdUI) 
{
	if (m_bStarted)
		pCmdUI->SetText(_T("&Stop Simulation"));
	else
		pCmdUI->SetText(_T("&Start Simulation"));
}


void CPLifeView::OnSimulationStart() 
{
	m_bStarted = !m_bStarted;
	OnIdle();
}


void CPLifeView::OnDestroy() 
{
	GetDocument()->StopSimulation();
	
	CScrollView::OnDestroy();
}


void CPLifeView::OnViewFullscreen() 
{
	AfxMainFrame().OnViewFullscreen();
}

//Fox BEGIN
void CPLifeView::OnSimulationSettings() 
{
	DialogBoxParam(AfxGetApp()->m_hInstance, 
		MAKEINTRESOURCE(DLG_SCRNSAVECONFIGURE),
		NULL, (DLGPROC) ConfigureDialog, (LPARAM)&GetDocument()->m_env);
}
//Fox END

void CPLifeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_LEFT:
			OnScroll(MAKEWORD(SB_LINEUP, -1), 0);
			break;

		case VK_RIGHT:
			OnScroll(MAKEWORD(SB_LINEDOWN, -1), 0);
			break;

		case VK_UP:
			OnScroll(MAKEWORD(-1, SB_LINEUP), 0);
			break;

		case VK_DOWN:
			OnScroll(MAKEWORD(-1, SB_LINEDOWN), 0);
			break;

		case VK_NEXT:
			OnScroll(MAKEWORD(-1, SB_PAGEDOWN), 0);
			break;

		case VK_PRIOR:
			OnScroll(MAKEWORD(-1, SB_PAGEUP), 0);
			break;

		case VK_HOME:
			OnScroll(MAKEWORD(SB_PAGEUP, -1), 0);
			break;

		case VK_END:
			OnScroll(MAKEWORD( SB_PAGEDOWN, -1), 0);
			break;

		default:
			CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
			break;
	}
}


void CPLifeView::OnMouseMove(UINT /*nFlags*/, CPoint point) 
{
	m_xScroll = 0;
	m_yScroll = 0;

	if (AfxMainFrame().m_bFullScreen)
	{
		int width  = ::GetSystemMetrics(SM_CXSCREEN); 
		int height = ::GetSystemMetrics(SM_CYSCREEN);

		if (point.x > width - 2)
			m_xScroll = VK_RIGHT;

		if (point.x < 1)
			m_xScroll = VK_LEFT;

		if (point.y > height - 2)
			m_yScroll = VK_DOWN;

		if (point.y < 1)
			m_yScroll = VK_UP;
	}
}


int CPLifeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}


////////////////////////////////////////////////////////////
// OnLButtonDown
//
void CPLifeView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint adjust = GetDeviceScrollPosition();

	point += adjust;

	GetDocument()->m_env.BiotOperation(this, point.x, point.y, AfxMainFrame().m_cursor);

	CScrollView::OnLButtonDown(nFlags, point);
}


void CPLifeView::OnIdle() 
{
	if (m_yScroll)
		OnKeyDown(m_yScroll, 0, 0);

	if (m_xScroll)
		OnKeyDown(m_xScroll, 0, 0);

	if (m_bStarted)
		GetDocument()->m_env.Skip(this);
}

