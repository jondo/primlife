// Registry.cpp
//
// Implementation of the our registry access.  Simplifies registry access for
// the rest of the program.  
//
// $Revision$
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Registry.h"


////////////////////////////////////////////////////////////
// Constructor
//
// Initializes registry key value
//
CRegistry::CRegistry()
{
	m_hKey = NULL;
}


////////////////////////////////////////////////////////////
// Destructor
//
// Frees the registry key is allocated
//
CRegistry::~CRegistry()
{
	CloseKey();
}

 
////////////////////////////////////////////////////////////
// CreateKey
//
// Creates a registry entry, and fills in m_hKey for 
// subsequent registry queries.
//
BOOL CRegistry::CreateKey(LPCSTR szAppKey, HKEY nRoot)
{
	// If the key is already allocated, close it
	CloseKey();

	DWORD dwDisposition;

	m_nRoot   = nRoot;
	m_sAppKey = szAppKey;

	// Attempt to create the key
	if (RegCreateKeyEx(m_nRoot, m_sAppKey, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_WRITE | KEY_READ, NULL, &m_hKey, &dwDisposition) != ERROR_SUCCESS)
	{
		TRACE("CRegistry::CreateKey failed\n");
		m_hKey = NULL;
		return FALSE;
	}
	return TRUE;
}


////////////////////////////////////////////
// CloseKey
// 
// Some applications will hang unless you
// close the registry.
//
BOOL CRegistry::CloseKey()
{
	BOOL bResult = TRUE;
	if (m_hKey != NULL)
	{
		if (RegCloseKey(m_hKey) != ERROR_SUCCESS)
			bResult = FALSE;

		m_hKey = NULL;
	}
	return bResult;
}


///////////////////////////////////////////////
// OpenKey
// 
// For opening the registry without creating it
//
BOOL CRegistry::OpenKey(LPCSTR szAppKey, HKEY nRoot)
{
	CloseKey();

	m_nRoot   = nRoot;
	m_sAppKey = szAppKey;

	if (RegOpenKeyEx(m_nRoot, m_sAppKey, 0, KEY_WRITE | KEY_READ, &m_hKey) != ERROR_SUCCESS)
	{
		TRACE("CRegistry::OpenKey failed\n");
		m_hKey = NULL;
		return FALSE;
	}
	return TRUE;
}


///////////////////////////////////////////////////////////////
// FlushKey
//
// Makes absolutely sure changes are written to the registry
// by the time the call returns.  Used primarily to ensure the
// log record counter is updated for the next thread or process
// 
BOOL CRegistry::FlushKey()
{
	if (m_hKey)
	{
		return (RegFlushKey(m_hKey) == ERROR_SUCCESS);
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////
// GetValue
//
// Attempts to retrieve an Integer value out of the registry.
// If the value is not the right type, or doesn't exist, or the 
// operation fails for some reason, this operation will
// return FALSE.  The value in value_ptr is unaffected if
// this routine returns fails.
//
BOOL CRegistry::GetValue(LPCSTR szSubKey, DWORD* value_ptr)
{
	ASSERT(value_ptr);

	BOOL bResult;
	DWORD nTempValue = *value_ptr;
	DWORD dwType;
	DWORD dwSize = sizeof(nTempValue);

	bResult =	(IsOpen() && RegQueryValueEx(m_hKey, szSubKey, NULL, &dwType, (LPBYTE)&nTempValue, &dwSize) == ERROR_SUCCESS &&
				dwType == REG_DWORD);

	if (bResult)
		*value_ptr = nTempValue;

	return bResult;
}


BOOL CRegistry::GetValue(LPCSTR szSubKey, long* value_ptr)
{
	return GetValue(szSubKey, (DWORD*) value_ptr);
}

///////////////////////////////////////////////////////////////
// SetValue
//
// Attempts to set a numeric value into the registry
//
BOOL CRegistry::SetValue(LPCSTR szSubKey, DWORD nValue)
{
	ASSERT(szSubKey);

	BOOL bResult;

    bResult =	(IsOpen() &&
				RegSetValueEx(m_hKey, szSubKey, 0, REG_DWORD, (LPBYTE)&nValue, sizeof(DWORD)) == ERROR_SUCCESS);

	return bResult;
}


///////////////////////////////////////////////////////////////
// GetBinary
//
// Attempts to retrieve a binary value out of the registry.
// If the value is not the right type, or doesn't exist, or the 
// operation fails for some reason, this operation will
// return FALSE.  The value in string_ptr is unaffected if
// this routine returns fails.
//
BOOL CRegistry::GetBinary(LPCSTR szSubKey, LPVOID ptr, DWORD& maxLength)
{
	ASSERT(ptr);
	ASSERT(szSubKey);
	ASSERT(maxLength > 0);

	BOOL bResult;
	DWORD dwType;

	bResult =	(IsOpen() && 
				RegQueryValueEx(m_hKey, szSubKey, NULL, &dwType, (LPBYTE)ptr, &maxLength) == ERROR_SUCCESS &&
				dwType == REG_BINARY);
	return bResult;
}


///////////////////////////////////////////////////////////////
// SetBinary
//
// Attempts to set a binary value into the registry
//
BOOL CRegistry::SetBinary(LPCSTR szSubKey, LPVOID pValue, DWORD length)
{
	ASSERT(szSubKey);
	ASSERT(pValue);

	BOOL bResult;

	bResult =	(IsOpen() && 
				RegSetValueEx(m_hKey, szSubKey, 0, REG_BINARY, (LPBYTE)pValue, length) == ERROR_SUCCESS);

	return bResult;
}

///////////////////////////////////////////////////////////////
// GetString
//
// Attempts to retrieve a String value out of the registry.
// If the value is not the right type, or doesn't exist, or the 
// operation fails for some reason, this operation will
// return FALSE.  The value in string_ptr is unaffected if
// this routine returns fails.
//
BOOL CRegistry::GetString(LPCSTR szSubKey, char* string_ptr, DWORD maxLength)
{
	ASSERT(string_ptr);
	ASSERT(maxLength > 0);

	BOOL  bResult;
	DWORD dwType;

	bResult =	(IsOpen() && 
				RegQueryValueEx(m_hKey, szSubKey, NULL, &dwType, (LPBYTE)string_ptr, &maxLength) == ERROR_SUCCESS &&
				dwType == REG_SZ);

	return bResult;
}


///////////////////////////////////////////////////////////////
// GetString
//
// Same as above, put limits the size of the registry string
// to MAX_PATH
//
BOOL CRegistry::GetString(LPCSTR szSubKey, CString& sString)
{
	char string[MAX_PATH];

	BOOL bResult = GetString(szSubKey, string, MAX_PATH);
	if (bResult)
		sString = string;
	return bResult;
}


///////////////////////////////////////////////////////////////
// SetString
//
// Attempts to set a string value into the registry
//
BOOL CRegistry::SetString(LPCSTR szSubKey, LPCSTR szValue)
{
	ASSERT(szValue);
	BOOL bResult;

	bResult =	(IsOpen() &&
				RegSetValueEx(m_hKey, szSubKey, 0, REG_SZ, (LPBYTE)szValue, lstrlen(szValue) + 1) == ERROR_SUCCESS);

	return bResult;
}


///////////////////////////////////////////////////////////////
// DeleteValue
//
// Removes a value from the registry altogether.
//
BOOL CRegistry::DeleteValue(LPCSTR szSubKey)
{
	BOOL bResult;

	bResult = (IsOpen() && RegDeleteValue(m_hKey, szSubKey) == ERROR_SUCCESS);

	return bResult;
}


///////////////////////////////////////////////////////////////
// GetAppName
//
// Gets the last part of the app key.  
//
CString CRegistry::GetAppName()
{
	CString sAppName;

	int slash = m_sAppKey.ReverseFind('\\');

	if (slash != -1)
		sAppName = m_sAppKey.Right(m_sAppKey.GetLength() - (slash + 1));

	return sAppName;
}


////////////////////////////////////////////////////////////////////////
// GetValue
//
//
DWORD CRegistry::GetValue(LPCSTR szSubKey, DWORD defValue)
{
  DWORD value = 0;

  if (CRegistry::GetValue(szSubKey, &value))
    return value;
  else
    return defValue;
}


int CRegistry::GetValue(LPCSTR szSubKey, int defValue)
{
  DWORD value = 0;

  if (CRegistry::GetValue(szSubKey, &value))
    return (int) value;
  else
    return defValue;
}


long CRegistry::GetValue(LPCSTR szSubKey, long defValue)
{
  DWORD value = 0;

  if (CRegistry::GetValue(szSubKey, &value))
    return (long) value;
  else
    return defValue;
}


void CRegistry::GetDefaultString(LPCSTR szSubKey, char* szValue, DWORD maxLength, char* defValue)
{
	if (!CRegistry::GetString(szSubKey, szValue, maxLength))
     lstrcpy(szValue, defValue);
}
  

