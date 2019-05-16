//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/FftComputer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_FFTCOMPUTER_H__
#define __GELIB_EXT_FFTCOMPUTER_H__

//#define USING_FFTW

#ifdef USING_FFTW
#include "fftw/inc/fftw3.h"
#endif

namespace GELib
{

#ifdef USING_FFTW
	typedef fftw_complex COMPLEX;
#else
	typedef  double COMPLEX[2];
#endif

class CFftComputer
{
public:
	CFftComputer();
	~CFftComputer();
	//_iDir -1: inverse fft , +1: fft
	bool CreatePlan(COMPLEX *_pComplexs,COMPLEX *_pResult,int _iSizeX,int _iSizeY,int _iDir);
	bool ExecutePlan();
	COMPLEX * AllocComplex(int iElementSize)
	{
#ifdef USING_FFTW
		return (COMPLEX*)fftw_malloc(iElementSize*sizeof(COMPLEX));
#else
		return new COMPLEX[iElementSize];
#endif
	}
	void DeleteComplex(COMPLEX * _pData)
	{
#ifdef USING_FFTW
		if(_pData)
		{
			fftw_free(_pData);
		}
#else
		if(_pData)
			delete[] _pData;
#endif
	}

private:
	bool   _FFT(int dir,int m,double *x,double *y);
	bool   _FFT2D(void);
	bool   _Powerof2(int n,int& m);
	void   _Reset();

private:
	double *m_pReal;
	double *m_pImgi;
	COMPLEX *m_pComplexRef;
	COMPLEX *m_pResultRef;
	int m_iDir;
	int m_iSizeX;
	int m_iSizeY;
	int m_iSizePowX;
	int m_iSizePowY;
#ifdef USING_FFTW
	fftw_plan	 m_fftPlan;
#endif
};

}//GELib

#endif//__GELIB_EXT_FFTCOMPUTER_H__
