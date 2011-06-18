// Registry.h : header file
// $Revision$
//
//
/////////////////////////////////////////////////////////////////////////////

#ifndef Registry_h
#define Registry_h


//CRC////////////////////////////////////////////////////////////////////////
//
// CRegistry
//
// Provides simplified access to registry values.  
//
// Collaborators:
//   None
//
// Usage Notes:
//
//	Call CreateKey before using any other function.  You may call CreateKey 
//  again to access a different registry key.  All functions are thread
//  safe EXCEPT CreateKey.
//
class CRegistry
{
public:
	CRegistry();
	virtual ~CRegistry();

	// You must call this function prior to calling other functions
	virtual BOOL CreateKey(LPCSTR szAppKey, HKEY nRoot = HKEY_LOCAL_MACHINE);
	virtual BOOL OpenKey(LPCSTR szAppKey, HKEY nRoot = HKEY_LOCAL_MACHINE);

	// Get/Set a DWORD
	BOOL GetValue(LPCSTR szSubKey, DWORD* value_ptr);
	BOOL GetValue(LPCSTR szSubKey, long* value_ptr);
	BOOL SetValue(LPCSTR szSubKey, DWORD nValue);

	// Get/Set a string
	BOOL GetString(LPCSTR szSubKey, char* szValue, DWORD maxLength);
	BOOL GetString(LPCSTR szSubKey, CString& sString);
	BOOL SetString(LPCSTR szSubKey, LPCSTR szValue);

	BOOL SetBinary(LPCSTR szSubKey, LPVOID pValue, DWORD length);
	BOOL GetBinary(LPCSTR szSubKey, LPVOID ptr, DWORD& maxLength);

	// Completely remove a value from the registry
	BOOL DeleteValue(LPCSTR szSubKey);

	BOOL CloseKey();
	BOOL FlushKey();

	BOOL IsOpen() { return (m_hKey != NULL); }

	// This function returns the last entry of the AppKey
	CString GetAppName();

	DWORD GetValue(LPCSTR szSubKey, DWORD defValue);
	int   GetValue(LPCSTR szSubKey, int defValue);
	long  GetValue(LPCSTR szSubKey, long defValue);
	
	void GetDefaultString(LPCSTR szSubKey, char* szValue, DWORD maxLength, char* defValue);


protected:
	HKEY    m_hKey;
	HKEY    m_nRoot;
	CString m_sAppKey;
};
     

// Retrieve our Registry
// Note: The prototype lives in registry.h, but the function resides
// in your code (so you can set up the global location of the registry,
// or subclass it, or whatever.)
CRegistry& AfxGetRegistry();

#endif
