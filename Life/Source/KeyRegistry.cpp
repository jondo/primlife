// KeyRegistry.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KeyRegistry.h"


//----------------------------CKeyRegistry------------------------------


////////////////////////////////////////////////////////////////////////
// IsGoodKey
//
bool CKeyRegistry::IsGoodKey()
{
	CString sName = GetName();
    int  length   = sName.GetLength();
	WORD result   = (WORD) GetKey();

	if (IsTemporaryKey())
	{
		time_t time = 0;
		DWORD dwValue = GetValue("key", 0);

		if (dwValue == 0)
			return false;

		CTime date((time_t)dwValue);

		int days = abs((CTime::GetCurrentTime() - date).GetDays());

		if (days > 15)
			return false;
	}

	sName.MakeUpper();

    for (int i = 0; i < length; i++)
      result -= (((WORD)sName[i]) << ((i + 3) % 7));

	return (result == 0xFADE && length > 2);
}


////////////////////////////////////////////////////////////////////////
// Valid
//
bool CKeyRegistry::IsValid()
{
	GetKey();
	return (((GetTickCount() % 14) != 7) || (((((m_lKey & 0x0007FFFF) * 3) / 5) << 19) & 0xFFF80000) ==
		(0xFFF80000 & m_lKey));
}


////////////////////////////////////////////////////////////////////////
// IsSiteKey
//
bool CKeyRegistry::IsSiteKey()
{
	return ((m_lKey & 0x00040000) == 0x00040000);
}


////////////////////////////////////////////////////////////////////////
// IsWorkAndHomeKey
//
bool CKeyRegistry::IsWorkAndHomeKey()
{
	return ((m_lKey & 0x00020000) == 0x00020000);
}


////////////////////////////////////////////////////////////////////////
// IsTemporaryKey
//
bool CKeyRegistry::IsTemporaryKey()
{
	return ((m_lKey & 0x00010000) == 0x00010000);
}


///////////////////////////////////////////////////////////////
// GetKey
//
// Retrieves the key out of the registry
//
long CKeyRegistry::GetKey()
{
	m_lKey = (long) GetValue("date", 0);
	return m_lKey;
}


///////////////////////////////////////////////////////////////
// GetHexKey
//
CString CKeyRegistry::GetHexKey()
{
	CString sString;
	sString.Format("%08X", GetKey());	
	return sString;
}


long CKeyRegistry::SetHexKey(CString sString)
{
	sString.MakeUpper();
	
	long key = 0;

	for (int i = 0; i < sString.GetLength(); i++)
	{
		key |= HexDigit(sString[i]);

		if (i < 7)
			key <<= 4;
	}
	return key;
}


///////////////////////////////////////////////////////////////
// HexDigit
//
long CKeyRegistry::HexDigit(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else
	{
		if (c == 'l' || 
			c == 'L' ||
			c == 'i')
			return 1;

		if (c == 'O' ||
			c == 'o')
			return 0;

		return (c - 'A' + 10); 
	}
}


///////////////////////////////////////////////////////////////
// GetName
//
// Retrieves the name out of the registry
//
CString CKeyRegistry::GetName()
{
	GetString("name", m_sName);
	m_sName.TrimRight();
	m_sName.TrimLeft();
	return m_sName;
}

void CKeyRegistry::SetKey(long lKey)
{
	m_lKey = lKey;

	if (IsTemporaryKey())
	{
		DWORD dwTime = GetValue("key", 0);

		if (dwTime == 0)
		{
			time_t time = CTime::GetCurrentTime().GetTime();
			SetValue("key", (DWORD) time);
		}

	}
	SetValue("date", lKey);
}

void CKeyRegistry::SetName(LPCTSTR szName)
{
	SetString("name", szName);
}


//-------------------------------CSoundRegistry-------------------------


////////////////////////////////////////////////////////////////////////
// CSoundRegistry::GetPath
//
// Play a specific sound event.  Transmitter is silent
//
CString CSoundRegistry::GetPath(LPCSTR szEvent)
{
	ASSERT(szEvent);

	CRegistry sound;

	CString sEvent = "AppEvents\\Schemes\\Apps\\";
	sEvent += m_sScheme;
	sEvent += '\\';
	sEvent += szEvent;
	sEvent += "\\.current";

	if (sound.OpenKey(sEvent, HKEY_CURRENT_USER))
	{
		sEvent.Empty();
		sound.GetString(NULL, sEvent);
		return sEvent;
//		sndPlaySound(sEvent, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
	}
	sEvent.Empty();
	return sEvent;
}


///////////////////////////////////////////////////////
// CSoundRegistry::CSoundRegistry
//
//
CSoundRegistry::CSoundRegistry(LPCSTR szScheme, LPCSTR szSchemeName)
{
	SetScheme(szScheme, szSchemeName);
}


///////////////////////////////////////////////////////
// CSoundRegistry::SetScheme
//
//
void CSoundRegistry::SetScheme(LPCSTR szScheme, LPCSTR szSchemeName)
{
	m_sScheme     = szScheme;
	m_sSchemeName = szSchemeName;
	m_bSetScheme  = FALSE;
}


///////////////////////////////////////////////////////
// CSoundRegistry::SoundEvent
//
//
void CSoundRegistry::SoundEvent(LPCSTR szEvent, LPCSTR szEventName, LPCSTR szPath)
{
	CRegistry sound;
	CString sEvent("AppEvents\\EventLabels\\");

	sEvent += szEvent;

	sound.CreateKey(sEvent, HKEY_CURRENT_USER);
	sound.SetString(NULL, szEventName);

	sEvent  = "AppEvents\\Schemes\\Apps\\";
	sEvent  += m_sScheme;

	if (!m_bSetScheme)
	{
		m_bSetScheme = TRUE;
		sound.CreateKey(sEvent, HKEY_CURRENT_USER);
		sound.SetString(NULL, m_sSchemeName);
	}

	sEvent += '\\';
	sEvent += szEvent;

	// If the key doesn't exist
	if (!sound.OpenKey(sEvent, HKEY_CURRENT_USER))
	{
		// Create the key with the right name
		sound.CreateKey(sEvent, HKEY_CURRENT_USER);
		sound.SetString(NULL, szEventName);

		// Set the default sound
		sEvent += "\\.current";
		sound.CreateKey(sEvent, HKEY_CURRENT_USER);
		sEvent  = szPath;
		sEvent += szEventName;
		sEvent += ".wav";
		sound.SetString(NULL, sEvent);
	}
}


