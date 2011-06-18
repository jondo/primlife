// CAppPalette

#include "stdafx.h"
#include "GlobalPalette.h"
#include "Palette.h"
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CAppPalette Static Data

/*static*/ LPCTSTR   CAppPalette::s_default_palette_res = MAKEINTRESOURCE(IDR_EXPLORER_PALETTE);
/*static*/ CPalette  CAppPalette::s_default_palette;

#define UNINITIALIZED ((CPalette*)1)
/*static*/ CPalette* CAppPalette::s_cur_palette_ptr = UNINITIALIZED;


/////////////////////////////////////////////////////////////////////////////
// CAppPalette Static Methods

/*static*/ CPalette*
CAppPalette::SetAppPalette(CPalette* palette_ptr)
{
	// Change the current palette for the application
	// and return a pointer to the previous palette.
	// Realize the palette in the foreground to cause
	// the screen to repaint correctly.
	// The *caller* is responsible for making sure that the
	// CPalette object gets destroyed.
	//
	// Throughout the application, all calls to CDC::SelectPalette()
	// in painting code should select palettes in the background.

	CPalette* old_palette_ptr = s_cur_palette_ptr;

	if (palette_ptr != NULL)
	{
		s_cur_palette_ptr = palette_ptr;
	}
	else
	{
		// The next time GetAppPalette() is called,
		// create or use the default palette as appropriate.
		s_cur_palette_ptr = UNINITIALIZED;
	}

	// Realize the palette (if the main window has been created).
	//
	// CMainFrame::OnQueryNewPalette() doesn't call SetAppPalette(),
	// so we don't need a 'do_realize' parameter like GetAppPalette().
	//
	CWnd* main_wnd_ptr = AfxGetMainWnd();
	if (main_wnd_ptr != NULL)
	{
		main_wnd_ptr->SendMessage(WM_QUERYNEWPALETTE);
	}

	return old_palette_ptr;
}

/*static*/ CPalette*
CAppPalette::GetAppPalette(BOOL do_realize /*= TRUE*/)
{
	if (s_cur_palette_ptr == UNINITIALIZED)
	{
		if (s_default_palette.GetSafeHandle() != NULL)
		{
			// The default palette has already been created.
			s_cur_palette_ptr = &s_default_palette;
		}
		else
		{
			// If the display device supports palettes,
			// create the default palette.
			// If not, or if creating the default palette
			// fails, set s_cur_palette_ptr to NULL.
			s_cur_palette_ptr = NULL;

			CDC ic;
			ic.CreateIC("DISPLAY", NULL, NULL, NULL);

			if (ic.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)	// Bit-wise AND
			{
				// Attempt to load the default palette from a resource.
				HPALETTE palette_handle = ::LoadPalette(s_default_palette_res);
				if (palette_handle != NULL)
				{
					s_default_palette.Attach(palette_handle);
					s_cur_palette_ptr = &s_default_palette;

					// Realize the palette (if the main window has been created).
					// CMainFrame::OnQueryNewPalette()'s call to GetAppPalette()
					// is the only one that passes us FALSE for do_realize.
					if (do_realize)
					{
						CWnd* main_wnd_ptr = AfxGetMainWnd();
						if (main_wnd_ptr != NULL)
						{
							main_wnd_ptr->SendMessage(WM_QUERYNEWPALETTE);
						}
					}
				}
			}
		}
	}

	return s_cur_palette_ptr;
}

/*static*/ void
CAppPalette::SetDefaultPalette(LPCTSTR resource_name)
{
	// Specify the resource to use for the default palette.
	//
	// WARNING: The current default palette must not be selected
	// into any device contexts when SetDefaultPalette() is called.
	//
	// Follow with SetAppPalette(NULL); to load and use
	// the new default palette.

	ASSERT(resource_name != 0);

	s_default_palette.DeleteObject();
	s_default_palette_res = resource_name;
}

