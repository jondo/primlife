#include "stdafx.h"
#include "Etools.h"
#include "Connect.h"
#include "Environ.h"
#include "Biots.h"


////////////////////////////////////////////////////////////////////
// Fifo
//
//
//
//
Fifo::Fifo()
{
  for (int i = 0; i < QUEUE_SIZE; i++)
    m_pVoid[i] = NULL;

  m_read  = 0;
  m_write = 0;
  m_size  = 0;
}


Fifo::~Fifo()
{
  for (int i = 0; i < QUEUE_SIZE; i++)
    if (m_pVoid[i] != NULL)
      delete m_pVoid[i];
}


void Fifo::Empty()
{
	m_read  = 0;
	m_write = 0;
	m_size  = 0;
	for (int i = 0; i < QUEUE_SIZE; i++)
		if (m_pVoid[i] != NULL)
		{	
			delete m_pVoid[i];
			m_pVoid[i] = NULL;
		} 
}


BOOL Fifo::Put(LPVOID pVoid)
{
	if (m_pVoid[m_write] == NULL)
	{
		m_pVoid[m_write++] = pVoid;
		if (m_write >= QUEUE_SIZE)
			m_write = 0;
		m_size++;
		ASSERT(m_size <= QUEUE_SIZE);
		return TRUE;
	}
	return FALSE;
}


LPVOID Fifo::Get()
{
	if (m_pVoid[m_read] != NULL)
	{
		LPVOID pTemp = m_pVoid[m_read];
		m_pVoid[m_read++] = NULL;
		if (m_read >= QUEUE_SIZE)
			m_read = 0;
		m_size--;
		ASSERT(m_size >= 0);
		return pTemp;
	}
	return NULL;	
}


////////////////////////////////////////////////////////////////////
// Side
//
//
//
//
Side::Side()
{
	m_lines = 0;
	m_pEnv = NULL;
}

void Side::Clear(BRect* pEnvRect)
{
	// Initially we have not connected, thus set up without connection
	Set(0,0,0,0);

	m_pEnv  = pEnvRect;
	m_lines = 1;
	m_outGoing.Empty();
	m_inComing.Empty();
}


BOOL Side::Export(Biot* pBiot)
{
  return m_outGoing.Put(pBiot);
}

CPostData* Side::Import()
{
  return (CPostData*) m_inComing.Get();
}

//Biot* Side::PeekBiot()
//{
//	return (Biot*) m_outGoing.Peek();
//}

Biot* Side::GetBiot()
{
  return (Biot*) m_outGoing.Get();
}

void Side::SendBiots(Side& otherSide)
{
  Biot* pBiot;
  CPostData* pPostData;

  pBiot = GetBiot();
  while (pBiot)
  {
    pPostData = new CPostData;
    if (pPostData)
    {
//!      pBiot->Save(*pPostData);
      if (!otherSide.RecvBiot(pPostData))
        delete pPostData;
    }
    delete pBiot;
    pBiot = GetBiot();
  }
}


BOOL Side::RecvBiot(CPostData* pData)
{
  return m_inComing.Put(pData);
}

//BYTE  Side::RecvSlots() 
//{
//	return (BYTE) m_inComing.FreeCount();
//}

int RightSide::SideSize()
{
	return m_bottom - m_top;
}

void RightSide::SetSide(BRect* pEnvRect)
{
	Clear(pEnvRect);
	m_line[0].Set(pEnvRect->m_right , pEnvRect->m_top, pEnvRect->m_right, pEnvRect->m_bottom);
}

void RightSide::AdjustBiot(Biot& biot)
{
  biot.MoveBiot(m_left - biot.m_left - 1, m_top);
}

void RightSide::RejectBiot(Biot& biot)
{
  biot.MoveBiot(m_left - biot.m_left - 1, m_top);
}

void LeftSide::SetSide(BRect* pEnvRect)
{
	Clear(pEnvRect);
	m_line[0].Set(pEnvRect->m_left, pEnvRect->m_top, pEnvRect->m_left, pEnvRect->m_bottom);
}

int LeftSide::SideSize()
{
  return m_bottom - m_top;
}

void LeftSide::AdjustBiot(Biot& biot)
{
  biot.MoveBiot(m_right - biot.m_right  + 1, m_top);
}

void LeftSide::RejectBiot(Biot& biot)
{
  biot.MoveBiot(m_right - biot.m_right  + 1, 0);
}

void TopSide::SetSide(BRect* pEnvRect)
{
	Clear(pEnvRect);
	m_line[0].Set(pEnvRect->m_left, pEnvRect->m_top, pEnvRect->m_right, pEnvRect->m_top);
}

int TopSide::SideSize()
{
  return m_right - m_left;
}

void TopSide::AdjustBiot(Biot& biot)
{
  biot.MoveBiot(m_left, m_bottom - biot.m_bottom + 1);
}

void TopSide::RejectBiot(Biot& biot)
{
  biot.MoveBiot(0, m_bottom - biot.m_bottom + 1);
}

void BottomSide::AdjustBiot(Biot& biot)
{
  biot.MoveBiot(m_left, m_top - biot.m_top - 1);
}

void BottomSide::RejectBiot(Biot& biot)
{
  biot.MoveBiot(0, m_top - biot.m_top - 1);
}
////////////////////////////////////////////////////////////////////
// BottomSide
//
//
//
//
void BottomSide::SetSide(BRect* pEnvRect)
{
  Clear(pEnvRect);
  m_line[0].Set(pEnvRect->m_left, pEnvRect->m_bottom, pEnvRect->m_right, pEnvRect->m_bottom);
}

int BottomSide::SideSize()
{
  return m_right - m_left;
}

////////////////////////////////////////////////////////////////////
// RightSide
//
//
//
//
void RightSide::SetConnect(int width)
{
  int right  = m_pEnv->Width() + RECT_WIDTH;
  int left   = m_pEnv->Width();
  int top    = (m_pEnv->Height() - width) / 2;
  int bottom = m_pEnv->Height() - top;

  Set(left, top, right, bottom);

  m_line[0].Set(left, bottom, right, bottom);
  m_line[1].Set(left, top, right, top);
  m_line[2].Set(left, bottom, left, m_pEnv->Width() + RECT_WIDTH);
  m_line[3].Set(left, top, left, -RECT_WIDTH);
  m_lines = 4;
}


////////////////////////////////////////////////////////////////////
// LeftSide
//
//
//
//
void LeftSide::SetConnect(int width)
{
  int right  = 0;
  int left   = -RECT_WIDTH;
  int top    = (m_pEnv->Height() - width) / 2;
  int bottom = m_pEnv->Height() - top;

  Set(left, top, right, bottom);

  m_line[0].Set(right, bottom, left, bottom);
  m_line[1].Set(right, top, left, top);
  m_line[2].Set(right, bottom, right, m_pEnv->Width() + RECT_WIDTH);
  m_line[3].Set(right, top, right, -RECT_WIDTH);
  m_lines = 4;
}


////////////////////////////////////////////////////////////////////
// TopSide
//
//
//
//
void TopSide::SetConnect(int width)
{
  m_left   = (m_pEnv->Width() - width) / 2;
  m_right  = m_pEnv->Width() - m_left;
  m_top    = -RECT_WIDTH;
  m_bottom = 0;

  Set(m_left, m_top, m_right, m_bottom);
  m_line[0].Set(m_left, m_bottom, m_left, m_top);
  m_line[1].Set(m_right, m_bottom, m_right, m_top);
  m_line[2].Set(m_left, m_bottom, -RECT_WIDTH, m_bottom);
  m_line[3].Set(m_right, m_bottom, m_pEnv->Width() + RECT_WIDTH, m_bottom);
  m_lines = 4;
}


void BottomSide::SetConnect(int width)
{
  m_left   = (m_pEnv->Width() - width) / 2;
  m_right  = m_pEnv->Width() - m_left;
  m_top    = m_pEnv->Height();
  m_bottom = m_pEnv->Height() + RECT_WIDTH;

  Set(m_left, m_top, m_right, m_bottom);
  m_line[0].Set(m_left, m_top, m_left, m_bottom);
  m_line[1].Set(m_right, m_top, m_right, m_bottom);
  m_line[2].Set(m_left, m_top, -RECT_WIDTH, m_top);
  m_line[3].Set(m_right, m_top, m_pEnv->Width() + RECT_WIDTH, m_top);
  m_lines = 4;
}



