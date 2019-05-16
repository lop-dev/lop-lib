//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/GroupSceneEntity.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_GROUPSCENEENTITY_H__
#define __GELIB_SCENE_GROUPSCENEENTITY_H__

#include <vector>
#include <GELib/Math/GeMath.h>
#include "SceneEntity.h"

namespace GELib
{

class CIOStream;


class CGroupSceneEntity : public CSceneEntity
{
public:
    CGroupSceneEntity(CScene *scene);
    virtual ~CGroupSceneEntity();

    virtual int GetClassId() { return SCENEENTITY_GROUP; }
    virtual bool Save(CIOStream &stream);
    virtual bool Load(CIOStream &stream, const CLoadParameter &param);
    virtual void PostLoad();

    virtual void SetMatrix(const CMatrix &matrix);
    virtual void GetMatrix(CMatrix &matrix);
    virtual void GetBoundingBox(CAABox &box);

	virtual void Show(bool show);
    virtual void SetLayer(int layer);
    virtual void SetVisibleGroup(int g);
    virtual void SetLodGroup(int g);

    void SetSelfMatrixOnly(const CMatrix &matrix);    // set self matrix without transform entities in group

    void AddEntity(CSceneEntity *e);
    void RemoveEntity(CSceneEntity *e);
    void RemoveAll();
    void ResetEntityGroups();

    void SetOpen(bool open) { m_bOpen = open; }
    bool GetOpen() const { return m_bOpen; }

    int GetNumEntities();
    CSceneEntity* GetEntity(int index);


private:
    std::vector<int> m_EntityIds;
    CMatrix m_Matrix;
    bool m_bOpen;
};


}//GELib

#endif//__GELIB_SCENE_GROUPSCENEENTITY_H__
