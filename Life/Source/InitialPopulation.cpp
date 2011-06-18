// InitialPopulation.cpp : implementation file
//

#include "stdafx.h"
#include "primordial life.h"
#include "HelpPropertySheet.h"
#include "InitialPopulation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInitialPopulation property page

IMPLEMENT_DYNCREATE(CInitialPopulation, CPropertyPage)

CInitialPopulation::CInitialPopulation() : CPropertyPage(CInitialPopulation::IDD)
{
	//{{AFX_DATA_INIT(CInitialPopulation)
	m_psp.dwFlags |= PSP_HASHELP;
	//}}AFX_DATA_INIT
}

CInitialPopulation::~CInitialPopulation()
{
}

void CInitialPopulation::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInitialPopulation)
	DDX_Control(pDX, IDC_SPIN_POPULATION, m_spinPopulation);
	DDX_Text(pDX, IDC_SEED, m_nSeed);
	DDX_Text(pDX, IDC_STARTING_POPULATION, m_nStartingPopulation);
	DDV_MinMaxInt(pDX, m_nStartingPopulation, 0, 50);
	DDX_CBIndex(pDX, IDC_TYPES_PER_BIOT, m_nArmTypesPerBiot);
	DDX_CBIndex(pDX, IDC_SEGMENTS_PER_ARM, m_nSegmentsPerArm);
	DDX_CBIndex(pDX, IDC_ARMS_PER_BIOT, m_nArmsPerBiot);
	DDX_Check(pDX, IDC_GENERATE, m_bGenerateOnExtinction);
	//}}AFX_DATA_MAP

	m_spinPopulation.SetRange(0, 50);
}


BEGIN_MESSAGE_MAP(CInitialPopulation, CPropertyPage)
	//{{AFX_MSG_MAP(CInitialPopulation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInitialPopulation message handlers

BOOL CInitialPopulation::OnSetActive() 
{
	CHelpPropertySheet* pSheet = (CHelpPropertySheet*) GetParent();	

	if (pSheet)
	{
		pSheet->SetHelpButtonContextId(_T("InitialPopulation.htm"));
	}
	return CPropertyPage::OnSetActive();
}

BOOL CInitialPopulation::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInitialPopulation::Exchange(CSettings& s, bool bSave)
{
	if (bSave)
	{
		s.m_nSeed                 = m_nSeed;
		s.m_nStartingPopulation   = m_nStartingPopulation;
		s.m_nArmTypesPerBiot      = m_nArmTypesPerBiot;
		s.m_nSegmentsPerArm       = m_nSegmentsPerArm;
		s.m_nArmsPerBiot          = m_nArmsPerBiot;
		s.m_bGenerateOnExtinction = m_bGenerateOnExtinction;
	}
	else
	{
		m_nSeed                 = s.m_nSeed;
		m_nStartingPopulation   = s.m_nStartingPopulation;
		m_nArmTypesPerBiot      = s.m_nArmTypesPerBiot;
		m_nSegmentsPerArm       = s.m_nSegmentsPerArm;
		m_nArmsPerBiot          = s.m_nArmsPerBiot;
		m_bGenerateOnExtinction = s.m_bGenerateOnExtinction;
	}
}