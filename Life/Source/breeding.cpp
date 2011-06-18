// Breeding.cpp : implementation file
//

#include "stdafx.h"
#include "Primordial Life.h"
#include "Environ.h"
#include "Biots.h"
#include "Breeding.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int g_nIcons[] = {IDI_ASEXUAL, IDI_MALE, IDI_ASEXUAL_FEMALE, IDI_FEMALE};

/////////////////////////////////////////////////////////////////////////////
// CBreedView

IMPLEMENT_DYNCREATE(CBreedView, CFormView)

CBreedView::CBreedView()
	: CFormView(CBreedView::IDD)
{
	//{{AFX_DATA_INIT(CBreedView)
	m_sParents = _T("");
	//}}AFX_DATA_INIT
	m_nIcon = 0;

//	m_energySliderPos = -1;
//	m_ageSliderPos    = -1;
	m_pBiot   = NULL;
}

CBreedView::~CBreedView()
{
}

void CBreedView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBreedView)
	DDX_Control(pDX, IDC_FERTILE, m_cFertileText);
	DDX_Control(pDX, IDC_WORLD_NAME, m_cWorldName);
	DDX_Control(pDX, IDC_LIFESPAN, m_cLifeSpan);
	DDX_Control(pDX, IDC_AGE, m_cAge);
	DDX_Control(pDX, IDC_SPECIES, m_cSpeciesList);
	DDX_Control(pDX, IDC_APPLY, m_cSexIcon);
	DDX_Control(pDX, IDC_NAME, m_cName);
	DDX_Control(pDX, IDC_GENERATION, m_cGeneration);
	DDX_Control(pDX, IDC_CHILDREN, m_cChildren);
	//}}AFX_DATA_MAP

	m_cName.SetLimitText(12);
//	m_cSpeciesSpin.SetRange(0, 15);


//	m_energySlider.SetRange(0, 100);
//	m_energySlider.SetTicFreq(5);
//	m_ageSlider.SetRange(0, 100);
//	m_ageSlider.SetTicFreq(5);
}


CMagnifyWnd* CBreedView::MagnifyWnd()
{
	return static_cast<CMagnifyWnd*> (GetParentOwner());
}

void CBreedView::UpdateBiot(Biot* pBiot, DWORD dwBiotID)
{
	CString sTemp;

	if (!m_hWnd)
		return;

	// Read of any edit settings
/*	if (pBiot   != m_pBiot &&
		m_pBiot != NULL)
	{
		Biot* pTempBiot = MagnifyWnd()->FindBiotByID(m_dwBiotID);

		if (pTempBiot)
		{
			m_cName.GetWindowText(sTemp);
			pTempBiot->SetFirstName(sTemp);

			pTempBiot->trait.SetSpecies(m_cSpeciesList.GetCurSel());

//			m_cMale.GetWindowText(sTemp);
//			int nPercent = atoi(sTemp);

//			if (nPercent >= 0 &&
//				nPercent <= 100)
//				pTempBiot->trait.SetChanceMale((BYTE) (((nPercent + 50) * 255) / 100));
//			if (nSpecies >= 0 &&
//				nSpecies <= 15)
//				pTempBiot->trait.SetSpecies(nSpecies);
		}
	}*/

	if (!pBiot)
	{
		m_pBiot = NULL;
		PostMessage(WM_COMMAND, IDCLOSE, 0);
		return;
	}

	// Set everything if biot changes
	if (pBiot != m_pBiot)
	{
		// Set Sex Icon
		if (pBiot->trait.IsMale())
		{
			if (pBiot->trait.IsAsexual())
				m_nIcon = 0;
			else
				m_nIcon = 1;
		}
		else
		{
			if (pBiot->trait.IsAsexual())
				m_nIcon = 2;
			else
				m_nIcon = 3;
		}
		m_cSexIcon.SetIcon(AfxGetApp()->LoadIcon(g_nIcons[m_nIcon]));

		// Set biot name
		m_cName.SetWindowText(pBiot->GetName());

		m_cSpeciesList.SetCurSel(pBiot->trait.GetSpecies());

		sTemp.Format("%lu", pBiot->m_generation);
		m_cGeneration.SetWindowText(sTemp);

		m_cAge.SetWindowText(CEnvStats::ToDays(pBiot->m_age));
		m_cLifeSpan.SetWindowText(CEnvStats::ToDays(pBiot->m_maxAge));

//		sTemp.Format("%ld", (((pBiot->trait.GetChanceMale() + 50) * 100) / 255));
//		m_cMale.SetWindowText(sTemp);
	}

	if (pBiot)
	{
		sTemp.Format("%ld", pBiot->m_totalChildren);
		m_cChildren.SetWindowText(sTemp);

		m_cWorldName.SetWindowText(pBiot->GetWorldName());

		m_cFertileText.SetWindowText(pBiot->GetFatherName());
	}

/*

		
		
		if (pBiot->genes2 > 0)
		m_

	DDX_Control(pDX, IDC_SPECIES, m_cSpecies);
	DDX_Control(pDX, IDC_NAME, m_cName);
	DDX_Control(pDX, IDC_GENERATION, m_cGeneration);
	DDX_Control(pDX, IDC_CHILDREN, m_cChildren);
	DDX_Control(pDX, IDC_SPECIES_SPIN, m_cSpeciesSpin);
	DDX_Control(pDX, IDC_SEX, m_sexIcon);
	DDX_Text(pDX, IDC_WORLD_NAME, m_sWorldName);
	DDX_Text(pDX, IDC_FERTILE, m_sFertile);
	DDX_Text(pDX, IDC_PARENTS, m_sParents);

		m_totalDistance.Format("Total Distance %lu", pBiot->totalDistance);
		m_benefit.Format("Total Benefit %lu, Total Children %d", pBiot->turnBenefit, pBiot->m_totalChildren);
		m_ratio.Format("Bonus Ratio %6.3f", pBiot->m_dBonusRatio);
		if (pBiot->genes2 > 0)
			m_ratio += " Fertilized";

		long energy = (long) ((pBiot->turnBenefit - pBiot->totalDistance) + (pBiot->m_dBonusRatio * pBiot->turnBenefit));
		m_Energy.Format("%ld", energy);

		if (energy <= 0)
			m_Energy += "  predator";
		else
			m_Energy += "  prey";

		m_line1.Format("%d", pBiot->trait.GetSpecies());

		double age = (double) pBiot->m_age / (1280.0);
		DWORD span = pBiot->m_maxAge / 1280;
		m_line2.Format("%4.1f days (lifespan of %d days)", age, span);

		CString sTitle;

		sTitle.Format("Biot %d", pBiot->m_Id);
		SetWindowText(sTitle);

		int nPos = m_energySlider.GetPos();
		if (m_energySliderPos != nPos && m_energySliderPos != -1)
		{
			pBiot->energy = (nPos * (pBiot->adultBaseEnergy << 1) - 50) / 100;
			m_energySliderPos = nPos;
		}
		else
		{
			m_energySliderPos = ((pBiot->energy * 100) + 50) / (pBiot->adultBaseEnergy << 1);
			m_energySlider.SetPos(m_energySliderPos);
		}

		nPos = m_ageSlider.GetPos();
		if (m_ageSliderPos != nPos && m_ageSliderPos != -1)
		{
			pBiot->m_age = (nPos * pBiot->m_maxAge) / 100;
			m_ageSliderPos = nPos;
		}
		else
		{
			m_ageSliderPos = (pBiot->m_age * 100) / pBiot->m_maxAge;
			m_ageSlider.SetPos(m_ageSliderPos);
		}

		
		if (pBiot != m_pBiot)
		{
			if (pBiot->trait.IsMale())
			{
				if (pBiot->trait.IsAsexual())
					m_sexIcon.SetIcon(AfxGetApp()->LoadIcon(IDI_ASEXUAL));
				else
					m_sexIcon.SetIcon(AfxGetApp()->LoadIcon(IDI_MALE));
			}
			else
			{
				if (pBiot->trait.IsAsexual())
					m_sexIcon.SetIcon(AfxGetApp()->LoadIcon(IDI_ASEXUAL_FEMALE));
				else
					m_sexIcon.SetIcon(AfxGetApp()->LoadIcon(IDI_FEMALE));
			}
			m_pBiot = pBiot;
		}

*/
	m_pBiot    = pBiot;
	m_dwBiotID = dwBiotID;

	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(CBreedView, CFormView)
	//{{AFX_MSG_MAP(CBreedView)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBreedView diagnostics

#ifdef _DEBUG
void CBreedView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBreedView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBreedView message handlers

void CBreedView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	UpdateData(FALSE);
}

void CBreedView::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
//	CFormView::PostNcDestroy();
}

void CBreedView::OnApply() 
{
	m_nIcon++;
	if (m_nIcon > 3)
		m_nIcon = 0;

	m_cSexIcon.SetIcon(AfxGetApp()->LoadIcon(g_nIcons[m_nIcon]));
}

void CBreedView::OnSave() 
{

	//!! NOTE WE DO NOT KNOW IF THIS POINTER IS STILL GOOD!
	if (m_pBiot)
	{
		CString sTemp;
		m_cName.GetWindowText(sTemp);
		m_pBiot->SetName(sTemp);


		m_cGeneration.GetWindowText(sTemp);
		m_pBiot->m_generation = atol(sTemp);

		m_cAge.GetWindowText(sTemp);
		m_pBiot->m_age = CEnvStats::ToGenerations(sTemp);
		m_cLifeSpan.GetWindowText(sTemp);
		m_pBiot->m_maxAge = CEnvStats::ToGenerations(sTemp);

		m_pBiot->trait.SetSpecies(m_cSpeciesList.GetCurSel());
		m_pBiot->trait.SetMaxAge((short)((m_pBiot->m_maxAge + 640) /  1280) - 1);
		m_pBiot->trait.SetAsexual((m_nIcon == 0 || m_nIcon == 2));
 		m_pBiot->trait.SetMale((m_nIcon == 0 || m_nIcon == 1));
	}
}
