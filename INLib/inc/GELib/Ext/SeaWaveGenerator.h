//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/SeaWaveGenerator.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_SEAWAVEGENERATOR_H__
#define __GELIB_EXT_SEAWAVEGENERATOR_H__

#include <GELib/Ext/FftComputer.h>
#include "WaterTypeDef.h"
#include <vector>

namespace GELib
{

class CSeaWaveGenerator
{
public:
	CSeaWaveGenerator(void);
	bool Initialize(const GeneratorDesc& _desc);
	bool UpdateDesc(const GeneratorDesc& _desc);
	bool GenerateFFTWave(double _fTotalTime, std::vector<float>* _pOutVector
		, std::vector<CVector4>* _pOutVectorWithDisplace, bool _bNormalize = true);
	const COMPLEX* GetHightMap( )const {return m_pFourierAmplitude;}
	~CSeaWaveGenerator(void);

private:
	double _Omega(double _fKLen);
	double _Phillips(double _fKx, double _fKy);
	bool _CreateSpatialSpectrum();
	bool _UpdateFourierAmplitude(double _fTotalTime);
	void _GetGaussianRandom(double& _fOutX, double& _fOutY);

private:
	GeneratorDesc m_generatorDesc;
	typedef double HoldType[6];
	HoldType		*m_pSpatialSpectrum;
	COMPLEX			*m_pFourierAmplitude;
	COMPLEX			*m_pFourierResult;
	CFftComputer	m_ffter;
	float			m_fClipPeriod;
	float			m_fMaxWaveHeight;
	bool			m_bHighFrequency;

	//for diaplace
	bool			m_bComputeDisplace;
	CFftComputer	m_ffterDisplaceX;
	CFftComputer	m_ffterDisplaceY;
	COMPLEX			*m_pDisplaceResultX;
	COMPLEX			*m_pDisplaceResultY;
	COMPLEX			*m_pFourierDisplaceX;
	COMPLEX			*m_pFourierDisplaceY;
};

}//GELib

#endif//__GELIB_EXT_SEAWAVEGENERATOR_H__
