//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/DynamicBufferManager.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_DYNAMICBUFFERMANAGER_H__
#define __GELIB_EXT_DYNAMICBUFFERMANAGER_H__

#include <vector>
#include <map>


namespace GELib
{


class CDynamicVertexBuffer
{
public:
    CDynamicVertexBuffer(int vertexCount, int vertexStride);
    virtual ~CDynamicVertexBuffer();

    void* Lock(int vertexCount);
    void Unlock();
    void Reset();

    int m_iResourceId;
    int m_iVertexCount;
    int m_iVertexStride;
    int m_iAllocated;
    int m_iAge;
};


class CDynamicIndexBuffer
{
public:
    CDynamicIndexBuffer(int indexCount, int indexStride);
    virtual ~CDynamicIndexBuffer();

    void* Lock(int indexCount);
    void Unlock();
    void Reset();

    int m_iResourceId;
    int m_iIndexCount;
    int m_iIndexStride;
    int m_iAllocated;
    int m_iAge;
};


class CDynamicBufferManager
{
public:
    CDynamicBufferManager();
    virtual ~CDynamicBufferManager();

	void Update();
    void SwapBuffers();

    // buffer must be locked/unlocked before allocating next one
    CDynamicVertexBuffer* AllocateDynamicVertexBuffer(int vertexCount, int vertexStride);
    CDynamicIndexBuffer* AllocateDynamicIndexBuffer(int indexCount, int indexStride);

    void* AllocateTempMemory(int size, int &key, int maxAge);
    int AllocateTempVertexBuffer(int count, int stride, int &key, int maxAge);

    int GetQuadIndexBuffer(int numQuads);

	// terrain
	int GetGridIndexBuffer(int width, int height, int step);
	int GetGridVertexBuffer(int width, int height, bool skirt);

private:

    std::vector<CDynamicVertexBuffer*> m_VertexBuffers;
    std::vector<CDynamicIndexBuffer*> m_IndexBuffers;
    int m_iQuadIndexBuffer;
    int m_iNumQuads;

	class CGridBuffer
	{
	public:
		int m_iBuffer;
		int m_iWidth;
		int m_iHeight;
		int m_iFlag;
	};
    std::vector<CGridBuffer> m_GridIndexBuffers;
    std::vector<CGridBuffer> m_GridVertexBuffers;

    class CTempMemory
    {
    public:
        int m_iLife;
        int m_iSize;
        char *m_pData;
    };

    typedef std::map<int, CTempMemory> TempMemoryMap;
    TempMemoryMap m_TempMemories;

    class CTempVertexBuffer
    {
    public:
        int m_iLife;
        int m_iCount;
		int m_iStride;
        int m_iVertexBuffer;
    };

    typedef std::map<int, CTempVertexBuffer> TempVertexBufferMap;
    TempVertexBufferMap m_TempVertexBuffers;

	int GetNextTempMemoryKey();
	int GetNextTempVertexBufferKey();
	int m_iTempMemoryKeyCounter;
	int m_iTempVertexBufferKeyCounter;
};



void SetDynamicBufferManager(CDynamicBufferManager *manager);
CDynamicBufferManager* GetDynamicBufferManager();


}//GELib



#endif//__GELIB_EXT_DYNAMICBUFFERMANAGER_H__
