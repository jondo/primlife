// CSettings
//
// Contains all the possible settings for an Environment
// and its biots.

#include "genotype.h"
#include "etools.h"

class BRect;

#pragma once

class CSettings 
{
	// Public Methods
public:
	CSettings() { Reset(600, 340); }

	// Identity constructor
	CSettings(CSettings &s){*this = s;}

	int  GetFriction() { return (int) ((1000.0 * friction) + 0.5F); }	
	void SetFriction(int iFriction) {friction = ((float)iFriction) / 1000.0F; }

	void Serialize(CArchive& a);
	void Reset(int nWidth, int nHeight);
	CSettings& operator=(CSettings& s);
	
	void SanityCheck();

	enum {
		BOTTOM = 0,
		TOP    = 1,
		RIGHT  = 2,
		LEFT   = 3,
		SIDES  = 4
	};

// Public Variables
public: //Fox BEGIN
    int m_nSick;
	int m_initialPopulation;		//Population used on restart/new
	int     m_leafEnergy;
	CSoundRegistry m_sound;
    HPEN hPen[MAX_LEAF];
    int     startNew;    // Should we use the old population, or the new?
    int     chance;
    int     nSexual;
    int     newType[MAX_LEAF];
    short   effectiveLength[MAX_LEAF];
    DWORD   m_generation;
    LONG    leafContact[MAX_LEAF][MAX_LEAF];
    LONG    leafMass[MAX_LEAF];
    LONG    startEnergy;
    BOOL    bSoundOn;
    BOOL    bBarrier;
    BOOL    bMouse;    
    BOOL    bParentAttack;
    BOOL    bSiblingsAttack;    

    BOOL    bNoFlickerSet;
    float   friction;
    BRect   barrier;
    int     maxLineSegments;
    LONG    regenCost;
    DWORD   regenTime;
	int  	m_nSeed;
	int		m_nStartingPopulation;
	int		m_nArmTypesPerBiot;
	int		m_nSegmentsPerArm;
	int		m_nArmsPerBiot;
	BOOL	m_bGenerateOnExtinction;

	CString m_sSideAddress[SIDES];
	BOOL    m_bSideEnable[SIDES];

	int m_nHeight;
	int m_nWidth;
	int m_nSizeChoice;
};//Fox END;
