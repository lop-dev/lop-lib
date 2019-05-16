//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/NavMeshAsset.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_NAVMESHASSET_H__
#define __GELIB_EXT_NAVMESHASSET_H__

#include "BaseAsset.h"
#include <GELib/Math/GeMath.h>
#include <vector>

class dtNavMesh;

namespace GELib
{

class CIOStream;
class IPlotter3D;


class CNavMeshManager;

class COffMeshConnection
{
public:
	COffMeshConnection();

	CVector3 m_P[2];
	unsigned char m_iDirection;		// 0 : one way, 1: bidirection
};

class CBuildNavMeshContext
{
public:
	CBuildNavMeshContext();

	float m_fActorWidth;
	float m_fActorHeight;
	float m_fClimbHeight;
	float m_fClimbSlope;

	int m_iTileSize;
	float m_fCellSize;
	float m_fCellHeight;

	float m_fEdgeMaxLen;
	float m_fEdgeMaxError;
	int m_iRegionMinSize;
	int m_iRegionMergeSize;
	int m_iVertsPerPoly;
	float m_fDetailSampleDist;
	float m_fDetailSampleMaxError;

	CVector3 *m_pVertices;
	int m_iNumVertices;
	int *m_pIndices;
	int m_iNumTriangles;

	COffMeshConnection *m_pConnections;
	int m_iNumConnections;
};

const int ASSETCLASS_NAV_MESH = 13;


class CNavMeshAsset : public CBaseAsset
{
public:

	friend class CNavMeshQuery;

    CNavMeshAsset(CNavMeshManager *manager, const char *cacheKey=0);
    virtual ~CNavMeshAsset();

	virtual int GetClassId() { return ASSETCLASS_NAV_MESH; }
    CNavMeshManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();

	bool Build(const CBuildNavMeshContext &ctx);

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
    void Unload();

	float GetActorWidth() { return m_fActorWidth; }
	float GetActorHeight() { return m_fActorHeight; }

	void SetConnections(const std::vector<COffMeshConnection> &con);
	void GetConnections(std::vector<COffMeshConnection> &con);
	void ClearConnections();

    void Draw(IPlotter3D &plotter);

private:

	float m_fActorWidth;
	float m_fActorHeight;
	float m_fClimbHeight;
	float m_fClimbSlope;

	dtNavMesh *m_pNavMesh;
	std::vector<COffMeshConnection> m_Connections;

    CNavMeshManager *m_pManager;
};


}//GELib

#endif//__GELIB_EXT_NAVMESHASSET_H__
