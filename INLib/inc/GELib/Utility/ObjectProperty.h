//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/ObjectProperty.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_OBJECTPROPERTY_H__
#define __GELIB_UTILITY_OBJECTPROPERTY_H__

#include <GELib/Math/GeMath.h>
#include <string>
#include <vector>

namespace GELib
{


class CIOStream;

class CObjectProperty
{
public:
    CObjectProperty() { m_eType = PT_NUMBER; m_iFlag = PF_NONE; }

    void SetInt(int value, short flag=0);
    void SetFloat(float value, short flag=0);
    void SetBool(bool value, short flag=0);
    void SetString(const char *value, short flag=0);
    void SetVector2(const CVector2 &value, short flag=0);
    void SetVector3(const CVector3 &value, short flag=0);
    void SetColor(CRGBAColor value, short flag=0);

    int GetInt();
    float GetFloat();
    bool GetBool();
    const char * GetString();
    CVector2 GetVector2();
    CVector3 GetVector3();
	CRGBAColor GetColor();

    enum EType
    {
        PT_STRING,
        PT_NUMBER,
        PT_BOOL,
        PT_VECTOR2,
        PT_VECTOR3,
        PT_COLOR
    };

    enum EFlag
    {
        PF_NONE = 0,
        PF_READ_ONLY = 1 << 0
    };

    std::string m_Name;
    std::string m_Value;

    // not saved
    short m_iFlag;
    EType m_eType;
};

class CObjectPropertyTable;

class CObjectPropertySet
{
public:

    CObjectPropertySet() { m_pParent = 0;}

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, CObjectPropertyTable *table);

    CObjectProperty* GetProperty(const char *name, bool searchParent=true);
    CObjectProperty* AppendProperty(const char *name);
    void RemoveProperty(const char *name);
    void Clear();

    void SetValue(const char *name, int value, short flag=0)
    {
        AppendProperty(name)->SetInt(value, flag);
    }

    void SetValue(const char *name, float value, short flag=0)
    {
        AppendProperty(name)->SetFloat(value, flag);
    }

    void SetValue(const char *name, bool value, short flag=0)
    {
        AppendProperty(name)->SetBool(value, flag);
    }

    void SetValue(const char *name, const char *value, short flag=0)
    {
        AppendProperty(name)->SetString(value, flag);
    }

    void SetValue(const char *name, const std::string &value, short flag=0)
    {
        AppendProperty(name)->SetString(value.c_str(), flag);
    }

    void SetValue(const char *name, const CVector2 &value, short flag=0)
    {
        AppendProperty(name)->SetVector2(value, flag);
    }

    void SetValue(const char *name, const CVector3 &value, short flag=0)
    {
        AppendProperty(name)->SetVector3(value, flag);
    }

    void SetValue(const char *name, CRGBAColor value, short flag=0)
    {
        AppendProperty(name)->SetColor(value, flag);
    }

    int GetInt(const char *name)
    {
        CObjectProperty *p = GetProperty(name);
        return p ? p->GetInt() : 0;
    }

    float GetFloat(const char *name)
    {
        CObjectProperty *p = GetProperty(name);
        return p ? p->GetFloat() : 0;
    }

    bool GetBool(const char *name)
    {
        CObjectProperty *p = GetProperty(name);
        return p ? p->GetBool() : false;
    }

    const char * GetString(const char *name)
    {
        CObjectProperty *p = GetProperty(name);
        return p ? p->GetString() : "";
    }

    CVector2 GetVector2(const char *name)
    {
        CObjectProperty *p = GetProperty(name);
        return p ? p->GetVector2() : CVector2(0, 0);
    }

    CVector3 GetVector3(const char *name)
    {
        CObjectProperty *p = GetProperty(name);
        return p ? p->GetVector3() : CVector3(0, 0, 0);
    }

    CRGBAColor GetColor(const char *name)
    {
        CObjectProperty *p = GetProperty(name);
        return p ? p->GetColor() : CRGBAColor(0, 0, 0);
    }

    void GetValue(const char *name, int &value)
    {
        value = GetInt(name);
    }

    void GetValue(const char *name, float &value)
    {
        value = GetFloat(name);
    }

    void GetValue(const char *name, bool &value)
    {
        value = GetBool(name);
    }

    void GetValue(const char *name, std::string &value)
    {
        value = GetString(name);
    }

    void GetValue(const char *name, CVector2 &value)
    {
        value = GetVector2(name);
    }

    void GetValue(const char *name, CVector3 &value)
    {
        value = GetVector3(name);
    }

    void GetValue(const char *name, CRGBAColor &value)
    {
        value = GetColor(name);
    }

    void GetProperties(std::vector<CObjectProperty*> &ep, short flag, bool hasFlag);

    std::string m_Name;
    std::string m_ParentName;
    CObjectPropertySet *m_pParent;

    typedef std::vector<CObjectProperty> PropertyList;
    PropertyList m_Properties;
};

class CObjectPropertyTable
{
public:

    CObjectPropertyTable();
    virtual ~CObjectPropertyTable();

    CObjectPropertySet* GetPropertySet(const char *name);
    CObjectPropertySet* AppendPropertySet(const char *name);

    bool SaveScript(const char *fname);
    bool LoadScript(const char *fname);
    void Unload();

    typedef std::vector<CObjectPropertySet> PropertySetList;
    PropertySetList m_PropertySets;
};


}//GELib

#endif//__GELIB_UTILITY_OBJECTPROPERTY_H__
