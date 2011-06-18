 ////////////////////////////////////////////////////////////////////
//
// Genotype
//
#ifndef Genotype_h
#define Genotype_h

#include "rand.h"
#include "PostData.h"

class Biot;

union CSTATE;

const int MAX_RATIO      = 20;
const int UNI_RATIO      = 5;
const int MAX_SEGMENTS   = 10;  // The number of segments which can make up a line
const int MAX_SYMMETRY   = 8;   // The number of lines allowed
const int MAX_GENES      = MAX_SYMMETRY * MAX_SEGMENTS; // Maximum number of line segments
const int MAX_LIMB_TYPES = 4;   // The number of distinct limb types allowed


///////////////////////////////////////////////////////////////////
// GeneSegment
//
//
const int DIM_COLOR  = 5;
enum {
  GREEN_LEAF = 0,
  BLUE_LEAF,
  RED_LEAF,
  LBLUE_LEAF,
  WHITE_LEAF,
  DARK_GREEN_LEAF,
  DARK_BLUE_LEAF,
  DARK_RED_LEAF,
  DARK_LBLUE_LEAF,
  GREY_LEAF,
  YELLOW_LEAF,
  BLACK_LEAF,
  PURPLE_LEAF,
  MAX_LEAF
};


enum {
	CONTACT_IGNORE = 0,
	CONTACT_EAT,
	CONTACT_EATEN,
	CONTACT_DESTROY,
	CONTACT_DESTROYED,
	CONTACT_DEFEND,
	CONTACT_DEFENDED,
	CONTACT_ATTACK
};

///////////////////////////////////////////////////////////////////
// GeneSegment
//
// A segment represents a line
//
class GeneSegment : Randomizer
{
  private:
    BYTE   m_color[2];
    BYTE   m_visible;
	BYTE   m_radius;
	short  m_angle;
	BYTE   m_startSegment;

	enum {
		MAX_SEGMENT_LENGTH = 16
	};

    static const short redrawAngle[MAX_SEGMENT_LENGTH + 1];
	
    
  public:

    void Randomize(int segment, BOOL bIsVisible);
    void Debug(int segment, BOOL bIsVisible);
    void Mutate(int chance, int segment);
	virtual void Serialize(CArchive& ar);

	double GetRadius() { return (double) m_radius; }
	double GetAdjustedRadius(BYTE radius) { return (double) (m_radius - radius); }
	BYTE   GetRawRadius() { return m_radius; }
	short  GetColor(int sex)  { return m_color[sex]; }
	int    GetAngle()  { return m_angle;           }
    BOOL   IsVisible() { return m_visible;         }
	int    GetStart()  { return m_startSegment;    }

	// Looks at the radius and angle difference to determine
	// if a line should be redrawn
	BOOL   ShouldRedraw(short angleDifference) {
			return (redrawAngle[m_radius] <= abs(angleDifference));
	}
};


enum {
  DEFAULT_TYPE_TRAIT,
  COLOR_TRAIT,
  PARENT_TRAIT,
  SIBLING_TRAIT,
  DISPERSE_TRAIT,
  CHILDREN_TRAIT,
  LINES_TRAIT,
  MIRROR_TRAIT,
  LINE_SEGMENTS_TRAIT,
  ADULT_TRAIT,
  SPECIES_TRAIT,
  MAX_TRAIT
};

/*enum {
  NO_COLOR_CHANGE,
  LINE_COLOR_CHANGE,
  GENE_COLOR_CHANGE,
  ALL_COLOR_CHANGE,
  MAX_COLOR_CHANGES
};*/


///////////////////////////////////////////////////////////////////
// GeneLimb
//
// A limb is made up of segments

class GeneLimb : Randomizer
{
public:
    void Randomize(int nSegmentsPerArm);
    void Debug(int nSegmentsPerArm);
    void Mutate(int chance);
    void Crossover(GeneLimb&  gLine);
    virtual void Serialize(CArchive& ar);

	GeneSegment& GetSegment(int segment) { return m_segment[segment]; }
	int GetSegmentsVisible();

	void ToggleSegments();
	bool m_toggleVisibleSegments[MAX_SEGMENTS];

private:
	GeneSegment m_segment[MAX_SEGMENTS];
};


///////////////////////////////////////////////////////////////////
// GeneTrait
//
// A regular female cannot reproduce without a male.
// An asexual female can reproduce with or without a male.
// An asexual male will never mate.
//
// A male has a different adult ratio than a female - so
// it can be bigger or smaller.
//

class GeneTrait : Randomizer
{
  private:
    BYTE  m_disperse;        // Do the children disperse when born?
	BYTE  m_children;        // How many babies?
    BYTE  m_attackChildren;  // Does the parent attack children
    BYTE  m_attackSiblings;  // Do sibling attack each other
    BYTE  m_species;         // What species is this (0 - 15)
    BYTE  m_adultRatio[2];   // How large does this adult get
    BYTE  m_lineCount;      // How many lines
 	BYTE  m_lineRef[MAX_SYMMETRY];
    BYTE  m_mirrored;        // Are the lines mirrored
	BYTE  m_sex;             // male(1) or female(0)
	BYTE  m_asexual;         // yes or no
	BYTE  m_chanceMale;      // Determines the chance a biot is a male
    short m_offset;          // Born with an initial orientation
	short m_maxAge;          // Max age gene (1440 * (m_maxAge + 1))
	GeneLimb m_geneLine[MAX_LIMB_TYPES]; 

	//Not part of genetic code - just a cache of angles
	short m_angle[MAX_SYMMETRY][MAX_SEGMENTS];
	

    static const int mirrorCoef[MAX_SYMMETRY];
    static const int mirrorAngle[MAX_SYMMETRY];
    static const int mirrorSix[MAX_SYMMETRY];

  public:
    void Randomize(int nArmsPerBiot, int nTypesPerBiot, int nSegmentsPerArm);
    void Debug(int nArmsPerBiot, int nTypesPerBiot, int nSegmentsPerArm);
    void Mutate(int chance);
    void Crossover(GeneTrait&  gTrait);
	virtual void Serialize(CArchive& ar);
	void CalculateAngles();
	void PickSex()                  { m_sex = ((int) m_chanceMale > Integer(255));  }
	void SetChanceMale(BYTE chanceMale) { m_chanceMale = chanceMale; }
	int  GetChanceMale() { return (int) m_chanceMale; }

	int  GetLineMirror(int nLine)   { return mirrorCoef[nLine];   }
    int  GetNumberOfChildren(void)  { return m_children;          }
    int  GetAttackChildren(void)    { return m_attackChildren;    }
    int  GetAttackSiblings(void)    { return m_attackSiblings;    }
    int  GetSpecies(void)           { return m_species;           }
    void SetSpecies(int nSpecies)   { m_species = (BYTE) nSpecies;}
    int  GetAdultRatio()            { return m_adultRatio[m_sex]; }
    int  GetLines(void) const       { return m_lineCount;         }
	int  GetLineTypeIndex(int line) { return m_lineRef[line];     }
	int  GetCompressedToggle(int nAngle, int nLine, int nSegment);

    short GetOffset(void)           { return m_offset;            }
    short GetAngle(int line, int segment) { return m_angle[line][segment]; }
 
	BOOL GetDisperseChildren(void)  { return m_disperse;          }
    BOOL IsMirrored(void)           { return m_mirrored;          }
	BOOL IsAsexual(void)            { return m_asexual;           }
	BOOL IsMale()                   { return m_sex;               }
	BOOL IsLineTypeVisible(int lineType);
	DWORD GetMaxAge(void)           { return (1280 * (DWORD) (m_maxAge + 1)); }

	void SetMale(bool bMale)        { if (bMale) m_sex = 1; else m_sex = 0; }
	void SetAsexual(bool bAsexual)  { if (bAsexual) m_asexual = 1; else m_asexual = 0; }
	void SetMaxAge(short maxAge)    { m_maxAge = maxAge; }

	GeneSegment& GetSegment(int line, int segment)
	{
		ASSERT(line < MAX_SYMMETRY);
		return m_geneLine[m_lineRef[line]].GetSegment(segment);
	}

	GeneSegment& GetSegmentType(int lineType, int segment)
	{
		ASSERT(lineType < MAX_LIMB_TYPES);
		return m_geneLine[lineType].GetSegment(segment);
	}

	GeneLimb& GetLineType(int lineType)
	{
		ASSERT(lineType < MAX_LIMB_TYPES);
		return m_geneLine[lineType];
	}
};


#endif