// CPostData.h : header file
// $Revision$
//
/////////////////////////////////////////////////////////////////////////////

#ifndef CPostData_h
#define CPostData_h

#include "stdio.h"

//CRC////////////////////////////////////////////////////////////////////////
//
// CPostData
//
// This class is designed to build a POST request message. 
//		
// Collaborators:
//   None
//
// Usage Notes:
//
//
//
extern const char *endLn;

class CPostData 
{
public:
	CPostData();

	CPostData(int initialSize);

	~CPostData();

	// Read a line 
	BOOL Read(LPBYTE pByte, int length);

	BOOL Write(FILE* fIn, int nSize);

	// Read in an entire file
	BOOL Write(HANDLE hFile);

	BOOL Write(LPBYTE pByte, int length);

	BOOL WriteStr(LPCSTR pChar);

	BOOL WriteLn(LPCSTR pChar);

	int GetSize(){ return m_used;}
	int GetRemainingSize() { return m_used - m_readPos; }
	LPBYTE GetRemainingBuffer() { return m_pBuffer + m_readPos; }
	void SetUsed(int used) { m_used = used; }

	void ClearBuffer() { m_used = 0; m_readPos = 0;}
	void Reset() { m_readPos = 0;}

	LPBYTE GetBuffer() {return m_pBuffer;}
	int GetBufferSize() { return m_size; }

	CPostData& operator <<( char ch );
	CPostData& operator <<( CString& sString );
	CPostData& operator <<( float  fValue );
	CPostData& operator <<( double fValue );
	CPostData& operator <<( DWORD dwValue );
	CPostData& operator <<( int   nValue );
	CPostData& operator <<( short sValue );
	CPostData& operator <<( WORD  wValue );
	CPostData& operator <<( BYTE  nValue );
	CPostData& operator <<( long  nValue );
	CPostData& operator <<( signed char nValue );
	CPostData& operator <<( POINT& pValue );

	CPostData& operator >>( float&  fValue );
	CPostData& operator >>( double& fValue );
	CPostData& operator >>( DWORD& dwValue );
	CPostData& operator >>( int&   nValue );
	CPostData& operator >>( short& sValue );
	CPostData& operator >>( WORD&  wValue );
	CPostData& operator >>( BYTE&  nValue );
	CPostData& operator >>( long&  nValue );
	CPostData& operator >>( signed char& nValue );
	CPostData& operator >>( POINT& pValue );
	CPostData& operator >>( CString& sString );

	BOOL Allocate(int length);

public:
	enum {
		MAX_LINE_LENGTH = 256
	};

private:
	LPBYTE m_pBuffer;
	int    m_used;
	int    m_size;
	int    m_grow;
	int    m_readPos;
};

 //////////////////////////////////////////////////////////
//
// operator << 
//
// Writes whatever into the memory buffer.
//
inline CPostData& CPostData::operator <<( char ch )
{
	Write((LPBYTE) &ch, 1);
	return *this;
}
  
/*inline CPostData& CPostData::operator <<( LPCSTR pChar )
{
	WriteStr(pChar);
	return *this;
}*/

inline CPostData& CPostData::operator <<( float fValue )
{
  Write((LPBYTE) &fValue, sizeof(float));
	return *this;
}  

inline CPostData& CPostData::operator <<( double fValue )
{
  Write((LPBYTE) &fValue, sizeof(double));
	return *this;
}  

inline CPostData& CPostData::operator <<( DWORD dwValue )
{
  Write((LPBYTE) &dwValue, sizeof(DWORD));
	return *this;
}


inline CPostData& CPostData::operator <<( int nValue )
{
	Write((LPBYTE) &nValue, sizeof(int));
	return *this;
}

inline CPostData& CPostData::operator <<( short sValue )
{
  Write((LPBYTE) &sValue, sizeof(short));
	return *this;
}  

inline CPostData& CPostData::operator <<( WORD wValue )
{
  Write((LPBYTE) &wValue, sizeof(WORD));
	return *this;
}  

inline CPostData& CPostData::operator <<( BYTE nValue )
{
  Write((LPBYTE) &nValue, sizeof(BYTE));
	return *this;
}

inline CPostData& CPostData::operator <<( long nValue )
{
    Write((LPBYTE) &nValue, sizeof(long));
	return *this;
}

inline CPostData& CPostData::operator <<( signed char nValue )
{
	Write((LPBYTE) &nValue, sizeof(signed char));
	return *this;
}

inline CPostData& CPostData::operator <<( POINT& pValue )
{
	Write((LPBYTE) &pValue, sizeof(POINT));
	return *this;
}

inline CPostData& CPostData::operator <<( CString& sString )
{
	int nValue = sString.GetLength();
	Write((LPBYTE) &nValue, sizeof(int));
	Write((LPBYTE) (LPCSTR) sString, nValue);
	return *this;
}

 //////////////////////////////////////////////////////////
//
// operator >>
//
// Reads from the memory buffer
//
inline CPostData& CPostData::operator >>( float& fValue )
{
  Read((LPBYTE) &fValue, sizeof(float));
	return *this;
}  

inline CPostData& CPostData::operator >>( double& fValue )
{
  Read((LPBYTE) &fValue, sizeof(double));
	return *this;
}  

inline CPostData& CPostData::operator >>( DWORD& dwValue )
{
  Read((LPBYTE) &dwValue, sizeof(DWORD));
	return *this;
}


inline CPostData& CPostData::operator >>( int& nValue )
{
    Read((LPBYTE) &nValue, sizeof(int));
	return *this;
}

inline CPostData& CPostData::operator >>( short& sValue )
{
  Read((LPBYTE) &sValue, sizeof(short));
	return *this;
}  

inline CPostData& CPostData::operator >>( WORD& wValue )
{
	Read((LPBYTE) &wValue, sizeof(WORD));
	return *this;
}  

inline CPostData& CPostData::operator >>( BYTE& nValue )
{
	Read((LPBYTE) &nValue, sizeof(BYTE));
	return *this;
}

inline CPostData& CPostData::operator >>( long& nValue )
{
	Read((LPBYTE) &nValue, sizeof(long));
	return *this;
}

inline CPostData& CPostData::operator >>( signed char& nValue )
{
	Read((LPBYTE) &nValue, sizeof(signed char));
	return *this;
}

inline CPostData& CPostData::operator >>( POINT& pValue )
{
	Read((LPBYTE) &pValue, sizeof(POINT));
	return *this;
}




#endif