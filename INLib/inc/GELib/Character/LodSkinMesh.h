//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/LodSkinMesh.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_LODSKINMESH_H__
#define __GELIB_CHARACTER_LODSKINMESH_H__

#include <vector>
#include <string>

#include "SkinMesh.h"

namespace GELib
{

class CIOStream;
class CLoadTask;
class CStdMaterial;
class CMeshUtility;
class CLoadParameter;
class CBaseAsset;
class CSkinMeshAsset;

class CLodSkinMesh
{
public:
    CLodSkinMesh();
    virtual ~CLodSkinMesh();

    bool Save(CBaseAsset *container, CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
    bool Import(const std::vector<CMeshUtility*> &meshUtils, CSkinMeshAsset *group, const CLoadParameter &param);
    void Unload();
	bool CreateLoadedResource();

    int GetNumLods();
    CSkinMesh* GetMesh(int lod);    // lod 0 is highest level (most detail)
    const CAABox& GetBoundingBox() { return m_Box; }

    const std::string& GetName() { return m_Name; }
    void SetName(const char *name) { m_Name = name; }

private:

    class CLod
    {
    public:
        CSkinMesh m_Mesh;
    };

    std::vector<CLod> m_Lods;
    CAABox m_Box;
    std::string m_Name;
	bool m_bNeedCreateLoadedResource;
};



inline int CLodSkinMesh::GetNumLods()
{
    return (int)m_Lods.size();
}

inline CSkinMesh* CLodSkinMesh::GetMesh(int lod)
{
    //assert(lod>=0 && lod<GetNumLods());
    if (lod>=0 && lod<GetNumLods())
        return &m_Lods[lod].m_Mesh;
    return 0;
}



}//GELib

#endif//__GELIB_CHARACTER_LODSKINMESH_H__
