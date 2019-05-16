//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/pointer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_POINTER_H__
#define __BCLIB_UTILITY_POINTER_H__

#include <stdio.h>
#include <BCLib/utility/atomic.h>

namespace BCLib
{
namespace Utility
{
//////////////////////////////////////////////////////////////////////////
/// @brief ÷«ƒ‹÷∏’Î
//////////////////////////////////////////////////////////////////////////
template<class TYPE, class COUNT_TYPE = CAtomic>
class CPointer
{
    template<class, class> friend class CPointer;
public:
    CPointer()
    :m_object(NULL)
    ,m_count(NULL)
    {
    }

    CPointer(TYPE* object)
    :m_object(object)
    ,m_count(NULL)
    {
        _newCount();
    }

    CPointer(const CPointer<TYPE, COUNT_TYPE>& pointer)
    :m_object(NULL)
    ,m_count(NULL)
    {
        m_object = pointer.m_object;
        m_count = pointer.m_count;
        _setObject();
    }

    template<class OTHER_TYPE>
    CPointer(const CPointer<OTHER_TYPE, COUNT_TYPE>& pointer)
    :m_object(static_cast<TYPE*>(const_cast<OTHER_TYPE*>(pointer.m_object)))
    ,m_count(pointer.getCount())
    {
        dynamicCheck(pointer);
        _setObject();
    }

    ~CPointer()
    {
        _clear();
    }

    TYPE& operator *()
    {
        return *m_object;
    }

    const TYPE& operator *() const
    {
        return *m_object;
    }

    TYPE* operator ->()
    {
        return m_object;
    }

    const TYPE* operator ->() const
    {
        return m_object;
    }

    CPointer& operator =(TYPE* object)
    {
        _clear();
        m_object = object;
        _newCount();
        return *this;
    }

    CPointer& operator =(const CPointer<TYPE, COUNT_TYPE>& pointer)
    {
        if(pointer.m_object == m_object)
        {
            return *this;
        }
        _clear();
        m_object = pointer.m_object;
        m_count = pointer.m_count;
        _setObject();
        return *this;
    }

    template<class OTHER_TYPE>
    CPointer& operator =(const CPointer<OTHER_TYPE, COUNT_TYPE>& pointer)
    {
        dynamicCheck(pointer);

        _clear();
        m_object = static_cast<TYPE*>(const_cast<OTHER_TYPE*>(pointer.m_object));

        if(m_object != NULL)
        {
            m_count = pointer.getCount();
            _setObject();
        }
        return *this;
    }

    bool operator ==(TYPE* object) const
    {
        return m_object == object;
    }

    bool operator ==(const CPointer<TYPE>& pointer) const
    {
        return m_object == pointer.m_object;
    }

    template<class OTHER_TYPE>
    bool operator ==(const CPointer<OTHER_TYPE>& pointer) const
    {
        dynamicCheck(pointer);

        if(m_object == NULL && pointer.m_object == NULL)
        {
            return true;
        }
        return m_object != NULL && m_object == static_cast<TYPE*>(pointer.m_object);
    }

    bool operator !=(TYPE* object) const
    {
        return m_object != object;
    }

    bool operator !=(const CPointer<TYPE>& pointer) const
    {
        return m_object != pointer.m_object;
    }

    template<class OTHER_TYPE>
    bool operator !=(const CPointer<OTHER_TYPE>& pointer) const
    {
        dynamicCheck(pointer);

        if(m_object == NULL && pointer.m_object == NULL)
        {
            return false;
        }
        return m_object != NULL && m_object != static_cast<TYPE*>(pointer.m_object);
    }

    //operator TYPE *() const
    //{
    //    return m_object;
    //}

    //template<class OTHER>
    //operator OTHER *() const
    //{
    //    return static_cast<OTHER*>(m_object);
    //}

    COUNT_TYPE* getCount() const
    {
        return m_count;
    }

private:
    inline void _clear()
    {
        if(m_object != NULL)
        {
            if(m_count->decrement() <= 0)
            {
                BCLIB_SAFE_DELETE(m_object);
                BCLIB_SAFE_DELETE(m_count);
                return ;
            }
        }
        m_count = NULL;
        m_object = NULL;
    }

    inline void _newCount()
    {
        if(m_object != NULL)
        {
            m_count = new COUNT_TYPE();
            if(m_count == NULL)
            {
                m_object = NULL;
                return ;
            }
            m_count->increment();
        }
    }

    inline void _setObject()
    {
        if(m_object != NULL)
        {
            m_count->increment();
            return ;
        }
        m_count = NULL;
    }

    template<class OTHER_TYPE>
    void dynamicCheck(const CPointer<OTHER_TYPE, COUNT_TYPE>& pointer)
    {
#if defined(_DEBUG)
        if(pointer.m_object && !dynamic_cast<TYPE*>(pointer.m_object))
        {
            // asert(0);
        }
#endif
    }

private:
    TYPE* m_object;
    COUNT_TYPE* m_count;
};
}//Utility
}//BCLib


#endif//__BCLIB_UTILITY_POINTER_H__
