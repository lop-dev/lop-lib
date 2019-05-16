//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/WaterSurface.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_WATERSURFACE_H__
#define __GELIB_EXT_WATERSURFACE_H__

#include <GELib/GeConfig.h>
#include "WaterTypeDef.h"

namespace GELib
{

class CRendererContext;
class CRenderer;
class CWaterInfo;

class CWaterSurface
{

public:
	GELIB_API CWaterSurface(void);
	GELIB_API virtual ~CWaterSurface(void);
	GELIB_API virtual bool CreateSurface(const SWaterDesc& _waterDesc) = 0;
	GELIB_API virtual bool UpdateDesc(const SWaterDesc& _waterDesc) = 0;
	GELIB_API virtual void* GetVertexData(unsigned int& _OutSize) = 0;
	GELIB_API virtual bool OnRecreateSource()
	{
		return false;
	}
	GELIB_API virtual bool BuildSurface(CRendererContext &rc)
	{
		return true;
	}
	GELIB_API virtual bool OnUpdateData()
	{
		return true;
	}
	GELIB_API virtual bool OnRender(CRenderer *renderer)
	{
		return false;
	}
    GELIB_API virtual bool GetWaterInfo(CWaterInfo *waterInfo)
    {
        return false;
    }

	GELIB_API virtual int GetHeightMap()
	{
		return -1;
	}
	GELIB_API virtual int GetNormalMap()
	{
		return -1;
	}
	GELIB_API virtual bool GetVisible()
	{
		return true;
	}

	GELIB_API void	IncreaseRef(){ ++m_iRefrenceCounter;}
	GELIB_API void	DecreaseRef(){ --m_iRefrenceCounter;}
	GELIB_API int   GetRefCounter(){ return m_iRefrenceCounter;}

protected:
	int m_iRefrenceCounter;
};

}//GELib


#endif//__GELIB_EXT_WATERSURFACE_H__
