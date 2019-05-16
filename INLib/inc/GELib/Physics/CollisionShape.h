//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Physics/CollisionShape.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_PHYSICS_COLLISIONSHAPE_H__
#define __GELIB_PHYSICS_COLLISIONSHAPE_H__

#include <string>
#include <vector>

#include <GELib/Math/GeMath.h>
#include <GELib/Ext/BaseAsset.h>

namespace GELib
{

class CIOStream;
class IPlotter3D;
class CMeshUtility;
class CCollisionShapeManager;
class CPhysicsSdk;
class CCollisionShapeImpl;

enum ECollisionShapeType
{
    COLLISION_SHAPE_DUMMY = 0,
    COLLISION_SHAPE_SPHERE,
    COLLISION_SHAPE_BOX,
    COLLISION_SHAPE_CAPSULE,
    COLLISION_SHAPE_CONVEX_MESH,
    COLLISION_SHAPE_TRIANGLE_MESH,
    COLLISION_SHAPE_HEIGHT_FIELD,
};

class CCollisionShapeElement
{
public:
    CCollisionShapeElement() { m_eType = COLLISION_SHAPE_DUMMY; m_Color = CRGBAColor(255, 255, 255); m_iAttribute = 0; }
    std::string m_Name;
    ECollisionShapeType m_eType;
    CRGBAColor m_Color;
    int m_iAttribute;
};

class CDummyElement : public CCollisionShapeElement
{
public:
	CDummyElement() { m_eType = COLLISION_SHAPE_DUMMY; }
    CMatrix m_Matrix;
};


class CSphereElement : public CCollisionShapeElement
{
public:
	CSphereElement() { m_eType = COLLISION_SHAPE_SPHERE; }
    CSphere m_Sphere;
};

class CBoxElement : public CCollisionShapeElement
{
public:
	CBoxElement() { m_eType = COLLISION_SHAPE_BOX; }
    COBox m_Box;      // m_Box.m_Matrix contains no scale
};

class CCapsuleElement : public CCollisionShapeElement
{
public:
	CCapsuleElement() { m_eType = COLLISION_SHAPE_CAPSULE; }
	CCapsule m_Capsule;
};

class CTriangleMeshElement : public CCollisionShapeElement
{
public:
	CTriangleMeshElement() { m_eType = COLLISION_SHAPE_TRIANGLE_MESH; }
    std::vector<CVector3> m_Vertices;
    std::vector<unsigned short> m_Indices;
    class Chunk
    {
    public:
        unsigned short m_iFromTriangle;
        unsigned short m_iToTriangle;
        int m_iAttribute;
    };
    std::vector<Chunk> m_Chunks;
    int GetAttribute(int triangleIndex);
	bool GetTriangle(int triangleIndex, CTriangle &tri);
};

class CHeightFieldElement : public CCollisionShapeElement
{
public:
	CHeightFieldElement() { m_eType = COLLISION_SHAPE_HEIGHT_FIELD; m_iRows = m_iColumns = 0;
							m_fRowScale = m_fColumnsScale = m_fHeightScale = 1.0f; m_iHoleMaterial = 0; m_iNumMaterials = 1; }
	int m_iRows;
	int m_iColumns;
	float m_fRowScale;
	float m_fColumnsScale;
	float m_fHeightScale;
	int m_iHoleMaterial;
	int m_iNumMaterials;
	std::vector<int> m_Samples;
};

class CShapeUserData
{
public:
	CShapeUserData()
	{
		m_pInternalData = 0;
	}

    ECollisionShapeType m_eType;
    short m_iTypeElementIndex;
    short m_iShapeIndex;
	void *m_pInternalData;
};

const int ASSETCLASS_COLLISION_SHAPE = 5;

class CCollisionShape : public CBaseAsset
{
public:
    CCollisionShape(CCollisionShapeManager *manager, const char *cacheKey=0);
    virtual ~CCollisionShape();

	virtual int GetClassId() { return ASSETCLASS_COLLISION_SHAPE; }
    CCollisionShapeManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
    void Unload();
	void ConvertToMeshUtility(CMeshUtility &meshUtil);
    void Draw(IPlotter3D &plotter, const CMatrix &matrix, int flag=0);

    CDummyElement& AddDummyElement();
    CSphereElement& AddSphereElement();
    CBoxElement& AddBoxElement();
    CCapsuleElement& AddCapsuleElement();
    CTriangleMeshElement& AddConvexMeshElement();
    CTriangleMeshElement& AddTriangleMeshElement();
    CHeightFieldElement& AddHeightFieldElement();

    int GetNumDummyElements() { return (int)m_DummyElements.size(); }
    CDummyElement& GetDummyElement(int index) { return m_DummyElements[index]; }

    int GetNumSphereElements() { return (int)m_SphereElements.size(); }
    CSphereElement& GetSphereElement(int index) { return m_SphereElements[index]; }

    int GetNumBoxElements() { return (int)m_BoxElements.size(); }
    CBoxElement& GetBoxElement(int index) { return m_BoxElements[index]; }

    int GetNumCapsuleElements() { return (int)m_CapsuleElements.size(); }
    CCapsuleElement& GetCapsulelement(int index) { return m_CapsuleElements[index]; }

    int GetNumConvexMeshElements() { return (int)m_ConvexMeshElements.size(); }
    CTriangleMeshElement& GetConvexMeshElement(int index) { return m_ConvexMeshElements[index]; }

    int GetNumTriangleMeshElements() { return (int)m_TriangleMeshElements.size(); }
    CTriangleMeshElement& GetTriangleMeshElement(int index) { return m_TriangleMeshElements[index]; }

    int GetNumHeightFieldElements() { return (int)m_HeightFieldElements.size(); }
    CHeightFieldElement& GetHeightFieldElement(int index) { return m_HeightFieldElements[index]; }

	int GetNumElements();
    CCollisionShapeElement* GetElement(int shapeIndex);
    CCollisionShapeElement* GetElement(CShapeUserData *shapeUserData);

    void ComputeBoundingBox();
    void GetBoundingBox(CAABox &box) { box = m_Box; }
    const CAABox& GetBoundingBox() { return m_Box; }

    int CreateShapeDesc(CPhysicsSdk *sdk, const CVector3 &scale);	// return id
    void DeleteShapeDesc(int id);

	CCollisionShapeImpl *m_pImpl;

private:

    std::vector<CDummyElement> m_DummyElements;
    std::vector<CSphereElement> m_SphereElements;
    std::vector<CBoxElement> m_BoxElements;
    std::vector<CCapsuleElement> m_CapsuleElements;
    std::vector<CTriangleMeshElement> m_ConvexMeshElements;
    std::vector<CTriangleMeshElement> m_TriangleMeshElements;
    std::vector<CHeightFieldElement> m_HeightFieldElements;

    CAABox m_Box;

    void NewImpl();
	void DeleteImpl();

    CCollisionShapeManager *m_pManager;
};



}//GELib

#endif//__GELIB_PHYSICS_COLLISIONSHAPE_H__
