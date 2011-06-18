
#ifndef biots_h
#define biots_h

#include "etools.h"
#include "vector.h"
#include "genotype.h"
#include "Brain.h"

// Sensor Definitions
//
// bit
// 1	


#define DSTERASE  (DWORD)0x00220326

 //////////////////////////////////////////////////////////////////////
//
// Line Equations
//  m = (y1 - y2) / (x1 - x2) 
//
//  b = (y2x1 - y1x2)/ (x1 - x2)
//
//  x = (b2 - b1) / (m1 - m2)
//

//  y = (b2m1 - b1m2) / (m1 - m2)
//
//  y = mx + b

class Motion
{
  public:
    BYTE period;
    BYTE nPeno;
    BYTE frequency;

    double dx;
    double dy;
    double dr;
};

////////////////////////////////////////////////////////////////////
// Collision
//
//
const int MAX_COLLISIONS = 5;

class Collision
{
  public:
    void setId(int ID) { id = ID; hits = 0; }
    void setSeen(DWORD age) { seenToggle = (short) age; }
    BOOL wasSeen(DWORD age) { return (seenToggle == (short) age); }
    int  addHit(void) { return ++hits; }
    int   id;
    short hits;
    short seenToggle;
};



//////////////////////////////////////////////////
// CRedraw
//
// Biot processes determine if a segment needs
// to be redrawn.  If so, it marks it for redrawing
//
// We use a union to accelerate our check, checking
// 4 bytes at a time.
//
class CRedraw
{
public:
	CRedraw() { ClearRedraw(); }

	BOOL ShouldRedraw() { return m_bShouldRedraw; }

	void SetRedraw(BOOL bRedraw = TRUE)
	{
		if (bRedraw)
			m_bShouldRedraw = TRUE;
	}

	void ClearRedraw() { m_bShouldRedraw = FALSE; }

private:
	BOOL m_bShouldRedraw;
};


//////////////////////////////////////////////////////////////////////
// Biot
//
// Biot State Variables
//
class Biot: public BRectItem
{
  public:
	Biot(Environment& environment);
	~Biot(void);
	
	int m_bitmapWidth;
	int m_bitmapHeight;
	void CheckBitmapSize(int width, int height);

	enum {
		IS_HUNGRY = 0,  // Histeresis
		IS_INJURED,     // Any injury
		IS_LOW_ENERGY,	// Has less than genetic energy
		IS_FERTILE,		// Is fertile
		IS_MALE,		// Is a male
		IS_ASEXUAL,		// Is asexual
		IS_OLD,			// upper genetic
		IS_YOUNG,		// lower genetic
		IS_ADULT,		// Full grown
	};

	enum {
		PERCENT_AGE,
		PERCENT_ENERGY,
	};

	// Biot Naming
	CString GetName();
	CString GetWorldName()           { return m_sWorldName;  }
	CString GetFullName();
	CString GetFatherName();
	void SetName(CString sName);
	void SetWorldName(CString sName) { m_sWorldName = sName; }


	void Mutate(int chance);

    void ClearSettings(void);
    void Draw(void);
    void SetErasePosition(void);
    void Erase(void);
    void FormBitmap(int pen = -1);
    void FormMask(void);
    void FreeBitmaps();
	enum {
		NORMAL,
		REDRAW,
		REFORM,
		RECALCULATE,
		GROW
	};
	void PrepareErase(int operation);
	void PrepareDraw(int operation);
	void EraseAndDraw(int operation);
    void WallBounce(int x, int y);
	
	void Motion(const double deltaX, const double deltaY, double Vx, double Vy, const double radius);

    void MoveBiot(int x, int y);

	void SetScreenRect() 
	{
		Set(leftX + origin.x, topY + origin.y, rightX + origin.x + 1, bottomY + origin.y + 1);
	}

#if defined(_DEBUG)
	void Trace(LPCSTR s) { if (m_Id == 22) TRACE(s); }
	void Trace(LPCSTR s, int value) { if (m_Id == 22) TRACE(s, value); }
#else
	void Trace(LPCSTR) {};
	void Trace(LPCSTR, int) {};
#endif

    BOOL Move(void);

	void  Reject(int side);
    int   RandomCreate(int nArmsPerBiot, int nTypesPerBiot, int nSegmentsPerArm);
    int   Initialize(BOOL bRandom = FALSE);
    int   Contacter(Biot* enemy, int dx, int dy, int& x, int& y);
	virtual void  Serialize(CArchive& ar);

    short LengthLoss(int nPeno, short delta);
    
    long Distance(POINT& start, POINT& stop);
    LONG Symmetric(int aRatio);
    LONG LineContact(int line, int eline, Biot* enemy, short* delta);
    BOOL AdjustState(int index, short delta);
	BOOL OnOpen();
  
	short Translate(double radius, double& newX, double& newY, int degrees, int aRatio);
    
    Biot& operator=(Biot& copyMe);

    int      newType;
    DWORD    m_age;
	DWORD    m_maxAge;

    BOOL     bDie;
    LONG     energy;
    LONG     childBaseEnergy;

    Collision collider[MAX_COLLISIONS];
    void RemoveCollisions(DWORD age);
    int  FindCollision(int enemyId);
    int  AddCollision(int enemyId);
    void ClearCollisions(void){for(int i = 0; i < MAX_COLLISIONS; i++) collider[i].id = -1; }

    Vector   vector;


	short    m_angle[MAX_GENES];
	short    m_angleDrawn[MAX_GENES];

	short    m_angleLimbType[MAX_LIMB_TYPES];
	short    m_angleLimbTypeDrawn[MAX_LIMB_TYPES];

	short    m_angleLimb[MAX_SYMMETRY];
	short    m_angleLimbDrawn[MAX_SYMMETRY];

	short    m_angleLimbTypeSegment[MAX_LIMB_TYPES][MAX_SEGMENTS];
	short    m_angleLimbTypeSegmentDrawn[MAX_LIMB_TYPES][MAX_SEGMENTS];

	short    MoveLimbTypeSegment(int nSegment, int nLimbType, int nRate);
	short    MoveLimbTypeSegments(int nLimbType, int nRate);
	short    MoveLimbSegments(int nLimbType, int nRate);
	short    MoveLimbSegment(int nSegment, int nLimb, int nRate);

	// The retraction actually drawn
	// The retraction requested
	// The segment being retracted
	short   m_retractDrawn[MAX_SYMMETRY];
	short   m_retractRadius[MAX_SYMMETRY];
	short   m_retractSegment[MAX_SYMMETRY];

	// Retract or extend the tip of a limb
	BYTE    RetractLine(int nSegment, int nLimb, int maxRadius);
	BYTE    ExtendLine(int nSegment, int nLimb);
	BYTE    RetractLimbType(int nSegment, int nLimbType, int maxRadius);
	BYTE    ExtendLimbType(int nSegment, int nLimbType);


	CommandLimbStore m_store[MAX_SYMMETRY];

	CommandArray m_commandArray;
	CommandArray m_commandArray2;
	int          m_internalState;

	BYTE     geneNo[MAX_GENES];
	BYTE     lineNo[MAX_GENES];

	BOOL     bTerminateEvent;


	BOOL     m_nSick;
	BOOL     m_bDrawn;
	BOOL     m_bSelected;

	void MoveArm(int nPeno, short degree);
	void MoveSegment(int nPeno, short degree);
	void SeekSegment(int nPeno, short degree, short offset);
	void SeekArm(int nPeno, short rate, short offset);
	void IncreaseAngle(int nPeno, short rate);
	BOOL MoveLineType(int nLineType, short rate, short offset);
	BOOL MoveSegmentType(int nLineType, int nSegment, short rate, short offset);
	BOOL MoveLine(int nLine, short rate, short offset);

	CRedraw redraw;

    POINT     startPt[MAX_GENES];
    POINT     stopPt[MAX_GENES];
    short     distance[MAX_GENES];
    GeneTrait trait;

	BOOL IsSegmentDamaged(int nPeno) { return (state[nPeno] != distance[nPeno]); }
	BOOL IsSegmentMissing(int nPeno) { return (state[nPeno] <= 0); }
	short GetSegmentLength(int nPeno) { return distance[nPeno]; }

	void CheckReproduction();

	// Bonus is related to your total size
	void SetBonus() { m_dBonusRatio = ((double)Area()) / 40000.0; }

private:
    Environment& env;

public:
	// Do not save these variables
	float m_statEnergy[MAX_ENERGY_HISTORY];
	int  m_statIndex;

    HBITMAP  m_hBitmap;

	// Normal Variables
//	GeneEvent	 event[GeneEvent::EVENT_MAX];
	CString      m_sName;
	CString      m_sWorldName;
	CString      m_sFatherName;
	CString      m_sFatherWorldName;

    // Genes stored from mating
//	GeneResponse        response2[GeneEvent::RESPONSE_GROUPS];
//	GeneCommandGroup    group2[GeneCommandGroup::COMMAND_GROUPS];
//	GeneEvent           event2[GeneEvent::EVENT_MAX];
    GeneTrait           trait2;

	double m_dBonusRatio;		// Storage for the bonus ratio
	int    m_livingChildren;	// The number of living children this biot has
	int    m_totalChildren;		// The number of childen this biot has contributed to
	DWORD  m_fatherId;          // The Id of the father (if any)
    DWORD  m_motherId;
	DWORD  m_mateId;
    DWORD  m_Id;
	DWORD  m_generation;		// Indicates how far down the line a child is
	DWORD  m_fatherGeneration;

    POINT    origin;
    short    nType[MAX_GENES];
    short    state[MAX_GENES];
//    BYTE     typeState[MAX_GENES];
	BYTE     color[MAX_GENES];

    BOOL     bInjured;
    int      genes2;
    int      topY;
    int      bottomY;
    int      leftX;
    int      rightX;
    int      ratio;
    int      lastType;
    int      lastLeft;
    int      lastTop;
    int      max_genes;
    int      genes;
    long     turnBenefit;
    long     totalDistance;
    long     colorDistance[WHITE_LEAF + 1];
    long     adultBaseEnergy;
    long     stepEnergy;
    int  PlaceRandom(void);
    BOOL PlaceNear(Biot& parent);
    BOOL ChangeDirection(Biot* enemy, int dx, int dy);
    void SetRatio(void);
    void SetSymmetry(void);
    void CopyGenes(Biot& enemy);
    void InjectGenes(int type, Biot& enemy);

	float PercentEnergy();

 
	short CheckWhite(int type)
	{
		// If IsMale(), 
		return (short) ((type == WHITE_LEAF && (env.options.nSexual == 2 || (env.options.nSexual == 3 && trait.IsAsexual())))?GREEN_LEAF:type);
//		return (short) ((type == WHITE_LEAF && !trait.IsMale() && trait.IsAsexual())?GREEN_LEAF:type);
	}

    int x1(int gene)
    {
      return startPt[gene].x + origin.x;
    }

    int x2(int gene)
    {
      return stopPt[gene].x + origin.x;
    }

    int y1(int gene)
    {
      return startPt[gene].y + origin.y;
    }

    int y2(int gene)
    {
      return stopPt[gene].y + origin.y;
    }

    BOOL AreSiblings(Biot& enemy){ return (m_motherId == enemy.m_motherId && m_motherId != 0); }

    BOOL OneIsChild(Biot& enemy){ return (m_Id == enemy.m_motherId || enemy.m_Id == m_motherId); }

    BOOL SiblingsAttack(Biot& enemy) { return (trait.GetAttackSiblings() || enemy.trait.GetAttackSiblings()); }

	BOOL ContactLine(Biot& enemy, int nEnemyPeno, int nPeno, short& delta, long& deltaEnergy);

    BOOL AttackChildren(Biot& enemy) { return (trait.GetAttackChildren() ||
                                              enemy.trait.GetAttackChildren()); }
    // Species match if there id is within one of each other.  There are
    // 16 identifiers possible.  This allows species to drift apart.  Otherwise
    // a mutation of id would case a single creature to be alone.
    BOOL SpeciesMatch(int enemySpecies) {
      enemySpecies = abs (enemySpecies - trait.GetSpecies());
      return (enemySpecies <= 1 || enemySpecies >= 15);
    }

	double PercentColor(int color) { return ((double) colorDistance[color]) / ((double) totalDistance); }

    int BaseRatio(void) { return ratio - (trait.GetAdultRatio() - 1); }

    void  ValidateBorderMovement(double& dx, double& dy);

	short minShort(int a, short b)
	{
		return (((short)a)<b?((short)a):b);
	}
};


//
// Move the bounding rectangle and the origin
//
inline void Biot::MoveBiot(int x, int y)
{
	Offset(x, y);
	origin.x += x;
	origin.y += y;
}


//////////////////////////////////////////////////////////////////////
// PrepareDraw
//
// For a new bitmap if required.
//
inline void Biot::PrepareDraw(int operation)
{
	if (operation != NORMAL)
		FormBitmap(newType);
}

#endif