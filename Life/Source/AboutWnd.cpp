// AboutWnd.cpp - Start-up "Splash Screen" Window
// $Revision$
//

#include "stdafx.h"

#include "resource.h"

#include "AboutWnd.h"
#include "GlobalPalette.h"
#include "LoadBitmap.h"
#include "Primordial Life.h"

#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define PALETTERGB(r,g,b)   (0x02000000 | RGB(r,g,b))

// Use this macro when you have selected a palette into the device context
// that is being rendered into.  The specified color will be matched to the
// nearest one in the palette.  This sets the same bit as PALETTERGB.
#define PALETTE_COLORREF(colorref)		(0x02000000 | (colorref))

// Use this macro when you DO NOT HAVE control of the device context that is
// being rendered into.  This will assure that if the color does not match
// one of the Windows static colors, then the color will be dithered.
#define DITHER_COLORREF(colorref)		((~0x02000000) & (colorref))

// (A subset of) the windows static colors.
static const COLORREF CLR_BLACK					= PALETTERGB(   0,   0,   0);
static const COLORREF CLR_DARK_RED				= PALETTERGB( 128,   0,   0);
static const COLORREF CLR_DARK_GREEN				= PALETTERGB(   0, 128,   0);
static const COLORREF CLR_DARK_YELLOW				= PALETTERGB( 128, 128,   0);
static const COLORREF CLR_DARK_BLUE				= PALETTERGB(   0,   0, 128);
static const COLORREF CLR_DARK_MAGENTA				= PALETTERGB( 128,   0, 128);
static const COLORREF CLR_DARK_CYAN				= PALETTERGB(   0, 128, 128);
static const COLORREF CLR_LIGHT_GRAY				= PALETTERGB( 192, 192, 192);
static const COLORREF CLR_RED					= PALETTERGB( 255,   0,   0);
static const COLORREF CLR_GREEN					= PALETTERGB(   0, 255,   0);
static const COLORREF CLR_YELLOW				= PALETTERGB( 255, 255,   0);
static const COLORREF CLR_BLUE					= PALETTERGB(   0,   0, 255);
static const COLORREF CLR_MAGENTA				= PALETTERGB( 255,   0, 255);
static const COLORREF CLR_CYAN					= PALETTERGB(   0, 255, 255);
static const COLORREF CLR_WHITE					= PALETTERGB( 255, 255, 255);


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSplashWnd static data

/*static*/ CSplashWnd*	CSplashWnd::s_pSplashWnd	= NULL;
/*static*/ BOOL			CSplashWnd::s_bIgnoreInput	= FALSE;


/////////////////////////////////////////////////////////////////////////////
// CSplashWnd static methods

/*static*/ void
CSplashWnd::ShowSplashScreen(CString sOwner, CWnd* pParentWnd /*= NULL*/)
{
	s_bIgnoreInput = FALSE;

	if (s_pSplashWnd == NULL)
	{
		// Allocate a new splash screen, and create the window.
		s_pSplashWnd = new CSplashWnd;
		if (!s_pSplashWnd->Create(sOwner, pParentWnd))
			delete s_pSplashWnd;
		else
			s_pSplashWnd->UpdateWindow();
	}
}

/*static*/ void
CSplashWnd::IgnoreInput(BOOL ignore /*= TRUE*/)
{
	// Ignore keystrokes and mouse clicks that
	// would normally close the splash screen.
	// This allows a window to be popped up on top
	// of the splash screen (like the Tip of the Day).
	// Be sure to also call HideSplashScreen()
	// or HideSplashScreenAfter() at some point.
	// This flag is reset automatically whenever
	// ShowSplashScreen() is called.

	s_bIgnoreInput = ignore;
}

/*static*/ void
CSplashWnd::HideSplashScreen()
{
	// Unconditionally hide the splash screen now
	if (s_pSplashWnd != NULL)
	{
		s_pSplashWnd->Hide();
	}
}

/*static*/ void
CSplashWnd::HideSplashScreenAfter(UINT milliseconds /* = 0*/)
{
	// Set a timer to destroy the splash screen.
	if (s_pSplashWnd != NULL)
	{
		if (milliseconds == 0)
		{
			milliseconds = AfxGetApp()->GetProfileInt("Splash", "HideAfter", 1000);
		}
		s_pSplashWnd->SetTimer(1, milliseconds, NULL);
	}
}

/*static*/ CWnd*
CSplashWnd::GetSplashWnd()
{
	return s_pSplashWnd;
}

/*BOOL
CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	// This is to be called by the CWinApp object's
	// PreTranslateMessage() method to intercept
	// key presses and mouse clicks to close the
	// splash-screen window.
	// The application's message pump is hooked in this
	// manner, rather than just this window's, so that
	// a click on the main window, as it appears behind
	// the splash screen, will accomplish the same thing.

	if (s_pSplashWnd == NULL || s_bIgnoreInput)
		return FALSE;	// message not handled

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		s_pSplashWnd->HideSplashScreen();
		return TRUE;	// message handled here
	}

	return FALSE;	// message not handled
} */


/////////////////////////////////////////////////////////////////////////////
// CSplashWnd non-static methods

CSplashWnd::CSplashWnd()
{
}


CSplashWnd::~CSplashWnd()
{
	// Clear the static window pointer.
	ASSERT(s_pSplashWnd == this);
	s_pSplashWnd = NULL;
}

BOOL
CSplashWnd::Create(CString sOwner, CWnd* pParentWnd /*= NULL*/)
{
	BOOL success;
	CString font_name;
	static int message = 1;

	m_text[5] = sOwner;
				  
	// Get applicable registry settings
	font_name			= "Arial";
	m_text_color		= PALETTE_COLORREF(CLR_BLACK);
	m_background_color	= PALETTE_COLORREF(CLR_WHITE);

	// Set up the device context for loading the bitmap.
	// Don't use the application palette if not running in a
	// paletted video mode so that BoundsChecker won't complain.
	// Note that we don't need to realize the palette
	// since we won't actually be doing any drawing.
	CClientDC dc(pParentWnd);
	CPalette* old_palette_ptr;
	CPalette* app_palette_ptr = CAppPalette::GetAppPalette();
	if (app_palette_ptr != NULL)
	{
		old_palette_ptr = dc.SelectPalette(app_palette_ptr, TRUE);
	}

	// Load the bitmap for the splash screen, rendering
	// it with the palette in the device context, if any.
	// If the resources specify a file name, try to load it.
	// As a fallback, use the IDB_SPLASH resource.
	HBITMAP bitmap_handle = ::LoadBmpResource(&dc, MAKEINTRESOURCE(IDB_SPLASH));

	// Attach this bitmap to our CBitmap object.
	// This call fails if bitmap_handle is NULL.
	// Note that the CBitmap destructor will destroy the bitmap.
	success = m_bitmap.Attach(bitmap_handle);

	if (success)
	{
		// TODO: Get this text from the VERSIONINFO resource.
		m_text[0] =
			"Copyright © 1996-98 Jason Spofford. "
			"All rights reserved.";

//		m_text2 = "This world is registered to:\r\nJason Joseph Spofford Twofordorwe\r\nSite License";
		m_text[1] = "Thanks for trying Primordial Life!";
		m_text[2] = "Get the most out of 'Life'\r\nby registering.";
		m_text[3] = "Please adopt this world by\r\nregistering today.";
		m_text[4] = "Please register so I might evolve...";

		if (m_text[5].IsEmpty())
		{
			m_text[5] = m_text[message++];

			if (message == 2)
				m_text[5] += _T("\r\n(Unregistered Copy)");

			if (message > 4)
				message = 1;
		}

		BITMAP bm;
		m_bitmap.GetBitmap(&bm);

		m_text_rect2.SetRect(187, 57, bm.bmWidth - 4, bm.bmHeight - 6);
		m_text_rect.SetRect(164, bm.bmHeight - 6, bm.bmWidth - 4, bm.bmHeight - 6);

		// Use DrawText to calculate the rectangle needed for the text,
		// but don't actually draw any text right now.
		// It will adjust rect so that it bounds the text to be drawn.
		// It uses the width of the rectangle passed to it as a guideline
		// for breaking the text into multiple lines.
		//
		// The height calculation converts font_size from
		// points to a character height in pixels.
		// See the documentation on CDC::CreateFont() for more details.
		VERIFY( m_font.CreateFont(
					- ::MulDiv(9, dc.GetDeviceCaps(LOGPIXELSY), 72), // int nHeight
					0,				// int nWidth
					0,				// int nEscapement
					0,				// int nOrientation
					0,				// int nWeight
					0,				// BYTE bItalic
					0,				// BYTE bUnderline
					0,				// BYTE cStrikeOut
					0,				// BYTE nCharSet
					0,				// BYTE nOutPrecision
					0,				// BYTE nClipPrecision
					0,				// BYTE nQuality
					0,				// BYTE nPitchAndFamily
					font_name ) );	// LPCTSTR lpszFacename
		CFont* old_font = dc.SelectObject(&m_font);
		CRect measure_rect = m_text_rect;
		dc.DrawText(
				m_text[0],
				&measure_rect,//DT_CENTER
				DT_CALCRECT | DT_TOP | DT_LEFT | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP );
		dc.SelectObject(old_font);

		// Set the height of the text and background rectangles
		// based on the measurements we just took.
		m_text_rect.top = m_text_rect.bottom - measure_rect.Height();

		VERIFY( m_font2.CreateFont(
					- ::MulDiv(13, dc.GetDeviceCaps(LOGPIXELSY), 72), // int nHeight
					0,				// int nWidth
					0,				// int nEscapement
					0,				// int nOrientation
					FW_BOLD
,				// int nWeight
					0,				// BYTE bItalic
					0,				// BYTE bUnderline
					0,				// BYTE cStrikeOut
					0,				// BYTE nCharSet
					0,				// BYTE nOutPrecision
					0,				// BYTE nClipPrecision
					0,				// BYTE nQuality
					0,				// BYTE nPitchAndFamily
					font_name ) );	// LPCTSTR lpszFacename
		old_font = dc.SelectObject(&m_font2);
		measure_rect = m_text_rect2;
		dc.DrawText(
				m_text[5],
				&measure_rect,//DT_CENTER
				DT_CALCRECT | DT_TOP | DT_LEFT | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP );
		dc.SelectObject(old_font);

		// Set the height of the text and background rectangles
		// based on the measurements we just took.
		
		// Create a window large enough to hold the bitmap and the text beneath.
		success = CreateEx(
			WS_EX_DLGMODALFRAME,		// dwExStyle
			AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),	// lpszClassName
			NULL,						// lpszWindowName
			WS_POPUP | WS_VISIBLE,		// dwStyle
			0, 0, bm.bmWidth, bm.bmHeight,
			pParentWnd->GetSafeHwnd(),	// hwndParent
			NULL );						// nIDorHMenu
	}

	// Select the application palette out of the device context
	if (app_palette_ptr != NULL)
	{
		dc.SelectPalette(old_palette_ptr, TRUE);
	}

	return success;
}


void CSplashWnd::Hide()
{
	// Destroy the window, and update the main frame.
	DestroyWindow();
	CWnd* main_window_ptr = AfxGetMainWnd();
	if (main_window_ptr != NULL)
	{
		main_window_ptr->UpdateWindow();
	}
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow(CWnd::FromHandle(lpCreateStruct->hwndParent));

	return 0;
}


void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	// Select the color palette in the foreground.
	CPalette* old_palette_ptr;
	CPalette* app_palette_ptr = CAppPalette::GetAppPalette();
	if (app_palette_ptr != NULL)
	{
		old_palette_ptr = dc.SelectPalette(app_palette_ptr, TRUE);
		dc.RealizePalette();
	}

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);

	// Draw the background rectangle and the text on top of it.
	dc.SetTextColor(m_text_color);
	dc.SetBkColor(m_background_color);
	CFont* old_font = dc.SelectObject(&m_font);
	dc.DrawText(
			m_text[0],
			&m_text_rect,
			DT_TOP | DT_LEFT | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP );
	dc.SelectObject(old_font);

	old_font = dc.SelectObject(&m_font2);
	dc.DrawText(
			m_text[5],
			&m_text_rect2,
			DT_TOP | DT_LEFT | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP );
	dc.SelectObject(old_font);

	// Select the application palette out of the device context
	if (app_palette_ptr != NULL)
	{
		dc.SelectPalette(old_palette_ptr, TRUE);
	}
}


void CSplashWnd::OnCancelMode() 
{
	CWnd::OnCancelMode();
	
	// A dialog or message box is popping up.
	// This message tells us that we need to kill our timer.
	KillTimer(1);
}


void CSplashWnd::OnTimer(UINT /*nIDEvent*/)
{
	// Destroy the splash screen window.
	Hide();
}


void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}
