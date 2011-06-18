// PLifeDoc.h : interface of the CPLifeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef PLifeDoc_h
#define PLifeDoc_h

#include "environ.h"

class CPLifeDoc : public CDocument
{
protected: // create from serialization only
	CPLifeDoc();
	DECLARE_DYNCREATE(CPLifeDoc)

// Attributes
public:
	Environment m_env;

	int Width() { return m_env.Width(); }
	int Height() { return m_env.Height(); }

// Operations
public:
	void StartSimulation();
	void StopSimulation();

	CScrollView* GetScrollView();

	BOOL OnNew(bool bShowDialog);
	void OnInitialUpdate();
	void GetEnvironmentSize(LPRECT pRect);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLifeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPLifeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool m_bInitializeWithOpen;

// Generated message map functions
protected:
	//{{AFX_MSG(CPLifeDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////
// AfxIsFirstNew - indicates whether this is the
// first document which is always defaulted
//
BOOL AfxIsFirstNew();

/////////////////////////////////////////////////////////////////////////////
#endif