//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/functionList.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_FUNCTIONLIST_H__
#define __BCLIB_UTILITY_FUNCTIONLIST_H__

#include <BCLib/utility/function.h>

#define BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_0
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_1    typename T1
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_2    BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_1, typename T2
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_3    BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_2, typename T3
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_4    BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_3, typename T4
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_5    BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_4, typename T5
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_6    BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_5, typename T6
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_7    BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_6, typename T7
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_8    BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_7, typename T8
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_9    BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_8, typename T9

#define BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_0    void
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_1    BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_0, T1
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_2    BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_1, T2
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_3    BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_2, T3
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_4    BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_3, T4
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_5    BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_4, T5
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_6    BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_5, T6
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_7    BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_6, T7
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_8    BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_7, T8
#define BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_9    BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_8, T9

#define BCLIB_UTILITY_FUNCTIONLIST(N)\
    template< BCLIB_UTILITY_FUNCTION_TEMPLATE_##N >\
    class CFunctionList\
    {\
    public:\
        void addFunction(CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>* fun)\
        {\
            m_funList.push_back(fun);\
        }\
        void delFunction(CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>* fun)\
        {\
            std::vector<CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>*>::iterator it = m_funList.begin();\
            for(it = m_funList.begin(); it != m_funList.end(); ++it){\
                CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>* tmp = *it;\
                if (fun == tmp){\
                    m_funList.erase(it);\
                    break;\
                }\
            }\
        }\
        R operator()(BCLIB_UTILITY_FUNCTION_TYPE_PARA_##N)\
        {\
            R res;\
            for(unsigned int i=0; i<m_funList.size(); ++i){\
                CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>* fun = m_funList[i];\
                if (fun == NULL){\
                    continue;\
                }\
                res = (*fun)(BCLIB_UTILITY_FUNCTION_PARA_##N);\
            }\
            return res;\
        }\
    private:\
        std::vector<CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>*> m_funList;\
    };\
    template<BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_##N>\
    class CFunctionList<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>\
    {\
    public:\
        void addFunction(CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>* fun)\
        {\
            m_funList.push_back(fun);\
        }\
        void delFunction(CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>* fun)\
        {\
            std::vector<CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>*>::iterator it = m_funList.begin();\
            for(it = m_funList.begin(); it != m_funList.end(); ++it){\
                CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>* tmp = *it;\
                if (fun == tmp){\
                    m_funList.erase(it);\
                    break;\
                }\
            }\
        }\
        void operator()(BCLIB_UTILITY_FUNCTION_TYPE_PARA_##N)\
        {\
            for(unsigned int i=0; i<m_funList.size(); ++i){\
                CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>* fun = m_funList[i];\
                if (fun == NULL){\
                    continue;\
                }\
                (*fun)(BCLIB_UTILITY_FUNCTION_PARA_##N);\
            }\
        }\
    private:\
        std::vector<CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>*> m_funList;\
    };


#define BCLIB_UTILITY_FUNCTIONLIST_SPECIAL(N)\
    template< BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_##N >\
    class CFunctionList<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>\
    {\
    public:\
        void addFunction(CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>* fun)\
        {\
            m_funList.push_back(fun);\
        }\
        void delFunction(CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>* fun)\
        {\
            std::vector<CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>*>::iterator it = m_funList.begin();\
            for(it = m_funList.begin(); it != m_funList.end(); ++it){\
                CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>* tmp = *it;\
                if (fun == tmp){\
                    m_funList.erase(it);\
                    break;\
                }\
            }\
        }\
        R operator()(BCLIB_UTILITY_FUNCTION_TYPE_PARA_##N)\
        {\
            R res;\
            for(unsigned int i=0; i<m_funList.size(); ++i){\
                CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>* fun = m_funList[i];\
                if (fun == NULL){\
                    continue;\
                }\
                res = (*fun)(BCLIB_UTILITY_FUNCTION_PARA_##N);\
            }\
            return res;\
        }\
    private:\
        std::vector<CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_##N>*> m_funList;\
    };\
    template<BCLIB_UTILITY_FUNCTION_TEMPLATE_TYPE_VOID_##N>\
    class CFunctionList<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>\
    {\
    public:\
        void addFunction(CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>* fun)\
        {\
            m_funList.push_back(fun);\
        }\
        void delFunction(CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>* fun)\
        {\
            std::vector<CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>*>::iterator it = m_funList.begin();\
            for(it = m_funList.begin(); it != m_funList.end(); ++it){\
                CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>* tmp = *it;\
                if (fun == tmp){\
                    m_funList.erase(it);\
                    break;\
                }\
            }\
        }\
        void operator()(BCLIB_UTILITY_FUNCTION_TYPE_PARA_##N)\
        {\
            for(unsigned int i=0; i<m_funList.size(); ++i){\
                CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>* fun = m_funList[i];\
                if (fun == NULL){\
                    continue;\
                }\
                (*fun)(BCLIB_UTILITY_FUNCTION_PARA_##N);\
            }\
        }\
    private:\
        std::vector<CFunction<BCLIB_UTILITY_FUNCTION_TEMPLATE_PARA_VOID_##N>*> m_funList;\
    };


namespace BCLib
{
namespace Utility
{

BCLIB_UTILITY_FUNCTIONLIST(9);

BCLIB_UTILITY_FUNCTIONLIST_SPECIAL(0);
BCLIB_UTILITY_FUNCTIONLIST_SPECIAL(1);
BCLIB_UTILITY_FUNCTIONLIST_SPECIAL(2);
BCLIB_UTILITY_FUNCTIONLIST_SPECIAL(3);
BCLIB_UTILITY_FUNCTIONLIST_SPECIAL(4);
BCLIB_UTILITY_FUNCTIONLIST_SPECIAL(5);
BCLIB_UTILITY_FUNCTIONLIST_SPECIAL(6);
BCLIB_UTILITY_FUNCTIONLIST_SPECIAL(7);
BCLIB_UTILITY_FUNCTIONLIST_SPECIAL(8);

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9> class CFunctionList;

}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_FUNCTIONLIST_H__
