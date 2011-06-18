// MagnifyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Primordial Life.h"
#include "MagnifyWnd.h"
#include "Environ.h"
#include "Biots.h"
//#include "GsProp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWinRect
//
//
void CWinRect::FromRegistry(LPCSTR szType, long dLeft, long dTop, long dRight, long dBottom)
{
	CString sKey = szType;
	sKey  += ".left";
	left   = AfxUserRegistry().GetValue(sKey, dLeft);

	sKey   = szType;
	sKey  += ".top";
	top    = AfxUserRegistry().GetValue(sKey, dTop);

	sKey   = szType;
	sKey  += ".right";
	right  = AfxUserRegistry().GetValue(sKey, dRight);


	sKey   = szType;
	sKey  += ".bottom";
	bottom = AfxUserRegistry().GetValue(sKey, dBottom);

	NormalizeRect();

	if (Width() > GetSystemMetrics(SM_CXSCREEN) || Width() < 40)
	{
		left  = dLeft;
		right = dRight;
	}

	if (Height() > GetSystemMetrics(SM_CYSCREEN) || Height() < 40)
	{
		top    = dTop;
		bottom = dBottom;
	}
			
	if (left < 0)
	{
 		right -= left;
		left   = 0;
	}

	if (top < 0)
	{
		bottom -= top;
		top     = 0;
	}

	if (bottom > GetSystemMetrics(SM_CYSCREEN))
	{
		top   -= (bottom - GetSystemMetrics(SM_CYSCREEN));
		bottom = GetSystemMetrics(SM_CYSCREEN);
	}

	if (right > GetSystemMetrics(SM_CXSCREEN))
	{
		left -= (right - GetSystemMetrics(SM_CXSCREEN));
		right = GetSystemMetrics(SM_CXSCREEN);
	}
}


void CWinRect::ToRegistry(LPCSTR szType)
{
	CString sKey = szType;
	sKey  += ".left";
	AfxUserRegistry().SetValue(sKey, left);

	sKey   = szType;
	sKey  += ".top";
	AfxUserRegistry().SetValue(sKey, top);

	sKey   = szType;
	sKey  += ".right";
	AfxUserRegistry().SetValue(sKey, right);


	sKey   = szType;
	sKey  += ".bottom";
	AfxUserRegistry().SetValue(sKey, bottom);
}


/////////////////////////////////////////////////////////////////////////////
// CMagnifyWnd
//
//
IMPLEMENT_DYNCREATE(CMagnifyWnd, CMiniFrameWnd)

CMagnifyWnd::CMagnifyWnd(CMagnifyWnd** pExternalRef)
{
	m_pEnv         = NULL;
	m_choice       = ID_SHAPE;
	m_pExternalRef = pExternalRef;
	m_pLastBiot    = NULL;
	ZeroMemory(m_energy, sizeof(m_energy));
	m_energyIndex = MAX_ENERGY_HISTORY - 1;
	m_toolbarHeight = 0;
}

CMagnifyWnd::~CMagnifyWnd()
{
}


void CMagnifyWnd::OnSize(UINT nType, int cx, int cy) 
{
	CMiniFrameWnd::OnSize(nType, cx, cy);

	if (nType == SIZE_RESTORED)
	{
		cy -= m_toolbarHeight;

		if (m_graph.GetSafeHwnd() != NULL)
			m_graph.MoveWindow(0, 0, cx, cy, FALSE);

		if (m_breedView.GetSafeHwnd() != NULL)
			m_breedView.MoveWindow(0, 0, cx, cy, FALSE);

		Invalidate();
	}
}


Biot* CMagnifyWnd::FindBiotByID(int nBiotId)
{
	ASSERT(m_pEnv);
	return m_pEnv->FindBiotByID(nBiotId);
}


/*static*/
BOOL CMagnifyWnd::CreateWnd(Environment* pEnv, CWnd* pParentWnd, CMagnifyWnd** pExternalRef)
{
	CMagnifyWnd* pWnd;

	pWnd = new CMagnifyWnd(pExternalRef);

	if (!pWnd)
		return NULL;

	pWnd->m_pEnv       = pEnv;
	pWnd->m_pParentWnd = pParentWnd;

	for (int i = 0; i < 5; i++)
		pWnd->m_colorDistance[i] = -1;

	CWinRect rect;

	rect.FromRegistry("Magnify", 100, 100, 500, 400);

	BOOL bSuccess = pWnd->Create(NULL, "", WS_SYSMENU | WS_VISIBLE | MFS_SYNCACTIVE | MFS_THICKFRAME, rect, pParentWnd);
	
	if (!bSuccess)
	{
		delete pWnd;
		return NULL;
	}

	CRect cRect;
	pWnd->GetClientRect(&cRect);

	// Subtract off the toolbar area
	CRect cToolbar;
	pWnd->m_wndToolBar.GetWindowRect(cToolbar);
	pWnd->m_toolbarHeight = (cToolbar.bottom - cToolbar.top);
		
	cRect.bottom -= pWnd->m_toolbarHeight;

	bSuccess = pWnd->m_graph.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, cRect, pWnd, ID_GRAPH, NULL);
	if (!bSuccess)
	{
		pWnd->DestroyWindow();
		AfxMessageBox("Unable to display graph object.\nYou may need to re-install.");
		return NULL;
	}

	pWnd->m_choice = AfxUserRegistry().GetValue("Magnify.choice", pWnd->m_choice);

	((CView*)&pWnd->m_breedView)->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, cRect, pWnd, 101, NULL);
	pWnd->m_breedView.ShowWindow(SW_HIDE);

	switch (pWnd->m_choice)
	{
		case ID_SHAPE:
			pWnd->OnShape();
			break;

		case ID_COMPOSITION:
			pWnd->OnComposition();
			break;

		case ID_ENERGY:
			pWnd->OnEnergy();
			break;

		case ID_BREEDING:
			pWnd->OnBreeding();
			break;
	}

	if (pExternalRef)
		*pExternalRef = pWnd;

	return bSuccess;
}


// Call OnClose to close
BEGIN_MESSAGE_MAP(CMagnifyWnd, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CMagnifyWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_COMPOSITION, OnComposition)
	ON_UPDATE_COMMAND_UI(ID_COMPOSITION, OnUpdateComposition)
	ON_COMMAND(ID_SHAPE, OnShape)
	ON_UPDATE_COMMAND_UI(ID_SHAPE, OnUpdateShape)
	ON_WM_SIZE()
	ON_COMMAND(ID_ENERGY, OnEnergy)
	ON_UPDATE_COMMAND_UI(ID_ENERGY, OnUpdateEnergy)
	ON_COMMAND(ID_BREEDING, OnBreeding)
	ON_UPDATE_COMMAND_UI(ID_BREEDING, OnUpdateBreeding)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMagnifyWnd message handlers
//
void CMagnifyWnd::OnPaint() 
{
	CPaintDC dc(this);
	
	if (m_pEnv->m_bIsSelected)
	{
		Biot* pBiot = m_pEnv->FindBiotByID(m_pEnv->m_selectedId);

		if (pBiot)
		{
			CRect rect = GetReducedClientRect();

			switch (m_choice)
			{
				case ID_SHAPE:
					m_pEnv->MagnifyBiot(dc, pBiot, rect);
					break;
			}
			return;
		}
	}
	RestInPeace(&dc);
}


//////////////////////////////////////////////////////
// RestInPeace
//
void CMagnifyWnd::RestInPeace(CDC* pDc)
{
	CRect rect      = GetReducedClientRect();
	LPCTSTR pszText = _T("Rest In Peace");

	pDc->FillSolidRect(rect, RGB(0, 0, 0));
	pDc->SetTextColor(RGB(255,255,255));
	pDc->SetBkColor(RGB(0,0,0));
	pDc->TextOut(15, 15, pszText);
}


//////////////////////////////////////////////////////
// GetReducedClientRect
//
// Gets the rect of the client minus the toolbar
//
CRect CMagnifyWnd::GetReducedClientRect()
{
	CRect rect;
	GetClientRect(rect);
	rect.bottom -= m_toolbarHeight;
	return rect;
}


void CMagnifyWnd::PaintNow(Biot* pBiot)
{
	if (pBiot)
	{
		BOOL bNewBiot = SetTitle(pBiot);

		switch(m_choice)
		{
			case ID_SHAPE:
			{
				CDC* pDC = GetDC();
		
				if (pDC)
				{
					CRect rect;
					GetClientRect(rect);
					rect.bottom -= m_toolbarHeight;

					m_pEnv->MagnifyBiot(*pDC, pBiot, rect);
					VERIFY(ReleaseDC(pDC));
				}
			}
			break;

			case ID_COMPOSITION:
				if (bNewBiot)
					OnComposition();
				else
					SetComposition(pBiot);
				break;

			case ID_ENERGY:
				if (bNewBiot)
					OnEnergy();
				else
					SetEnergy(pBiot);
				break;

			case ID_BREEDING:
				if (bNewBiot)
					OnBreeding();
				else
					SetBreeding(pBiot);
				break;
		}
	}
	else
	{
		m_graph.ShowWindow(SW_HIDE);
		m_breedView.ShowWindow(SW_HIDE);

		CDC* pDC = GetDC();
		if (pDC)
		{
			RestInPeace(pDC);
			VERIFY(ReleaseDC(pDC));
		}
	}
}


BOOL CMagnifyWnd::DestroyWindow() 
{
	CWinRect rect;
	GetWindowRect(rect);
	rect.ToRegistry("Magnify");

	AfxUserRegistry().SetValue("Magnify.choice", m_choice);

	if (m_graph.GetSafeHwnd() != NULL)
		m_graph.DestroyWindow();

	if (m_breedView.GetSafeHwnd() != NULL)
		m_breedView.DestroyWindow();

	return CMiniFrameWnd::DestroyWindow();
}


// Do NOT REMOVE!! - normally this deletes the object!!
void CMagnifyWnd::PostNcDestroy() 
{
	if (m_pExternalRef != NULL)
		*m_pExternalRef = NULL;

	CMiniFrameWnd::PostNcDestroy();
}


BOOL CMagnifyWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
//	cs.hMenu      = ::LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAGNIFY_MENU));
	cs.dwExStyle |= WS_EX_TOOLWINDOW;	

	return CMiniFrameWnd::PreCreateWindow(cs);
}


BOOL CMagnifyWnd::SetTitle(Biot* pBiot, BOOL bChange)
{
	LPCTSTR pszTitle = NULL;

	if (pBiot && (m_pLastBiot != pBiot || bChange))
	{
		switch (m_choice)
		{
			case ID_SHAPE:
				pszTitle = "Magnify Biot";
				break;

			case ID_COMPOSITION:
				pszTitle = "Biot Line Types";
				break;

			case ID_ENERGY:
				pszTitle = "Energy Percentage over the Last Day";
				break;

			case ID_BREEDING:
				pszTitle = "Biot Breeding";
				break;
		}

		CString sTitle = pszTitle;
		sTitle += " - ";
		sTitle += pBiot->GetFullName();
		SetWindowText(sTitle);
		m_pLastBiot = pBiot;
		return TRUE;
	}
	m_pLastBiot = pBiot;
	return FALSE;
}


//////////////////////////////////////////////////////////
// COMPOSITION METHODS
//
//
void CMagnifyWnd::OnComposition() 
{
	m_choice = ID_COMPOSITION;

	m_graph.SetDataReset(9);
	m_graph.SetGraphType(2);
	m_graph.SetGraphStyle(4);
	m_graph.SetBackground(0);

	m_graph.SetNumPoints(5);
	m_graph.SetNumSets(1);

	m_graph.SetAutoInc(1);
	m_graph.SetColorData(10);	// Green
	m_graph.SetColorData(9);	// Light Blue
	m_graph.SetColorData(12);	// Light Red
	m_graph.SetColorData(11);	// Light Cyan
	m_graph.SetColorData(15); 	// White

	m_graph.SetAutoInc(1);
	m_graph.SetLegendText("Chloroplasts");
	m_graph.SetLegendText("Shields");
	m_graph.SetLegendText("Teeth");
	m_graph.SetLegendText("Eyes");
	m_graph.SetLegendText("Injectors");

	Biot* pBiot = m_pEnv->FindBiotByID(m_pEnv->m_selectedId);
	SetTitle(pBiot, TRUE);
	SetComposition(pBiot, TRUE);
	m_breedView.ShowWindow(SW_HIDE);
	m_graph.ShowWindow(SW_SHOWNORMAL);
}


void CMagnifyWnd::SetComposition(Biot* pBiot, BOOL bDraw)
{
	if (!pBiot)
		return;

	int i;
	BOOL bSet;

	for (i = 0, bSet = FALSE; i < 5; i++)
	{
		if (m_colorDistance[i] != pBiot->colorDistance[i])
		{
			bSet = TRUE;
			m_colorDistance[i] = pBiot->colorDistance[i];
		}
	}

	if (bSet || bDraw)
	{
		m_graph.SetAutoInc(1);

		for (i = 0; i < 5; i++)
			m_graph.SetGraphData((float) pBiot->colorDistance[i]);

		m_graph.SetDrawMode(3);
	}
}


void CMagnifyWnd::OnUpdateComposition(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_choice == ID_COMPOSITION);
	pCmdUI->Enable(m_pEnv->FindBiotByID(m_pEnv->m_selectedId) != NULL);
}


//////////////////////////////////////////////////////////
// SHAPE METHODS
//
//
void CMagnifyWnd::OnShape() 
{
	m_choice = ID_SHAPE;
	SetTitle(m_pEnv->FindBiotByID(m_pEnv->m_selectedId), TRUE);
	m_graph.ShowWindow(SW_HIDE);
	m_breedView.ShowWindow(SW_HIDE);
}


void CMagnifyWnd::OnUpdateShape(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_choice == ID_SHAPE);
	pCmdUI->Enable(m_pEnv->GetSelectedBiot() != NULL);
}


//////////////////////////////////////////////////////////
// ENERGY METHODS
//
//
void CMagnifyWnd::OnEnergy() 
{
	m_choice = ID_ENERGY;

	m_graph.SetDataReset(9);
	m_graph.SetGraphType(6);
	m_graph.SetGraphStyle(4);
	m_graph.SetBackground(0);
	m_graph.SetPatternedLines(1);
	m_graph.SetPatternData(6);
	m_graph.SetRandomData(0);
	m_graph.SetLabels(3); // Labels on Y axis only
//	m_graph.SetLineStats(3);
//	m_graph.SetLeftTitle("Energy");
//	m_graph.SetBottomTitle("Last Day");
	m_graph.SetYAxisTicks(10);
	m_graph.SetYAxisMax(100.0);
	m_graph.SetYAxisMin(0.0);
	m_graph.SetYAxisStyle(2);

	m_graph.SetTicks(1); // Only ticks on both axis
	m_graph.SetTickEvery(5);

	m_graph.SetNumPoints(MAX_ENERGY_HISTORY);
	m_graph.SetNumSets(1);

 //	m_graph.SetAutoInc(1);
/*	double d = -1.0;
	CString sTemp;
	for (int i = 0; i < MAX_ENERGY_HISTORY; i++)
	{
		sTemp.Format("%2.1f", d);
		d += 0.05;
		m_graph.SetLabelText(sTemp);
	}*/
	Biot* pBiot = m_pEnv->FindBiotByID(m_pEnv->m_selectedId);
	SetTitle(pBiot, TRUE);
	SetEnergy(pBiot, TRUE);
	m_breedView.ShowWindow(SW_HIDE);
	m_graph.ShowWindow(SW_SHOWNORMAL);
}


void CMagnifyWnd::SetEnergy(Biot* pBiot, BOOL bDraw)
{
	if (!pBiot)
		return;

	int i = 0;

	if (!bDraw && (pBiot->m_age & 0x3F) != 0)
		return;

	m_graph.SetAutoInc(1);

	for (i = 0; i < pBiot->m_statIndex; i++)
		m_energy[MAX_ENERGY_HISTORY - 1 - i] = pBiot->m_statEnergy[pBiot->m_statIndex - i - 1];

	for (i = pBiot->m_statIndex; i < MAX_ENERGY_HISTORY; i++)
		m_energy[i - pBiot->m_statIndex] = pBiot->m_statEnergy[i];


	for (i = 0; i < MAX_ENERGY_HISTORY; i++)
		m_graph.SetGraphData(m_energy[i]);

	m_graph.SetAutoInc(1);
	if (m_energy[MAX_ENERGY_HISTORY - 1] > m_energy[MAX_ENERGY_HISTORY - 2])
		m_graph.SetColorData(10);	// Green
	else
		if (m_energy[MAX_ENERGY_HISTORY - 1] < m_energy[MAX_ENERGY_HISTORY - 2])
			m_graph.SetColorData(12);	// Red
		else
			m_graph.SetColorData(14); //Yellow

	m_graph.SetDrawMode(3);
}


void CMagnifyWnd::OnUpdateEnergy(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_choice == ID_ENERGY);
	pCmdUI->Enable(m_pEnv->GetSelectedBiot() != NULL);
}


void CMagnifyWnd::OnBreeding() 
{
	Biot* pBiot = m_pEnv->FindBiotByID(m_pEnv->m_selectedId);

	m_choice = ID_BREEDING;
	m_graph.ShowWindow(SW_HIDE);

	SetTitle(pBiot, TRUE);
	m_breedView.UpdateData(FALSE);
	SetBreeding(pBiot);
	m_breedView.ShowWindow(SW_SHOWNORMAL);
}


void CMagnifyWnd::SetBreeding(Biot* pBiot) 
{
	if (!m_breedView.IsWindowVisible())
		m_breedView.ShowWindow(SW_SHOWNORMAL);

	m_breedView.UpdateBiot(pBiot, pBiot->m_Id);
}


void CMagnifyWnd::OnUpdateBreeding(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_choice == ID_BREEDING);
	pCmdUI->Enable(m_pEnv->GetSelectedBiot() != NULL);
}


int CMagnifyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMiniFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM
		| CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_BIOTSTATS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	return 0;
}


