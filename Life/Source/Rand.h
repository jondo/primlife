/*
------------------------------------------------------------------------------

rand.h: definitions for a random number generator

------------------------------------------------------------------------------
*/
#ifndef RAND_H
#define RAND_H

typedef unsigned   int  ub4;   /* unsigned 4-byte quantities */
#define RANDSIZL   (8)
#define RANDSIZ    (1<<RANDSIZL)


//////////////////////////////////////////////////////////////////////
//
// Randomizer
//
// Ensures that the randomizer is seeded and provides a little more
// functionality to rand().
//
//
class Randomizer
{
private:
	static ub4 randrsl[RANDSIZ];
	static ub4 randcnt;
	static ub4 mm[RANDSIZ];
	static ub4 aa, bb, cc;
  
	void RandInit(int seed);
	void Isaac();

	unsigned int rand()
	{ 
		return (!randcnt-- ? (Isaac(), randcnt=RANDSIZ-1, randrsl[randcnt]) : randrsl[randcnt]);
	}

public:
	Randomizer(void);

	// Set up seed
	void RandSeed(int seed);

	// Save or load seed
	void Serialize(CArchive& ar);

	// Helpful random methods
	int Integer(int max)
	{
		return rand() % max;
	}

	short Short(int max);

	BYTE Byte(int max)
	{
		return (BYTE) (rand() % max);
	}

	BYTE Byte()
	{
		return (BYTE) rand();
	}

	int Sign(void)
	{
		if (((int)rand()) < 0)
			return -1;
		else
			return 1;
	} 
                    
	int Flip(void)
	{
		return (rand() & 0x0100);
	} 

	bool boolean(void)
	{
		return ((rand() & 0x0010) == 0x0010);
	}

	BOOL Bool(void)
	{
		return ((rand() & 0x0010) == 0x0010);
	} 

	int Int1024(void)
	{
		return (rand() & 0x000003FF);
	}

	int Int256(void)
	{
		return ((rand() & 0x000000FF));
	}

	int Int(void)
	{ 
		return rand();
	}

	// Returns -1.0 to 1.0
	float Float(void)
	{
		return ((((float)(Int256() * 2)) / ((float) 255)) - (float) 1);
	}

	DWORD Dword(void)
	{
		return rand();
	}
};


//////////////////////////////////////////////////////////////////////
//
// RandomKey
//
// Ensures that the randomizer is seeded and provides a little more
// functionality to rand().
//
//
class RandomKey
{
private:
	static ub4 randrsl[RANDSIZ];
	static ub4 randcnt;
	static ub4 mm[RANDSIZ];
	static ub4 aa, bb, cc;
  
	void RandInit(int seed);
	void Isaac();

	unsigned int rand()
	{ 
		return (!randcnt-- ? (Isaac(), randcnt=RANDSIZ-1, randrsl[randcnt]) : randrsl[randcnt]);
	}

	// Set up seed
	void RandSeed(int seed);

public:
	bool CheckKey(long lKey);
	void SetKey(long& lKey);
};


#endif