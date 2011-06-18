// BiotEditor.cpp : implementation file
//

#include "stdafx.h"
#include "Primordial Life.h"
#include "BiotEditor.h"
#include "Environ.h"
#include "Biots.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBiotEditor dialog


CBiotEditor::CBiotEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CBiotEditor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBiotEditor)
	m_benefit = _T("");
	m_ratio = _T("");
	m_totalDistance = _T("");
	m_Energy = _T("");
	m_line1 = _T("");
	m_line2 = _T("");
	//}}AFX_DATA_INIT

	m_energySliderPos = -1;
	m_ageSliderPos    = -1;
	m_pBiot = NULL;
}


void CBiotEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBiotEditor)
	DDX_Control(pDX, IDC_SEX, m_sexIcon);
	DDX_Control(pDX, IDC_AGE_SLIDER, m_ageSlider);
	DDX_Control(pDX, IDC_ENERGY_SLIDER, m_energySlider);
	DDX_Text(pDX, IDC_BENEFIT, m_benefit);
	DDX_Text(pDX, IDC_RATIO, m_ratio);
	DDX_Text(pDX, IDC_TOTAL_DISTANCE, m_totalDistance);
	DDX_Text(pDX, IDC_ENERGY, m_Energy);
	DDX_Text(pDX, IDC_LINE1, m_line1);
	DDX_Text(pDX, IDC_LINE2, m_line2);
	//}}AFX_DATA_MAP

	m_energySlider.SetRange(0, 100);
	m_energySlider.SetTicFreq(5);
	m_ageSlider.SetRange(0, 100);
	m_ageSlider.SetTicFreq(5);
}


BEGIN_MESSAGE_MAP(CBiotEditor, CDialog)
	//{{AFX_MSG_MAP(CBiotEditor)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CBiotEditor::UpdateBiot(Biot* pBiot)
{
	if (m_hWnd)
	{
		if (!pBiot)
		{
			PostMessage(WM_COMMAND, IDCLOSE, 0);
			return;
		}

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




		UpdateData(FALSE);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CBiotEditor message handlers

BOOL CBiotEditor::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bResult = CDialog::DestroyWindow();

	m_hWnd = NULL;
	return bResult;
}
