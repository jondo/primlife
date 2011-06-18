// Brain.cpp
//
// Implements the biot brain
//
#include "stdafx.h"
#include "Rand.h"
#include "Genotype.h"
#include "Brain.h"
#include "Environ.h"
#include "Biots.h"

//
// ProductTerm (Picks a subset sensor bits that must be true or false)
//

void ProductTerm::Mutate(int nChance)
{
	Randomizer rand;

	if (rand.Int1024() < nChance)
		m_dwMask = rand.Dword();

	if (rand.Int1024() < nChance)
		m_dwInvert = rand.Dword();
}


void ProductTerm::Randomize()
{
	Randomizer rand;

	m_dwMask   = rand.Dword();
	m_dwInvert = rand.Dword();
}

// Helpful settings during the debug sessions
void ProductTerm::Debug()
{
	m_dwMask = 0xFFFFFFFF;
	m_dwInvert = 0x00000000;
}

void ProductTerm::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_dwMask;
		ar << m_dwInvert;
	}
	else
	{
		ar >> m_dwMask;
		ar >> m_dwInvert;
	}
}


//
// ProductArray (Contains an array of product definitions)
//

void ProductArray::Mutate(int nChance)
{
	for (int i = 0; i < GetTermCount(); i++)
		m_productTerm[i].Mutate(nChance);

	for (i = 0; i < GetSumCount(); i++)
		m_productSum[i].Mutate(nChance);
}


void ProductArray::Randomize()
{
	for (int i = 0; i < GetTermCount(); i++)
		m_productTerm[i].Randomize();

	for (i = 0; i < GetSumCount(); i++)
		m_productSum[i].Randomize();
}


void ProductArray::Debug()
{
	for (int i = 0; i < GetTermCount(); i++)
		m_productTerm[i].Debug();

	for (i = 0; i < GetSumCount(); i++)
		m_productSum[i].Debug();
}


void ProductArray::Serialize(CArchive& ar)
{
	for (int i = 0; i < GetTermCount(); i++)
		m_productTerm[i].Serialize(ar);

	for (i = 0; i < GetSumCount(); i++)
		m_productSum[i].Serialize(ar);
}

void ProductArray::Crossover(ProductArray& productArray)
{
	Randomizer rand;

	for (int i = 0; i < GetTermCount(); i++)
		if (rand.Bool())
			m_productTerm[i] = productArray.m_productTerm[i];

	// Product sums are currently allowed to crossover at the term level.
	for (i = 0; i < GetSumCount(); i++)
		m_productSum[i].Crossover(productArray.m_productSum[i]);
}

ProductArray& ProductArray::operator=(ProductArray& productArray)
{
	for (int i = 0; i < GetTermCount(); i++)
		m_productTerm[i] = productArray.m_productTerm[i];

	for (i = 0; i < GetSumCount(); i++)
		m_productSum[i] = productArray.m_productSum[i];

	return *this;
}


//
// ProductSum  (Each command refers to a set of product terms
// defined in the ProductArray)
//
void ProductSum::Mutate(int nChance)
{
	Randomizer rand;

	for (int i = 0; i < GetCount(); i++)
		if (rand.Int1024() < nChance)
			m_reference[i] = rand.Byte(ProductArray::MAX_PRODUCT_TERMS);

	if (rand.Int1024() < nChance)
		m_bTrue = rand.Bool();
}

void ProductSum::Randomize()
{
	Randomizer rand;

	for (int i = 0; i < GetCount(); i++)
		m_reference[i] = rand.Byte(ProductArray::MAX_PRODUCT_TERMS);

	m_bTrue = rand.Bool();
}


void ProductSum::Debug()
{
	for (int i = 0; i < GetCount(); i++)
		m_reference[i] = (BYTE) i;

	m_bTrue = TRUE;
}


void ProductSum::Serialize(CArchive& ar)
{
	Randomizer rand;

	if (ar.IsStoring())
	{
		for (int i = 0; i < GetCount(); i++)
			ar << m_reference[i];

		ar << m_bTrue;
	}
	else
	{
		for (int i = 0; i < GetCount(); i++)
		{
			ar >> m_reference[i];
			if (m_reference[i] >= ProductArray::MAX_PRODUCT_TERMS)
				m_reference[i] = rand.Byte(ProductArray::MAX_PRODUCT_TERMS);
		}
		ar >> m_bTrue;
	}
}


void ProductSum::Crossover(ProductSum& productSum)
{
	Randomizer rand;

	for (int i = 0; i < GetCount(); i++)
		if (rand.Bool())
			m_reference[i] = productSum.m_reference[i];

	if (rand.Bool())
		m_bTrue = productSum.m_bTrue;
}


ProductSum& ProductSum::operator=(ProductSum& productSum)
{
	for (int i = 0; i < GetCount(); i++)
		m_reference[i] = productSum.m_reference[i];

	m_bTrue = productSum.m_bTrue;
	return *this;
}

// Essentially ORs all the product terms together.  The first
// true result short circuits the evaluation and returns
// immediately.
bool ProductSum::IsTrue(ProductArray& productArray, DWORD dwSensor)
{
	for (int i = 0; i < GetCount(); i++)
		if (productArray[m_reference[i]].IsTrue(dwSensor))
		{
			BTRACE1("TRUE (%d)\n", (int) m_bTrue);
			return (m_bTrue != 0);
		}
	BTRACE1("FALSE (%d)\n", (int) m_bTrue);
	return (m_bTrue == 0);
}



//
// CommandArgument
//
//

void CommandArgument::Serialize(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_command;
		ar >> m_limb;
		ar >> m_segment;
		ar >> m_rate;
		ar >> m_degrees;
	}
	else
	{
		ar << m_command;
		ar << m_limb;
		ar << m_segment;
		ar << m_rate;
		ar << m_degrees;
	}
}

void CommandArgument::Randomize(void)
{
	Randomizer rand;
	m_command    = rand.Integer(COMMAND_MAX_TYPES);
	m_rate       = rand.Byte();
	m_degrees    = rand.Byte();
	m_limb       = rand.Byte(MAX_SYMMETRY);
	m_segment    = rand.Byte(MAX_SEGMENTS);
}

void CommandArgument::Mutate(int chance)
{
	Randomizer rand;

	if (rand.Int1024() < chance)
		m_command    = rand.Integer(COMMAND_MAX_TYPES);

	if (rand.Int1024() < chance)
		m_rate       = rand.Byte();

	if (rand.Int1024() < chance)
		m_degrees    = rand.Byte();

	if (rand.Int1024() < chance)
		m_limb       = rand.Byte(MAX_SYMMETRY);

	if (rand.Int1024() < chance)
		m_segment    = rand.Byte(MAX_SEGMENTS);
}


int CommandArgument::GetLimb(int actualLimb)
{ 
	if (m_limb == MAX_SYMMETRY)
		return (int) m_limb;

	if ((int) m_limb + actualLimb >= MAX_SYMMETRY)
		return (int) m_limb + actualLimb - MAX_SYMMETRY;
	 else
		return (int) m_limb + actualLimb;
}

//
// CommandArray (Contains an array of command definitions)
//

void CommandArray::Mutate(int nChance)
{
	for (int i = 0; i < GetCommandCount(); i++)
		m_command[i].Mutate(nChance);

	m_productArray.Mutate(nChance);

	for (i = 0; i < GetTypeCount(); i++)
		m_commandLimbType[i].Mutate(nChance);
}


void CommandArray::Randomize()
{
	for (int i = 0; i < GetCommandCount(); i++)
		m_command[i].Randomize();

	m_productArray.Randomize();

	for (i = 0; i < GetTypeCount(); i++)
		m_commandLimbType[i].Randomize();
}


void CommandArray::Serialize(CArchive& ar)
{
	for (int i = 0; i < GetCommandCount(); i++)
		m_command[i].Serialize(ar);

	m_productArray.Serialize(ar);

	for (i = 0; i < GetTypeCount(); i++)
		m_commandLimbType[i].Serialize(ar);
}

void CommandArray::Crossover(CommandArray& commandArray)
{
	Randomizer rand;

	for (int i = 0; i < GetCommandCount(); i++)
		if (rand.Bool())
			m_command[i] = commandArray.m_command[i];

	m_productArray.Crossover(commandArray.m_productArray);

	for (i = 0; i < GetTypeCount(); i++)
		m_commandLimbType[i].Crossover(commandArray.m_commandLimbType[i]);
}

CommandArray& CommandArray::operator=(CommandArray& commandArray)
{
	for (int i = 0; i < GetCommandCount(); i++)
		m_command[i] = commandArray.m_command[i];

	m_productArray = commandArray.m_productArray;

	for (i = 0; i < GetTypeCount(); i++)
		m_commandLimbType[i] = commandArray.m_commandLimbType[i];

	return *this;
}


//////////////////////////////////////////////////
// CommandLimbType contains an array of referenes
// to commands in the command array.  This level
// of indirection allows a limb type to pick any
// command in the command array.
//
//
void CommandLimbType::Mutate(int nChance)
{
	Randomizer rand;

	for (int i = 0; i < GetCount(); i++)
	{
		if (rand.Int1024() < nChance)
		{
			m_comref[i] = rand.Byte(CommandArray::MAX_COMMANDS);
			ASSERT(m_comref[i] < 64);
		}

		if (rand.Int1024() < nChance)
			m_sumref[i] = rand.Byte(ProductArray::MAX_PRODUCT_SUMS);
	}
}

void CommandLimbType::Randomize()
{
	Randomizer rand;

	for (int i = 0; i < GetCount(); i++)
	{
		m_comref[i] = rand.Byte(CommandArray::MAX_COMMANDS);
		ASSERT(m_comref[i] < 64);
		m_sumref[i]    = rand.Byte(ProductArray::MAX_PRODUCT_SUMS);
	}
}

void CommandLimbType::Serialize(CArchive& ar)
{
	Randomizer rand;

	if (ar.IsStoring())
	{
		for (int i = 0; i < GetCount(); i++)
		{
			ar << m_comref[i];
			ar << m_sumref[i];
		}
	}
	else
	{
		for (int i = 0; i < GetCount(); i++)
		{
			ar >> m_comref[i];
			if (m_comref[i] >= CommandArray::MAX_COMMANDS)
				m_comref[i] = rand.Byte(CommandArray::MAX_COMMANDS);
			ar >> m_sumref[i];
			if (m_sumref[i] >= ProductArray::MAX_PRODUCT_SUMS)
				m_sumref[i] = rand.Byte(ProductArray::MAX_PRODUCT_SUMS);
		}
	}
}


void CommandLimbType::Crossover(CommandLimbType& commandLimbType)
{
	Randomizer rand;

	for (int i = 0; i < GetCount(); i++)
	{
		if (rand.Bool())
			m_comref[i] = commandLimbType.m_comref[i];
		if (rand.Bool())
			m_sumref[i] = commandLimbType.m_sumref[i];
	}
}


CommandLimbType& CommandLimbType::operator=(CommandLimbType& commandLimbType)
{
	for (int i = 0; i < GetCount(); i++)
	{
		m_comref[i] = commandLimbType.m_comref[i];
		ASSERT(m_comref[i] < 64);
		m_sumref[i]    = commandLimbType.m_sumref[i];
	}
	return *this;
}


/////////////////////////////////////////////////////////////////////
// CommandLimbStore
//
// Each limb, regardless of type, has associated state information
// to store.
//
void CommandLimbStore::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nLimbType;
		ar << m_nLimb;
		ar.Write(&command, sizeof(command));
	}
	else
	{
		ar >> m_nLimbType;
		ar >> m_nLimb;
		ar.Read(&command, sizeof(command));
	}
}

void CommandLimbStore::Initialize(int nLimbType, int nLimb, Biot& biot)
{
	m_nLimbType = nLimbType;
	m_nLimb     = nLimb;
	m_pBiot     = &biot;

	for (m_index = 0; m_index < CommandLimbType::MAX_COMMANDS_PER_LIMB; m_index++)
	{
		m_pArg = &biot.m_commandArray.GetCommandArgument(nLimbType, m_index);

		switch (m_pArg->GetCommand())
		{
			case CommandArgument::COMMAND_FLAP_LIMB_SEGMENT:
				command[m_index].flapLimbSegment.Initialize(*this);
				break;

			case CommandArgument::COMMAND_FLAP_LIMB_TYPE_SEGMENT:
				command[m_index].flapLimbTypeSegment.Initialize(*this);
				break;

			case CommandArgument::COMMAND_MOVE_LIMB_SEGMENT:
				command[m_index].moveLimbSegment.Initialize(*this);
				break;

			case CommandArgument::COMMAND_MOVE_LIMB_SEGMENTS:
				command[m_index].moveLimbSegments.Initialize(*this);
				break;

			case CommandArgument::COMMAND_MOVE_LIMB_TYPE_SEGMENT:
				command[m_index].moveLimbTypeSegment.Initialize(*this);
				break;

			case CommandArgument::COMMAND_MOVE_LIMB_TYPE_SEGMENTS:
				command[m_index].moveLimbTypeSegments.Initialize(*this);
				break;

			case CommandArgument::COMMAND_RETRACT_LIMB_TYPE:
				command[m_index].retractLimbType.Initialize(*this);
				break;

			case CommandArgument::COMMAND_RETRACT_LIMB:
				command[m_index].retractLimb.Initialize(*this);
				break;

			case CommandArgument::COMMAND_NOP:
				command[m_index].nop.Initialize(*this);
				break;

			case CommandArgument::COMMAND_MEMORY:
				command[m_index].memory.Initialize(*this);
				break;

			default:
				ASSERT(0);
				break;
		}
	}
}

void CommandLimbStore::Execute(Biot& biot, DWORD dwSensor)
{
	m_pBiot     = &biot;
	m_dwSensor  = dwSensor;

	for (m_index = 0; m_index < CommandLimbType::MAX_COMMANDS_PER_LIMB; m_index++)
	{
		m_pArg = &biot.m_commandArray.GetCommandArgument(m_nLimbType, m_index);
		switch (m_pArg->GetCommand())
		{
		    case CommandArgument::COMMAND_FLAP_LIMB_SEGMENT:
				command[m_index].flapLimbSegment.Execute(*this);
				break;

			case CommandArgument::COMMAND_FLAP_LIMB_TYPE_SEGMENT:
				command[m_index].flapLimbTypeSegment.Execute(*this);
				break;

			case CommandArgument::COMMAND_MOVE_LIMB_SEGMENT:
				command[m_index].moveLimbSegment.Execute(*this);
				break;

			case CommandArgument::COMMAND_MOVE_LIMB_SEGMENTS:
				command[m_index].moveLimbSegments.Execute(*this);
				break;

			case CommandArgument::COMMAND_MOVE_LIMB_TYPE_SEGMENT:
				command[m_index].moveLimbTypeSegment.Execute(*this);
				break;

			case CommandArgument::COMMAND_MOVE_LIMB_TYPE_SEGMENTS:
				command[m_index].moveLimbTypeSegments.Execute(*this);
				break;

			case CommandArgument::COMMAND_RETRACT_LIMB_TYPE:
				command[m_index].retractLimbType.Execute(*this);
				break;

			case CommandArgument::COMMAND_RETRACT_LIMB:
				command[m_index].retractLimb.Execute(*this);
				break;

			case CommandArgument::COMMAND_NOP:
//				command[m_index].nop.Execute(*this);
				break;

			case CommandArgument::COMMAND_MEMORY:
				command[m_index].memory.Execute(*this);
				break;

			default:
				ASSERT(0);
				break;
		}
	}
}


inline bool CommandLimbStore::IsSensorTrue()
{
	return m_pBiot->m_commandArray.IsTrue(m_nLimbType, m_index, m_dwSensor);
}


///////////////////////////////////////////////////////////////
//CommandFlapLine
//
// Don't give me no flap!  Flaps an individual line.
//
void CommandFlapLimbTypeSegment::Initialize(CommandLimbStore& store)
{
	// Determine limb type to move
	m_nLimbType = store.m_pArg->GetLimbType();
	m_nSegment  = store.m_pArg->GetSegment();

	// Is this line type even visible?
	if (!store.m_pBiot->trait.IsLineTypeVisible(m_nLimbType) ||
		!store.m_pBiot->trait.GetLineType(m_nLimbType).GetSegment(m_nSegment).IsVisible())
	{
		// Record we should ignore this command
		m_nLimbType = MAX_LIMB_TYPES;
		return;
	}

	// How far are we going to move it?
	m_nMaxDegrees     = store.m_pArg->GetDegrees();

	// How far have we moved it thus far?
	m_nAppliedDegrees = 0; 

	// What rate?
	m_nRate = store.m_pArg->GetRate();

	m_bGoingUp = true;
}

void CommandFlapLimbTypeSegment::Execute(CommandLimbStore& store)
{
	if (m_nLimbType == MAX_LIMB_TYPES)
		return;

	if (store.IsSensorTrue())
	{
		if (m_bGoingUp)
		{
			bool bIsNegative = (m_nAppliedDegrees < 0);

			// If we are true, we need to adjust back full extension
			if (m_nAppliedDegrees < m_nMaxDegrees)
			{
				m_nAppliedDegrees += store.m_pBiot->MoveLimbTypeSegment(m_nSegment, m_nLimbType, min(m_nRate, m_nMaxDegrees-m_nAppliedDegrees));
				m_bGoingUp = (m_nAppliedDegrees == m_nMaxDegrees);
			}

			if (bIsNegative && m_nAppliedDegrees >= 0)
			{
				// Apply impulse
				FlapLimbTypeSegments(*store.m_pBiot);
			}
		}
		else
		{
			bool bIsPositive = (m_nAppliedDegrees > 0);

			// If we are true, we need to adjust back full extension
			if (m_nAppliedDegrees > -m_nMaxDegrees)
			{
				m_nAppliedDegrees += store.m_pBiot->MoveLimbTypeSegment(m_nSegment, m_nLimbType, -min(m_nRate, m_nAppliedDegrees + m_nMaxDegrees));

				m_bGoingUp = (m_nAppliedDegrees == -m_nMaxDegrees);
			}

			if (bIsPositive && m_nAppliedDegrees <= 0)
			{
				// Apply impulse
				FlapLimbTypeSegments(*store.m_pBiot);
			}
		}
	}
	else
	{
		// Return the segment to the mormal position.
		if (m_nAppliedDegrees > 0)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbTypeSegment(m_nSegment, m_nLimbType, -min(m_nRate, m_nAppliedDegrees));
		}
		
		if (m_nAppliedDegrees < 0)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbTypeSegment(m_nSegment, m_nLimbType, min(m_nRate, m_nAppliedDegrees));
		}
	}
}


void CommandFlapLimbTypeSegment::FlapLimbTypeSegments(Biot& biot)
{
	for (int i = 0; i < biot.trait.GetLines(); i++)
	{
		if (m_nLimbType == biot.trait.GetLineTypeIndex(i))
		{
			int nPeno = m_nSegment * MAX_SYMMETRY + i;
			Flap(biot, nPeno);
		}
	}
}


void CommandFlapLimbTypeSegment::Flap(Biot& biot, int nPeno)
{
	// If we are damaged, we don't get a boost from flapping
	if (!biot.IsSegmentMissing(nPeno))
	{
		double Vx = biot.startPt[nPeno].x - biot.stopPt[nPeno].x;
		double Vy = biot.startPt[nPeno].y - biot.stopPt[nPeno].y;
		double dr = biot.vector.rotationComponent(
			(double) biot.startPt[nPeno].x,
			(double) biot.startPt[nPeno].y,
			biot.startPt[nPeno].x + Vx,
			biot.startPt[nPeno].y + Vy);

		double radius = biot.vector.distance(biot.startPt[nPeno].x, biot.startPt[nPeno].y);

		if (dr != 0)
		{
			double dv = biot.vector.motionComponent(biot.vector.distance(Vx, Vy), dr);
			Vx = -biot.vector.fraction(dv, (int) biot.startPt[nPeno].x, radius); 
			Vy = -biot.vector.fraction(dv, (int) biot.startPt[nPeno].y, radius); 
		}
		else
		{
//			Vx = -Vx;
//			Vy = -Vy;
		}
		biot.vector.accelerateX(Vx*20);
		biot.vector.accelerateY(Vy*20);
		biot.vector.accelerateRotation(dr*10);
	}
}

///////////////////////////////////////////////////////////////
//Don't give me no flap!
//
void CommandFlapLimbSegment::Initialize(CommandLimbStore& store)
{
	// Determine limb type to move
	m_nLimb     = store.m_pArg->GetLimb(store.m_nLimb);
	m_nSegment  = store.m_pArg->GetSegment();

	// Is this line type even visible?
	if (m_nLimb >= store.m_pBiot->trait.GetLines() ||
		!store.m_pBiot->trait.GetLineType(store.m_pBiot->trait.GetLineTypeIndex(m_nLimb)).GetSegment(m_nSegment).IsVisible())
	{
		// Record we should ignore this command
		m_nLimb = MAX_SYMMETRY;
		return;
	}

	// How far are we going to move it?
	m_nMaxDegrees     = store.m_pArg->GetDegrees();

	// How far have we moved it thus far?
	m_nAppliedDegrees = 0; 

	// What rate?
	m_nRate = store.m_pArg->GetRate();

	m_bGoingUp = true;
}

void CommandFlapLimbSegment::Execute(CommandLimbStore& store)
{
	if (m_nLimb == MAX_SYMMETRY)
		return;

	if (store.IsSensorTrue())
	{
		if (m_bGoingUp)
		{
			bool bIsNegative = (m_nAppliedDegrees < 0);

			// If we are true, we need to adjust back full extension
			if (m_nAppliedDegrees < m_nMaxDegrees)
			{
				m_nAppliedDegrees += store.m_pBiot->MoveLimbSegment(m_nSegment, m_nLimb, min(m_nRate, m_nMaxDegrees-m_nAppliedDegrees));
				m_bGoingUp = (m_nAppliedDegrees < m_nMaxDegrees);
			}

			if (bIsNegative && m_nAppliedDegrees >= 0)
			{
				// Apply impulse
				Flap(*store.m_pBiot);
			}
		}
		else
		{
			bool bIsPositive = (m_nAppliedDegrees > 0);

			// If we are true, we need to adjust back full extension
			if (m_nAppliedDegrees > -m_nMaxDegrees)
			{
				m_nAppliedDegrees += store.m_pBiot->MoveLimbSegment(m_nSegment, m_nLimb, -min(m_nRate, m_nAppliedDegrees + m_nMaxDegrees));

				m_bGoingUp = (m_nAppliedDegrees <= -m_nMaxDegrees);
			}

			if (bIsPositive && m_nAppliedDegrees <= 0)
			{
				// Apply impulse
				Flap(*store.m_pBiot);
			}
		}
	}
	else
	{
		// Return the segment to the mormal position.
		if (m_nAppliedDegrees > 0)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbSegment(m_nSegment, m_nLimb, -min(m_nRate, m_nAppliedDegrees));
		}
		
		if (m_nAppliedDegrees < 0)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbSegment(m_nSegment, m_nLimb, min(m_nRate, m_nAppliedDegrees));
		}
	}
}


void CommandFlapLimbSegment::Flap(Biot& biot)
{
	int nPeno = m_nSegment * MAX_SYMMETRY + m_nLimb;

	// If we are damaged, we don't get a boost from flapping
	if (!biot.IsSegmentMissing(nPeno))
	{

		//TODO: This vector is calculated with the assumption that the origin is the
		// center of mass.  We want to change that to the center of the bounding
		// rectangle.
		double Vx = biot.startPt[nPeno].x - biot.stopPt[nPeno].x;
		double Vy = biot.startPt[nPeno].y - biot.stopPt[nPeno].y;
		double dr = biot.vector.rotationComponent(
			(double) biot.startPt[nPeno].x,
			(double) biot.startPt[nPeno].y,
			biot.startPt[nPeno].x + Vx,
			biot.startPt[nPeno].y + Vy);

		double radius = biot.vector.distance(biot.startPt[nPeno].x , biot.startPt[nPeno].y);

		if (dr != 0)
		{
			double dv = biot.vector.motionComponent(biot.vector.distance(Vx, Vy), dr);
			Vx = -biot.vector.fraction(dv, (int) biot.startPt[nPeno].x, radius); 
			Vy = -biot.vector.fraction(dv, (int) biot.startPt[nPeno].y, radius); 
		}
		else
		{
//			Vx = -Vx;
//			Vy = -Vy;
		}
		biot.vector.accelerateX(Vx*20);
		biot.vector.accelerateY(Vy*20);
		biot.vector.accelerateRotation(dr*10);
	}
}

///////////////////////////////////////////////////////////////
void CommandMoveLimbSegment::Initialize(CommandLimbStore& store)
{
		// Determine limb type to move
	m_nLimb     = store.m_pArg->GetLimb(store.m_nLimb);
	m_nSegment  = store.m_pArg->GetSegment();

	// Is this line type even visible?
	if (m_nLimb >= store.m_pBiot->trait.GetLines() ||
		!store.m_pBiot->trait.GetLineType(store.m_pBiot->trait.GetLineTypeIndex(m_nLimb)).GetSegment(m_nSegment).IsVisible())
	{
		// Record we should ignore this command
		m_nLimb = MAX_SYMMETRY;
		return;
	}

	// How far are we going to move it?
	m_nMaxDegrees     = store.m_pArg->GetDegrees();

	// How far have we moved it thus far?
	m_nAppliedDegrees = 0; 

	// What rate?
	m_nRate = store.m_pArg->GetRate();
}

void CommandMoveLimbSegment::Execute(CommandLimbStore& store)
{
	if (m_nLimb == MAX_SYMMETRY)
		return;

	if (store.IsSensorTrue())
	{
		// If we are true, we need to adjust back full extension
		if (m_nAppliedDegrees < m_nMaxDegrees)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbSegment(m_nSegment, m_nLimb, min(m_nRate, m_nMaxDegrees-m_nAppliedDegrees));
		}
	}
	else
	{
		// If we are false, we need to adjust back full extension
		if (m_nAppliedDegrees > 0)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbSegment(m_nSegment, m_nLimb, -min(m_nRate, m_nAppliedDegrees));
		}
	}
}

///////////////////////////////////////////////////////////////
void CommandMoveLimbTypeSegment::Initialize(CommandLimbStore& store)
{
	// Determine limb type to move
	m_nLimbType = store.m_pArg->GetLimbType();
	m_nSegment  = store.m_pArg->GetSegment();

	// Is this line type even visible?
	if (!store.m_pBiot->trait.IsLineTypeVisible(m_nLimbType) ||
		!store.m_pBiot->trait.GetLineType(m_nLimbType).GetSegment(m_nSegment).IsVisible())
	{
		// Record we should ignore this command
		m_nLimbType = MAX_LIMB_TYPES;
		return;
	}

	// How far are we going to move it?
	m_nMaxDegrees     = store.m_pArg->GetDegrees();

	// How far have we moved it thus far?
	m_nAppliedDegrees = 0; 

	// What rate?
	m_nRate = store.m_pArg->GetRate();
}


void CommandMoveLimbTypeSegment::Execute(CommandLimbStore& store)
{
	if (m_nLimbType == MAX_LIMB_TYPES)
		return;

	if (store.IsSensorTrue())
	{
		// If we are true, we need to adjust back full extension
		if (m_nAppliedDegrees < m_nMaxDegrees)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbTypeSegment(m_nSegment, m_nLimbType, min(m_nRate, m_nMaxDegrees-m_nAppliedDegrees));
		}
	}
	else
	{
		// If we are false, we need to adjust back full extension
		if (m_nAppliedDegrees > 0)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbTypeSegment(m_nSegment, m_nLimbType, -min(m_nRate, m_nAppliedDegrees));
		}
	}
}

///////////////////////////////////////////////////////////////
void CommandMoveLimbSegments::Initialize(CommandLimbStore& store)
{
	// Determine limb type to move
	m_nLimb = store.m_pArg->GetLimb(store.m_nLimb);

	// Is this line type even visible?
	if (m_nLimb >= store.m_pBiot->trait.GetLines())
	{
		// Record we should ignore this command
		m_nLimb = MAX_SYMMETRY;
		return;
	}

	// How far are we going to move it?
	m_nMaxDegrees  = store.m_pArg->GetDegrees();

	// How far have we moved it thus far?
	m_nAppliedDegrees = 0; 

	// What rate?
	m_nRate = store.m_pArg->GetRate();

}

void CommandMoveLimbSegments::Execute(CommandLimbStore& store)
{
	if (m_nLimb == MAX_SYMMETRY)
		return;

	if (store.IsSensorTrue())
	{
		// If we are true, we need to adjust back full extension
		if (m_nAppliedDegrees < m_nMaxDegrees)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbSegments(m_nLimb, min(m_nRate, m_nMaxDegrees-m_nAppliedDegrees));
		}
	}
	else
	{
		// If we are false, we need to adjust back full extension
		if (m_nAppliedDegrees > 0)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbSegments(m_nLimb, -min(m_nRate, m_nAppliedDegrees));
		}
	}
}

///////////////////////////////////////////////////////////////
void CommandMoveLimbTypeSegments::Initialize(CommandLimbStore& store)
{
	// Determine limb type to move
	m_nLimbType = store.m_pArg->GetLimbType();

	// Is this line type even visible?
	if (!store.m_pBiot->trait.IsLineTypeVisible(m_nLimbType))
	{
		// Record we should ignore this command
		m_nLimbType = MAX_LIMB_TYPES;
		return;
	}

	// How far are we going to move it?
	m_nMaxDegrees  = store.m_pArg->GetDegrees();

	// How far have we moved it thus far?
	m_nAppliedDegrees = 0; 

	// What rate?
	m_nRate = store.m_pArg->GetRate();

}

void CommandMoveLimbTypeSegments::Execute(CommandLimbStore& store)
{
	if (m_nLimbType == MAX_LIMB_TYPES)
		return;

	if (store.IsSensorTrue())
	{
		// If we are true, we need to adjust back full extension
		if (m_nAppliedDegrees < m_nMaxDegrees)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbTypeSegments(m_nLimbType, min(m_nRate, m_nMaxDegrees-m_nAppliedDegrees));
		}
	}
	else
	{
		// If we are false, we need to adjust back full extension
		if (m_nAppliedDegrees > 0)
		{
			m_nAppliedDegrees += store.m_pBiot->MoveLimbTypeSegments(m_nLimbType, -min(m_nRate, m_nAppliedDegrees));
		}
	}
}

///////////////////////////////////////////////////////////////
void CommandRetractLimb::Initialize(CommandLimbStore& store)
{
	// Lets indicate there is nothing yet to do
	m_nSegment        = MAX_SEGMENTS;

	// We may be controlling a different limb
	m_nLimb     = store.m_pArg->GetLimb(store.m_nLimb);

	// That limb may not be visible
	if (m_nLimb >= store.m_pBiot->trait.GetLines())
		return;

	// What limb type is this limb?
	int nRealLimbType = store.m_pBiot->trait.GetLineTypeIndex(m_nLimb);

	// Walk all the segments and determine the max segment 
	for (int i = MAX_SEGMENTS - 1; i >= 0; i--)
	{
		GeneSegment& segment = store.m_pBiot->trait.GetSegmentType(nRealLimbType, i);

		if (m_nSegment == MAX_SEGMENTS)
		{
			if (segment.IsVisible())
			{
				m_nMaxRadius     = segment.GetRawRadius();
				m_nAppliedRadius = m_nMaxRadius;
				m_nSegment       = i;
			}
		}
		else
		{
			// We found another visible segment (can't retract when only one line exists!)
			if (segment.IsVisible())
				break;
		}
	}

	// We must not have found another visible segment!
	if (i < 0)
	{
		m_nSegment = MAX_SEGMENTS;
	}
}

void CommandRetractLimb::Execute(CommandLimbStore& store)
{
	if (m_nSegment == MAX_SEGMENTS)
		return;

	if (store.IsSensorTrue())
	{
		// If we are true, we need to adjust back full extension
		if (m_nAppliedRadius > 0)
		{
			m_nAppliedRadius -= store.m_pBiot->RetractLine(m_nSegment, m_nLimb, m_nMaxRadius);
		}
	}
	else
	{
		// If we are false, we need to adjust back full extension
		if (m_nAppliedRadius < m_nMaxRadius)
		{
			m_nAppliedRadius += store.m_pBiot->ExtendLine(m_nSegment, m_nLimb);
		}
	}
}


///////////////////////////////////////////////////////////////
//
// Tell all the limbs of a particular type to retract
void CommandRetractLimbType::Initialize(CommandLimbStore& store)
{
	// Lets indicate there is nothing yet to do
	m_nSegment        = MAX_SEGMENTS;

	// What limb type is this limb?
	m_nLimbType = store.m_pArg->GetLimbType();

	// Maybe this limb type isn't visible
	if (!store.m_pBiot->trait.IsLineTypeVisible(m_nLimbType))
		return;

	// Walk all the segments and determine the max segment 
	for (int i = MAX_SEGMENTS - 1; i >= 0; i--)
	{
		GeneSegment& segment = store.m_pBiot->trait.GetSegmentType(m_nLimbType, i);

		if (m_nSegment == MAX_SEGMENTS)
		{
			if (segment.IsVisible())
			{
				m_nMaxRadius     = segment.GetRawRadius();
				m_nAppliedRadius = m_nMaxRadius;
				m_nSegment       = i;
			}
		}
		else
		{
			// We found another visible segment (can't retract when only one line exists!)
			if (segment.IsVisible())
				break;
		}
	}
	// We must not have found another visible segment!
	if (i < 0)
	{
		m_nSegment = MAX_SEGMENTS;
	}
}


void CommandRetractLimbType::Execute(CommandLimbStore& store)
{
	if (m_nSegment == MAX_SEGMENTS)
		return;

	if (store.IsSensorTrue())
	{
		// If we are true, we need to adjust back full extension
		if (m_nAppliedRadius > 0)
		{
			m_nAppliedRadius -= store.m_pBiot->RetractLimbType(m_nSegment, m_nLimbType, m_nMaxRadius);
		}
	}
	else
	{
		// If we are false, we need to adjust back full extension
		if (m_nAppliedRadius < m_nMaxRadius)
		{
			m_nAppliedRadius += store.m_pBiot->ExtendLimbType(m_nSegment, m_nLimbType);
		}
	}
}


///////////////////////////////////////////////////////////////
void CommandNOP::Initialize(CommandLimbStore& /*store*/)
{
 // Nothing to do!
}

void CommandNOP::Execute(CommandLimbStore& /*store*/)
{
 	BTRACE0("CommandNOP::Execute\n");
  // Nothing to do!
}


///////////////////////////////////////////////////////////////
void CommandMemory::Initialize(CommandLimbStore& store)
{
	CommandArgument& arg = *store.m_pArg;

	m_bSet = false;
	if (arg.SetAlgorithmOne())
		m_type = WAIT_FOR_TRUE_SET;
	else
		m_type = WAIT_AND_SET;

	m_time   = arg.SetDuration();

}

void CommandMemory::Execute(CommandLimbStore& store)
{
	CommandArgument& arg = *store.m_pArg;

	if (!m_bSet)
	{
		// Before set:
		// Case one:  Certain things are true for a period of time, we set the bit
		// Case two:  A period of time passes, we set the bit
		switch (m_type)
		{
		case WAIT_FOR_TRUE_SET:
			if (!store.IsSensorTrue())
				m_time = arg.SetDuration();

		case WAIT_AND_SET:
			break;
		}

		m_time--;
		if (m_time <= 0)
		{
			// We can either clear or set the state bit
			if (store.m_pArg->WhatIsConsideredSet())
			{
				store.m_pBiot->m_internalState |= arg.WhichStateBit();
			}
			else
			{
				store.m_pBiot->m_internalState &= (~arg.WhichStateBit());
			}

			// Now that we triggered, set the time we should maintain this state
			m_time = arg.ClearDuration();
			m_bSet = true;

			// Set our clearing algorithm
			if (arg.ClearAlgorithmOne())
				m_type = WAIT_FOR_FALSE_CLEAR;
			else
				m_type = WAIT_AND_CLEAR;
			BTRACE1("Memory %d set\n", arg.WhichStateBit());
		}
	}
	else
	{
		// After set:
		// Case one:    Once set, certain things are false for a period of time, we clear the bit
		// Case two:    Once set, we clear the bit after a period of time
		// Case three:  Once set, we never clear the bit
		switch (m_type)
		{
		case WAIT_AND_CLEAR:
			break;

		case WAIT_FOR_FALSE_CLEAR:
			if (!store.IsSensorTrue())
				m_time = arg.ClearDuration();
			break;
		}

		m_time--;
		if (m_time <= 0)
		{
			if (arg.WhatIsConsideredSet())
			{
				store.m_pBiot->m_internalState &= (~arg.WhichStateBit());
			}
			else
			{
				store.m_pBiot->m_internalState |= arg.WhichStateBit();
			}
			m_time = arg.SetDuration();
			m_bSet = false;

			if (arg.SetAlgorithmOne())
				m_type = WAIT_FOR_FALSE_CLEAR;
			else
				m_type = WAIT_AND_CLEAR;
		}
	}
}








