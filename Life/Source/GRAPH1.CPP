// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "graph1.h"

// Dispatch interfaces referenced by this interface
#include "Picture.h"

/////////////////////////////////////////////////////////////////////////////
// CGraph

IMPLEMENT_DYNCREATE(CGraph, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CGraph properties

long CGraph::GetAutoInc()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetAutoInc(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long CGraph::GetBackground()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetBackground(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

CString CGraph::GetBottomTitle()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetBottomTitle(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

long CGraph::GetColorData()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetColorData(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString CGraph::GetCtlVersion()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetCtlVersion(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

long CGraph::GetDataReset()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetDataReset(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

long CGraph::GetDrawMode()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetDrawMode(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

long CGraph::GetDrawStyle()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetDrawStyle(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}

short CGraph::GetExtraData()
{
	short result;
	GetProperty(0x9, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetExtraData(short propVal)
{
	SetProperty(0x9, VT_I2, propVal);
}

short CGraph::GetFontSize()
{
	short result;
	GetProperty(0xb, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetFontSize(short propVal)
{
	SetProperty(0xb, VT_I2, propVal);
}

long CGraph::GetFontStyle()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetFontStyle(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

long CGraph::GetFontUse()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetFontUse(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

long CGraph::GetForeground()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetForeground(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

CString CGraph::GetGraphCaption()
{
	CString result;
	GetProperty(0xf, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetGraphCaption(LPCTSTR propVal)
{
	SetProperty(0xf, VT_BSTR, propVal);
}

float CGraph::GetGraphData()
{
	float result;
	GetProperty(0x10, VT_R4, (void*)&result);
	return result;
}

void CGraph::SetGraphData(float propVal)
{
	SetProperty(0x10, VT_R4, propVal);
}

short CGraph::GetGraphStyle()
{
	short result;
	GetProperty(0x11, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetGraphStyle(short propVal)
{
	SetProperty(0x11, VT_I2, propVal);
}

CString CGraph::GetGraphTitle()
{
	CString result;
	GetProperty(0x12, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetGraphTitle(LPCTSTR propVal)
{
	SetProperty(0x12, VT_BSTR, propVal);
}

long CGraph::GetGraphType()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetGraphType(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

long CGraph::GetGridStyle()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetGridStyle(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

CString CGraph::GetImageFile()
{
	CString result;
	GetProperty(0x15, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetImageFile(LPCTSTR propVal)
{
	SetProperty(0x15, VT_BSTR, propVal);
}

long CGraph::GetIndexStyle()
{
	long result;
	GetProperty(0x16, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetIndexStyle(long propVal)
{
	SetProperty(0x16, VT_I4, propVal);
}

short CGraph::GetLabelEvery()
{
	short result;
	GetProperty(0x17, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetLabelEvery(short propVal)
{
	SetProperty(0x17, VT_I2, propVal);
}

long CGraph::GetLabels()
{
	long result;
	GetProperty(0x18, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetLabels(long propVal)
{
	SetProperty(0x18, VT_I4, propVal);
}

CString CGraph::GetLabelText()
{
	CString result;
	GetProperty(0x19, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetLabelText(LPCTSTR propVal)
{
	SetProperty(0x19, VT_BSTR, propVal);
}

CString CGraph::GetLeftTitle()
{
	CString result;
	GetProperty(0x1a, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetLeftTitle(LPCTSTR propVal)
{
	SetProperty(0x1a, VT_BSTR, propVal);
}

long CGraph::GetLegendStyle()
{
	long result;
	GetProperty(0x1b, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetLegendStyle(long propVal)
{
	SetProperty(0x1b, VT_I4, propVal);
}

CString CGraph::GetLegendText()
{
	CString result;
	GetProperty(0x1c, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetLegendText(LPCTSTR propVal)
{
	SetProperty(0x1c, VT_BSTR, propVal);
}

long CGraph::GetLineStats()
{
	long result;
	GetProperty(0x1d, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetLineStats(long propVal)
{
	SetProperty(0x1d, VT_I4, propVal);
}

short CGraph::GetNumPoints()
{
	short result;
	GetProperty(0x1e, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetNumPoints(short propVal)
{
	SetProperty(0x1e, VT_I2, propVal);
}

short CGraph::GetNumSets()
{
	short result;
	GetProperty(0x1f, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetNumSets(short propVal)
{
	SetProperty(0x1f, VT_I2, propVal);
}

long CGraph::GetPalette()
{
	long result;
	GetProperty(0x20, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetPalette(long propVal)
{
	SetProperty(0x20, VT_I4, propVal);
}

short CGraph::GetPatternData()
{
	short result;
	GetProperty(0x21, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetPatternData(short propVal)
{
	SetProperty(0x21, VT_I2, propVal);
}

long CGraph::GetPatternedLines()
{
	long result;
	GetProperty(0x22, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetPatternedLines(long propVal)
{
	SetProperty(0x22, VT_I4, propVal);
}

long CGraph::GetPrintStyle()
{
	long result;
	GetProperty(0x24, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetPrintStyle(long propVal)
{
	SetProperty(0x24, VT_I4, propVal);
}

long CGraph::GetRandomData()
{
	long result;
	GetProperty(0x26, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetRandomData(long propVal)
{
	SetProperty(0x26, VT_I4, propVal);
}

long CGraph::GetSeeThru()
{
	long result;
	GetProperty(0x27, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetSeeThru(long propVal)
{
	SetProperty(0x27, VT_I4, propVal);
}

long CGraph::GetSymbolData()
{
	long result;
	GetProperty(0x28, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetSymbolData(long propVal)
{
	SetProperty(0x28, VT_I4, propVal);
}

long CGraph::GetThickLines()
{
	long result;
	GetProperty(0x29, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetThickLines(long propVal)
{
	SetProperty(0x29, VT_I4, propVal);
}

short CGraph::GetThisPoint()
{
	short result;
	GetProperty(0x2a, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetThisPoint(short propVal)
{
	SetProperty(0x2a, VT_I2, propVal);
}

short CGraph::GetThisSet()
{
	short result;
	GetProperty(0x2b, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetThisSet(short propVal)
{
	SetProperty(0x2b, VT_I2, propVal);
}

short CGraph::GetTickEvery()
{
	short result;
	GetProperty(0x2c, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetTickEvery(short propVal)
{
	SetProperty(0x2c, VT_I2, propVal);
}

long CGraph::GetTicks()
{
	long result;
	GetProperty(0x2d, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetTicks(long propVal)
{
	SetProperty(0x2d, VT_I4, propVal);
}

float CGraph::GetXPosData()
{
	float result;
	GetProperty(0x2e, VT_R4, (void*)&result);
	return result;
}

void CGraph::SetXPosData(float propVal)
{
	SetProperty(0x2e, VT_R4, propVal);
}

float CGraph::GetYAxisMax()
{
	float result;
	GetProperty(0x2f, VT_R4, (void*)&result);
	return result;
}

void CGraph::SetYAxisMax(float propVal)
{
	SetProperty(0x2f, VT_R4, propVal);
}

float CGraph::GetYAxisMin()
{
	float result;
	GetProperty(0x30, VT_R4, (void*)&result);
	return result;
}

void CGraph::SetYAxisMin(float propVal)
{
	SetProperty(0x30, VT_R4, propVal);
}

long CGraph::GetYAxisPos()
{
	long result;
	GetProperty(0x31, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetYAxisPos(long propVal)
{
	SetProperty(0x31, VT_I4, propVal);
}

long CGraph::GetYAxisStyle()
{
	long result;
	GetProperty(0x32, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetYAxisStyle(long propVal)
{
	SetProperty(0x32, VT_I4, propVal);
}

short CGraph::GetYAxisTicks()
{
	short result;
	GetProperty(0x33, VT_I2, (void*)&result);
	return result;
}

void CGraph::SetYAxisTicks(short propVal)
{
	SetProperty(0x33, VT_I2, propVal);
}

CString CGraph::GetDataFields()
{
	CString result;
	GetProperty(0x34, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetDataFields(LPCTSTR propVal)
{
	SetProperty(0x34, VT_BSTR, propVal);
}

LPUNKNOWN CGraph::GetDataSource()
{
	LPUNKNOWN result;
	GetProperty(0x35, VT_UNKNOWN, (void*)&result);
	return result;
}

void CGraph::SetDataSource(LPUNKNOWN propVal)
{
	SetProperty(0x35, VT_UNKNOWN, propVal);
}

CString CGraph::GetFontName()
{
	CString result;
	GetProperty(0x36, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetFontName(LPCTSTR propVal)
{
	SetProperty(0x36, VT_BSTR, propVal);
}

long CGraph::GetHot()
{
	long result;
	GetProperty(0x37, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetHot(long propVal)
{
	SetProperty(0x37, VT_I4, propVal);
}

CString CGraph::GetLabelField()
{
	CString result;
	GetProperty(0x39, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetLabelField(LPCTSTR propVal)
{
	SetProperty(0x39, VT_BSTR, propVal);
}

CString CGraph::GetLegendField()
{
	CString result;
	GetProperty(0x3a, VT_BSTR, (void*)&result);
	return result;
}

void CGraph::SetLegendField(LPCTSTR propVal)
{
	SetProperty(0x3a, VT_BSTR, propVal);
}

BOOL CGraph::GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}

void CGraph::SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}

short CGraph::GetBorderStyle()
{
	short result;
	GetProperty(DISPID_BORDERSTYLE, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetBorderStyle(short propVal)
{
	SetProperty(DISPID_BORDERSTYLE, VT_I4, propVal);
}

OLE_HANDLE CGraph::GetHWnd()
{
	OLE_HANDLE result;
	GetProperty(DISPID_HWND, VT_I4, (void*)&result);
	return result;
}

void CGraph::SetHWnd(OLE_HANDLE propVal)
{
	SetProperty(DISPID_HWND, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// CGraph operations

void CGraph::Refresh()
{
	InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void CGraph::LockControl(BOOL Lock, BOOL Redraw)
{
	static BYTE parms[] =
		VTS_BOOL VTS_BOOL;
	InvokeHelper(0x4b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Lock, Redraw);
}

void CGraph::AboutBox()
{
	InvokeHelper(0xfffffdd8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
