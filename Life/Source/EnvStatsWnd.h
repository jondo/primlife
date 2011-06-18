///////////////////////////////////////////////////////////
// CEnvStatsWnd
//
//
//
#pragma once

#include "MagnifyWnd.h"

class CEnvStatsList;
class Environment;

/////////////////////////////////////////////////////////////////////////////
// CEnvStatsWnd frame
//
class CEnvStatsWnd : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CEnvStatsWnd)
protected:

	Environment* m_pEnv;
	CWnd*        m_pParentWnd;
    
	CGraph m_graph;

	CEnvStatsWnd** m_pExternalRef;

	CBreedView    m_breedView;
	CToolBar      m_wndToolBar;
	int           m_toolbarHeight;

// Attributes
public:
	int   m_choice;
	HBITMAP hBitmap;

// Operations
public:
	CEnvStatsWnd(CEnvStatsWnd** pExternalRef = NULL);
	virtual ~CEnvStatsWnd();

	void PaintNow();
	void SetTitle(bool bChange = false);
	void SetPopulation(bool bDraw = false);
	void SetEnergySpread(bool bDraw = false);
	void SetLineTypes(bool bDraw = false);
	void SetAgeSpread(bool bDraw = false);

	static BOOL CreateWnd(Environment* pEnv, CWnd* pParentWnd, CEnvStatsWnd** pExternalRef);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnvStatsWnd)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnvStatsWnd)
	afx_msg void OnPaint();
	afx_msg void OnUpdateShape(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAgeSpread();
	afx_msg void OnUpdateAgeSpread(CCmdUI* pCmdUI);
	afx_msg void OnLineTypes();
	afx_msg void OnUpdateLineTypes(CCmdUI* pCmdUI);
	afx_msg void OnPopulation();
	afx_msg void OnUpdatePopulation(CCmdUI* pCmdUI);
	afx_msg void OnEnergySpread();
	afx_msg void OnUpdateEnergySpread(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



