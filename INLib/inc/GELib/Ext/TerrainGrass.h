//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/TerrainGrass.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_TERRAINGRASS_H__
#define __GELIB_EXT_TERRAINGRASS_H__

#include <GELib/Math/GeMath.h>

namespace GELib
{


const int TERRAIN_GRASS_INFO_MAX = 16;
const int TERRAIN_GRASS_DISTRIBUTION_MAX = 16;
const int TERRAIN_GRASS_CELL_SIZE = 8;
const int TERRAIN_GRASS_NUM_PER_CELL = TERRAIN_GRASS_CELL_SIZE * TERRAIN_GRASS_CELL_SIZE * 16;

class CTerrainGrassInfo
{
public:
	CTerrainGrassInfo();
	int m_iTextureType;
	float m_fWidth;
	float m_fHeight;
	float m_fWindStrength;
	float m_fWindFrequency;
	float m_ScaleVariation[2];
	float m_NormalVariation[2];
};

class CTerrainGrassDistribution
{
public:
	CTerrainGrassDistribution();

	class Distribution
	{
	public:
		Distribution();
		unsigned char m_iInfo;
		unsigned short m_iCount;
		float m_fScale;
	};

	Distribution m_Dis[4];
};

class CTerrainGrassVertex
{
public:
	short m_Position[3];
	short m_Scale;
	int m_Normal;	// normal and grass type
};


}//GELib


#endif//__GELIB_EXT_TERRAINGRASS_H__
