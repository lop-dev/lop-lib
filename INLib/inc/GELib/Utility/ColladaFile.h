//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/ColladaFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_COLLADAFILE_H__
#define __GELIB_UTILITY_COLLADAFILE_H__

#include <vector>
#include <string>
#include <GELib/GeConfig.h>
#include <GELib/Math/GeMath.h>

namespace GELib
{


class CColladaFileImpl;

class CColladaFile
{
public:

    GELIB_API CColladaFile();
    GELIB_API virtual ~CColladaFile();

    GELIB_API bool Load(const char *fileName, bool ignoreMesh=false, bool ignoreAnimation=false);
    GELIB_API bool Load(const void *fileInMemory, int fileSize, bool ignoreMesh=false, bool ignoreAnimation=false);
    GELIB_API bool Save(const char *fileName);
    GELIB_API void Unload();

    enum EUpAxis
    {
        X_UP,
        Y_UP,
        Z_UP
    };

    class SceneInfo
    {
    public:
        SceneInfo();
        EUpAxis upAxis;
        CVector3 ambient;
    };

    class MaterialMap
    {
    public:
        MaterialMap();
        std::string fileName;
    };

    class Material
    {
    public:
        Material();
        std::string id;
        std::string name;
        CVector3 ambient;
        CVector3 diffuse;
        CVector3 specular;
        CVector3 emission;
        float shininess;
        float transparency;
        bool twoSided;
        MaterialMap diffuseMap;
        MaterialMap specularMap;
        MaterialMap shinenessMap;
        MaterialMap emissionMap;
        MaterialMap bumpMap;
    };

    class Vertex
    {
    public:
        Vertex();
        CVector3 position;
        CVector3 tangent;
        CVector3 bitangent;
        CVector3 normal;
        CVector2 texCoords[4];
        CVector4 color;
        short blendIndices[4];
        float blendWeights[4];
    };

    class Chunk
    {
    public:
        Chunk();
        Material *material;
        std::string materialName;
        int firstIndex;
        int numFaces;
        int minVertexIndex;
        int maxVertexIndex;
    };

    class Skin;

    class Mesh
    {
    public:
        Mesh();
        std::string id;
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<int> indices;
        std::vector<Chunk> chunks;
        int numTexCoords;
        int numBlendWeights;
        bool hasNormals;
        bool hasTangents;
        bool hasVertexColors;
        CAABox box;
        Skin *skin;
    };

    enum
    {
        ANI_X =  1<<0,
        ANI_Y =  1<<1,
        ANI_Z =  1<<2,
        ANI_RX = 1<<3,
        ANI_RY = 1<<4,
        ANI_RZ = 1<<5,
        ANI_SX = 1<<6,
        ANI_SY = 1<<7,
        ANI_SZ = 1<<8,
		ANI_TRANSFORM = 1<<9,
        ANI_XYZ =  (1<<0) | (1<<1) | (1<<2),
        ANI_RXYZ = (1<<3) | (1<<4) | (1<<5),
        ANI_SXYZ = (1<<6) | (1<<7) | (1<<8),
    };

    class AnimationKey
    {
    public:
        AnimationKey();
        float time;
        int channels;
        CMatrix matrix;
        CVector3 value;
    };

    class Node;

    class Animation
    {
    public:
        Animation();
        std::string id;
        std::vector<AnimationKey> keys;
        std::string targetId;
        int channels;
        Node *target;
    };

    class Joint
    {
    public:
        Joint();
        std::string nodeId;
        Node *node;
        CMatrix bindInvMatrix;
    };

    class BlendWeight
    {
    public:
        BlendWeight();
        int numWeights;
        float weights[4];
        short indices[4];
    };

    class Skin
    {
    public:
        Skin();
        std::string id;
        std::string meshId;
        std::vector<Joint> joints;
		std::vector<std::string> skeletonNames;
        std::vector<BlendWeight> vertexBlendWeights;
        Mesh *mesh;
        CMatrix bindMatrix;
    };

	class Morph
	{
	public:
        std::string id;
		std::string meshId;
	};

    class Camera
    {
    public:
        Camera();
        std::string id;
        float znear;
        float zfar;
        float fov;
    };

    enum ELightType
    {
        POINT_LIGHT,
        SPOT_LIGHT,
        DIRECTIONAL_LIGHT
    };

    class Light
    {
    public:
        Light();

        std::string id;
        ELightType lightType;
        CVector3 color;
        float intensity;
        float innerCone;
        float outerCone;
        float attenuationStart;
        float attenuationEnd;
        float aspectRatio;
        bool  overshoot;
    };

    enum ENodeType
    {
        NODE,
        GEOM,
        SKIN,
        LIGHT,
        CAMERA,
        JOINT,
    };

    class Node
    {
    public:
        Node();
        virtual ~Node();
        void RemoveChildren(Node *c);

        ENodeType type;
        std::string id;
        std::string sid;
        std::string name;
        std::string userProperty;

        Mesh *mesh;
        Skin *skin;
        Light *light;
        Camera *camera;
        Animation *animation;
        std::vector<Animation*> animationChannels;

        CMatrix matrix;
        CMatrix localMatrix;
        Node *parent;
        std::vector<Node*> children;
    };

    SceneInfo m_SceneInfo;
    std::vector<Node*> m_Nodes;
    std::vector<Material*> m_Materials;
    std::vector<Camera*> m_Cameras;
    std::vector<Light*> m_Lights;
    std::vector<Mesh*> m_Meshes;
    std::vector<Skin*> m_Skins;
    std::vector<Morph*> m_Morphs;
    std::vector<Animation*> m_Animations;
    std::vector<Animation*> m_CompositeAnimations;

    GELIB_API Node* GetNode(const char *id, Node *root=0);
    GELIB_API Node* GetNodeByName(const char *name, Node *root=0);
    GELIB_API Node* GetNodeBySid(const char *sid, Node *root=0);
    GELIB_API Camera* GetCamera(const char *id);
    GELIB_API Light* GetLight(const char *id);
    GELIB_API Mesh* GetMesh(const char *id);
    GELIB_API Skin* GetSkin(const char *id);
    GELIB_API Skin* GetSkinByMeshId(const char *meshId);
    GELIB_API Morph* GetMorph(const char *id);
    GELIB_API Animation* GetAnimation(const char *id);
    GELIB_API void GetAnimationsByTargetId(const char *targetId, std::vector<Animation*> &animations);
    GELIB_API Material* GetMaterial(const char *id);
    GELIB_API void RemoveLeafPivotNodes();
    GELIB_API void MakeNodeNameUnique();

private:

	CColladaFileImpl *m_pImpl;
};



}//GELib

#endif//__GELIB_UTILITY_COLLADAFILE_H__
