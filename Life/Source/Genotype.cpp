#include "stdafx.h"
#include "environ.h"
#include "biots.h"

////////////////////////////////////////////////////////
// GeneSegment
//
// Defines a segment of a line
//

// Take the distance around the circle and divide it into 360 degrees
// That gives you the number of degrees per unit (or pixel)
const short GeneSegment::redrawAngle[MAX_SEGMENT_LENGTH + 1] = 
{ 180, 23, 12, 10, 9, 8, 7, 6, 5, 5, 4, 4, 3, 3, 3, 3, 2 };
//{ 360, 45, 22, 15, 11, 9, 7, 6, 5, 5, 4, 4, 3, 3, 3, 3, 2 };
 
void GeneSegment::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_angle;
		ar >> m_radius;
		ar >> m_visible;
		ar >> m_startSegment;
		ar >> m_color[0];
		ar >> m_color[1];

		// A little crash protection
		if (m_radius > MAX_SEGMENT_LENGTH)
			m_radius = MAX_SEGMENT_LENGTH;

		if (m_color[0] >= DIM_COLOR - 1)
			m_color[0] = 0;

		if (m_color[1] >= DIM_COLOR)
			m_color[1] = 0;
	}
	else
	{
		ar << m_angle;
		ar << m_radius;
		ar << m_visible;
		ar << m_startSegment;
		ar << m_color[0];
		ar << m_color[1];
	}
}

void GeneSegment::Randomize(int segment, BOOL bIsVisible)
{
	// Perhaps we just do 16 next time, or 32
	m_radius = (BYTE)  (Integer(MAX_SEGMENT_LENGTH - 1) + 2);

	if (segment == 0)
		m_angle  = (short) (Integer(45) + 1);
	else
		m_angle  = (short) (Integer(350) - 175);
  
	m_visible = (BYTE) bIsVisible;

	m_startSegment = Byte(MAX_SEGMENTS);

	m_color[0]   = Byte(DIM_COLOR - 1);
//	if (m_color[0] != GREEN_LEAF)
//		m_color[0]  = Byte(DIM_COLOR);

	m_color[1]   = Byte(DIM_COLOR);
	if (m_color[1] != WHITE_LEAF && m_color[1] != GREEN_LEAF)
		m_color[1]  = Byte(DIM_COLOR);
}


// Settings to help debug a biot
void GeneSegment::Debug(int segment, BOOL bIsVisible)
{
	m_radius = (BYTE) MAX_SEGMENT_LENGTH;
	m_angle = 0;//5;
	// 0 and 45 put the legs right on top of each other for 8 legs

	m_visible = (BYTE) bIsVisible;
	m_startSegment = segment - 1;
	if (segment != 2)
	{
		m_color[0]   = GREEN_LEAF;
		m_color[1]   = WHITE_LEAF;
	}
	else
	{
		m_color[0]   = BLUE_LEAF;
		m_color[1]   = RED_LEAF;
	}
}


void GeneSegment::Mutate(int chance, int segment)
{
	if (Int1024() < chance)
		m_radius = (BYTE) (Integer(MAX_SEGMENT_LENGTH - 1) + 2);

	if (Int1024() < chance)
	{
		if (segment == 0)
			m_angle  = (short)(Integer(45) + 1);
		else
			m_angle = (short)(Integer(350) - 175);
	}

	if (Int1024() < chance)
		m_startSegment = Byte(MAX_SEGMENTS);

	if (Int1024() < chance)
		m_visible = !m_visible;
  
	if (Int1024() < chance)
		m_color[0]   = Byte(DIM_COLOR - 1);

	if (Int1024() < chance)
		m_color[1]   = Byte(DIM_COLOR);

}


////////////////////////////////////////////////////////
// GeneLimb
//
// Contains GeneSegments
//
void GeneLimb::Randomize(int nSegmentsPerArm)
{
	for (int i = 0; i < MAX_SEGMENTS; i++)
	{
		switch (nSegmentsPerArm)
		{
		default:
		case 0:
			m_segment[i].Randomize(i, Bool());
			break;

		case 1:
			m_segment[i].Randomize(i, (i < 3)?Bool():FALSE);
			break;

		case 2:
			if (i < 4)
				m_segment[i].Randomize(i, TRUE);
			else
				m_segment[i].Randomize(i, (i < 7)?Bool():FALSE);
			break;

		case 3:
			if (i < 7)
				m_segment[i].Randomize(i, TRUE);
			else
				m_segment[i].Randomize(i, (i < 10)?Bool():FALSE);
			break;
		}
	}
	ToggleSegments();
}


void GeneLimb::Debug(int nSegmentsPerArm)
{
	for (int i = 0; i < MAX_SEGMENTS; i++)
	{
		m_segment[i].Debug(i, (i < MAX_SEGMENTS/2)?TRUE:FALSE);
	}
/*		switch (nSegmentsPerArm)
		{
		default:
		case 0:
			m_segment[i].Debug(i, TRUE);
			break;

		case 1:
			m_segment[i].Debug(i, (i < 3)?TRUE:FALSE);
			break;

		case 2:
			if (i < 4)
				m_segment[i].Debug(i, TRUE);
			else
				m_segment[i].Debug(i, (i < 7)?TRUE:FALSE);
			break;

		case 3:
			if (i < 7)
				m_segment[i].Debug(i, TRUE);
			else
				m_segment[i].Debug(i, (i < 10)?TRUE:FALSE);
			break;
		}
*/	
	ToggleSegments();
}


void GeneLimb::Mutate(int chance)
{
	for (int i = 0; i < MAX_SEGMENTS; i++)
		m_segment[i].Mutate(chance, i);

	ToggleSegments();
}


void GeneLimb::Crossover(GeneLimb&  gLine)
{
  for (int i = 0; i < MAX_SEGMENTS; i++)
    if (Bool())
      m_segment[i] = gLine.m_segment[i];

	ToggleSegments();
}


void GeneLimb::Serialize(CArchive& ar)
{
	for (int i = 0; i < MAX_SEGMENTS; i++)
		m_segment[i].Serialize(ar);

	if (ar.IsLoading())
		ToggleSegments();
}


int GeneLimb::GetSegmentsVisible()
{
	for (int i = 0, j = 0; i < MAX_SEGMENTS; i++)
		if (m_segment[i].IsVisible())
			j++;

	return j;
}

// This method is saving 0 for non-visible segments
// and toggling between -1 and 1 for visible segments
void GeneLimb::ToggleSegments()
{
	bool bToggle = false;
	for (int i = 0; i < MAX_SEGMENTS; i++)
	{
		if (m_segment[i].IsVisible())
		{
			m_toggleVisibleSegments[i] = bToggle;

			bToggle = !bToggle;
		}
		else
		{
			m_toggleVisibleSegments[i] = false;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// GeneTrait
//
// Contains GeneLines plus overall biot genes
//
const int GeneTrait::mirrorAngle[MAX_SYMMETRY] = { 0, 180,  0,  180,  90, 90, 270, 270 };
const int GeneTrait::mirrorCoef[MAX_SYMMETRY]  = { 1,  -1, -1,    1,  -1,  1,  -1,   1 };
const int GeneTrait::mirrorSix[MAX_SYMMETRY]   = { 0, 120,  0,  120, 240, 240,  0,   0 };

void GeneTrait::Serialize(CArchive& ar)
{
	for (int i = 0; i < MAX_LIMB_TYPES; i++)
			m_geneLine[i].Serialize(ar);

	if (ar.IsLoading())
	{
		ar >> m_disperse;
		ar >> m_children;
		ar >> m_attackChildren;
		ar >> m_attackSiblings;
		ar >> m_species;
		ar >> m_adultRatio[0];
		ar >> m_adultRatio[1];
		ar >> m_lineCount;
		ar >> m_offset;

		ar.Read(m_lineRef, sizeof(m_lineRef));

		ar >> m_mirrored;
		ar >> m_sex;
		ar >> m_asexual;
		ar >> m_chanceMale;
		ar >> m_maxAge;

		if (m_children > 8 || m_children == 0)
			m_children = 8;

		if (m_species >= 16)
			m_species = 16;

		if (m_adultRatio[0] > 6 || m_adultRatio[0] == 0)
			m_adultRatio[0] = 6;

		if (m_adultRatio[1] > 6 || m_adultRatio[1] == 0)
			m_adultRatio[1] = 6;

		if (m_lineCount > 8 || m_lineCount == 0)
			m_lineCount = 8;

		if (m_sex > 1)
			m_sex = 1;

		if (m_asexual > 1)
			m_asexual = 1;

		for (i = 0; i < MAX_SYMMETRY; i++)
			if (m_lineRef[i] >= MAX_LIMB_TYPES)
				m_lineRef[i] = 0;

		CalculateAngles();
	}
	else
	{
		ar << m_disperse;
		ar << m_children;
		ar << m_attackChildren;
		ar << m_attackSiblings;
		ar << m_species;
		ar << m_adultRatio[0];
		ar << m_adultRatio[1];
		ar << m_lineCount;
		ar << m_offset;

		ar.Write(m_lineRef, sizeof(m_lineRef));
  
		ar << m_mirrored;
		ar << m_sex;
		ar << m_asexual;
		ar << m_chanceMale;
		ar << m_maxAge;
	}
}


int GeneTrait::GetCompressedToggle(int nAngle, int nLine, int nSegment)
{
	ASSERT(nLine < MAX_SYMMETRY);
	ASSERT(nSegment < MAX_SEGMENTS);

	if (IsMirrored())
	{
		if (mirrorCoef[nLine] == -1)
			if (m_geneLine[m_lineRef[nLine]].m_toggleVisibleSegments[nSegment])
				return -nAngle;
			else
				return nAngle;
	}

	if (m_geneLine[m_lineRef[nLine]].m_toggleVisibleSegments[nSegment])
		return nAngle;
	else
		return -nAngle;
}


void GeneTrait::CalculateAngles()
{
	const int* pAngle;
	if (GetLines() == 6)
		pAngle = &mirrorSix[0];
	else
		pAngle = &mirrorAngle[0];

	for (int line = 0; line < GetLines(); line++)
	{
		GeneLimb& gLine = m_geneLine[m_lineRef[line]];
		for (int segment = 0; segment < MAX_SEGMENTS; segment++)
		{
			GeneSegment& gSegment = gLine.GetSegment(segment);
			if (gSegment.IsVisible())
			{
				if (IsMirrored())
					m_angle[line][segment] = (short) (GetOffset() + (gSegment.GetAngle() * mirrorCoef[line]) + pAngle[line]);
				else
					m_angle[line][segment] = (short) (GetOffset() + gSegment.GetAngle() + (line * 360) / GetLines());
			}
		}	
	}
}


void GeneTrait::Randomize(int nArmsPerBiot, int nTypesPerBiot, int nSegmentsPerArm)
{
	int i;

	m_disperse       = (BYTE) Bool();
    m_children       = (BYTE) (Integer(8) + 1);
	m_attackChildren = (BYTE) Bool();
	m_attackSiblings = (BYTE) Bool();
	m_species        = (BYTE) Integer(2);  // Make fewer species to begin with
	m_adultRatio[0]  = (BYTE) (Integer(6) + 1);
	m_adultRatio[1]  = (BYTE) (Integer(6) + 1);
	m_mirrored       = (BYTE) Bool();

	switch(nArmsPerBiot)
	{
	default:
	case 0:
		m_lineCount  = (BYTE) (Integer(MAX_SYMMETRY) + 1);
		break;

	case 1:
		m_lineCount  = (BYTE) (Integer(2) + 1);
		break;

	case 2:
		m_lineCount  = (BYTE) (Integer(2) + 3);
		break;

	case 3:
		m_lineCount  = (BYTE) (Integer(2) + 5);
		break;

	case 4:
		m_lineCount  = (BYTE) (Integer(2) + 7);
		break;
	}

	// Initially,we don't want odd mirrored creatures
	if ((m_lineCount  & 0x01) == 0x01)
		m_mirrored = FALSE;

	m_offset         = (short) Integer(360);

	for (i = 0; i < MAX_LIMB_TYPES; i++)
		m_geneLine[i].Randomize(nSegmentsPerArm);

	m_sex        = (BYTE) Bool();
	m_asexual    = (BYTE) Bool();
    m_chanceMale = (BYTE) (Int256() / 2 + 64);
	m_maxAge     = (short) Int256();

	// We start out with uniform appearance
	for (i = 0; i < MAX_SYMMETRY; i++)
	{
		m_lineRef[i] = Byte(nTypesPerBiot + 1);
	}

	CalculateAngles();
}


void GeneTrait::Debug(int nArmsPerBiot, int nTypesPerBiot, int nSegmentsPerArm)
{
	int i;

	m_disperse       = (BYTE) TRUE;
    m_children       = (BYTE) 1;
	m_attackChildren = (BYTE) FALSE;
	m_attackSiblings = (BYTE) FALSE;
	m_species        = (BYTE) 0;  // Make fewer species to begin with
	m_adultRatio[0]  = (BYTE) 1;
	m_adultRatio[1]  = (BYTE) 1;
	m_mirrored       = (BYTE) TRUE;

	m_lineCount  = 1;//(BYTE) MAX_SYMMETRY;
/*	switch(nArmsPerBiot)
	{
	default:
	case 0:
		m_lineCount  = (BYTE) MAX_SYMMETRY;
		break;

	case 1:
		m_lineCount  = (BYTE) 2;
		break;

	case 2:
		m_lineCount  = (BYTE) 4;
		break;

	case 3:
		m_lineCount  = (BYTE) 6;
		break;

	case 4:
		m_lineCount  = (BYTE) 8;
		break;
	}
*/
	// Initially,we don't want odd mirrored creatures
	if ((m_lineCount  & 0x01) == 0x01)
		m_mirrored = FALSE;

	m_offset         = (short) 0; 

	for (i = 0; i < MAX_LIMB_TYPES; i++)
		m_geneLine[i].Debug(nSegmentsPerArm);

	m_sex        = (BYTE) FALSE;
	m_asexual    = (BYTE) TRUE;
    m_chanceMale = (BYTE) 0;
	m_maxAge     = (short) 255;

	// We start out with uniform appearance
	for (i = 0; i < MAX_SYMMETRY; i++)
	{
		m_lineRef[i] = 0;
	}

	CalculateAngles();
}


void GeneTrait::Mutate(int chance)
{
	int i;

	if (Int1024() < chance)
		m_disperse       = (BYTE) Bool();

	if (Int1024() < chance)
	    m_children       = (BYTE) (Integer(8) + 1);

	if (Int1024() < chance)
		m_attackChildren = (BYTE) Bool();

	if (Int1024() < chance)
		m_attackSiblings = (BYTE) Bool();

	if (Int1024() < chance)
	{
		if (Sign() > 0)
			m_species++;
		else
			m_species--;
	}

	if (Int1024() < chance)
		m_adultRatio[0]     = (BYTE) (Integer(6) + 1);

	if (Int1024() < chance)
		m_adultRatio[1]     = (BYTE) (Integer(6) + 1);

	if (Int1024() < chance)
		m_lineCount         = (BYTE) (Integer(8) + 1);

	if (Int1024() < chance)
		m_mirrored       = (BYTE) Bool();

	if (Int1024() < chance)
		m_offset         = (short) Integer(360);

	for (i = 0; i < MAX_LIMB_TYPES; i++)
		m_geneLine[i].Mutate(chance);

	if (Int1024() < chance)
		m_sex = (BYTE) Bool();

	if (Int1024() < chance)
		m_asexual = (BYTE) Bool();

	if (Int1024() < chance)
		m_chanceMale = Byte();

	if (Int1024() < chance)
		m_maxAge = (short) Int256();

	for (i = 0; i < MAX_SYMMETRY; i++)
		if (Int1024() < chance)
			m_lineRef[i] = Byte(MAX_LIMB_TYPES);

	CalculateAngles();
}


void GeneTrait::Crossover(GeneTrait&  gTrait)
{
	int i;

	for (i = 0; i < MAX_LIMB_TYPES; i++)
		m_geneLine[i].Crossover(gTrait.m_geneLine[i]);

	for (i = 0; i < MAX_SYMMETRY; i++)
		if (Bool())
			m_lineRef[i] = gTrait.m_lineRef[i];

	if (Bool())
		m_offset = gTrait.m_offset;
	
	if (Bool())
		m_disperse       = gTrait.m_disperse;

	if (Bool())
	    m_children       = gTrait.m_children;

	if (Bool())
		m_attackChildren = gTrait.m_attackChildren;

	if (Bool())
		m_attackSiblings = gTrait.m_attackSiblings;

	if (Bool())
		m_species        = gTrait.m_species;

	if (Bool())
		m_adultRatio[0]  = gTrait.m_adultRatio[0];

	if (Bool())
		m_adultRatio[1]  = gTrait.m_adultRatio[1];

	if (Bool())
		m_lineCount      = gTrait.m_lineCount;

	if (Bool())
		m_mirrored       = gTrait.m_mirrored;

	if (Bool())
		m_asexual        = gTrait.m_asexual;

	if (Bool())
		m_chanceMale     = gTrait.m_chanceMale;

	CalculateAngles();
}


BOOL GeneTrait::IsLineTypeVisible(int lineType)
{
	for (int i = 0; i < m_lineCount; i++)
		if (lineType == m_lineRef[i])
			return TRUE;

	return FALSE;
}


