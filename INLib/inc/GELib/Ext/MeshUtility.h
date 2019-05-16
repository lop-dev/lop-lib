//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/MeshUtility.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_MESHUTILITY_H__
#define __GELIB_EXT_MESHUTILITY_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>
#include <GELib/Utility/ColladaFile.h>
#include <string>

struct ID3DXMesh;

namespace GELib
{

std::string NameRule(const char *nodeName);
int NameRuleNumber(const char *nodeName);
std::string TrimNameRule(const char *nodeName);
std::string TrimNamePivot(const char *nodeName);

class CMeshUtility
{
public:

    CMeshUtility();
    virtual ~CMeshUtility();

    bool Load(const char *fileName);
    bool Load(CColladaFile *collada, CColladaFile::Mesh *mesh);
    void Unload();

    void SplitSkin(int maxBones);

    void ComputeTangent();
    void ComputeTangent2(bool computeNormal);
    void ComputeUVAtlus(int texCoordIndex);
    void Optimize();
	void SortMergeChunks();

    void ConvertToCollada(CColladaFile::Mesh *mesh, CColladaFile *collada);

    class Vertex
    {
    public:
        Vertex();
		void Reset();
        CVector3 position;
        CVector3 tangent;
        CVector3 bitangent;
        CVector3 normal;
        CVector2 texCoords[4];
        CVector4 color;
        short blendIndices[4];
        float blendWeights[4];
    };

    class Material
    {
    public:
        Material();

        std::string m_Name;

        std::string m_DiffuseTextureName;
        std::string m_SpecularTextureName;
        std::string m_EmissiveTextureName;
        std::string m_NormalTextureName;

        GELib::CVector3 m_Ambient;
        GELib::CVector3 m_Diffuse;
        GELib::CVector3 m_Specular;
        GELib::CVector3 m_Emissive;

        float m_fSpecularPower;
        float m_fOpacity;
        float m_fAlphaRef;
        bool m_bAlphaTest;
        bool m_bTwoSided;
        int m_iBlendMode;
    };

    class Chunk
    {
    public:
        int m_iMaterial;
        int m_iFirstIndex;
        int m_iNumTriangles;
        int m_iChunkId; // internal use
        std::vector<int> m_BoneIndices;
    };

    class Bone
    {
    public:
        std::string m_Name;
        CMatrix m_BindInvMatrix;
        CAABox m_Box;
    };

    void AllocateBuffers(int numVertices, int numIndices);

    int m_iNumVertices;
    int m_iNumIndices;
    int m_iNumTexCoords;
    int m_iUVAtlasTexCoordIndex;
    int m_iNumBlendWeights;
    bool m_bHasVertexColors;
    GELib::CAABox m_Box;

    std::vector<Vertex> m_Vertices;
    std::vector<int> m_Indices;
    std::vector<Chunk> m_Chunks;
    std::vector<Material> m_Materials;
    std::vector<Bone> m_Bones;

private:

	void RemoveMaterialNameRule();
    void ProcessSkin(CColladaFile::Skin *skin);
    bool LoadCollada(const char *fileName);

    ID3DXMesh* CreateD3DXMesh();
    void CreateFromD3DXMesh(ID3DXMesh *mesh);
    std::vector<Chunk> m_SaveChunks;
};


}//GELib

#endif//__GELIB_EXT_MESHUTILITY_H__
