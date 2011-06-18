// PostData.cpp
//
// Implementation of the CPostData class
//
// $Revision$
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PostData.h"


const char *endLn = "\r\n";

CPostData::CPostData()
{
	m_pBuffer	= NULL;
	m_used	= 0;
	m_size	= 0;
	m_grow    = 2048; 
	m_readPos = 0;
}

CPostData::CPostData(int initialSize)
{
	if (initialSize < 256)
		initialSize = 256;

	m_grow    = initialSize;
	m_size    = 0;
	m_used    = m_readPos = 0;
	m_pBuffer = NULL;
    Allocate(initialSize + 5);
}

CPostData::~CPostData()
{
	if (m_pBuffer)
		free(m_pBuffer);
}


//////////////////////////////////////////////////////////
//
// Read
//
// Reads a line from the buffer into your buffer up to a
// maximum number of characters as specified by maxLength.
// The command fails if your buffer is too small.
//
// The line will be null terminated if successful.
//
BOOL CPostData::Read(LPBYTE pByte, int length)
{
    if (m_used - m_readPos >= length)
    {
      memcpy(pByte, m_pBuffer + m_readPos, length);
      m_readPos += length;
      return TRUE;
    }
    return FALSE;
} 

 //////////////////////////////////////////////////////////
//
// Write
//
// Writes into the buffer from a file.
//
BOOL CPostData::Write(HANDLE hFile)
{
	DWORD nRead;
	
	do
	{
		if (!Allocate(m_grow / 2))
			return FALSE;

		nRead = 0;
		ReadFile(hFile, (LPVOID)(m_pBuffer + m_used), (DWORD) (m_size - m_used - 1), &nRead, NULL);
		m_used += nRead;

		// NULL TERMINATE
		m_pBuffer[m_used] = '\0';

	} while (nRead > 0);

	return TRUE;
}


 //////////////////////////////////////////////////////////
//
// Write
//
// Writes into the buffer from a file.
//
BOOL CPostData::Write(FILE* fIn, int nSize)
{
	if (Allocate(nSize + 1) && fread((m_pBuffer + m_used), nSize, 1, fIn))
  {
  	m_used += nSize;
		m_pBuffer[m_used] = '\0';
    return TRUE;
  }
	return FALSE;
}


 //////////////////////////////////////////////////////////
//
// Allocate
//
// Allocates memory as needed.
//
BOOL CPostData::Allocate(int length)
{
int step;

	// Make sure we have enough space;
	if (length + m_used > m_size - 1)
	{
		// Set the size to what we need
		if (m_grow > length)
			step = m_grow;
		else
			step = length + 1;

		m_size += step;

		// We need to realloc
		if (m_pBuffer)
		{
			LPBYTE pTemp;
			pTemp = (LPBYTE) realloc(m_pBuffer, m_size);
			if (!pTemp)
			{
				m_size -= step;
				return FALSE;
			}

			m_pBuffer = pTemp;
			m_grow *= 2;
		}
		else
		{
			m_pBuffer = (LPBYTE) malloc(m_size);
			if (!m_pBuffer)
			{
				m_size = 0;
				return FALSE;
			}
		}
	}
	return TRUE;
}


 //////////////////////////////////////////////////////////
//
// Write
//
// Writes a binary blob into a memory buffer and expands
// it if necessary.
//
BOOL CPostData::Write(LPBYTE pByte, int length)
{
	if (length > 0 && pByte)
	{
		if (!Allocate(length))
			return FALSE;

		memcpy(m_pBuffer + m_used, pByte, length);

		m_used += length;

		// NULL TERMINATE
		// Don't overwrite the memory image
//		m_pBuffer[m_used] = '\0';

		return TRUE;
	}
	return FALSE;
}


 //////////////////////////////////////////////////////////
//
// WriteStr
//
// Writes a string into a memory buffer
//
BOOL CPostData::WriteStr(LPCSTR pChar)
{
	return Write((LPBYTE) pChar, lstrlen(pChar));
}


//////////////////////////////////////////////////////////
//
// WriteLn
//
// Writes a string line (with \r\n) into a memory buffer
//
BOOL CPostData::WriteLn(LPCSTR pChar)
{
	if (Write((LPBYTE) pChar, lstrlen(pChar)) == TRUE)
	  return Write((LPBYTE) endLn, 2);
	return FALSE;
}

//////////////////////////////////////////////////////////
// Retrieves a CString
//
CPostData& CPostData::operator >>( CString& sString )
{
	int nValue;

	Read((LPBYTE) &nValue, sizeof(long));

	LPSTR szBuffer = sString.GetBuffer(nValue);
	if (szBuffer)
	{
		Read((LPBYTE) szBuffer, nValue);
		sString.ReleaseBuffer(nValue);
	}
	return *this;
}


