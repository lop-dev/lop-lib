//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ui/UiClass.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UI_UICLASS_H__
#define __GELIB_UI_UICLASS_H__

#include <vector>

namespace GELib
{


class CUiObject;
class CUiSystem;
typedef CUiObject* (*CreateInstanceFunc) (CUiSystem *system);

class CUiClass
{
public:
    CUiClass(const char *className, const char *superClassName, CreateInstanceFunc createInstanceFunc);
    bool IsA(const CUiClass &c);

    const char *m_ClassName;
    const char *m_SuperClassName;
    CreateInstanceFunc m_CreateInstanceFunc;
    CUiClass *m_pSuperClass;
    std::vector<CUiClass*> m_DerivedClasses;
	CUiClass *m_pNext;
	int m_iId;
    int m_iIndex;
    int m_iLastDerivedClassIndex;
};

void InitializeUiClass();
CUiClass* GetUiClass(const char *className);
CUiClass* GetUiClass(int classId);

/*
#define UICLASS_DECLARE(cls)                \
	static GELib::CUiClass s_Class;				\
	static GELib::CUiObject* CreateInstance(GELib::CUiSystem *system);     \
    virtual GELib::CUiClass* GetClass() const;

#define UICLASS_IMPLEMENT(cls, superCls)         \
    GELib::CUiClass cls::s_Class(#cls, #superCls, cls::CreateInstance);       \
    GELib::CUiObject* cls::CreateInstance(GELib::CUiSystem *system) { return new cls(system); }       \
    GELib::CUiClass* cls::GetClass() const { return &cls::s_Class; }
*/

#define UICLASS_DECLARE(cls)                \
	static GELib::CUiClass s_Class;				\
	static GELib::CUiObject* CreateInstance(GELib::CUiSystem *system) { return new cls(system); }     \
    virtual GELib::CUiClass* GetClass() const { return &cls::s_Class; };

#define UICLASS_IMPLEMENT(cls, superCls)         \
    GELib::CUiClass cls::s_Class(#cls, #superCls, cls::CreateInstance);



}//GELib

#endif//__GELIB_UI_UICLASS_H__
