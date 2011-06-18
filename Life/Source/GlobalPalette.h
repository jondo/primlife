// AppPalette.h
//
// CLASS:
//	CAppPalette knows the 'current color palette' for the application.
//
// RESPONSIBILITIES:
//	  -	Hold the global pointer to the current color palette
//		that others Set and Get using access methods.
//	  -	Create and destroy a default application color palette
//		that is returned by the Get accessor when no other
//		palette has been explicitly set
//

#ifndef AppPalette_h
#define AppPalette_h


class CAppPalette
{
public:
	static CPalette* SetAppPalette(CPalette* palette_ptr);
	static CPalette* GetAppPalette(BOOL do_realize = TRUE);

	// Specify the resource to use for the default palette.
	//
	// WARNING: The current default palette must not be selected
	// into any device contexts when SetDefaultPalette() is called.
	//
	// Follow with SetAppPalette(NULL); to load and use
	// the new default palette.
	//
	static void SetDefaultPalette(UINT resource_id);
	static void SetDefaultPalette(LPCTSTR resource_name);

private:
	static LPCTSTR   s_default_palette_res;
	static CPalette  s_default_palette;
	static CPalette* s_cur_palette_ptr;
};


/////////////////////////////////////////////////////////////////////////////
// CAppPalette inlines

inline /*static*/ void
CAppPalette::SetDefaultPalette(UINT resource_id)
{
	ASSERT(resource_id < 0x8000);
	SetDefaultPalette( MAKEINTRESOURCE(resource_id) );
}


#endif // AppPalette_h
