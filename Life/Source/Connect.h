 ///////////////////////////////////////////////////////////////////////////////
//
// Connect
//
// This class is responsible for establishing and controlling connections to
// other worlds.
//
//

#ifndef connect_h
#define connect_h

#include "Etools.h"
#include "PostData.h"

class Biot;

////////////////////////////////////////////////////////////////
// Fifo
//
//
class Fifo
{
  public:
      enum {
        QUEUE_SIZE = 5
      };

      Fifo();
      ~Fifo();

//      int Count();

 //     int FreeCount() 
 //     {
 //       return QUEUE_SIZE - Count();
 //     }

      BOOL   Put(LPVOID pClass);
      LPVOID Get();
//	  LPVOID Peek();
		void Empty();

                 
  private:
      int    m_read;
      int    m_write; 
      int    m_size;
      LPVOID m_pVoid[QUEUE_SIZE];
};


//////////////////////////////////////////////////////////////////////
//
// Side
//
//
class Side : public BRect
{
  public:
    Side();
    
    // We want this width larger than the largest biot width
    enum {
      RECT_WIDTH = 200
    };

    enum {
      LEFT   = 0,
      RIGHT  = 1,
      TOP    = 2,
      BOTTOM = 3
    };

    void Clear(BRect* pEnvRect);
    virtual void SetConnect(int width) = 0;

    int IsIntersect(CLine& bLine, int& x, int& y)
    {
      for (int i = 0; i < m_lines; i++)
        if (m_line[i].Intersect(bLine, x, y))
          return TRUE; 
    
      return FALSE;
    }

    BOOL Export(Biot* pBiot);
    virtual CPostData* Import();

   void SendBiots(Side& otherSide);
    BOOL RecvBiot(CPostData* pData);
    Biot* GetBiot();

    virtual void AdjustBiot(Biot& biot) = 0;
    virtual void RejectBiot(Biot& biot) = 0;
    virtual int  SideSize() = 0;
	virtual void SetSide(BRect* pEnvRect) = 0;

    protected:
      int m_lines;
      CLine m_line[4];
      BRect* m_pEnv;
      Fifo m_inComing;
      Fifo m_outGoing;
};


class RightSide : public Side
{
  public:
    void SetSide(BRect* pEnvRect);
    void SetConnect(int width);
    void AdjustBiot(Biot& biot);
    void RejectBiot(Biot& biot);
    int  SideSize();
};


class LeftSide : public Side
{
  public:
    void SetSide(BRect* pEnvRect);
    void SetConnect(int width);
    void AdjustBiot(Biot& biot);
    void RejectBiot(Biot& biot);
    int  SideSize();
};


class TopSide : public Side
{
  public:
    void SetSide(BRect* pEnvRect);
    void SetConnect(int width);
    void AdjustBiot(Biot& biot);
    void RejectBiot(Biot& biot);
    int  SideSize();
};


class BottomSide : public Side
{
  public:
    void SetSide(BRect* pEnvRect);
    void SetConnect(int width);
    void AdjustBiot(Biot& biot);
    void RejectBiot(Biot& biot);
    int  SideSize();
}; 




#endif