// WorldSize.cpp : implementation file
//

#include "stdafx.h"
#include "primordial life.h"
#include "HelpPropertySheet.h"
#include "WorldSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorldSize property page

IMPLEMENT_DYNCREATE(CWorldSize, CPropertyPage)

CWorldSize::CWorldSize() : CPropertyPage(CWorldSize::IDD)
{
	//{{AFX_DATA_INIT(CWorldSize)
	//}}AFX_DATA_INIT
	m_psp.dwFlags |= PSP_HASHELP;
}

CWorldSize::~CWorldSize()
{
}

void CWorldSize::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorldSize)
	DDX_Control(pDX, IDC_WIDTH, m_widthCtrl);
	DDX_Control(pDX, IDC_HEIGHT, m_heightCtrl);
	DDX_Control(pDX, IDC_ENTIRE_SCREEN, m_sizeCtrl);
	DDX_Control(pDX, IDC_WIDTH_TEXT, m_widthText);
	DDX_Control(pDX, IDC_HEIGHT_TEXT, m_heightText);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 200, 4000);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 200, 4000);
	DDX_Radio(pDX, IDC_ENTIRE_SCREEN, m_nSizeChoice);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorldSize, CPropertyPage)
	//{{AFX_MSG_MAP(CWorldSize)
	ON_BN_CLICKED(IDC_ENTIRE_SCREEN, OnEntireScreen)
	ON_BN_CLICKED(IDC_ENTIRE_WINDOW, OnEntireWindow)
	ON_BN_CLICKED(IDC_CUSTOM, OnCustom)
	ON_BN_CLICKED(IDC_FIT_WINDOW, OnFitWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorldSize message handlers
BOOL CWorldSize::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!AfxGetPLife().IsNormal())
		m_nSizeChoice = 2;

	EnableControls(m_nSizeChoice == 3);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CWorldSize::EnableControls(BOOL bEnable)
{
	m_widthText.EnableWindow(bEnable);
	m_heightText.EnableWindow(bEnable);
	m_heightText.EnableWindow(bEnable);
	m_widthCtrl.EnableWindow(bEnable);
	m_heightCtrl.EnableWindow(bEnable);
}

void CWorldSize::Exchange(CSettings& s, bool bSave)
{
	if (bSave)
	{
		s.m_nWidth      = m_nWidth;     
		s.m_nHeight     = m_nHeight;    
		s.m_nSizeChoice = m_nSizeChoice;
	}
	else
	{
		m_nWidth      = s.m_nWidth;     
		m_nHeight     = s.m_nHeight;    
		m_nSizeChoice = s.m_nSizeChoice;
	}
}


void CWorldSize::OnEntireScreen() 
{
	EnableControls(FALSE);
}

void CWorldSize::OnEntireWindow() 
{
	EnableControls(FALSE);
}

void CWorldSize::OnCustom() 
{
	EnableControls(TRUE);
}

void CWorldSize::OnFitWindow() 
{
	EnableControls(FALSE);
}

BOOL CWorldSize::OnSetActive() 
{
	CHelpPropertySheet* pSheet = (CHelpPropertySheet*) GetParent();	

	if (pSheet)
	{
		pSheet->SetHelpButtonContextId(_T("Habitat.htm"));
	}
	return CPropertyPage::OnSetActive();
}
