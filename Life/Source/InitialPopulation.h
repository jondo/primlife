#if !defined(AFX_INITIALPOPULATION_H__92E49E63_00A0_11D3_8FD9_DF2C58812444__INCLUDED_)
#define AFX_INITIALPOPULATION_H__92E49E63_00A0_11D3_8FD9_DF2C58812444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InitialPopulation.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInitialPopulation dialog
#include "Settings.h"

class CInitialPopulation : public CPropertyPage
{
	DECLARE_DYNCREATE(CInitialPopulation)

// Construction
public:
	CInitialPopulation();
	~CInitialPopulation();

// Dialog Data
	//{{AFX_DATA(CInitialPopulation)
	enum { IDD = IDD_INITIAL_POPULATION };
	CSpinButtonCtrl	m_spinPopulation;
	int  	m_nSeed;
	int		m_nStartingPopulation;
	int		m_nArmTypesPerBiot;
	int		m_nSegmentsPerArm;
	int		m_nArmsPerBiot;
	BOOL	m_bGenerateOnExtinction;
	//}}AFX_DATA

	void Exchange(CSettings& s, bool bSave);


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CInitialPopulation)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInitialPopulation)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITIALPOPULATION_H__92E49E63_00A0_11D3_8FD9_DF2C58812444__INCLUDED_)
