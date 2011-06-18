// UtilPalette.cpp: Utility functions for managing color palettes
// $Revision$
//
// The CreateIdentityPalette() and ClearSystemPalette() functions are
// based on sample code in the Microsoft WinG Programmer's Reference.
//

//*** KEEP THE FOLLOWING 2 LINES TOGETHER WITH stdafx.h FIRST ***//
#include "stdafx.h"

#include "Palette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// TODO: Consider making these members of a class derived from CPalette

// Create an identity palette from a bitmap's
// array of RGBQUAD structures.
//
HPALETTE
CreateIdentityPalette(RGBQUAD rgb_quads[], int color_count)
{
	int i;

	// This is effectively a LOGPALETTE structure,
	// but we can't use LOGPALETTE directly because
	// it defines an array with only [1] PALETTEENTRY.
	struct {
		WORD m_version;
		WORD m_entry_count;
		PALETTEENTRY m_entries[256];
	} palette =
	{
		0x300,
		256
	};

	// Just use the screen DC where we need it
	HDC hdc = GetDC(NULL);

	if (GetSystemPaletteUse(hdc) == SYSPAL_NOSTATIC)
	{
		// For SYSPAL_NOSTATIC, just copy the color table into
		// a PALETTEENTRY array and replace the first and last entries
		// with black and white

		// Fill in the palette with the given values, marking each
		// as PC_NOCOLLAPSE 
		for (i = 0; i < color_count; i++)
		{
			palette.m_entries[i].peRed   = rgb_quads[i].rgbRed;
			palette.m_entries[i].peGreen = rgb_quads[i].rgbGreen;
			palette.m_entries[i].peBlue  = rgb_quads[i].rgbBlue;
			palette.m_entries[i].peFlags = PC_NOCOLLAPSE;
		}

		// Mark any unused entries PC_NOCOLLAPSE 
		for ( ; i < 256; i++)
		{
			palette.m_entries[i].peFlags = PC_NOCOLLAPSE;
		}

		// Make sure the last entry is white
		// This may replace an entry in the array!
		palette.m_entries[255].peRed   = 255;
		palette.m_entries[255].peGreen = 255;
		palette.m_entries[255].peBlue  = 255;
		palette.m_entries[255].peFlags = 0;

		// And the first is black
		// This may replace an entry in the array!
		palette.m_entries[0].peRed   = 0;
		palette.m_entries[0].peGreen = 0;
		palette.m_entries[0].peBlue  = 0;
		palette.m_entries[0].peFlags = 0;
	}
	else
	{
		// For SYSPAL_STATIC, get the twenty static colors into
		// the array, then fill in the empty spaces with the
		// given color table

		// Get the static colors from the system palette.
		// static_count is the number of static colors at each
		// end of the palette.
		int static_count = GetDeviceCaps(hdc, NUMRESERVED) / 2;
		GetSystemPaletteEntries(hdc, 0, 256, palette.m_entries);

		// Set the peFlags of the lower static colors to zero
		for (i = 0; i < static_count; i++)
		{
			palette.m_entries[i].peFlags = 0;
		}

		// Fill in the entries from the given color table
		int usable_count = color_count - static_count;
		for ( ; i < usable_count; i++)
		{
			palette.m_entries[i].peRed   = rgb_quads[i].rgbRed;
			palette.m_entries[i].peGreen = rgb_quads[i].rgbGreen;
			palette.m_entries[i].peBlue  = rgb_quads[i].rgbBlue;
			palette.m_entries[i].peFlags = PC_NOCOLLAPSE;
		}

		// Mark any empty entries as PC_NOCOLLAPSE 
		for ( ; i < (256 - static_count); i++)
		{
			palette.m_entries[i].peFlags = PC_NOCOLLAPSE;
		}

		// Set the peFlags of the upper static colors to zero
		for (i = (256 - static_count); i < 256; i++)
		{
			palette.m_entries[i].peFlags = 0;
		}
	}

	// Remember to release the DC!
	ReleaseDC(NULL, hdc);

	// Return the palette
	return CreatePalette((LOGPALETTE *)&palette);
}


// Before you realize an identity palette for the first time,
// it is a good idea to clear the system palette by realizing
// a completely black palette using ClearSystemPalette().
// This will ensure that palette-managed applications executed
// before your application will not affect the identity mapping
// of your carefully constructed palette.
//
void
ClearSystemPalette()
{
	// This is effectively a LOGPALETTE structure,
	// but we can't use LOGPALETTE directly because
	// it defines an array with only [1] PALETTEENTRY.
	struct
	{
		WORD m_version;
		WORD m_entry_count;
		PALETTEENTRY m_entries[256];
	} palette =
	{
		0x300,
		256
	};

	// Reset everything in the system palette to black
	for (int i = 0; i < 256; i++)
	{
		palette.m_entries[i].peRed   = 0;
		palette.m_entries[i].peGreen = 0;
		palette.m_entries[i].peBlue  = 0;
		palette.m_entries[i].peFlags = PC_NOCOLLAPSE;
	}

	// Create, select, realize, deselect, and delete the palette
	HDC screen_dc = GetDC(NULL);
	HPALETTE screen_palette = CreatePalette((LOGPALETTE *)&palette);
	if (screen_palette != 0)
	{
		screen_palette = SelectPalette(screen_dc, screen_palette, FALSE);
		RealizePalette(screen_dc);
		screen_palette = SelectPalette(screen_dc, screen_palette, FALSE);
		DeleteObject(screen_palette);
	}
	ReleaseDC(NULL, screen_dc);
}


// Load a palette from a COLOR_PALETTE resource.
// This is a user-defined resource created by saving the
// palette of a bitmap to a .PAL file in Microsoft's
// binary RIFF format.  You can do this using programs
// like PaintShop Pro.  See notes in the code about the
// browser_foreground and dc_ptr parameters.
//
HPALETTE
LoadPalette(LPCTSTR resource_name, BOOL browser_foreground /*= FALSE*/, CDC* dc_ptr /*= NULL*/)
{
	ASSERT(resource_name != NULL);

	HPALETTE palette_handle = NULL;
	struct SPaletteFile
	{
		DWORD		riff_tag;		// "RIFF"
		long		riff_length;	// length of RIFF file
		DWORD		pal_tag;		// "PAL " (with a trailing space)
		DWORD		data_tag;		// "data"
		long		pal_length;		// length of palette structure
		LOGPALETTE	log_palette;
	};

	// AfxFindResourceHandle() searches the .EXE and all
	// CDynLinkLibrary (.DLL) objects for the resource.
	// It returns an HINSTANCE, which is apparently really
	// the same thing as an HMODULE.
	HMODULE module = static_cast<HMODULE>(
		AfxFindResourceHandle(resource_name, RT_COLOR_PALETTE) );
	if (module != NULL)
	{

	// Find the specified resource within this module.
	HRSRC res = ::FindResource(module, resource_name, RT_COLOR_PALETTE);
	if (res != NULL)
	{

	HGLOBAL global = ::LoadResource(module, res);
	if (global != NULL)
	{

		SPaletteFile* palette_ptr = static_cast<SPaletteFile*>(::LockResource(global));
		if (palette_ptr != NULL)
		{
			ASSERT(palette_ptr->riff_tag == * /*static_cast*/(DWORD*)("RIFF"));
			ASSERT(palette_ptr->pal_tag  == * /*static_cast*/(DWORD*)("PAL "));
			ASSERT(palette_ptr->data_tag == * /*static_cast*/(DWORD*)("data"));

			if (browser_foreground)
			{
				// This palette will be realized in the foreground,
				// and the application may be run at the same time as
				// another application that uses a similar color palette.
				// We want to minimize color palette flash when the user
				// switches between the two applications.
				//
				// This feature has been designed with Netscape's Navigator
				// and Microsoft's Internet Explorer in mind, anticipating
				// that products will run in tandem with a web
				// browser such as these.  A standard set of 216 colors has
				// been defined for web browsers to use, but the actual
				// mapping to color indexes is implementation dependent.
				// Not only do these two browsers differ in the ordering
				// of the colors, but they reserve a different number of
				// color palette entries for their own use.  Windows, of
				// course, reserves indexes 0-9 and 246-255, which includes
				// 8 of the standard 216 colors.  Netscape uses indexes 10-16
				// for its own use and positions the remaining 208 colors
				// at 17-224, leaving 225-245 free.  Internet Explorer uses
				// 10-28 and 233-245, putting the standard colors at 29-232.
				// [Note that Internet Explorer actually drops 4 of the
				// standard colors, namely 0033FF, 00FF33, 3300FF, and 33FF00,
				// substituting 0033CC, 00CC33, 3300CC, and 33CC00, respectively.]
				//
				// In order to minimize color palette flash, we must reserve
				// palette entries for each of these colors. Setting the
				// PC_NOCOLLAPSE flag prevents an entry from being mapped
				// to another entry that has already been allocated for that
				// color.  For example, if Netscape is running and it set
				// entry 10 in the system palette to a color that just happens
				// to match, say, the standard color at index 200, we still
				// want that color allocated at index 200 when we realize
				// our palette.
				//
				// For the "holes" in our palette that correspond to colors
				// used by the other application, we want to use the PC_EXPLICIT
				// flag.  This leaves the color in that entry that the other
				// application allocated the last time it was in the foreground.
				// We mark such "holes" in our palette by setting them to black
				// (0,0,0).
				//
				// If there is a final "hole" immediately before the static colors
				// that Windows reserves, these slots should be left free for other
				// applications to use.  Since these are set to black, we can just
				// leave them alone they will be collapses onto the static black
				// entry that is always located at index 0.


				// If we were passed a DC, use it; otherwise, use the screen DC
				HDC dc_handle = (dc_ptr != NULL) ? dc_ptr->m_hDC : ::GetDC(NULL);

				// Set static_count to the number of static colors
				// at each end of the system palette.
				int static_count =
					(::GetSystemPaletteUse(dc_handle) == SYSPAL_NOSTATIC)
						? 1	// black and white are at either end
						: GetDeviceCaps(dc_handle, NUMRESERVED) / 2;

				// Set i to the index just past the last non-static entry.
				int i;
				
				if (palette_ptr->log_palette.palNumEntries < (::GetDeviceCaps(dc_handle, SIZEPALETTE) - static_count ))
					i = palette_ptr->log_palette.palNumEntries;
				else
					i = (::GetDeviceCaps(dc_handle, SIZEPALETTE) - static_count );

				// Move i up to the index of the last non-static entry
				// that is not set to black.
				for ( i-- ;		// Last non-static entry
				      ( i >= static_count &&
				        palette_ptr->log_palette.palPalEntry[i].peRed   == 0 &&
				        palette_ptr->log_palette.palPalEntry[i].peGreen == 0 &&
				        palette_ptr->log_palette.palPalEntry[i].peBlue  == 0 );
					  i-- )
				{ /* empty loop body */ }

				for ( ; i >= static_count; i-- )
				{
					if ( palette_ptr->log_palette.palPalEntry[i].peRed   == 0 &&
				         palette_ptr->log_palette.palPalEntry[i].peGreen == 0 &&
				         palette_ptr->log_palette.palPalEntry[i].peBlue  == 0 )
					{
						// Black indicates an entry not used by this application.
						// Set it to the color already in the system palette.
						*((WORD *) (&palette_ptr->log_palette.palPalEntry[i].peRed)) = (WORD)i;
						palette_ptr->log_palette.palPalEntry[i].peFlags = PC_EXPLICIT | PC_NOCOLLAPSE;
					}
					else
					{
						// One of the colors we care about.
						// Make sure it gets its own, unique palette entry.
						palette_ptr->log_palette.palPalEntry[i].peFlags = PC_NOCOLLAPSE;
					}
				}


				// If we used the screen DC, release it.
				if (dc_ptr == NULL)
				{
					::ReleaseDC(NULL, dc_handle);
				}

			} // end if (browser_foreground)


			// Actually create the palette
			palette_handle = ::CreatePalette(&palette_ptr->log_palette);
		}
		::UnlockResource(global);
		::FreeResource(global);

	} // global
	} // res
	} // module

#ifdef _DEBUG
	if (palette_handle == NULL)
	{
		TRACE((HIWORD(resource_name) == 0)
				? "Error loading color palette resource %d\n"
				: "Error loading color palette resource %s\n",
			resource_name);
	}
#endif

	return palette_handle;
}