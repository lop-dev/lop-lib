//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/WaterSceneEntity.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_WATERSCENEENTITY_H__
#define __GELIB_SCENE_WATERSCENEENTITY_H__

#include <GELib/Scene/SceneEntity.h>
#include <GELib/Math/GeMath.h>
#include <GELib/Renderer/Renderer.h>
#include <GELib/Ext/WaterTypeDef.h>

namespace GELib
{
    class CScene;
	class CWaterMeshRenderObject;
	struct SWaterDesc;

    class GELIB_API CWaterEntity : public CSceneEntity
    {
    public:
        CWaterEntity(CScene *scene);
        virtual ~CWaterEntity();
		virtual int GetClassId() { return SCENEENTITY_WATER; }
		virtual bool Save(CIOStream &stream);
		virtual bool Load(CIOStream &stream, const CLoadParameter &param);
		virtual bool IsLoading();
		virtual void PostLoad();
		virtual void Update(float t);

		virtual void EnableRender(bool bEnable);
		virtual void Show(bool show);
		virtual void SetLayer(int layer);

		virtual bool Create(const SWaterDesc& _desc);

        virtual bool IntersectRay(const CVector3 &orig, const CVector3 &dir, float *t=0, CVector3 *normal=0);
        virtual bool IntersectLine(const CVector3 &p1, const CVector3 &p2, float *t=0, CVector3 *normal=0);

		virtual void GetBoundingBox(CAABox &box);
		virtual void SetMatrix(const CMatrix &matrix);
		virtual void GetMatrix(CMatrix &matrix);

        virtual void GetAssets(CAssetHelper &helper);

        CTextureAsset* LoadTextureFromFile(const char *fileName, bool replaceExist);
        CMeshAsset* LoadMeshFromFile(const char *fileName, bool replaceExist);

        CWaterMeshRenderObject* GetRenderObject() { return m_pRenderObject; }

    private:
        CWaterMeshRenderObject* m_pRenderObject;
		SWaterDesc m_tempWaterDesc;
		CMatrix m_tempMatrix;
		int m_iClassID;
    };
}//GELib

#endif//__GELIB_SCENE_WATERSCENEENTITY_H__
