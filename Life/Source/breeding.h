/////////////////////////////////////////////////////////////////////////////
// CBreedView form view
//
//
//
#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "Resource.h"

class CMagnifyWnd;

class CBreedView : public CFormView
{
public:
	CBreedView();      
	DECLARE_DYNCREATE(CBreedView)

// Form Data
public:
	//{{AFX_DATA(CBreedView)
	enum { IDD = IDD_BREEDING };
	CStatic	m_cFertileText;
	CStatic	m_cWorldName;
	CEdit	m_cLifeSpan;
	CEdit	m_cAge;
	CComboBox	m_cSpeciesList;
	CButton	m_cSexIcon;
	CEdit	m_cName;
	CStatic	m_cGeneration;
	CStatic	m_cChildren;
	CString	m_sParents;
	//}}AFX_DATA

//	int m_energySliderPos;
//	int m_ageSliderPos;
	Biot* m_pBiot;
	DWORD m_dwBiotID;
	int   m_nIcon;

	CMagnifyWnd* MagnifyWnd();

	void UpdateBiot(Biot* pBiot, DWORD dwBiotID);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBreedView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
	public:
	virtual ~CBreedView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBreedView)
	afx_msg void OnApply();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

