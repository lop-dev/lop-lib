//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/keyExecMgr.h
//  author:     League of Perfect
/// @brief      key -> func
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_KEYEXECMGR_H__
#define __BCLIB_UTILITY_KEYEXECMGR_H__

#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/function.h>

namespace BCLib
{
namespace Utility
{
template<typename Key, typename ExecFuncParams>
class BCLIB_UTILITY_API CKeyExecMgr
{
public:
    typedef void(*FKeyExec)(ExecFuncParams&);

private:
    typedef CFunction<void, ExecFuncParams&> TKeyExecFunc;
    struct SKeyExecFunc
    {
        SKeyExecFunc()
        :m_objAddr(0)
        ,m_funcAddr(0)
        ,m_pKeyExecFunc(NULL)
        {
        }

        uint64 m_objAddr;
        uint64 m_funcAddr;
        TKeyExecFunc* m_pKeyExecFunc;
    };

    typedef std::vector<SKeyExecFunc*> TVectorSKeyExecFunc;
    typedef typename TVectorSKeyExecFunc::iterator TVectorSKeyExecFuncIter;

    typedef CHashMap<Key, TVectorSKeyExecFunc> THashMapKeyExec;
    typedef typename THashMapKeyExec::iterator THashMapKeyExecIter;

    THashMapKeyExec m_keyExecHashMap;

public:
    virtual ~CKeyExecMgr()
    {
        unregisterAllFunc();
    }

    bool registerFunc(const Key key, FKeyExec pFunKeyExec)
    {
        if(pFunKeyExec == NULL)
        {
            return false;
        }

        SKeyExecFunc* pKeyExecFunc = new SKeyExecFunc();
        if(pKeyExecFunc == NULL)
        {
            return false;
        }

        TKeyExecFunc* pFunction = new TKeyExecFunc(pFunKeyExec);
        if(pFunction == NULL)
        {
            delete pKeyExecFunc;
            return false;
        }
        pKeyExecFunc->m_pKeyExecFunc = pFunction;

        pKeyExecFunc->m_funcAddr = _funcAddr2int(pFunKeyExec);

        _addKeyExecFunc(key, pKeyExecFunc);

        return true;
    }

    template<typename NAME>
    bool registerFunc(const Key key, void(NAME::* pExecFunc)(ExecFuncParams&), NAME* obj)
    {
        if(pExecFunc == NULL)
        {
            return false;
        }

        SKeyExecFunc* pKeyExecFunc = new SKeyExecFunc();
        if(pKeyExecFunc == NULL)
        {
            return false;
        }

        typedef CFunctionObject<void, NAME, ExecFuncParams&> TKeyExecMemFunc;
        TKeyExecMemFunc* pFunction = new TKeyExecMemFunc(pExecFunc, obj);
        if(pFunction == NULL)
        {
            delete pKeyExecFunc;
            return false;
        }
        pKeyExecFunc->m_pKeyExecFunc = pFunction;

        pKeyExecFunc->m_objAddr = *((uint32 *)obj);
        pKeyExecFunc->m_funcAddr = _funcAddr2int(pExecFunc);
        _addKeyExecFunc(key, pKeyExecFunc);

        return true;
    }

    void unregisterFunc(Key key, FKeyExec pFunKeyExec)
    {
        uint64 objAddr = 0;
        uint64 funcAddr = _funcAddr2int(pFunKeyExec);
        _removeKeyExecFunc(key, objAddr, funcAddr);
    }

    template<typename NAME>
    void unregisterFunc(Key key, void(NAME::* pFunKeyExec)(ExecFuncParams&), NAME* obj)
    {
        if(obj == NULL)
        {
            return;
        }

        uint64 objAddr = *((uint32*)obj);
        uint64 funcAddr = _funcAddr2int(pFunKeyExec);
        _removeKeyExecFunc(key, objAddr, funcAddr);
    }

    void dispatchParams(Key key, ExecFuncParams& params)
    {
        THashMapKeyExecIter it = m_keyExecHashMap.find(key);
        if(it != m_keyExecHashMap.end())
        {
            TVectorSKeyExecFunc& execFuncs = it->second;
            TVectorSKeyExecFuncIter execIter = execFuncs.begin();
            TVectorSKeyExecFuncIter execEnd = execFuncs.end();

            while(execIter != execEnd)
            {
                SKeyExecFunc* pKeyExecFunc = *execIter;
                if(pKeyExecFunc && pKeyExecFunc->m_pKeyExecFunc)
                {
                    (*pKeyExecFunc->m_pKeyExecFunc)(params);
                }
                ++execIter;
            }
        }
    }

    void unregisterAllFunc()
    {
        THashMapKeyExecIter it = m_keyExecHashMap.begin();
        while(it != m_keyExecHashMap.end())
        {
            TVectorSKeyExecFunc& execFuncs = it->second;
            TVectorSKeyExecFuncIter execIter = execFuncs.begin();
            TVectorSKeyExecFuncIter execEnd = execFuncs.end();

            while(execIter != execEnd)
            {
                SKeyExecFunc* pKeyExecFunc = *execIter;
                if(pKeyExecFunc != NULL)
                {
                    delete pKeyExecFunc->m_pKeyExecFunc;
                    delete pKeyExecFunc;
                }

                ++execIter;
            }
            execFuncs.clear();

            ++it;
        }

        m_keyExecHashMap.clear();
    }

private:
    void _addKeyExecFunc(const Key key, SKeyExecFunc* pKeyExecFunc)
    {
        THashMapKeyExecIter it = m_keyExecHashMap.find(key);
        if(it != m_keyExecHashMap.end())
        {
            if(!_isExist(*pKeyExecFunc, it->second))
            {
                TVectorSKeyExecFunc& keyExecFuncs = it->second;
                keyExecFuncs.push_back(pKeyExecFunc);
            }
        }
        else
        {
            TVectorSKeyExecFunc keyExecFuncs;
            keyExecFuncs.push_back(pKeyExecFunc);
            m_keyExecHashMap.setValue(key, keyExecFuncs);
        }
    }

    void _removeKeyExecFunc(const Key key, uint64 objAddr, uint64 funcAddr)
    {
        THashMapKeyExecIter it = m_keyExecHashMap.find(key);
        if(it != m_keyExecHashMap.end())
        {
            TVectorSKeyExecFunc& execFuncs = it->second;
            TVectorSKeyExecFuncIter execIter = execFuncs.begin();
            TVectorSKeyExecFuncIter execEnd = execFuncs.end();
            while(execIter != execEnd)
            {
                SKeyExecFunc* pKeyExecFunc = *execIter;
                if(pKeyExecFunc == NULL)
                {
                    return;
                }

                if(pKeyExecFunc->m_objAddr == objAddr && pKeyExecFunc->m_funcAddr == funcAddr)
                {
                    execFuncs.erase(execIter);

                    delete pKeyExecFunc->m_pKeyExecFunc;
                    delete pKeyExecFunc;
                    return;
                }
                ++execIter;
            }
        }
    }

    // Check whether the same function was registered
    bool _isExist(const SKeyExecFunc& keyExecFunc, TVectorSKeyExecFunc& keyExecFuncVector) const
    {
        TVectorSKeyExecFuncIter it = keyExecFuncVector.begin();
        TVectorSKeyExecFuncIter end = keyExecFuncVector.end();

        while(it != end)
        {
            SKeyExecFunc* pKeyExecFunc = *it;
            if(pKeyExecFunc == NULL)
            {
                continue;
            }

            if(pKeyExecFunc->m_objAddr == keyExecFunc.m_objAddr && pKeyExecFunc->m_funcAddr == keyExecFunc.m_funcAddr)
            {
                return true;
            }
            ++it;
        }

        return false;
    }

    // Use to get member function's address
    template<class NAME>
    uint64 _funcAddr2int(void(NAME::* funCmdExec)(ExecFuncParams&)) const
    {
        union
        {
            void(NAME::*_func)(ExecFuncParams&);
            uint64 _id;
        } tmp;
        tmp._id = 0;
        tmp._func = funCmdExec;
        return tmp._id;
    }

    // Use to get c-style function's address
    uint64 _funcAddr2int(FKeyExec funCmdExec) const
    {
        union
        {
            FKeyExec _func;
            uint64 _id;
        } tmp;
        tmp._id = 0;
        tmp._func = funCmdExec;
        return tmp._id;
    }
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_KEYEXECMGR_H__
