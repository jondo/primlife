//////////////////////////////////////////////
// Brain.h
//
// Implemented by Jason Spofford
//
//
#pragma once

#if defined(DEBUG_BRAIN)
#define BTRACE0(sz)              TRACE0(sz)
#define BTRACE1(sz, p1)          TRACE1(sz, p1)
#define BTRACE2(sz, p1, p2)      TRACE2(sz, p1, p2)
#define BTRACE3(sz, p1, p2, p3)  TRACE3(sz, p1, p2, p3)
#else
#define BTRACE0(sz) 
#define BTRACE1(sz, p1)
#define BTRACE2(sz, p1, p2)
#define BTRACE3(sz, p1, p2, p3)
#endif 

///////////////////////////////////////////////////////////////
// ProductTerm
//
// Contains one product term for 32 bit wide sensor data.
// A product term is not internally crossed over.
// Sensor data is combined with the mask and inversion to 
// determine if the if the product term is true and possibly
// an action should be taken.
//
class ProductTerm
{
public:
	void Mutate(int nChance);
	void Randomize();
	void Debug();
	void Serialize(CArchive &ar);
	bool IsTrue(DWORD m_sensor);
	ProductTerm& operator=(ProductTerm& term);

protected:
	DWORD m_dwMask;
	DWORD m_dwInvert;
};

// For the given sensor list, is this term
// true or false?
inline bool ProductTerm::IsTrue(DWORD dwSensor)
{
	return (((dwSensor ^ m_dwInvert) & m_dwMask) == m_dwMask);
}

inline ProductTerm& ProductTerm::operator=(ProductTerm& term)
{
	m_dwInvert = term.m_dwInvert;
	m_dwMask   = term.m_dwMask;
	return *this;
}


//////////////////////////////////////////////////////////////////////////
// ProductSum
//
// Refers to a selection of product terms in the product array to
// determine if a command should be executed.
// Implements "Sum of the Products(Terms)"
//
// If all terms are false, the result will be m_bTrue.
// m_bTrue provides the ability to invert the overall result.
//
class ProductArray; // Forward declare of the array of ProductTerm's and ProductSum's
class ProductSum
{
public:
	enum {
		MAX_PRODUCT_SUM_TERMS = 8
	};

	int GetCount();

	void Mutate(int nChance);
	void Randomize();
	void Debug();
	void Serialize(CArchive& ar);
	void Crossover(ProductSum& productSum);

	ProductSum& operator=(ProductSum& productSum);

	bool IsTrue(ProductArray& productArray, DWORD dwSensor);

protected:
	BYTE m_reference[MAX_PRODUCT_SUM_TERMS];
	BOOL m_bTrue;
};

inline int ProductSum::GetCount()
{
	return MAX_PRODUCT_SUM_TERMS;
}


//////////////////////////////////////////////////////////////////////////////
// Product Array
//
// Contains all the product terms of a biot and all the product sums of a biot
//
// A ProductSum refers to a selection of ProductTerms.  ProductTerms can be 
// shared among ProductSums to provide flexibility in evolution.
//
class ProductArray
{
public:
	enum {
		MAX_PRODUCT_TERMS = 256,
		MAX_PRODUCT_SUMS  = 64
	};

	int  GetSumCount();

	void Mutate(int nChance);
	void Randomize();
	void Debug();
	void Serialize(CArchive& ar);
	void Crossover(ProductArray& productArray);
	ProductArray& operator=(ProductArray& productArray);

	bool IsTrue(int nProductSum, DWORD dwSensor);
	ProductTerm& operator [](int index);

protected:
	int  GetTermCount();

protected:
	ProductTerm m_productTerm[MAX_PRODUCT_TERMS];
	ProductSum  m_productSum[MAX_PRODUCT_SUMS];
};

inline ProductTerm& ProductArray::operator [](int index)
{
	ASSERT(index < GetTermCount() && index >= 0);
	return m_productTerm[index];
}

inline int ProductArray::GetTermCount()
{
	return MAX_PRODUCT_TERMS;
}

inline int ProductArray::GetSumCount()
{
	return MAX_PRODUCT_SUMS;
}

inline bool ProductArray::IsTrue(int nProductSum, DWORD dwSensor)
{
	ASSERT(nProductSum < GetSumCount() && nProductSum >= 0);
	BTRACE2("ProductSum %d, Sensor %x, Result ", nProductSum, dwSensor);
	return m_productSum[nProductSum].IsTrue(*this, dwSensor);
}



///////////////////////////////////////////////////////////////
// CommandArgument
//
// Contains a command and the associated arguments.  Does not
// contain runtime variables required to execute the command.
//
struct CommandArgument
{
public:

	// Genetic Functions
    void Randomize();
    void Mutate(int chance);
	void Serialize(CArchive& ar);

	enum {
		COMMAND_FLAP_LIMB_SEGMENT = 0, 
		COMMAND_FLAP_LIMB_TYPE_SEGMENT, 
		COMMAND_MOVE_LIMB_SEGMENT,
		COMMAND_MOVE_LIMB_SEGMENTS,
		COMMAND_MOVE_LIMB_TYPE_SEGMENT,
		COMMAND_MOVE_LIMB_TYPE_SEGMENTS,
		COMMAND_RETRACT_LIMB_TYPE,
		COMMAND_RETRACT_LIMB,
		COMMAND_NOP,
		COMMAND_MEMORY,
		COMMAND_MAX_TYPES
	};

	// What command?
	int   m_command;

	// What relative limb or which limb type
	BYTE  m_limb;

	// What segment of a limb? (limited to MAX_SEGMENTS)
	BYTE  m_segment;

	// How fast to move it  (each command interpretes this differently)
	BYTE  m_rate;

	// How many degrees to move (each command looks at this differently)
	BYTE  m_degrees;

	int GetLimb(int actualLimb);
	short GetSegment()  { return m_segment; }
	int GetCommand()  { return m_command; }
	int GetLimbType() { return m_limb & 0x03; } // four limb types
	short GetRate()     { return (short) (m_rate & 0x03); }
	short GetDegrees()  { return m_degrees; }


	// Memory Perspective
	bool WhatIsConsideredSet() { return (0x10 & m_limb) == 0x10; }
	int  WhichStateBit()       { return (0x00000001 << ((int)(0x07 & m_limb))); }
	int  SetDuration()         { return (int) m_degrees * 4; }
	bool SetAlgorithmOne()     { return (m_segment & 0x02) == 0x02; }
	int  ClearDuration()       { return (int) m_rate * 4; }
	bool ClearAlgorithmOne()   { return (m_segment & 0x01) == 0x01; }
	bool ClearAlgorithmTwo()   { return (m_segment & 0x04) == 0x04; }

};



//////////////////////////////////////////////////////////////////////////
// class CommandLimbType
//
// Refers to commands in the command array for each limb type.
// Contains a reference to a product sum and the command that
// product sum actuates.  Commands are for a type of limb as
// opposed to a specific limb.  This supports symmetrical responses
// for the same limb type across the biot.  
//
class CommandLimbType
{
public:
	enum {
		MAX_COMMANDS_PER_LIMB = 16
	};

	int GetCount();

	void Mutate(int nChance);
	void Randomize();
	void Serialize(CArchive& ar);
	void Crossover(CommandLimbType& commandLimbType);

	int GetCommand(int index);
	int GetProductSum(int index);

	CommandLimbType& operator=(CommandLimbType& commandLimbType);

protected:
	BYTE  m_comref[MAX_COMMANDS_PER_LIMB];
	BYTE  m_sumref[MAX_COMMANDS_PER_LIMB];
};

inline int CommandLimbType::GetCount()
{
	return MAX_COMMANDS_PER_LIMB;
}

inline int CommandLimbType::GetCommand(int index)
{
	ASSERT(index < MAX_COMMANDS_PER_LIMB  && index >= 0);
	return m_comref[index];
}

inline int CommandLimbType::GetProductSum(int index)
{
	ASSERT(index < MAX_COMMANDS_PER_LIMB  && index >= 0);
	return m_sumref[index];
}


///////////////////////////////////////////////////////////////
// CommandArray
//
// Contains all the biot commands.  Also contains the 
// ProductArray for processing sensor data and associating that
// data with an actuator.
//
class CommandArray
{
public:
	enum {
		MAX_COMMANDS = 64
	};

	int  GetCommandCount();
	int  GetTypeCount();

	void Mutate(int nChance);
	void Randomize();
	void Serialize(CArchive& ar);
	void Crossover(CommandArray& commandArray);
	CommandArray& operator=(CommandArray& commandArray);
	CommandArgument& GetCommandArgument(int nLimbType, int nCommand);
	bool IsTrue(int nLimbType, int nCommand, DWORD dwSensor);

protected:
	CommandArgument m_command[MAX_COMMANDS];
	ProductArray    m_productArray;
	CommandLimbType m_commandLimbType[MAX_LIMB_TYPES];
};

inline bool CommandArray::IsTrue(int nLimbType, int nCommand, DWORD dwSensor)
{
	ASSERT(nLimbType < MAX_LIMB_TYPES && nLimbType >= 0);
	return m_productArray.IsTrue(m_commandLimbType[nLimbType].GetProductSum(nCommand), dwSensor);
}
	
inline CommandArgument& CommandArray::GetCommandArgument(int nLimbType, int nCommand)
{ 
	ASSERT(nLimbType < MAX_LIMB_TYPES && nLimbType >= 0);
	return m_command[m_commandLimbType[nLimbType].GetCommand(nCommand)];
}

inline int CommandArray::GetCommandCount()
{
	return MAX_COMMANDS;
}

inline int CommandArray::GetTypeCount()
{
	return MAX_LIMB_TYPES;
}


class Biot;
class CommandLimbStore;


///////////////////////////////////////////////////////////////
//Don't give me no flap!
//
class CommandFlapLimbSegment
{
public:
	void Initialize(CommandLimbStore& store);
	void Execute(CommandLimbStore& store);
	void Flap(Biot& biot);

protected:
	int   m_nLimb;
	int   m_nSegment;
	int   m_nRate;
	int   m_nMaxDegrees;
	int   m_nAppliedDegrees;
	bool  m_bGoingUp;
};


///////////////////////////////////////////////////////////////
//Don't give me no flap!
//
class CommandFlapLimbTypeSegment 
{
public:
	void Initialize(CommandLimbStore& store);
	void Execute(CommandLimbStore& store);
	void Flap(Biot& biot, int nPeno);
	void FlapLimbTypeSegments(Biot& biot);

protected:
	int   m_nLimbType;
	int   m_nSegment;
	int   m_nRate;
	int   m_nMaxDegrees;
	int   m_nAppliedDegrees;
	bool  m_bGoingUp;
};

///////////////////////////////////////////////////////////////
class CommandMoveLimbSegment
{
public:
	void Initialize(CommandLimbStore& store);
	void Execute(CommandLimbStore& store);

protected:
	int   m_nLimb;
	int   m_nSegment;
	int   m_nRate;
	int   m_nMaxDegrees;
	int   m_nAppliedDegrees;
};


///////////////////////////////////////////////////////////////
class CommandMoveLimbSegments
{
public:
	void Initialize(CommandLimbStore& store);
	void Execute(CommandLimbStore& store);

protected:
	int   m_nLimb;
	int   m_nRate;
	int   m_nMaxDegrees;
	int   m_nAppliedDegrees;
};

///////////////////////////////////////////////////////////////
class CommandMoveLimbTypeSegment
{
public:
	void Initialize(CommandLimbStore& store);
	void Execute(CommandLimbStore& store);

protected:
	int   m_nLimbType;
	int   m_nSegment;
	int   m_nRate;
	int   m_nMaxDegrees;
	int   m_nAppliedDegrees;
};


///////////////////////////////////////////////////////////////
class CommandMoveLimbTypeSegments
{
public:
	void Initialize(CommandLimbStore& store);
	void Execute(CommandLimbStore& store);

protected:
	int   m_nLimbType;
	int   m_nRate;
	int   m_nMaxDegrees;
	int   m_nAppliedDegrees;
};

///////////////////////////////////////////////////////////////
class CommandRetractLimbType
{
public:
	void Initialize(CommandLimbStore& store);
	void Execute(CommandLimbStore& store);

protected:
	int   m_nSegment;
	int   m_nLimbType;
	int   m_nMaxRadius;
	int   m_nAppliedRadius;
};


///////////////////////////////////////////////////////////////
class CommandRetractLimb
{
public:
	void Initialize(CommandLimbStore& store);
	void Execute(CommandLimbStore& store);

protected:
	int   m_nSegment;
	int   m_nLimb;
	int   m_nMaxRadius;
	int   m_nAppliedRadius;
};

///////////////////////////////////////////////////////////////
class CommandNOP
{
public:
	void Initialize(CommandLimbStore& store);
	void Execute(CommandLimbStore& store);

protected:
};


///////////////////////////////////////////////////////////////
class CommandMemory
{
public:
	void Initialize(CommandLimbStore& store);
	void Execute(CommandLimbStore& store);

	enum {
		WAIT_FOR_TRUE_SET = 0,
		WAIT_AND_SET,
		WAIT_FOR_FALSE_CLEAR,
		WAIT_AND_CLEAR
	};
		
protected:
	int   m_time;
	int   m_type;
	bool  m_bSet;
};


union CommandType {
	CommandFlapLimbSegment         flapLimbSegment;
	CommandFlapLimbTypeSegment     flapLimbTypeSegment;
	CommandMoveLimbSegment         moveLimbSegment;
	CommandMoveLimbSegments        moveLimbSegments;
	CommandMoveLimbTypeSegment     moveLimbTypeSegment;
	CommandMoveLimbTypeSegments    moveLimbTypeSegments;
	CommandRetractLimbType         retractLimbType;
 	CommandRetractLimb             retractLimb;
	CommandNOP                     nop;
	CommandMemory                  memory;
};


/////////////////////////////////////////////////////////////////////
// CommandLimbStore
//
// Each limb, regardless of type, has associated state information
// to store.
//
class CommandLimbStore
{
public:
	void Serialize(CArchive& ar);
	void Initialize(int nLimbType, int nLimb, Biot& biot);
	void Execute(Biot& biot, DWORD dwSensor);

	// These need to be serialized
	int              m_nLimbType;
	int              m_nLimb;

	// These do not require serialization
	int              m_index;
	DWORD            m_dwSensor;
	Biot*            m_pBiot;
	CommandArgument* m_pArg;

	// Callback for each
	bool IsSensorTrue();

protected:
	// This requires serialization
	CommandType  command[CommandLimbType::MAX_COMMANDS_PER_LIMB];
};


