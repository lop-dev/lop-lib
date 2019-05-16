//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Scene/SceneLayerOption.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_SCENE_SCENELAYEROPTION_H__
#define __GELIB_SCENE_SCENELAYEROPTION_H__

#include <string>
#include <vector>

namespace GELib
{

class CSceneLayerOption
{
public:
    CSceneLayerOption();
    ~CSceneLayerOption();

	enum ELayerOption
	{
		LAYER_VISIBLE = 1 << 0,
		LAYER_SELECTABLE = 1 << 1,
		LAYER_MOVABLE = 1 << 2
	};

    class CLayer
    {
    public:
        std::string m_Name;
		int m_iFlags;
    };

	void SetLayerOption(int index, int option, bool on);
	bool GetLayerOption(int index, int option);
	bool IsLayerVisible(int index);
	bool IsLayerSelectable(int index);
	bool IsLayerMovable(int index);

    void SetNumLayers(int num);
    int GetNumLayers();
    CLayer* GetLayer(int index);

private:
    std::vector<CLayer> m_Layers;
};

class CSceneVisibleGroup
{
public:
	CSceneVisibleGroup();
	~CSceneVisibleGroup();

	bool IsEntityVisible(int g);

	const static int NUM_GROUPS = 32;
	int GetNumGroups() { return NUM_GROUPS; }

	int m_iVisibleGroup;
	std::string m_Names[NUM_GROUPS];
};


}//GELib
#endif//__GELIB_SCENE_SCENELAYEROPTION_H__
