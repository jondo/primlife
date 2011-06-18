 // LoadBmpFile.cpp
// $Revision$
//
// Adapted from Microsoft sample source code and from
// AfxLoadSysColorBitmap() [\MSDEV\MFC\SRC\BARTOOL.CPP]

//*** KEEP THE FOLLOWING 2 LINES TOGETHER WITH stdafx.h FIRST ***//
#include "stdafx.h"

#include "LoadBitmap.h"


/////////////////////////////////////////////////////////////////////////////
// Macros
//
// Device-Independent Bitmaps (DIBs) use the high-endian 32-bit
// RGBQUAD format (0xbb 0xgg 0xrr 0x00) for their color tables.
// These macros represent R,G,B and RGB colors as RGBQUAD DWORDs.

#ifndef _MAC
	#define RGB_TO_RGBQUAD(r,g,b)   (RGB(b,g,r))
	#define CLR_TO_RGBQUAD(clr)     (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))
	#define RGBQUAD_TO_CLR(rgbq)	CLR_TO_RGBQUAD(rgbq)
#else
	#define RGB_TO_RGBQUAD(r,g,b)   (RGB(r,g,b) << 8)
	#define CLR_TO_RGBQUAD(clr)     (clr << 8)
	#define RGBQUAD_TO_CLR(rgbq)	(rgbq >> 8)
#endif


/////////////////////////////////////////////////////////////////////////////
// Static (module-private) function

static void
MapColorTable(DWORD* color_table, int color_table_size,
			  const COLORMAP* color_map, int color_map_size)
{
	// Edit the bitmap's color table, replacing entries
	// for specific colors with current system colors.
	//
	// The color_map parameter is a pointer to an array
	// of COLORMAP structures; color_map_size is the size
	// of the array. The 'from' member of each COLORMAP
	// structure specifies a color given as an RGB(r,g,b)
	// value, and the 'to' member is one of the COLOR_*
	// system color indexes that gets passed to ::GetSysColor().
	//
	// If color_map_size <= 0, do nothing.
	// If color_map_size > 0 but color_map == NULL, map the standard greys.

	// >>>TODO: Figure out why some colors don't get mapped right
	// on some bitmaps.
	//   clouds.bmp: index 53 (255,255,255) -> black
	//      looks like speckles in the densest part of each cloud.

	// Define standard mappings from basic greys
	// to the corresponding system colors.
	static const COLORMAP grey_color_map[] =
	{
		{ RGB(0x00, 0x00, 0x00),  COLOR_BTNTEXT },       // black
		{ RGB(0x80, 0x80, 0x80),  COLOR_BTNSHADOW },     // dark grey
		{ RGB(0xC0, 0xC0, 0xC0),  COLOR_BTNFACE	},       // bright grey
		{ RGB(0xFF, 0xFF, 0xFF),  COLOR_BTNHIGHLIGHT }   // white
	};
	static const int grey_color_map_size = sizeof(grey_color_map) / sizeof(grey_color_map[0]);


	if (color_table_size > 0 && color_map_size > 0)
	{
		ASSERT(color_table != NULL);

		if (color_map == NULL)
		{
			color_map = grey_color_map;
		}

		// color table is in RGBQUAD DIB format
		for (int color_index = 0; color_index < color_table_size; color_index++)
		{
			COLORREF clr = RGBQUAD_TO_CLR(color_table[color_index]);

			// look for matching RGBQUAD color in original
			for (int i = 0; i < color_map_size; i++)
			{
				if (clr == color_map[i].from)
				{
					color_table[color_index] =
						CLR_TO_RGBQUAD(::GetSysColor(color_map[i].to));
					break;	// exit inner (i) loop
				}
			}
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
//
// Create a Windows bitmap from a .BMP or .DIB file.
//
// How embarrassing for Microsoft that they *still* don't
// provide a simple API call for this common operation.
//
// TODO: See whether LoadBmpFile() can be replaced with some Windows call.
// ::LoadImage(... LR_LOADFROMFILE [| LR_LOADMAP3DCOLORS])
// might work, but the documentation says these options are
// "Winows 95 only," so we need to test it on NT first.
//
HBITMAP
LoadBmpFile(CDC* dc_ptr, const char* file_name,
			const COLORMAP* color_map /*= NULL*/, int color_map_size /*= 0*/)
{
	// Retrieve a handle identifying the file.
	HANDLE hfbm = CreateFile(file_name, GENERIC_READ, 
		FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, 
		(HANDLE)NULL);
	if (hfbm == INVALID_HANDLE_VALUE)
	{
		TRACE("LoadBmpFile - Error opening \"%s\"\n", file_name);
		return NULL;
	}

	// Retrieve the BITMAPFILEHEADER structure.
	DWORD dwRead;
	BITMAPFILEHEADER bmfh;
	if (!ReadFile(hfbm, &bmfh, sizeof(BITMAPFILEHEADER), &dwRead, (LPOVERLAPPED)NULL))
	{
		CloseHandle(hfbm);
		TRACE("LoadBmpFile - Error reading file header for \"%s\"\n", file_name);
		return NULL;
	}

	// Retrieve the BITMAPINFOHEADER structure.
	BITMAPINFOHEADER bmih;
	if (!ReadFile(hfbm, &bmih, sizeof(BITMAPINFOHEADER),
		&dwRead, (LPOVERLAPPED)NULL))
	{
		CloseHandle(hfbm);
		TRACE("LoadBmpFile - Error reading info header for \"%s\"\n", file_name);
		return NULL;
	}

	// Allocate memory for the BITMAPINFO structure.
	HGLOBAL hmem1 = GlobalAlloc(GHND, 
		sizeof(BITMAPINFOHEADER) + ((1<<bmih.biBitCount) * sizeof(RGBQUAD)) );
	BITMAPINFO* lpbmi = static_cast<BITMAPINFO*>(GlobalLock(hmem1)); 
	if (hmem1 == NULL || lpbmi == NULL)
	{
		if( lpbmi != NULL )
			GlobalUnlock(hmem1);

		if( hmem1 != NULL )
			GlobalFree(hmem1);

		CloseHandle(hfbm);
		TRACE("LoadBmpFile - Error locking memory for \"%s\"\n", file_name);
		return NULL;
	}

	// Load BITMAPINFOHEADER into the BITMAPINFO structure.
	lpbmi->bmiHeader.biSize          = bmih.biSize; 
	lpbmi->bmiHeader.biWidth         = bmih.biWidth; 
	lpbmi->bmiHeader.biHeight        = bmih.biHeight; 
	lpbmi->bmiHeader.biPlanes        = bmih.biPlanes; 
	lpbmi->bmiHeader.biBitCount      = bmih.biBitCount; 
	lpbmi->bmiHeader.biCompression   = bmih.biCompression; 
	lpbmi->bmiHeader.biSizeImage     = bmih.biSizeImage; 
	lpbmi->bmiHeader.biXPelsPerMeter = bmih.biXPelsPerMeter; 
	lpbmi->bmiHeader.biYPelsPerMeter = bmih.biYPelsPerMeter; 
	lpbmi->bmiHeader.biClrUsed       = bmih.biClrUsed; 
	lpbmi->bmiHeader.biClrImportant  = bmih.biClrImportant; 

	if (bmih.biBitCount <= 8)
	{
		// Retrieve the color table for the bitmap.
		// (1 << bmih.biBitCount) == (2 ^ bmih.biBitCount)
		int color_table_size = 1 << bmih.biBitCount;
		if (!ReadFile(hfbm, lpbmi->bmiColors, (color_table_size * sizeof(RGBQUAD)),
			&dwRead, (LPOVERLAPPED)NULL))
		{
			GlobalUnlock(hmem1); 
			GlobalFree(hmem1);
			CloseHandle(hfbm);
			TRACE("LoadBmpFile - Error reading color table for \"%s\"\n", file_name);
			return NULL;
		}

		// Use the C cast operator since static_cast<>() doesn't like this.
		DWORD* color_table = /*static_cast*/(DWORD*)(lpbmi->bmiColors);
		MapColorTable(color_table, color_table_size, color_map, color_map_size);
	}

	// Allocate memory for the required number of bytes. 
	HGLOBAL hmem2 = GlobalAlloc(GHND, (bmfh.bfSize - bmfh.bfOffBits)); 
	void* lpvBits = GlobalLock(hmem2); 
	if (hmem2 == NULL || lpvBits == NULL)
	{
		GlobalUnlock(hmem1); 

		GlobalFree(hmem1);
		if( lpvBits != NULL )
			GlobalUnlock(hmem2);

		if( hmem2 != NULL )
			GlobalFree(hmem2);

		CloseHandle(hfbm);
		TRACE("LoadBmpFile - Error locking memory for \"%s\"\n", file_name);
		return NULL;
	}

	// Retrieve the bitmap data.
	if (!ReadFile(hfbm, lpvBits, (bmfh.bfSize - bmfh.bfOffBits),
		&dwRead, (LPOVERLAPPED)NULL))
	{
		GlobalUnlock(hmem2); 
		GlobalUnlock(hmem1); 
		GlobalFree(hmem1);
		GlobalFree(hmem2);
		CloseHandle(hfbm);
		TRACE("LoadBmpFile - Error reading bitmap data for \"%s\"\n", file_name);
		return NULL;
	}

	// Create a bitmap from the data stored in the .BMP file.
	HDC dc_handle;
	if (dc_ptr == NULL)
	{
		dc_handle = ::GetDC(NULL);
	}
	else
	{
		dc_handle = dc_ptr->m_hDC;
	}
	HBITMAP hbm = CreateDIBitmap(dc_handle, &bmih, CBM_INIT, lpvBits, lpbmi, DIB_RGB_COLORS);
	if (dc_ptr == NULL)
	{
		::ReleaseDC(NULL, dc_handle);
	}

	// Unlock the global memory objects and close the .BMP file. 
	GlobalUnlock(hmem2); 
	GlobalUnlock(hmem1); 
	GlobalFree(hmem1);
	GlobalFree(hmem2);

	CloseHandle(hfbm);

#ifdef _DEBUG
	if (hbm == NULL)
	{
		TRACE("LoadBmpFile - Error creating DIB for \"%s\"\n", file_name);
	}
#endif
	return hbm;
}


/////////////////////////////////////////////////////////////////////////////
//
// Create a Windows bitmap from a bitmap resource.
// 
// Windows supplies several functions to load bitmap resources,
// but none of them allow you to pass in a device context.
// This means that in paletted display modes (like 256-color),
// it just matches the colors in the bitmap to the fixed system
// colors since it has no way of knowing what other colors are
// available.  See MS KnowledgeBase article Q67883 for more info.
//
// How embarrassing for Microsoft that they *still* don't
// provide a simple API call for this common operation.
//
HBITMAP
LoadBmpResource(CDC* dc_ptr, LPCSTR resource_name,
				const COLORMAP* color_map /*= NULL*/, int color_map_size /*= 0*/)
{
	ASSERT(resource_name != NULL);

	HBITMAP bitmap_handle = NULL;

	// AfxFindResourceHandle() searches the .EXE and all
	// CDynLinkLibrary (.DLL) objects for the resource.
	// It returns an HINSTANCE, which is apparently really
	// the same thing as an HMODULE.
	HMODULE module = static_cast<HMODULE>(
		AfxFindResourceHandle(resource_name, RT_BITMAP) );
	if (module != NULL)
	{

	// Find the specified resource within this module.
	HRSRC res = ::FindResource(module, resource_name, RT_BITMAP);
	if (res != NULL)
	{

	HGLOBAL global = ::LoadResource(module, res);
	if (global != NULL)
	{
		BITMAPINFO* bmi_ptr = static_cast<BITMAPINFO*>(::LockResource(global));
		if (bmi_ptr != NULL)
		{
			int color_table_size =
				(bmi_ptr->bmiHeader.biBitCount <= 8)
					? 1 << bmi_ptr->bmiHeader.biBitCount
					: 0;

			BITMAPINFO* temp_bmi_ptr = bmi_ptr;
			if (color_map_size > 0)
			{
				// Make a copy of the BITMAPINFO structure (which
				// contains the BITMAPINFOHEADER and the color table)
				// so that we can modify the color table without
				// changing the original resource image.
				int info_size = sizeof(BITMAPINFOHEADER) + color_table_size * sizeof(RGBQUAD);
				temp_bmi_ptr = (BITMAPINFO*) new char[info_size];
				if (temp_bmi_ptr == NULL)
				{
					// The memory allocation failed.
					// Set the pointer back to the original resource
					// and don't map the colors.
					temp_bmi_ptr = bmi_ptr;
					TRACE("WARNING: Memory allocation failed in ::LoadBmpResource(). Not mapping colors.\n");
				}
				else
				{
					// Copy the structure
					memcpy(temp_bmi_ptr, bmi_ptr, info_size);

					// Use the C cast operator since static_cast<>() doesn't like this.
					MapColorTable((DWORD*)(temp_bmi_ptr->bmiColors), color_table_size,
									color_map, color_map_size);
				}
			}
	
			// Create a bitmap from the data stored in the bitmap resource.
			HDC dc_handle;
			if (dc_ptr == NULL)
			{
				dc_handle = ::GetDC(NULL);
			}
			else
			{
				dc_handle = dc_ptr->m_hDC;
			}
			bitmap_handle = CreateDIBitmap(
								dc_handle,
								&temp_bmi_ptr->bmiHeader,				// Header
								CBM_INIT,
								&bmi_ptr->bmiColors[color_table_size],	// Bitmap data bits
								temp_bmi_ptr,							// Header + color table
								DIB_RGB_COLORS);
			if (dc_ptr == NULL)
			{
				::ReleaseDC(NULL, dc_handle);
			}

			// If we made a copy of the color table, free it
			if (temp_bmi_ptr != bmi_ptr)
			{
				delete [] (char*)temp_bmi_ptr;
			}
			
		}
		::UnlockResource(global);
		::FreeResource(global);

	} // global
	} // res
	} // module

	return bitmap_handle;
}
