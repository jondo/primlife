#if !defined(AFX_BIOTEDITOR_H__8F57F6A0_48A7_11D1_8879_A5577DF1060A__INCLUDED_)
#define AFX_BIOTEDITOR_H__8F57F6A0_48A7_11D1_8879_A5577DF1060A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BiotEditor.h : header file
//
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CBiotEditor dialog
class Biot;

class CBiotEditor : public CDialog
{
// Construction
public:
	CBiotEditor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBiotEditor)
	enum { IDD = IDD_EDITOR };
	CStatic	m_sexIcon;
	CSliderCtrl	m_ageSlider;
	CSliderCtrl	m_energySlider;
	CString	m_benefit;
	CString	m_ratio;
	CString	m_totalDistance;
	CString	m_Energy;
	CString	m_line1;
	CString	m_line2;
	//}}AFX_DATA

	int m_energySliderPos;
	int m_ageSliderPos;
	Biot* m_pBiot;

	void UpdateBiot(Biot* pBiot);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBiotEditor)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBiotEditor)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIOTEDITOR_H__8F57F6A0_48A7_11D1_8879_A5577DF1060A__INCLUDED_)
