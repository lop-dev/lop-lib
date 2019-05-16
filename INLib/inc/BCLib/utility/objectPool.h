//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/objectPool.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_OBJECTPOOL_H__
#define __BCLIB_UTILITY_OBJECTPOOL_H__

#include <BCLib/utility/noncopyable.h>


namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CUnitPool : public CNoncopyable
{
public:
    CUnitPool(uint32 unitSize, uint32 unitCntPerPage = 64);
    virtual ~CUnitPool();

    uint32 getUnitSize() const;
    uint32 getUnitCntPerPage() const;

    bool batchAllocUnit(uint32 unitCnt);
    char* allocUnit();
    void freeUnit(char* unit);

private:
    uint32 m_unitSize;
    uint32 m_unitCntPerPage;
    char* m_pages;
    char* m_nowPageBuff;
    uint32 m_nodeIdx;
    void* m_freeNodeList;
};

template<typename TYPE>
class CObjectPool
{
public:
    CObjectPool(uint32 objectCntPerPage = 64)
    :m_objectPool(sizeof(TYPE), objectCntPerPage)
    {
    }
    ~CObjectPool()
    {
    }

    bool resize(uint32 objectCnt)
    {
        return m_objectPool.batchAllocUnit(objectCnt);
    }

    TYPE* construct()
    {
        TYPE* object = (TYPE*)m_objectPool.allocUnit();
        new (object)TYPE();
        return object;
    }

    template<class P1>
    TYPE* construct(P1 p1)
    {
        TYPE* object = (TYPE*)m_objectPool.allocUnit();
        new (static_cast<void*>(object)) TYPE(p1);
        return object;
    }

    template<class P1, class P2>
    TYPE* construct(P1 p1, P2 p2)
    {
        TYPE* object = (TYPE*)m_objectPool.allocUnit();
        new (static_cast<void*>(object)) TYPE(p1, p2);
        return object;
    }

    template<class P1, class P2, class P3>
    TYPE* construct(P1 p1, P2 p2, P3 p3)
    {
        TYPE* object = (TYPE*)m_objectPool.allocUnit();
        new (static_cast<void*>(object)) TYPE(p1, p2, p3);
        return object;
    }

    template<class P1, class P2, class P3, class P4>
    TYPE* construct(P1 p1, P2 p2, P3 p3, P4 p4)
    {
        TYPE* object = (TYPE*)m_objectPool.allocUnit();
        new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4);
        return object;
    }

    template<class P1, class P2, class P3, class P4, class P5>
    TYPE* construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
    {
        TYPE* object = (TYPE*)m_objectPool.allocUnit();
        new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4, p5);
        return object;
    }

    template<class P1, class P2, class P3, class P4, class P5, class P6>
    TYPE* construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
    {
        TYPE* object = (TYPE*)m_objectPool.allocUnit();
        new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4, p5, p6);
        return object;
    }

    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
    TYPE* construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
    {
        TYPE* object = (TYPE*)m_objectPool.allocUnit();
        new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4, p5, p6, p7);
        return object;
    }

    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
    TYPE* construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
    {
        TYPE* object = (TYPE*)m_objectPool.allocUnit();
        new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4, p5, p6, p7, p8);
        return object;
    }

    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
    TYPE* construct(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
    {
        TYPE* object = (TYPE*)m_objectPool.allocUnit();
        new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4, p5, p6, p7, p8, p9);
        return object;
    }

    void destroy(TYPE*& object)
    {
        if(NULL == object)
        {
            return;
        }
        object->~TYPE();
        m_objectPool.freeUnit((char*&)object);
        object = NULL;
    }

private:
    CUnitPool m_objectPool;
};

template<typename TYPE>
class CObjectArrayPool
{
public:
    CObjectArrayPool(uint32 objectArraySize, uint32 objectArrayCntPerPage = 64)
    :m_objectArraySize(objectArraySize <= 0 ? 1 : objectArraySize)
    ,m_objectArrayPool(sizeof(TYPE) * m_objectArraySize, objectArrayCntPerPage)
    {
    }
    ~CObjectArrayPool()
    {
    }

    uint32 getObjectArraySize() const
    {
        return m_objectArraySize;
    }

    bool resize(uint32 objectArrayCnt)
    {
        return m_objectArrayPool.batchAllocUnit(objectArrayCnt);
    }

    TYPE* constructArray()
    {
        TYPE* objectArray = (TYPE*)m_objectArrayPool.allocUnit();
        if(NULL == objectArray)
        {
            return NULL;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            new (object)TYPE();
        }
        return objectArray;
    }

    template<class P1>
    TYPE* constructArray(P1 p1)
    {
        TYPE* objectArray = (TYPE*)m_objectArrayPool.allocUnit();
        if(NULL == objectArray)
        {
            return NULL;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            new (static_cast<void*>(object)) TYPE(p1);
        }
        return objectArray;
    }

    template<class P1, class P2>
    TYPE* constructArray(P1 p1, P2 p2)
    {
        TYPE* objectArray = (TYPE*)m_objectArrayPool.allocUnit();
        if(NULL == objectArray)
        {
            return NULL;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            new (static_cast<void*>(object)) TYPE(p1, p2);
        }
        return objectArray;
    }

    template<class P1, class P2, class P3>
    TYPE* constructArray(P1 p1, P2 p2, P3 p3)
    {
        TYPE* objectArray = (TYPE*)m_objectArrayPool.allocUnit();
        if(NULL == objectArray)
        {
            return NULL;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            new (static_cast<void*>(object)) TYPE(p1, p2, p3);
        }
        return objectArray;
    }

    template<class P1, class P2, class P3, class P4>
    TYPE* constructArray(P1 p1, P2 p2, P3 p3, P4 p4)
    {
        TYPE* objectArray = (TYPE*)m_objectArrayPool.allocUnit();
        if(NULL == objectArray)
        {
            return NULL;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4);
        }
        return objectArray;
    }

    template<class P1, class P2, class P3, class P4, class P5>
    TYPE* constructArray(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
    {
        TYPE* objectArray = (TYPE*)m_objectArrayPool.allocUnit();
        if(NULL == objectArray)
        {
            return NULL;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4, p5);
        }
        return objectArray;
    }

    template<class P1, class P2, class P3, class P4, class P5, class P6>
    TYPE* constructArray(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
    {
        TYPE* objectArray = (TYPE*)m_objectArrayPool.allocUnit();
        if(NULL == objectArray)
        {
            return NULL;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4, p5, p6);
        }
        return objectArray;
    }

    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7>
    TYPE* constructArray(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
    {
        TYPE* objectArray = (TYPE*)m_objectArrayPool.allocUnit();
        if(NULL == objectArray)
        {
            return NULL;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4, p5, p6, p7);
        }
        return objectArray;
    }

    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
    TYPE* constructArray(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
    {
        TYPE* objectArray = (TYPE*)m_objectArrayPool.allocUnit();
        if(NULL == objectArray)
        {
            return NULL;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4, p5, p6, p7, p8);
        }
        return objectArray;
    }

    template<class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
    TYPE* constructArray(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
    {
        TYPE* objectArray = (TYPE*)m_objectArrayPool.allocUnit();
        if(NULL == objectArray)
        {
            return NULL;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            new (static_cast<void*>(object)) TYPE(p1, p2, p3, p4, p5, p6, p7, p8, p9);
        }
        return objectArray;
    }

    void destroyArray(TYPE*& objectArray)
    {
        if(NULL == objectArray)
        {
            return;
        }
        TYPE* object = objectArray;
        for(uint32 i=0; i<m_objectArraySize; ++i, ++object)
        {
            object->~TYPE();
        }
        m_objectArrayPool.freeUnit((char*&)objectArray);
        objectArray = NULL;
    }

private:
    uint32 m_objectArraySize;
    CUnitPool m_objectArrayPool;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_OBJECTPOOL_H__
