///////////////////////////////////////////////////////////
// CMagnifyWnd
//
//
//
#pragma once

#include "GRAPH1.H"

class Environment;
class Biot;

#include "Breeding.h"

class CWinRect : public CRect
{
public:
	void FromRegistry(LPCSTR szType, long dLeft, long dTop, long dRight, long dBottom);
	void ToRegistry(LPCSTR szType);
};


const int MAX_ENERGY_HISTORY = 21;

/////////////////////////////////////////////////////////////////////////////
// CMagnifyWnd frame

class CMagnifyWnd : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CMagnifyWnd)
protected:

	Environment* m_pEnv;
	CWnd*        m_pParentWnd;
	Biot*        m_pLastBiot;
    
	CGraph m_graph;

	CMagnifyWnd** m_pExternalRef;

	CBreedView    m_breedView;
	CToolBar      m_wndToolBar;
	int           m_toolbarHeight;

protected:
	void  RestInPeace(CDC* pDc);
	CRect GetReducedClientRect();


// Attributes
public:
	CMagnifyWnd(CMagnifyWnd** pExternalRef = NULL);
	virtual ~CMagnifyWnd();

	HBITMAP hBitmap;

	long m_colorDistance[5];

	float m_energy[MAX_ENERGY_HISTORY];
	long  m_energyIndex;

// Operations
public:
	void PaintNow(Biot* pBiot);
	BOOL SetTitle(Biot* pBiot, BOOL bChange = FALSE);
	void SetComposition(Biot* pBiot, BOOL bDraw = FALSE);
	void SetEnergy(Biot* pBiot, BOOL bDraw = FALSE);
	int   m_choice;
	void SetBreeding(Biot* pBiot);
	static BOOL CreateWnd(Environment* pEnv, CWnd* pParentWnd, CMagnifyWnd** pExternalRef);

	Biot* FindBiotByID(int nBiotId); 


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagnifyWnd)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMagnifyWnd)
	afx_msg void OnPaint();
	afx_msg void OnComposition();
	afx_msg void OnUpdateComposition(CCmdUI* pCmdUI);
	afx_msg void OnShape();
	afx_msg void OnUpdateShape(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnergy();
	afx_msg void OnUpdateEnergy(CCmdUI* pCmdUI);
	afx_msg void OnBreeding();
	afx_msg void OnUpdateBreeding(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



