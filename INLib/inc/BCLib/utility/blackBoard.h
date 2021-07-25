//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/blackBoard.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_BLACKBOARD_H__
#define __BCLIB_UTILITY_BLACKBOARD_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CBlackBoard
{
public:
    CBlackBoard();
    virtual ~CBlackBoard();

    virtual void clean();

    template<class T>
    void setValue(int nKey, T tValue)
    {
        IntBlackBoardValueMapIt itBlack = m_intBlackBoardValueMap.find(nKey);
        SBlackBoardTemplateValue<T>* templateValue = NULL;
        if(itBlack == m_intBlackBoardValueMap.end())
        {
            templateValue = new SBlackBoardTemplateValue<T>(tValue);
            m_intBlackBoardValueMap[nKey] = templateValue;
            return;
        }
        templateValue = (SBlackBoardTemplateValue<T> *)itBlack->second;
        if(templateValue != NULL && templateValue->m_ValueType == EBLACKBOARD_VALUE_TYPE_TEMPLATE)
        {
            templateValue->m_value = tValue;
        }
    }

    template<class T>
    T& getValue(int nKey, T& defaultVal)
    {
        IntBlackBoardValueMapIt itBlack = m_intBlackBoardValueMap.find(nKey);
        if(itBlack == m_intBlackBoardValueMap.end())
        {
            return defaultVal;
        }
        SBlackBoardTemplateValue<T>* templateValue = (SBlackBoardTemplateValue<T>*)itBlack->second;
        if(templateValue != NULL && templateValue->m_ValueType == EBLACKBOARD_VALUE_TYPE_TEMPLATE)
        {
            return templateValue->m_value;
        }
        return defaultVal;
    }

    template<class T>
    void copyValue(int keySrc, int keyDes)
    {
        IntBlackBoardValueMapIt itSrc = m_intBlackBoardValueMap.find(keySrc);
        IntBlackBoardValueMapIt itDes = m_intBlackBoardValueMap.find(keyDes);
        if(itSrc == m_intBlackBoardValueMap.end() || itDes == m_intBlackBoardValueMap.end())
        {
            return;
        }

        SBlackBoardTemplateValue<T>* templateValueSrc = (SBlackBoardTemplateValue<T>*)itSrc->second;
        SBlackBoardTemplateValue<T>* templateValueDes = (SBlackBoardTemplateValue<T>*)itDes->second;
        templateValueDes->m_value = templateValueSrc->m_value;
    }

    void delValue(int nKey, bool autoDelete = false);

private:
    enum EBLACKBOARD_VALUE_TYPE
    {
        EBLACKBOARD_VALUE_TYPE_BASIC,
        EBLACKBOARD_VALUE_TYPE_TEMPLATE
    };

    struct SBlackBoardValue
    {
        SBlackBoardValue(EBLACKBOARD_VALUE_TYPE valueType = EBLACKBOARD_VALUE_TYPE_BASIC)
        :m_ValueType(valueType)
        {
        }

        EBLACKBOARD_VALUE_TYPE m_ValueType;
    };

    template<class T>
    struct SBlackBoardTemplateValue : public SBlackBoardValue
    {
        SBlackBoardTemplateValue(T value)
        :SBlackBoardValue(EBLACKBOARD_VALUE_TYPE_TEMPLATE)
        ,m_value(value)
        {
        }

        T m_value;
    };

private:
    typedef std::map<int, SBlackBoardValue*> IntBlackBoardValueMap;
    typedef IntBlackBoardValueMap::iterator IntBlackBoardValueMapIt;

    typedef std::map<std::string, SBlackBoardValue*> StrBlackBoardValueMap;
    typedef StrBlackBoardValueMap::iterator StrBlackBoardValueMapIt;

    IntBlackBoardValueMap m_intBlackBoardValueMap;
};
}//Utility
}//BCLib
#endif//__BCLIB_UTILITY_BLACKBOARD_H__
