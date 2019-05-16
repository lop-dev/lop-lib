//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/GraphicDevice/GraphicDeviceResource.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_GRAPHICDEVICE_GRAPHICDEVICERESOURCE_H__
#define __GELIB_GRAPHICDEVICE_GRAPHICDEVICERESOURCE_H__

#include <vector>
#include <string>
#include <map>
#include <cassert>

#include <GELib/GraphicDevice/GraphicDeviceTypes.h>

#define RES_USE_CHUNK_NEW

namespace GELib
{


class CGraphicDevice;

class CGraphicDeviceResource
{
public:
    CGraphicDeviceResource();
    virtual ~CGraphicDeviceResource();
    virtual void Release();
    virtual void OnLostDevice();
    virtual void OnResetDevice();

    int m_iId;
    int m_iHash;
    int m_iSize;
    void *m_hHandle;
    short m_iRefCount;
    bool m_bNeedRecreate;
    EResourceType m_eType;
    CGraphicDevice *m_pDevice;
    std::string m_Name;
};

class CTextureResource : public CGraphicDeviceResource
{
public:
    CTextureResource();
    virtual ~CTextureResource();
    virtual void Release();
    virtual void OnResetDevice();

    short m_iWidth;
    short m_iHeight;
    short m_iDepth;
    short m_iMipLevels;
    short m_iUsage;
    bool m_bLinear;
    bool m_bGammaCorrection;
    EPixelFormat m_eFormat;

    // xbox
    void *m_pPhysicalData;
};

class CSurfaceResource : public CGraphicDeviceResource
{
public:
    CSurfaceResource();
    virtual ~CSurfaceResource();
    virtual void Release();
    virtual void OnResetDevice();

    short m_iWidth;
    short m_iHeight;
    EPixelFormat m_eFormat;
    EMultiSampleType m_eMultiSampleType;
    bool m_bAllocateEDRAM;
    short m_iNumTiles;
    short m_iUsage;
};

class CRenderTargetResource : public CGraphicDeviceResource
{
public:
    CRenderTargetResource();
    virtual ~CRenderTargetResource();

    int m_iColorTexture;
    int m_iColorSurface;
    int m_iDepthStencilTexture;
    int m_iDepthStencilSurface;
    bool m_bUseDefaultDepthSurface;
    EMultiSampleType m_eMultiSampleType;
    bool m_bGammaCorrectionWrite;
};

class CAnimatedTextureResource : public CGraphicDeviceResource
{
public:
    CAnimatedTextureResource();
    virtual ~CAnimatedTextureResource();

    int m_iTextureIndex;
    float m_fFPS;
    std::vector<int> m_Textures;
};

class CBufferResource : public CGraphicDeviceResource
{
public:
    CBufferResource();
    virtual ~CBufferResource();
    virtual void Release();
    virtual void OnResetDevice();

    int m_iCount;
    short m_iStride;
    short m_iUsage;
};

class CShaderResource : public CGraphicDeviceResource
{
public:
    CShaderResource();
    virtual ~CShaderResource();
    virtual void Release();

    void SetCode(const void *code, int size);
    bool IsCodeEqual(const void *code, int size);

    std::vector<unsigned char> m_Code;
};

class CVertexDeclarationResource : public CGraphicDeviceResource
{
public:
    CVertexDeclarationResource();
    virtual ~CVertexDeclarationResource();
    virtual void Release();

    void SetSpec(const CVertexSpec &spec);
    bool IsSpecEqual(const CVertexSpec &spec);

    CVertexSpec m_Spec;
};

class COcclusionQueryResource : public CGraphicDeviceResource
{
public:
    COcclusionQueryResource();
    virtual ~COcclusionQueryResource();
    virtual void Release();
    virtual void OnResetDevice();
};

const int RESID_INDEX_BITS  = 15;
const int RESID_SERIAL_BITS = 12;
const int RESID_TYPE_BITS   = 4;
const int RESID_INDEX_MAX   = (1 << RESID_INDEX_BITS) - 1;
const int RESID_SERIAL_MAX  = (1 << RESID_SERIAL_BITS) - 1;
const int RESID_TYPE_MAX    = (1 << RESID_TYPE_BITS) - 1;
const int RESID_TYPE_EMPTY  = (1 << (RESID_INDEX_BITS + RESID_SERIAL_BITS)) - 1;

inline int ResourceIdToIndex(int id)
{
    return id & RESID_INDEX_MAX;
}
inline int ResourceIdToSerial(int id)
{
    return (id >> RESID_INDEX_BITS) & RESID_SERIAL_MAX;
}
inline EResourceType ResourceIdToType(int id)
{
    return EResourceType((id >> (RESID_INDEX_BITS + RESID_SERIAL_BITS)) & RESID_TYPE_MAX);
}
inline int MakeResourceId(int index, int serial, EResourceType type)
{
    return index + (serial << RESID_INDEX_BITS) + (type << (RESID_INDEX_BITS + RESID_SERIAL_BITS));
}
inline void ChangeResourceIdByType(CGraphicDeviceResource *r, EResourceType type)
{
    r->m_iId = (r->m_iId & RESID_TYPE_EMPTY) + (type << (RESID_INDEX_BITS + RESID_SERIAL_BITS));
}

int ComputeResourceHash(const void *data, int size);
int ComputeResourceHash(const char *str);

template<typename T> class CGraphicDeviceResourceManager
{
public:
    typedef std::vector<T*> ListType;
    typedef std::multimap<int, int> HashType;

    CGraphicDeviceResourceManager(CGraphicDevice *device, int chunkSize)
    {
        m_iListSize = 0;
        m_iNumEntries = 0;
        m_iTotalSize = 0;
        m_iSerial = 0;
        m_iFirstEmptySlot = -1;
        m_pDevice = device;

#ifdef RES_USE_CHUNK_NEW
        m_iChunkSize = chunkSize;
        m_iChunkFreeIndex = m_iChunkSize;
#endif
    }
    virtual ~CGraphicDeviceResourceManager()
    {
        FreeAll();
    }

    T* Alloc(EResourceType rt, const char *name=0, int hash=0)
    {
        if (m_iListSize >= RESID_INDEX_MAX)
        {
            assert(false && (bool)"exceed max allowed resources");
            return 0;
        }

        T *r;
        if (m_FreeResources.empty())
        {
#ifdef RES_USE_CHUNK_NEW
            if (m_iChunkFreeIndex >= m_iChunkSize)
            {
                char *chunk = new char[sizeof(T) * m_iChunkSize];
                if (chunk == 0)
                    return 0;
                m_ChunkPool.push_back(chunk);
                m_iChunkFreeIndex = 0;
            }
            r = new(m_ChunkPool.back() + sizeof(T) * m_iChunkFreeIndex++) T;	// placement new, call constructor
#else
            r = new T;
            if (r == 0)
                return 0;
#endif
        }
        else
        {
            r = new(m_FreeResources.back()) T;	// placement new, call constructor
            m_FreeResources.pop_back();
        }

        int idx;
        if (m_iFirstEmptySlot >= 0)
        {
            idx = m_iFirstEmptySlot;
            m_Resources[idx] = r;

            int i;
            for (i=m_iFirstEmptySlot+1; i<m_iListSize; ++i)
            {
                if (m_Resources[i] == 0)
                {
                    m_iFirstEmptySlot = i;
                    break;
                }
            }
            if (i >= m_iListSize)
                m_iFirstEmptySlot = -1;
        }
        else
        {
            // no empty slot, add new one
            m_Resources.push_back(r);
            idx = m_iListSize++;
        }


        if (name)
        {
            hash = ComputeResourceHash(name);
            r->m_Name = name;
        }
        if (hash)
        {
            m_Hash.insert(HashType::value_type(hash, idx));
        }

        r->m_pDevice = m_pDevice;
        r->m_eType = rt;
        r->m_iId = MakeResourceId(idx, GetNextSerial(), rt);
        r->m_iRefCount = 1;
        r->m_iHash = hash;
        ++m_iNumEntries;
        return r;
    }

    void Free(T *r)
    {
        if (r == 0)
            return;
        if ((--r->m_iRefCount <= 0) && GetByID(r->m_iId))
        {
            // clear slot
            int idx = ResourceIdToIndex(r->m_iId);
            m_Resources[idx] = 0;
            if (m_iFirstEmptySlot<0 || idx<m_iFirstEmptySlot)
                m_iFirstEmptySlot = idx;

            // clear hash
            if (r->m_iHash)
            {
                HashType::iterator i = m_Hash.lower_bound(r->m_iHash);
                HashType::iterator end = m_Hash.upper_bound(r->m_iHash);
                while (i != end)
                {
                    if ((*i).second == idx)
                    {
                        m_Hash.erase(i);
                        break;
                    }
                    ++i;
                }
            }

            --m_iNumEntries;
            DecreaseTotalSize(r);
            r->~T();	// call destructor
            m_FreeResources.push_back(r);
        }
    }

    T* GetByIndex(int idx)
    {
        if (idx<0 || idx>=m_iListSize)
            return 0;
        return m_Resources[idx];
    }

    T* GetByID(int id)
    {
        T *r = GetByIndex(ResourceIdToIndex(id));
        if (r==0 || r->m_iId!=id)
            return 0;
        return r;
    }

    T* GetByName(const char *name, EResourceType type=RES_UNKNOWN)
    {
        if (name == 0)
            return 0;
        int hash = ComputeResourceHash(name);
        HashType::iterator i = m_Hash.lower_bound(hash);
        HashType::iterator end = m_Hash.upper_bound(hash);
        while (i != end)
        {
            int idx = (*i).second;
            T *r = GetByIndex(idx);
            if (r && (type==RES_UNKNOWN || type==r->m_eType) && r->m_Name==name)
                return r;
            ++i;
        }
        return 0;
    }

    int GetMaxIndex()
    {
        return m_iListSize - 1;
    }

    void GetTotalSize(int &numEntries, int &totalSize, EResourceType type=RES_UNKNOWN)
    {
        if (type == RES_UNKNOWN)
        {
            numEntries = m_iNumEntries;
            totalSize = m_iTotalSize;
            return;
        }

        numEntries = 0;
        totalSize = 0;
        int i;
        for (i=0; i<m_iListSize; ++i)
        {
            T *r = GetByIndex(i);
            if (r && type==r->m_eType)
            {
                ++numEntries;
                totalSize += r->m_iSize;
            }
        }
    }

    void IncreaseTotalSize(T *r)
    {
        m_iTotalSize += r->m_iSize;
    }

    void DecreaseTotalSize(T *r)
    {
        m_iTotalSize -= r->m_iSize;
    }

    void OnLostDevice()
    {
        int i;
        for (i=0; i<m_iListSize; ++i)
        {
            T *r = GetByIndex(i);
            if (r)
                r->OnLostDevice();
        }
    }

    void OnResetDevice()
    {
        int i;
        for (i=0; i<m_iListSize; ++i)
        {
            T *r = GetByIndex(i);
            if (r)
                r->OnResetDevice();
        }
    }

    void FreeAll()
    {
        int i;
        for (i=0; i<m_iListSize; ++i)
        {
            T *r = GetByIndex(i);
            assert(r==0 && (bool)"All resources must be deleted before shutdown");
            if (r)
            {
                r->m_iRefCount = 1;
                Free(r);
            }
        }

#ifdef RES_USE_CHUNK_NEW
        for (i=0; i<(int)m_ChunkPool.size(); ++i)
            delete[] m_ChunkPool[i];
        m_ChunkPool.clear();
        m_iChunkFreeIndex = 0;
#else
        for (i=0; i<(int)m_FreeResources.size(); ++i)
            delete m_FreeResources[i];
#endif

        m_Resources.clear();
        m_FreeResources.clear();
        m_Hash.clear();
        m_iListSize = 0;
        m_iNumEntries = 0;
        m_iTotalSize = 0;
        m_iFirstEmptySlot = -1;
    }

protected:
    int GetNextSerial()
    {
        ++m_iSerial;
        if (m_iSerial > RESID_SERIAL_MAX)
            m_iSerial = 1;
        return m_iSerial;
    }

    ListType m_Resources;
    ListType m_FreeResources;
    HashType m_Hash;
    int m_iListSize;
    int m_iSerial;
    int m_iNumEntries;
    int m_iTotalSize;
    int m_iFirstEmptySlot;
    CGraphicDevice *m_pDevice;

#ifdef RES_USE_CHUNK_NEW
    std::vector<char*> m_ChunkPool;
    int m_iChunkSize;
	int m_iChunkFreeIndex;
#endif

};

class CTextureResourceManager : public CGraphicDeviceResourceManager<CTextureResource>
{
public:
    CTextureResourceManager(CGraphicDevice *device, int chunkSize) : CGraphicDeviceResourceManager<CTextureResource>(device, chunkSize) {}
};

class CSurfaceResourceManager : public CGraphicDeviceResourceManager<CSurfaceResource>
{
public:
    CSurfaceResourceManager(CGraphicDevice *device, int chunkSize) : CGraphicDeviceResourceManager<CSurfaceResource>(device, chunkSize) {}
};

class CRenderTargetResourceManager : public CGraphicDeviceResourceManager<CRenderTargetResource>
{
public:
    CRenderTargetResourceManager(CGraphicDevice *device, int chunkSize) : CGraphicDeviceResourceManager<CRenderTargetResource>(device, chunkSize) {}
};

class CBufferResourceManager : public CGraphicDeviceResourceManager<CBufferResource>
{
public:
    CBufferResourceManager(CGraphicDevice *device, int chunkSize) : CGraphicDeviceResourceManager<CBufferResource>(device, chunkSize) {}
};

class CShaderResourceManager : public CGraphicDeviceResourceManager<CShaderResource>
{
public:
    CShaderResourceManager(CGraphicDevice *device, int chunkSize) : CGraphicDeviceResourceManager<CShaderResource>(device, chunkSize) {}
    int CreateByCode(EResourceType type, const void *code, int size, int &hash);
};

class CVertexDeclarationResourceManager : public CGraphicDeviceResourceManager<CVertexDeclarationResource>
{
public:
    CVertexDeclarationResourceManager(CGraphicDevice *device, int chunkSize) : CGraphicDeviceResourceManager<CVertexDeclarationResource>(device, chunkSize) {}
    int CreateBySpec(const CVertexSpec &spec, int &hash);
};

class COcclusionQueryResourceManager : public CGraphicDeviceResourceManager<COcclusionQueryResource>
{
public:
    COcclusionQueryResourceManager(CGraphicDevice *device, int chunkSize) : CGraphicDeviceResourceManager<COcclusionQueryResource>(device, chunkSize) {}
};

class CAnimatedTextureResourceManager : public CGraphicDeviceResourceManager<CAnimatedTextureResource>
{
public:
    CAnimatedTextureResourceManager(CGraphicDevice *device, int chunkSize) : CGraphicDeviceResourceManager<CAnimatedTextureResource>(device, chunkSize) {}
};


}//GELib

#endif//__GELIB_GRAPHICDEVICE_GRAPHICDEVICERESOURCE_H__
