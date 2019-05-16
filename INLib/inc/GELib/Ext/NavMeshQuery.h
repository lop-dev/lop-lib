//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/NavMeshQuery.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_NAVMESHQUERY_H__
#define __GELIB_EXT_NAVMESHQUERY_H__

#include <GELib/Math/GeMath.h>
#include <vector>
#include <float.h>

class dtNavMeshQuery;
class dtQueryFilter;

namespace GELib
{


class CNavMeshAsset;

class CNavPath
{
public:
	std::vector<CVector3> m_P;
};

class CNavMeshQuery
{
public:
    CNavMeshQuery();
    virtual ~CNavMeshQuery();

	void Initialize(CNavMeshAsset *m);
	void Uninitialize();

	void SetExtents(const CVector3 &e) { m_Extents = e; }
	const CVector3& GetExtents() { return m_Extents; }

	unsigned int GetNearstPoly(const CVector3 &pos, float maxDistance = FLT_MAX);									// return 0 if not on nav mesh
	void FindPath(const CVector3 &startPos, const CVector3 &endPos, CNavPath &path);
	void FindPath(const CVector3 &startPos, unsigned int startPoly, const CVector3 &endPos, CNavPath &path);
	bool RayCast(const CVector3 &startPos, const CVector3 &endPos, float *t=0);			// return success or fail, *t is not modified if hit nothing
	bool RayCast(const CVector3 &startPos, unsigned int startPoly, const CVector3 &endPos, float *t=0);			// return success or fail, *t is not modified if hit nothing
	bool GetHeight(const CVector3 &pos, float &h);
	bool GetHeight(const CVector3 &pos, unsigned int poly, float &h);

private:

	CNavMeshAsset *m_pNavMesh;
	dtNavMeshQuery *m_pNavQuery;
	dtQueryFilter *m_pFilter;
	CVector3 m_Extents;
};


}//GELib

#endif//__GELIB_EXT_NAVMESHQUERY_H__
