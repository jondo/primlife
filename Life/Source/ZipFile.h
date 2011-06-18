// ZipFile.h
//
// Original Author: Kevin English
//
// Declaration of CCompressFile, CUncompressFile, and their base class CZBaseFile.
//
// These classes provide CFile wrappers around the public domain ZLib library
//
// These classes are heavily based on the functions found in the gzio.c
// module of the ZLib Library.
//
// These classes currently maintain a CRC value, but do not do anything with it
// and do not provide any member functions for accessing it.
//
// This CFile implementation is for streaming only.  Random access methods
// are unimplemented.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ZipFile_h_
#define _ZipFile_h_

//#ifdef _DEBUG
//#pragma comment ( lib, "MkiZipD")
//#else
//#pragma comment ( lib, "MkiZip")
//#endif

#include <afx.h>
#define STDC
#include "zlib.h"

#define Z_BUFSIZE 4096

//=============================================================================
//=============================================================================

//
// NOTE: m_z_eof gets set then the source file (m_file) is at EOF.  This class
// may still has some data to read of the the compression stream once this occurs
//

class CZBaseFile : public CFile
{
	DECLARE_DYNAMIC(CZBaseFile)

	CZBaseFile(int hFile);
	CZBaseFile(LPCTSTR lpszFileName, UINT nOpenFlags);

// Attributes
	virtual DWORD GetPosition() const;
	BOOL GetStatus(CFileStatus& rStatus);
	virtual CString GetFileName() const;
	virtual CString GetFileTitle() const;
	virtual CString GetFilePath() const;
	virtual void SetFilePath(LPCTSTR lpszNewName);

// Operations
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags,
		CFileException* pError = NULL);

	DWORD SeekToEnd();
	void SeekToBegin();

	// backward compatible ReadHuge and WriteHuge
	DWORD ReadHuge(void* lpBuffer, DWORD dwCount);
	void WriteHuge(const void* lpBuffer, DWORD dwCount);

// Overridables

	virtual LONG Seek(LONG lOff, UINT nFrom);
	virtual void SetLength(DWORD dwNewLen);
	virtual DWORD GetLength() const;

	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);

	virtual void LockRange(DWORD dwPos, DWORD dwCount);
	virtual void UnlockRange(DWORD dwPos, DWORD dwCount);

	virtual void Abort();
	virtual void Flush();

	virtual void Close();

	void WrapWrite(CFile* file, const void *p, UINT n);
	UINT WrapRead(CFile* file, void *p, UINT n);

	int get_byte();
	ULONG getLong();

	enum MODEVALUE
	{
		mode_none,
		mode_read,
		mode_write
	};

	MODEVALUE m_mode;

	void SetTestMode(MODEVALUE mode)
	{
		if (m_mode == mode_none)
			m_mode = mode;
		ASSERT(m_mode == mode);
	}

public:
	virtual ~CZBaseFile();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual UINT GetBufferPtr(UINT nCommand, UINT nCount = 0,
		void** ppBufStart = NULL, void** ppBufMax = NULL);

protected:

    z_stream m_stream;
    int      m_z_err;   /* error code for last stream operation */
    int      m_z_eof;   /* set if end of input file */
    BYTE     m_buf[Z_BUFSIZE];  /* input buffer */
    UINT     m_size;  /* input buffer */
    DWORD    m_crc;     /* crc32 of uncompressed data */
	DWORD    m_position;
};

//=============================================================================
//=============================================================================

class CCompressFile : public CZBaseFile
{
	DECLARE_DYNAMIC(CCompressFile)

	CCompressFile(int hFile, int level=Z_DEFAULT_COMPRESSION);
	CCompressFile(LPCTSTR lpszFileName, UINT nOpenFlags, int level=Z_DEFAULT_COMPRESSION);
	virtual ~CCompressFile();

	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);
	virtual void Flush();
	virtual void Close();

private:
	UINT EndRead(void* lpBuf, UINT nCount);
};

//=============================================================================
//=============================================================================

class CUncompressFile : public CZBaseFile
{
	DECLARE_DYNAMIC(CUncompressFile)

	CUncompressFile(int hFile);
	CUncompressFile(LPCTSTR lpszFileName, UINT nOpenFlags);
	virtual ~CUncompressFile();

	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);
	virtual void Flush();
	virtual void Close();

	// This just pass through to the source CFile, so they may be misleading
	virtual DWORD GetLength() const;
	virtual DWORD GetPosition() const;
	virtual DWORD GetExpandedLength() const;


#ifdef _DEBUG
	CFile    m_TraceFile;
#endif
};


#endif
