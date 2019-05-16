//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/spointer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_SPOINTER_H__
#define __BCLIB_UTILITY_SPOINTER_H__

#include <BCLib/utility/baseDef.h>
#include <BCLib/utility/atomic.h>


#if defined(_WIN32)
#pragma warning(error:4584)
#endif


namespace BCLib
{
namespace Utility
{
////////////////////////////////////////////////////////////////////////////////////////////
//! 引用计数基类
class BCLIB_UTILITY_API CRefCnt
{
    template<class> friend class CCPointer;
public:
    CRefCnt()
    {
    }
    virtual ~CRefCnt()
    {
    }

private:
    inline BCLib::int32 _incRef() const
    {
        return const_cast<CRefCnt*>(this)->m_atom.increment();
    }

    inline BCLib::int32 _decRef() const
    {
        return const_cast<CRefCnt*>(this)->m_atom.decrement();
    }

private:
    CAtomic m_atom;
};

////////////////////////////////////////////////////////////////////////////////////////////
//! 删除对象用模板函数,需要特殊处理请自行特化
template<class OBJTYPE>
inline void destroyObj4SPointer(const OBJTYPE* pObj)
{
    BCLIB_SAFE_DELETE(pObj);
}

////////////////////////////////////////////////////////////////////////////////////////////
//! 引用计数型智能指针,const型
template<class OBJTYPE>
class CCPointer
{
    template<class> friend class CCPointer;
public:
    CCPointer();
    CCPointer(const OBJTYPE*);
    CCPointer(const CCPointer<OBJTYPE>&);
    template<class OTHER_OBJTYPE>
    CCPointer(const OTHER_OBJTYPE*);
    template<class OTHER_OBJTYPE>
    CCPointer(const CCPointer<OTHER_OBJTYPE>&);
    ~CCPointer();

    CCPointer& operator =(const OBJTYPE*);
    CCPointer& operator =(const CCPointer<OBJTYPE>&);
    template<class OTHER_OBJTYPE>
    CCPointer& operator =(const OTHER_OBJTYPE*);
    template<class OTHER_OBJTYPE>
    CCPointer& operator =(const CCPointer<OTHER_OBJTYPE>&);

    const OBJTYPE& operator *() const;
    const OBJTYPE* operator ->() const;

    template<class OBJ_TYPE> friend bool operator ==(const CCPointer<OBJ_TYPE>&, const OBJ_TYPE*);
    template<class OBJ_TYPE> friend bool operator ==(const OBJ_TYPE*, const CCPointer<OBJ_TYPE>&);
    template<class OBJ_TYPE> friend bool operator ==(const CCPointer<OBJ_TYPE>&, const CCPointer<OBJ_TYPE>&);
    template<class OBJ_TYPE> friend bool operator !=(const CCPointer<OBJ_TYPE>&, const OBJ_TYPE*);
    template<class OBJ_TYPE> friend bool operator !=(const OBJ_TYPE*, const CCPointer<OBJ_TYPE>&);
    template<class OBJ_TYPE> friend bool operator !=(const CCPointer<OBJ_TYPE>&, const CCPointer<OBJ_TYPE>&);

    typedef const OBJTYPE* CCPointer<OBJTYPE>::* unspecified_bool_type;
    operator unspecified_bool_type() const;

protected:
    void _clearObj(const OBJTYPE*) const;
    void _setObj(const OBJTYPE*);

protected:
    const OBJTYPE* m_pObj;
};

////////////////////////////////////////////////////////////////////////////////////////////
//! 引用计数型智能指针,非const型
template<class OBJTYPE>
class CSPointer : public CCPointer<OBJTYPE>
{
    template<class> friend class CSPointer;
public:
    CSPointer();
    CSPointer(OBJTYPE*);
    CSPointer(const CSPointer<OBJTYPE>&);
    template<class OTHER_OBJTYPE>
    CSPointer(OTHER_OBJTYPE*);
    template<class OTHER_OBJTYPE>
    CSPointer(const CSPointer<OTHER_OBJTYPE>&);
    ~CSPointer();

    CSPointer& operator =(OBJTYPE*);
    CSPointer& operator =(const CSPointer<OBJTYPE>&);
    template<class OTHER_OBJTYPE>
    CSPointer& operator =(OTHER_OBJTYPE*);
    template<class OTHER_OBJTYPE>
    CSPointer& operator =(const CSPointer<OTHER_OBJTYPE>&);

    OBJTYPE& operator *() const;
    OBJTYPE* operator ->() const;

};

////////////////////////////////////////////////////////////////////////////////////////////
template<class OBJTYPE>
CCPointer<OBJTYPE>::CCPointer()
:m_pObj(NULL)
{
    ;
}

template<class OBJTYPE>
CCPointer<OBJTYPE>::CCPointer(const OBJTYPE* pObj)
:m_pObj(NULL)
{
    _setObj(pObj);
}

template<class OBJTYPE>
CCPointer<OBJTYPE>::CCPointer(const CCPointer<OBJTYPE>& ptr)
:m_pObj(NULL)
{
    _setObj(ptr.m_pObj);
}

template<class OBJTYPE>
template<class OTHER_OBJTYPE>
CCPointer<OBJTYPE>::CCPointer(const OTHER_OBJTYPE* pOObj)
:m_pObj(NULL)
{
    _setObj(static_cast<const OBJTYPE*>(pOObj));
}

template<class OBJTYPE>
template<class OTHER_OBJTYPE>
CCPointer<OBJTYPE>::CCPointer(const CCPointer<OTHER_OBJTYPE>& optr)
:m_pObj(NULL)
{
    _setObj(static_cast<const OBJTYPE*>(optr.m_pObj));
}

template<class OBJTYPE>
CCPointer<OBJTYPE>::~CCPointer()
{
    _clearObj(m_pObj);
    m_pObj = NULL;
}

template<class OBJTYPE>
CCPointer<OBJTYPE>& CCPointer<OBJTYPE>::operator =(const OBJTYPE* pObj)
{
    const OBJTYPE* pOld = m_pObj;
    _setObj(pObj);
    _clearObj(pOld);
    return *this;
}

template<class OBJTYPE>
CCPointer<OBJTYPE>& CCPointer<OBJTYPE>::operator =(const CCPointer<OBJTYPE>& ptr)
{
    const OBJTYPE* pOld = m_pObj;
    _setObj(ptr.m_pObj);
    _clearObj(pOld);
    return *this;
}

template<class OBJTYPE>
template<class OTHER_OBJTYPE>
CCPointer<OBJTYPE>& CCPointer<OBJTYPE>::operator =(const OTHER_OBJTYPE* pOObj)
{
    const OBJTYPE* pOld = m_pObj;
    _setObj(static_cast<const OBJTYPE*>(pOObj));
    _clearObj(pOld);
    return *this;
}

template<class OBJTYPE>
template<class OTHER_OBJTYPE>
CCPointer<OBJTYPE>& CCPointer<OBJTYPE>::operator =(const CCPointer<OTHER_OBJTYPE>& optr)
{
    const OBJTYPE* pOld = m_pObj;
    _setObj(static_cast<const OBJTYPE*>(optr.m_pObj));
    _clearObj(pOld);
    return *this;
}

template<class OBJTYPE>
const OBJTYPE& CCPointer<OBJTYPE>::operator *() const
{
    return *const_cast<const OBJTYPE*>(m_pObj);
}

template<class OBJTYPE>
const OBJTYPE* CCPointer<OBJTYPE>::operator ->() const
{
    return const_cast<const OBJTYPE*>(m_pObj);
}

template<class OBJTYPE>
bool operator ==(const CCPointer<OBJTYPE>& ptr, const OBJTYPE* pObj)
{
    return ptr.m_pObj == pObj;
}

template<class OBJTYPE>
bool operator ==(const OBJTYPE* pObj, const CCPointer<OBJTYPE>& ptr)
{
    return pObj == ptr.m_pObj;
}

template<class OBJTYPE>
bool operator ==(const CCPointer<OBJTYPE>& ptrA, const CCPointer<OBJTYPE>& ptrB)
{
    return ptrA.m_pObj == ptrB.m_pObj;
}

template<class OBJTYPE>
bool operator !=(const CCPointer<OBJTYPE>& ptr, const OBJTYPE* pObj)
{
    return ptr.m_pObj != pObj;
}

template<class OBJTYPE>
bool operator !=(const OBJTYPE* pObj, const CCPointer<OBJTYPE>& ptr)
{
    return pObj != ptr.m_pObj;
}

template<class OBJTYPE>
bool operator !=(const CCPointer<OBJTYPE>& ptrA, const CCPointer<OBJTYPE>& ptrB)
{
    return ptrA.m_pObj != ptrB.m_pObj;
}

template<class OBJTYPE>
void CCPointer<OBJTYPE>::_clearObj(const OBJTYPE* pObj) const
{
    if(pObj)
    {
        if(static_cast<const CRefCnt*>(pObj)->_decRef() <= 0)
        {
            destroyObj4SPointer(pObj);
        }
    }
}

template<class OBJTYPE>
void CCPointer<OBJTYPE>::_setObj(const OBJTYPE* pObj)
{
    if(pObj)
    {
        static_cast<const CRefCnt*>(pObj)->_incRef();
    }
    m_pObj = pObj;
}

template<class OBJTYPE>
CCPointer<OBJTYPE>::operator unspecified_bool_type() const
{
    return m_pObj == 0 ? 0 : &CCPointer<OBJTYPE>::m_pObj;
}

////////////////////////////////////////////////////////////////////////////////////////////
template<class OBJTYPE>
CSPointer<OBJTYPE>::CSPointer()
:CCPointer<OBJTYPE>()
{
    ;
}

template<class OBJTYPE>
CSPointer<OBJTYPE>::CSPointer(OBJTYPE* pObj)
:CCPointer<OBJTYPE>(pObj)
{
    ;
}

template<class OBJTYPE>
CSPointer<OBJTYPE>::CSPointer(const CSPointer<OBJTYPE>& ptr)
:CCPointer<OBJTYPE>(ptr)
{
    ;
}

template<class OBJTYPE>
template<class OTHER_OBJTYPE>
CSPointer<OBJTYPE>::CSPointer(OTHER_OBJTYPE* pOObj)
:CCPointer<OBJTYPE>(static_cast<OBJTYPE*>(pOObj))
{
    ;
}

template<class OBJTYPE>
template<class OTHER_OBJTYPE>
CSPointer<OBJTYPE>::CSPointer(const CSPointer<OTHER_OBJTYPE>& optr)
:CCPointer<OBJTYPE>(static_cast<const OBJTYPE*>(optr.m_pObj))
{
    ;
}

template<class OBJTYPE>
CSPointer<OBJTYPE>::~CSPointer()
{
    ;
}

template<class OBJTYPE>
CSPointer<OBJTYPE>& CSPointer<OBJTYPE>::operator =(OBJTYPE* pObj)
{
    CCPointer<OBJTYPE>::operator =(pObj);
    return *this;
}

template<class OBJTYPE>
CSPointer<OBJTYPE>& CSPointer<OBJTYPE>::operator =(const CSPointer<OBJTYPE>& ptr)
{
    CCPointer<OBJTYPE>::operator =(ptr);
    return *this;
}

template<class OBJTYPE>
template<class OTHER_OBJTYPE>
CSPointer<OBJTYPE>& CSPointer<OBJTYPE>::operator =(OTHER_OBJTYPE* pOObj)
{
    CCPointer<OBJTYPE>::operator =(static_cast<OBJTYPE*>(pOObj));
    return *this;
}

template<class OBJTYPE>
template<class OTHER_OBJTYPE>
CSPointer<OBJTYPE>& CSPointer<OBJTYPE>::operator =(const CSPointer<OTHER_OBJTYPE>& optr)
{
    CCPointer<OBJTYPE>::operator =(static_cast<const OBJTYPE*>(optr.m_pObj));
    return *this;
}

template<class OBJTYPE>
OBJTYPE& CSPointer<OBJTYPE>::operator *() const
{
    return *const_cast<OBJTYPE*>(CCPointer<OBJTYPE>::m_pObj);
}

template<class OBJTYPE>
OBJTYPE* CSPointer<OBJTYPE>::operator ->() const
{
    return const_cast<OBJTYPE*>(CCPointer<OBJTYPE>::m_pObj);
}
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_SPOINTER_H__
