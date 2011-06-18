// PLifeDoc.cpp : implementation of the CPLifeDoc class
//

#include "stdafx.h"
#include "Primordial Life.h"
#include "Settings.h"

#include "MainFrm.h"

#include "evolve.h"
#include "environ.h"
#include "PLifeDoc.h"
#include "InitialPopulation.h"
#include "WorldSize.h"
#include "NetworkDlg.h"
#include "HelpPropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPLifeDoc

IMPLEMENT_DYNCREATE(CPLifeDoc, CDocument)

BEGIN_MESSAGE_MAP(CPLifeDoc, CDocument)
	//{{AFX_MSG_MAP(CPLifeDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLifeDoc construction/destruction

CPLifeDoc::CPLifeDoc()
{
	m_bInitializeWithOpen = false;
}

CPLifeDoc::~CPLifeDoc()
{
}


CScrollView* CPLifeDoc::GetScrollView()
{
	POSITION pos = GetFirstViewPosition();
	return (CScrollView*) GetNextView(pos);
}


void CPLifeDoc::OnInitialUpdate()
{
	if (m_bInitializeWithOpen)
	{
		m_env.OnOpen(GetScrollView());
		AfxMainFrame().OnViewBestfitwindow();
//		GetScrollView()->ResizeParentToFit(FALSE);
//		GetScrollView()->Invalidate();
	}
	else
	{
		OnNew(false);
	}
}


BOOL CPLifeDoc::OnNewDocument()
{
	return OnNew(true);
}


BOOL CPLifeDoc::OnNew(bool bShowDialog)
{
	m_bInitializeWithOpen = false;
	if (AfxIsFirstNew())
		return TRUE;

	Randomizer rand;

	CScrollView* pView = GetScrollView();
	CRect rect;

	ASSERT(pView);

	pView->GetClientRect(rect);

	if (CMainFrame::s_pMainFrame->IsWindowVisible() && 
		CMainFrame::s_pMainFrame->m_wndStatusBar.IsWindowVisible())
		rect.bottom -= CMainFrame::s_pMainFrame->m_wndStatusBar.CalcFixedLayout(TRUE, TRUE).cy;

	if (AfxGetPLife().IsNormal())
	{
		if (rect.right < 200)
			rect.right = 200;

		if (rect.bottom < 200)
			rect.bottom = 200;
	}

	CSettings settings;
	CHelpPropertySheet sheet("New Simulation");
	CInitialPopulation pop;
	CWorldSize         world;
	CNetworkDlg        net;

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.m_psh.dwFlags |= PSH_HASHELP;
	sheet.AddPage(&world);
	sheet.AddPage(&pop);
	sheet.AddPage(&net);

	BYTE buffer[2048];
	DWORD dwLength = 2048;

	if (AfxUserRegistry().GetBinary("NewDefaultSettings", buffer, dwLength) == TRUE &&
		dwLength > 0)
	{
		try 
		{
			CMemFile memfile(buffer, dwLength);
			CArchive archive(&memfile, CArchive::load);
			settings.Serialize(archive);
			settings.m_nWidth  = rect.right;
			settings.m_nHeight = rect.bottom;
			settings.m_nSeed   = GetTickCount();
		}
		catch(CException* pEx)
		{
			pEx->Delete();
		}
	}

	pop.Exchange(settings, false);
	net.Exchange(settings, false);
	world.Exchange(settings, false);

	if (!bShowDialog || sheet.DoModal() == IDOK)
	{
		if (!CDocument::OnNewDocument())
			return FALSE;

		pop.Exchange(settings, true);
		net.Exchange(settings, true);
		world.Exchange(settings, true);

		switch (world.m_nSizeChoice)
		{
		case 0:
			rect.bottom = GetSystemMetrics(SM_CYSCREEN);
			rect.right  = GetSystemMetrics(SM_CXSCREEN);
			break;

		case 1:
			break;

		case 3:
			rect.bottom = world.m_nHeight;
			rect.right  = world.m_nWidth;

			// Used to save size here
			break;

		// Just use the current window size
		default:
			break;
		}

		if (AfxGetPLife().IsNormal())
		{
			try 
			{
				CMemFile memfile;
				CArchive archive(&memfile, CArchive::store);
				settings.Serialize(archive);
				archive.Close();

				// Detach so we can get at the buffer directly
				DWORD  dwLength = memfile.GetLength();
				LPBYTE pBuffer  = memfile.Detach();

				AfxUserRegistry().SetBinary("NewDefaultSettings", pBuffer, dwLength);

				free(pBuffer);
			}
			catch(CException* pEx)
			{
				pEx->Delete();
				ASSERT(0);
			}
		}

		if (world.m_nSizeChoice == 1)
		{
			AfxMainFrame().ShowWindow(SW_SHOWMAXIMIZED);
			pView->GetClientRect(rect);
			m_env.OnNew(pView, rect, pop.m_nStartingPopulation, pop.m_nSeed,
						pop.m_nArmsPerBiot, pop.m_nArmTypesPerBiot, pop.m_nSegmentsPerArm);
			UpdateAllViews(NULL, 1);
			pView->Invalidate();
		}
		else
		{
			m_env.OnNew(pView, rect, pop.m_nStartingPopulation, pop.m_nSeed,
				pop.m_nArmsPerBiot, pop.m_nArmTypesPerBiot, pop.m_nSegmentsPerArm);

			if (world.m_nSizeChoice == 0)
			{
				AfxMainFrame().OnViewFullscreen();
			}
			else
			{
				if (AfxGetPLife().IsNormal())
					AfxMainFrame().OnViewBestfitwindow();
			}

			UpdateAllViews(NULL, 1);
			pView->Invalidate();
		}
		return TRUE;
	}
	return FALSE;
}




void CPLifeDoc::StartSimulation()
{
	m_env.PlayResource("PL.Start", TRUE);
}


void CPLifeDoc::StopSimulation()
{
	m_env.OnStop();
}


/////////////////////////////////////////////////////////////////////////////
// CPLifeDoc serialization

void CPLifeDoc::Serialize(CArchive& ar)
{
	// Now save/load the environment
	m_env.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CPLifeDoc diagnostics

#ifdef _DEBUG
void CPLifeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPLifeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPLifeDoc commands

BOOL CPLifeDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CPLifeDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	m_bInitializeWithOpen = true;

	if (GetScrollView()->GetSafeHwnd() != NULL)
		m_env.OnOpen(GetScrollView());

	return TRUE;
}

void CPLifeDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDocument::OnCloseDocument();
}

void CPLifeDoc::DeleteContents() 
{
	m_env.OnStop();
	m_env.DeleteContents();	
	CDocument::DeleteContents();
}

void CPLifeDoc::GetEnvironmentSize(LPRECT pRect)
{ 
	m_env.GetRECT(pRect);
}

