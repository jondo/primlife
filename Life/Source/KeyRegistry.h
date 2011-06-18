// Registry.h : header file
// $Revision$
//
/////////////////////////////////////////////////////////////////////////////

#ifndef KeyRegistry_h
#define KeyRegistry_h

#include "Registry.h"

//CRC////////////////////////////////////////////////////////////////////////
//
// CKeyRegistry
//
// Registry for 4.0 common to both screen saver and the window program.
//
class CKeyRegistry : public CRegistry
{
public:
	enum {
		MAX_NAME_SIZE = 35
	};

	long GetKey();
	CString GetName();

	void SetKey(long lKey);
	void SetName(LPCTSTR szName);

	bool IsGoodKey();
	bool IsSiteKey();
	bool IsWorkAndHomeKey();
	bool IsTemporaryKey();

	CString GetHexKey();
	long SetHexKey(CString sString);
	long HexDigit(char c);

	bool IsValid();

	// Valid after IsGoodKey
	long    m_lKey;
	CString m_sName;
};


//CRC////////////////////////////////////////////////////////////////////////
//
// CSoundRegistry
//
// Provides a means to set registry sounds or read them
//
class CSoundRegistry
{
public:
	CSoundRegistry(){};
	CSoundRegistry(LPCSTR szScheme, LPCSTR szSchemeName);
	void SetScheme(LPCSTR szScheme, LPCSTR szSchemeName);
	void SoundEvent(LPCSTR szEvent, LPCSTR szEventName, LPCSTR szPath);
	CString GetPath(LPCSTR szEvent);

private:
	BOOL    m_bSetScheme;
	CString m_sScheme;
	CString m_sSchemeName;
};



#endif
