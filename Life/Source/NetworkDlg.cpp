// NetworkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "primordial life.h"
#include "HelpPropertySheet.h"
#include "NetworkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkDlg property page

IMPLEMENT_DYNCREATE(CNetworkDlg, CPropertyPage)

CNetworkDlg::CNetworkDlg() : CPropertyPage(CNetworkDlg::IDD)
{
	//{{AFX_DATA_INIT(CNetworkDlg)
	//}}AFX_DATA_INIT
	m_psp.dwFlags |= PSP_HASHELP;
}

CNetworkDlg::~CNetworkDlg()
{
}

void CNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkDlg)
	DDX_Control(pDX, IDC_LEFT_ENABLE, m_cSideEnable[CSettings::LEFT]);
	DDX_Control(pDX, IDC_BOTTOM_ENABLE, m_cSideEnable[CSettings::BOTTOM]);
	DDX_Control(pDX, IDC_TOP_ENABLE, m_cSideEnable[CSettings::TOP]);
	DDX_Control(pDX, IDC_RIGHT_ENABLE, m_cSideEnable[CSettings::RIGHT]);
	DDX_Control(pDX, IDC_TOP_ADDRESS, m_cSideAddress[CSettings::TOP]);
	DDX_Control(pDX, IDC_RIGHT_ADDRESS, m_cSideAddress[CSettings::RIGHT]);
	DDX_Control(pDX, IDC_LEFT_ADDRESS, m_cSideAddress[CSettings::LEFT]);
	DDX_Control(pDX, IDC_BOTTOM_ADDRESS, m_cSideAddress[CSettings::BOTTOM]);

	DDX_Text(pDX, IDC_LEFT_ADDRESS, m_sSideAddress[CSettings::LEFT]);
	DDX_Text(pDX, IDC_RIGHT_ADDRESS, m_sSideAddress[CSettings::RIGHT]);
	DDX_Text(pDX, IDC_TOP_ADDRESS, m_sSideAddress[CSettings::TOP]);
	DDX_Text(pDX, IDC_BOTTOM_ADDRESS, m_sSideAddress[CSettings::BOTTOM]);

	for (int i = 0; i < CSettings::SIDES; i++)
		DDV_MaxChars(pDX, m_sSideAddress[i], 64);

	DDX_Check(pDX, IDC_TOP_ENABLE, m_bSideEnable[CSettings::TOP]);
	DDX_Check(pDX, IDC_LEFT_ENABLE, m_bSideEnable[CSettings::LEFT]);
	DDX_Check(pDX, IDC_RIGHT_ENABLE, m_bSideEnable[CSettings::RIGHT]);
	DDX_Check(pDX, IDC_BOTTOM_ENABLE, m_bSideEnable[CSettings::BOTTOM]);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CNetworkDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CNetworkDlg)
	ON_BN_CLICKED(IDC_RIGHT_ENABLE, OnRightEnable)
	ON_BN_CLICKED(IDC_LEFT_ENABLE, OnLeftEnable)
	ON_BN_CLICKED(IDC_TOP_ENABLE, OnTopEnable)
	ON_BN_CLICKED(IDC_BOTTOM_ENABLE, OnBottomEnable)
	ON_BN_CLICKED(IDC_VIEW_SERVERS, OnViewServers)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkDlg message handlers

BOOL CNetworkDlg::OnSetActive() 
{
	CHelpPropertySheet* pSheet = (CHelpPropertySheet*) GetParent();	

	if (pSheet)
	{
		pSheet->SetHelpButtonContextId(_T("World.htm"));
	}
	
	for (int i = 0; i < CSettings::SIDES; i++)
		m_cSideAddress[i].EnableWindow(m_bSideEnable[i] == TRUE);

	return CPropertyPage::OnSetActive();
}

void CNetworkDlg::OnRightEnable() 
{
	m_cSideAddress[CSettings::RIGHT].EnableWindow(m_cSideEnable[CSettings::RIGHT].GetCheck() != 0);
}

void CNetworkDlg::OnLeftEnable() 
{
	m_cSideAddress[CSettings::LEFT].EnableWindow(m_cSideEnable[CSettings::LEFT].GetCheck() != 0);
}

void CNetworkDlg::OnTopEnable() 
{
	m_cSideAddress[CSettings::TOP].EnableWindow(m_cSideEnable[CSettings::TOP].GetCheck() != 0);
}

void CNetworkDlg::OnBottomEnable() 
{
	m_cSideAddress[CSettings::BOTTOM].EnableWindow(m_cSideEnable[CSettings::BOTTOM].GetCheck() != 0);
}

void CNetworkDlg::OnViewServers() 
{
	// TODO: Add your control notification handler code here
	
}

void CNetworkDlg::Exchange(CSettings& s, bool bSave)
{
	if (bSave)
	{
		for (int i = 0; i < CSettings::SIDES; i++)
		{
			s.m_bSideEnable[i]    = m_bSideEnable[i];
			s.m_sSideAddress[i] = m_sSideAddress[i];
		}
	}
	else
	{
		for (int i = 0; i < CSettings::SIDES; i++)
		{
			m_bSideEnable[i]    = s.m_bSideEnable[i];
			m_sSideAddress[i] = s.m_sSideAddress[i];
		}
	}
}
