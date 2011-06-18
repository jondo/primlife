// UtilPalette.h: Utility functions for managing color palettes
// $Revision$
//

#ifndef UtilPalette_h
#define UtilPalette_h


#define COLOR_PALETTE	260


//---------------------------------------------------------------------------
// Definitions for use in program code only
//
#ifndef RC_INVOKED

#define RT_COLOR_PALETTE	MAKEINTRESOURCE(COLOR_PALETTE)

HPALETTE CreateIdentityPalette(RGBQUAD rgb_quads[], int color_count);
void ClearSystemPalette();
HPALETTE LoadPalette(LPCTSTR resource_name, BOOL browser_foreground = FALSE, CDC* dc_ptr = NULL);

#endif // !RC_INVOKED


#endif
