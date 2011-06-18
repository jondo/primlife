#include "stdafx.h"
#include "primordial life.h"
#include "settings.h"


void CSettings::Serialize(CArchive& a)
{
	const int nVersion = 2;

	if (a.IsStoring())
	{
		a << nVersion;
		a << m_nSeed;
		a << m_nStartingPopulation;
		a << m_nArmTypesPerBiot;
		a << m_nSegmentsPerArm;
		a << m_nArmsPerBiot;
		a << m_bGenerateOnExtinction;

		for (int i = 0; i < SIDES; i++)
		{
			a << m_sSideAddress[i];
			a << m_bSideEnable[i];
		}

		a << m_nHeight;
		a << m_nWidth;
		a << m_nSizeChoice;

	}
	else
	{
		int nThisVersion;
		a >> nThisVersion;

		if (nThisVersion != nVersion)
		{
			throw new CArchiveException(CArchiveException::badIndex);
		}

		a >> m_nSeed;
		a >> m_nStartingPopulation;
		a >> m_nArmTypesPerBiot;
		a >> m_nSegmentsPerArm;
		a >> m_nArmsPerBiot;
		a >> m_bGenerateOnExtinction;

		for (int i = 0; i < SIDES; i++)
		{
			a >> m_sSideAddress[i];
			a >> m_bSideEnable[i];
		}

		a >> m_nHeight;
		a >> m_nWidth;
		a >> m_nSizeChoice;
	}
}

void CSettings::SanityCheck()
{
	if (m_nStartingPopulation > 50)
		m_nStartingPopulation = 50;

	for (int i = 0; i < SIDES; i++)
	{
		if (m_sSideAddress[i].IsEmpty())
			m_bSideEnable[i]  = FALSE;
	}

	if (m_nHeight > 4000)
		m_nHeight = 4000;

	if (m_nWidth > 4000)
		m_nWidth = 4000;

	if (m_nSizeChoice  > 3 ||
		m_nSizeChoice < 0)
		m_nSizeChoice = 2;
}


CSettings& CSettings::operator=(CSettings& s)
{
	m_nSick               = s.m_nSick;
	m_initialPopulation   = s.m_initialPopulation;
	m_leafEnergy          = s.m_leafEnergy;
	m_sound               = s.m_sound;
    //hPen                  = s.hPen;
    startNew              = s.startNew;    
    chance                = s.chance;
    nSexual               = s.nSexual;
    //newType               = s.newType;
    //effectiveLength       = s.effectiveLength;
    m_generation          = s.m_generation;
    //leafContact           = s.leafContact;
    //leafMass              = s.leafMass;
    startEnergy           = s.startEnergy;
    bSoundOn              = s.bSoundOn;
    bBarrier              = s.bBarrier;
    bMouse                = s.bMouse;    
    bParentAttack         = s.bParentAttack;
    bSiblingsAttack       = s.bSiblingsAttack;    

    bNoFlickerSet         = s.bNoFlickerSet;
    friction              = s.friction;    
    maxLineSegments       = s.maxLineSegments;
    regenCost             = s.regenCost;
    regenTime             = s.regenTime;

  	m_nSeed                 = s.m_nSeed;                
	m_nStartingPopulation   = s.m_nStartingPopulation;  
	m_nArmTypesPerBiot      = s.m_nArmTypesPerBiot;     
	m_nSegmentsPerArm       = s.m_nSegmentsPerArm;      
	m_nArmsPerBiot          = s.m_nArmsPerBiot;         
	m_bGenerateOnExtinction = s.m_bGenerateOnExtinction;

	for (int i = 0; i < SIDES; i++)
	{
		m_sSideAddress[i] = s.m_sSideAddress[i];
		m_bSideEnable[i]  = s.m_bSideEnable[i];
	}

	m_nHeight     = s.m_nHeight;
	m_nWidth      = s.m_nWidth;
	m_nSizeChoice = s.m_nSizeChoice;

	return *this;
}

void CSettings::Reset(int nWidth, int nHeight)
{
  	m_nSeed                 = GetTickCount();
	m_nStartingPopulation   = 20;
	m_nArmTypesPerBiot      = 1;
	m_nSegmentsPerArm       = 10;
	m_nArmsPerBiot          = 0;
	m_bGenerateOnExtinction = TRUE;

	for (int i = 0; i < SIDES; i++)
	{
		m_sSideAddress[i] = _T("");
		m_bSideEnable[i]  = FALSE;
	}

	m_nHeight     = nWidth;
	m_nWidth      = nHeight;
	m_nSizeChoice = 1;

}
