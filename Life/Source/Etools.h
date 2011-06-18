//////////////////////////////////////////////////////////////////////
// etools
//
// A collection of somewhat generic tools.
//
//

#ifndef etools_h
#define etools_h


#include <math.h>
#include <float.h>
const double PI      = 3.1415926535898;



//////////////////////////////////////////////////////////////////////
// BRect
//
// A rectangle
//
class BRect
{
public:
	int m_left;
	int m_right;
	int m_bottom;
	int m_top;

public:

	BRect(const BRect* pRect) { *this = *pRect; }
	BRect(const RECT* pRect)  { Set(pRect);     }
	BRect() : m_left(0), m_right(0), m_top(0), m_bottom(0) { };

	void GetRECT(RECT* pRect)
	{  
		pRect->left   = m_left;
		pRect->right  = m_right;
		pRect->top    = m_top;
		pRect->bottom = m_bottom;
	}

	int Width(void)   {return m_right  - m_left;}
	int Height(void)  {return m_bottom - m_top; }

	int Left(void)    {return m_left;  }
	int Top(void)     {return m_top;   }
	int Bottom(void)  {return m_bottom;}
	int Right(void)   {return m_right; }

	void  Normalize(void);

	int Touches(const BRect& c) const
	{
		return (c.m_right  < m_left  ||
				c.m_left   > m_right ||
				c.m_bottom   < m_top   ||
				c.m_top    > m_bottom)?0:1;
	} 

	int IsContainedBy(const BRect& c) const
	{
		return (m_left   >= c.m_left &&
				m_right  <= c.m_right &&
				m_top    >= c.m_top   &&
				m_bottom <= c.m_bottom)?1:0;
	}

	bool Inside(int x, int y)
	{
		return !(x < m_left || x > m_right || y > m_bottom || y < m_top);
	}

	int Area() { return Width() * Height(); }

	int Contains(int x, int y);

	void Set(int newLeft, int newTop, int newRight, int newBottom)
	{
		if (newTop > newBottom)
		{
			m_top    = newBottom;
			m_bottom = newTop;
		}
		else
		{
			m_top    = newTop;
			m_bottom = newBottom;
		}

		if (newLeft > newRight)
		{
			m_left  = newRight;
			m_right = newLeft;
		}
		else
		{
			m_left  = newLeft;
			m_right = newRight;
		}
	}


	void  Set(const RECT* pRect)
	{
		m_left   = pRect->left;
		m_right  = pRect->right;
		m_top    = pRect->top;
		m_bottom = pRect->bottom;
		Normalize();
	}

	void Offset(int dx, int dy)
	{
		m_left   += dx;
		m_right  += dx;
		m_top    += dy;
		m_bottom += dy;
	}

	int CenterX(void) const
	{
		return (m_right + m_left) >> 1;
	}

	int CenterY(void) const
	{
		return (m_bottom + m_top) >> 1;
	}

	POINT& Center(POINT& xy) const
	{
		xy.x = CenterX();
		xy.y = CenterY();
		return xy;
	}
  
	int OffCenterX(BRect &c) const
	{
		return ((m_right + m_left) - (c.m_right + c.m_left)) >> 1;
	}
	
	int OffCenterY(BRect &c) const
	{
		return ((m_bottom + m_top) - (c.m_bottom + c.m_top)) >> 1;
	}

	virtual void Serialize(CArchive& ar);

	// Rounds a number up to the next size
	int NextSize(int value);  

	const BRect& operator=(const BRect& rect)
	{ 
		m_left   = rect.m_left;
		m_right  = rect.m_right;
		m_top    = rect.m_top;
		m_bottom = rect.m_bottom;
		return *this;
	}

	bool operator==(const BRect& rect) const
	{ 
		return (m_left == rect.m_left &&
				m_right == rect.m_right &&
				m_bottom == rect.m_bottom &&
				m_top    == rect.m_top);
	}

    BRect operator-(BRect& rect) const
	{ 
		BRect srect(this);
		srect.m_left   -= rect.m_left;
		srect.m_right  -= rect.m_right;
		srect.m_top    -= rect.m_top;
		srect.m_bottom -= rect.m_bottom;
		return srect;
	}
};


////////////////////////////////////////////////////////////////////////
// BRectItem
//
// A BRectItem is a rectangle that is tracked withing a two dimensional
// sorted array.  
//
class BRectItem : public BRect
{
public:
	// Constructors
	BRectItem(RECT* pRect) : BRect(pRect), m_indexLeft(-1),
		m_indexRight(-1), m_indexTop(-1), m_indexBottom(-1) { };

	BRectItem() : BRect(), m_indexLeft(-1),
		m_indexRight(-1), m_indexTop(-1), m_indexBottom(-1) { };

	bool IsSame(BRectItem* pItem) { return pItem == this; }

public:
	// From a BRectItem, I need to be able to tell where that
	// it appears in BRectSort (for each BRectArray)
	int m_indexLeft;
	int m_indexRight;
	int m_indexTop;
	int m_indexBottom;
};

	
//////////////////////////////////////////////////////////////////////////
// BRectArray
//
// Sorts one array of BRectItems based on a single coordinate.
// An abstract base class for BRectArray[Left, Rigth, Top, Bottom]
//
class BRectArray : public CTypedPtrArray<CPtrArray, BRectItem*>
{
public:
	// Incrementally perform updates to the array
	void InsertRect(BRectItem* pItem);
	void MoveRect(BRectItem* pItem);
	void RemoveRect(BRectItem* pItem);

	// Used to determine if a biot at a position really is
	// the m_right biot
	bool ValidateRect(int nPos, BRectItem* pItem);
	void TraceDebug(int nIndex);

	// Returns a spot in the array whereby everything is 
	// guarranteed to be greater than the coordinate.
	int GreaterPos(int nCoordinate);

	// Returns a spot in the array whereby everything that
	// is equal or greater than the coordinate.
	int GreaterOrEqualPos(int nCoordinate);

	// Sorts the array - returns TRUE if the array actually
	// required sorting.
	bool Sort();

	// For adding rects to the array without sorting
	// Call Sort after adding multiple members
	void AddRect(BRectItem* pItem);

	//Given a BRectItem:
	//	Determine the grid coordinate
	virtual int  Coordinate(BRectItem* pItem) = 0;
	//	Determine the array position
	virtual int  GetPos(BRectItem* pItem) = 0;
	//	Set the array position
	virtual void SetPos(BRectItem* pItem, int nIndex) = 0;

protected:

	// Recursive version of the sort method
	bool Sort(int nStartIndex, int nStopIndex);
};

// Simply call the recursive method
inline bool BRectArray::Sort()
{
	return Sort(0, GetSize());
}


///////////////////////////////////////////////////////////////////
//  BRectArray[Left, Right, Top, Bottom]
//
// Subclasses that automatically handle the coordinate and position
// they are responsible for.  
//
class BRectArrayLeft : public BRectArray
{
public:
	int  Coordinate(BRectItem* pItem) { return pItem->m_left; }
	int  GetPos(BRectItem* pItem)   { return pItem->m_indexLeft; }
	void SetPos(BRectItem* pItem, int nIndex) { pItem->m_indexLeft = nIndex; }
};

class BRectArrayRight : public BRectArray
{
public:
	int  Coordinate(BRectItem* pItem) { return pItem->m_right; }
	int  GetPos(BRectItem* pItem)   { return pItem->m_indexRight; }
	void SetPos(BRectItem* pItem, int nIndex) { pItem->m_indexRight = nIndex; }
};

class BRectArrayTop : public BRectArray
{
public:
	int  Coordinate(BRectItem* pItem) { return pItem->m_top; }
	int  GetPos(BRectItem* pItem)   { return pItem->m_indexTop; }
	void SetPos(BRectItem* pItem, int nIndex) { pItem->m_indexTop = nIndex; }
};

class BRectArrayBottom : public BRectArray
{
public:
	int  Coordinate(BRectItem* pItem) { return pItem->m_bottom; }
	int  GetPos(BRectItem* pItem)   { return pItem->m_indexBottom; }
	void SetPos(BRectItem* pItem, int nIndex) { pItem->m_indexBottom = nIndex; }
};


// Forward declaration
class BRectSortPos;

//////////////////////////////////////////////////////////////////////
// BRectSort
//
// Keeps track of rectangles (does not manage memory) in such a manner
// as to efficiently determine which rectangles are within or enter into
// an arbitrary rectangle or touch a point.
//
class BRectSort
{
public:
	// There are four array indices defined for BRectArray
	enum {
		ARRAY_LEFT    = 0,
		ARRAY_TOP     = 1,
		ARRAY_RIGHT   = 2,
		ARRAY_BOTTOM  = 3,
		ARRAY_MAX     = 4
	};

	// Constructor
	BRectSort(); 

	// On by default, you can turn it off temporarily for faster adds
	void SetIncrementalSort(bool bIncrementOn = true);

	// Returns true if a sort was necessary (good for debugging)
	bool SortAll();

	// Add a rectangle
	void Add(BRectItem* pItem);

	// Indicate a rectangle has moved
	void Move(BRectItem* pItem);

	// If you know your original position - this one is more efficient
	void Move(BRectItem* pItem, BRect* pOrigRect);

	// Remove a rectangle
	void Remove(BRectItem* pItem);

	// Searching methods
	// Find all rectangles that contain a point
	// Find all rectangles that intersect with this rectangle
	// Find all rectangles contained by this rectangle
	BRectItem* IterateRects(BRectSortPos& sortPos);

	void FreeAll();
	void TraceDebug();

//	int InterateRectsByIndex(BRectSortPos& sortPos);


protected:	

	bool FindShortestArray(BRectSortPos& sortPos, int nLeft, int nTop, int nRight, int nBottom);

	BRectArrayLeft   m_arrayLeft;
	BRectArrayRight  m_arrayRight;
	BRectArrayTop    m_arrayTop;
	BRectArrayBottom m_arrayBottom;
	BRectArray*      m_pArray[ARRAY_MAX];
	bool             m_bIncrementalSort;
};


/*inline int BRectSort::InterateRectsByIndex(BRectSortPos& sortPos)
{
	BRectItem* pItem = InterateRects(sortPos);
	return pItem->
*/

// Allows multiple items to be added, moved or removed without sorting each time
// Don't forget to set it back on!
inline void BRectSort::SetIncrementalSort(bool bIncrementOn)
{
	if (!m_bIncrementalSort && bIncrementOn)
	{
		// Make sure we re-sort now that we are on.
		SortAll();
	}
	m_bIncrementalSort = bIncrementOn;
}


//////////////////////////////////////////////////////////////////////
// BRectSortPos
//
// Keeps track of iterations for the searching methods 
//
class BRectSortPos : public BRectItem
{
public:
	BRectSortPos() : m_nType(TYPE_INVALID) {};

	void FindRectsInPoint(const int x, const int y);
	void FindRectsIntersecting(const BRect& rect);
	void FindRectsContained(const BRect& rect);

	friend BRectSort;

protected:
	enum {
		TYPE_POINT,
		TYPE_INTERSECT,
		TYPE_CONTAINED,
		TYPE_INVALID
	};

	// What type of search?
	int  m_nType;

	// Is this the first time?
	bool m_bInitialized;

	int m_nShortIndex;

	int m_nIndex[BRectSort::ARRAY_MAX];
};


inline void BRectSortPos::FindRectsInPoint(const int x, const int y)
{
//	m_nIndex[ARRAY_LEFT]   = rect.m_left;
//	m_nIndex[ARRAY_TOP]    = rect.m_top;
	m_left         = x;
	m_top          = y;
	m_bInitialized = false;
	m_nType        = TYPE_POINT;
}


inline void BRectSortPos::FindRectsIntersecting(const BRect& rect)
{
//	m_nIndex[ARRAY_LEFT]   = rect.m_left;
//	m_nIndex[ARRAY_RIGHT]  = rect.m_right;
//	m_nIndex[ARRAY_TOP]    = rect.m_top;
//	m_nIndex[ARRAY_BOTTOM] = rect.m_bottom;

	*((BRect*) this)          = rect;
	m_bInitialized = false;
	m_nType        = TYPE_INTERSECT;
}


inline void BRectSortPos::FindRectsContained(const BRect& rect)
{
//	m_nIndex[ARRAY_LEFT]   = rect.m_left;
//	m_nIndex[ARRAY_RIGHT]  = rect.m_right;
//	m_nIndex[ARRAY_TOP]    = rect.m_top;
//	m_nIndex[ARRAY_BOTTOM] = rect.m_bottom;

	*((BRect*) this)          = rect;
	m_bInitialized = false;
	m_nType        = TYPE_CONTAINED;
}


//////////////////////////////////////////////////////////////////////
//
// CLine
//
//
// Equations:
//
// m = (y1 - y2) / (x1 - x2) 
//
// b = (y2x1 - y1x2)/ (x1 - x2)
//
// x = (b2 - b1) / (m1 - m2)
//
// y = (b2m1 - b1m2) / (m1 - m2)
//
// y = mx + b
//
class CLine
{
  public:
	CLine()
	{
		#if defined(_DEBUG)
		m_state = -1;
		#endif
	}

    CLine(int X1, int Y1, int X2, int Y2)
	{
		Set(X1, Y1, X2, Y2);
    }

	CLine(const POINT& p1, const POINT& p2)
	{
		Set(p1.x, p1.y, p2.x, p2.y);
	}

    CLine(BRect& brect)
	{
		Set(brect.m_left, brect.m_top, brect.m_right, brect.m_bottom);
    }

    CLine(RECT& rect)
	{
		Set(rect.left, rect.top, rect.right, rect.bottom);
    }

    void Set(int X1, int Y1, int X2, int Y2);

	// Starting from a point and slope, set the line
	// for a wide range (+/- 10000)
	void Set(int X1, int Y1, double slope);

    void Offset(int dx, int dy);

    bool Intersect(CLine &l, int& x, int& y) const;

    void   SetSlope();
	double GetSlope() const;
	double GetInvertedSlope() const;

	int outOfRangeY(int y) const
	{
		return (y > m_maxY || y < m_minY);
	}

    int outOfRangeX(int x) const
    {
		return (x > m_maxX || x < m_minX);
    }

    int Round(double dValue) const;

	// Returns the angle between the starting point
	// and the ending point in radians
	double Angle();

  private:
	void SetState();
	void MinMax();

	enum {
		STATE_NAN      = 1,
		STATE_INFINITE = 2,
		STATE_NORMAL   = 3
	};

	int m_state;
    int m_x1;
    int m_x2;
    int m_y1;
    int m_y2;
	int m_maxY;
	int m_minY;
	int m_maxX;
	int m_minX;
    double m_slope;
    double m_yIntercept;
};

inline void CLine::SetState()
{
	if (_isnan(m_slope))
	{
		m_state = STATE_NAN;
	}
	else
	{
		if (!_finite(m_slope))
		{
			m_state = STATE_INFINITE;
		}
		else
		{
			m_state = STATE_NORMAL;
		}
	}
}

inline void CLine::SetSlope()
{
	m_slope      = double(m_y1 - m_y2) / double(m_x1 - m_x2);
	m_yIntercept = double((m_y2 * m_x1) - (m_y1 * m_x2)) / double(m_x1 - m_x2);
	SetState();
}

inline int CLine::Round(double dValue) const
{
	return int((dValue < 0.0)?(dValue - 0.5):(dValue + 0.5));
}

inline double CLine::GetSlope() const
{
	return m_slope;
}

inline double CLine::GetInvertedSlope() const
{
	return  (-1 / m_slope);
}

inline void CLine::Offset(int dx, int dy)
{
	m_x1   += dx;
	m_x2   += dx;
	m_maxX += dx;
	m_minX += dx;

	m_y1   += dy;
	m_y2   += dy;
	m_maxY += dy;
	m_minY += dy;

	if (m_state == STATE_NORMAL)
		m_yIntercept += double(dy);
}

inline void CLine::MinMax()
{
	if (m_x1 > m_x2)
	{
		m_maxX = m_x1;
		m_minX = m_x2;
	}
	else
	{
		m_maxX = m_x2;
		m_minX = m_x1;
	}

	if (m_y1 > m_y2)
	{
		m_maxY = m_y1;
		m_minY = m_y2;
	}
	else
	{
		m_maxY = m_y2;
		m_minY = m_y1;
	}
}

inline double CLine::Angle()
{
	switch (m_state)
	{
	default:
		ASSERT(0);
	case STATE_NAN:
		return 0.0;

	case STATE_INFINITE:
		if (m_y1 > m_y2)
		{
			return -PI / 2;
		}
		else
		{
			return PI / 2;
		}

	case STATE_NORMAL:
		if (m_x1 > m_x2)
		{
			return atan(m_slope) + PI;
		}
		else
		{
			return atan(m_slope);
		}
	}
}

#endif