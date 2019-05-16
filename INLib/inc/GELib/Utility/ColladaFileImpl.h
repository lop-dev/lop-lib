//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/ColladaFileImpl.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_COLLADAFILEIMPL_H__
#define __GELIB_UTILITY_COLLADAFILEIMPL_H__

#include "ColladaFile.h"
#include <GELib/Math/GeMath.h>
#include "pugixml.hpp"

namespace GELib
{


class CColladaFile;

class CColladaFileImpl
{
public:

	CColladaFileImpl(CColladaFile *c);
	~CColladaFileImpl();

    bool Load(const char *fileName, bool ignoreMesh, bool ignoreAnimation);
    bool Load(const void *fileInMemory, int fileSize, bool ignoreMesh, bool ignoreAnimation);
    bool Save(const char *fileName);
    void Unload();

private:

	bool ParseDoc(bool ignoreMesh, bool ignoreAnimation);

    bool ReadAttribute(pugi::xml_node e, const char *attrName, std::string &value);
    bool ReadElement(pugi::xml_node e, float &value);
    bool ReadElement(pugi::xml_node e, bool &value);
    bool ReadElement(pugi::xml_node e, std::string &value);
    bool ReadElement(pugi::xml_node e, CVector3 &value);
    bool ReadElement(pugi::xml_node e, CVector4 &value);
    bool ReadElement(pugi::xml_node e, CMatrix &value);
    pugi::xml_node  FindChildElement(pugi::xml_node e, const char *childName, const char *attrName, const char *attrValue);
    const char * GetUrl(pugi::xml_node e, const char *childName);
    int GetSourceStride(pugi::xml_node source, int floatArrayCount);

    int  ParseFloatArray(const char *text);
    int  ParseStringArray(const char *text);

    void ReadSceneInfo();
    void ReadCameras();
    void ReadLights();
    void ReadMaterials();
	void ReadMaterialMap(pugi::xml_node fx, pugi::xml_node channel, CColladaFile::MaterialMap &m);
    void ReadAnimations();
    void ReadAnimation(pugi::xml_node animation);
    void ReadAnimationChannel(pugi::xml_node animation, pugi::xml_node channel);
    void ReadSkins();
    void ReadMorphs();
    void ReadMeshes();
    void ReadScene();
    void ReadNode(pugi::xml_node node, CColladaFile::Node *parent);

	void CompositNodeAnimations(CColladaFile::Node *n);
    void BindMaterials(pugi::xml_node instance, CColladaFile::Mesh *mesh);
    bool BindSkinJoint();

	pugi::xml_node AddElement(pugi::xml_node parent, const char *name, const char *value);
    pugi::xml_node AddElement(pugi::xml_node parent, const char *name, float value);
    pugi::xml_node AddElement(pugi::xml_node parent, const char *name, const CVector3 &value);
    pugi::xml_node AddElement(pugi::xml_node parent, const char *name, const CMatrix &value);

    void WriteSceneInfo();
    void WriteCameras();
    void WriteLights();
    void WriteMaterials();
    void WriteAnimations();
    void WriteSkins();
    void WriteMeshes();
    void WriteNodes();
    void WriteNode(CColladaFile::Node *n, pugi::xml_node parent);

    std::vector<float> m_FloatArray;
    std::vector<std::string> m_StringArray;

	CColladaFile *m_pCollada;
    pugi::xml_document *m_pDoc;
};


}//GELib


#endif//__GELIB_UTILITY_COLLADAFILEIMPL_H__
