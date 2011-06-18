// LoadBmpFile.h
// $Revision$

#ifndef LoadBmpFile_h
#define LoadBmpFile_h

extern HBITMAP LoadBmpFile(CDC* dc_ptr, const char* file_name,
							const COLORMAP* color_map = NULL, int color_map_size = 0);
extern HBITMAP LoadBmpResource(CDC* dc_ptr, LPCSTR resource_name,
							const COLORMAP* color_map = NULL, int color_map_size = 0);

#endif // LoadBmpFile_h

