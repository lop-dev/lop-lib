//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/classInfo.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_CLASSINFO_H__
#define __BCLIB_UTILITY_CLASSINFO_H__

#include <vector>

namespace BCLib
{
namespace Utility
{
class CBaseClass;
typedef CBaseClass* (*CreateInstanceFunc) ();

class CClassInfo
{
public:
    CClassInfo(const char* className, const char* superClassName, CreateInstanceFunc createInstanceFunc);
    bool IsA(const CClassInfo& c);

    const char* m_ClassName;
    const char* m_SuperClassName;
    CreateInstanceFunc m_CreateInstanceFunc;
    CClassInfo* m_pSuperClass;
    CClassInfo* m_pNext;
    std::vector<CClassInfo*> m_DerivedClasses;
    short m_iId;
    short m_iLastDerivedClassId;
};


#define BCLIB_DECL_CLASS(cls)                     \
    static BCLib::Utility::CClassInfo ClassInfo;          \
    static BCLib::Utility::CBaseClass* CreateInstance();    \
    virtual BCLib::Utility::CClassInfo* GetCClassInfo() const;

#define BCLIB_IMPL_CLASS(cls, superClass)         \
    BCLib::Utility::CClassInfo cls::ClassInfo(#cls, #superClass, cls::CreateInstance);       \
    BCLib::Utility::CBaseClass* cls::CreateInstance() { return new cls; }                      \
    BCLib::Utility::CClassInfo* cls::GetCClassInfo() const { return &cls::ClassInfo; }



void InitializeClassInfo();
CClassInfo* GetCClassInfo(const char* name);
CClassInfo* GetCClassInfo(short classId);



class CBaseClass
{
public:
    BCLIB_DECL_CLASS(CBaseClass);

    CBaseClass();
    virtual ~CBaseClass();

    const char* GetCClassName();
    short GetClassId();
    bool IsA(const CClassInfo& c);
};


template<class T>
T* DynamicCast(CBaseClass* c)
{
    return (c && c->IsA(T::ClassInfo)) ? (T *)c : 0;
}
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_CLASSINFO_H__
