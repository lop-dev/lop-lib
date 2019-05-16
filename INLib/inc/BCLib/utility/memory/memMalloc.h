//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/memory/memMalloc.h
//  author:     League of Perfect
/// @brief      内存管理(malloc/free),支持自定义内存来源,支持crash恢复
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_MEMORY_MEMMALLOC_H__
#define __BCLIB_UTILITY_MEMORY_MEMMALLOC_H__

#include <BCLib/utility/function.h>

namespace BCLib
{
namespace Utility
{
//////////////////////////////////////////////////////////////////////////
/// @brief 内存管理模块
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CMemAlloc
{
public:
    /// @brief 创建外部内存的函数原型
    /// @return void* 创建的外部内存地址,失败返回0;返回的外部内存起始地址必须8字节对齐,且必须用0清空内存!!!
    /// @param size 创建的外部内存大小
    typedef void* (*MEM_OPEN_FUN)(size_t size);
    /// @brief 释放外部内存的函数原型
    /// @return void
    /// @param p 释放的外部内存地址
    typedef void(*MEM_CLOSE_FUN)(void* p);

    /// @brief 初始化内存管理模块
    /// @return size_t 0成功,非0失败
    /// @param memOpen 创建外部内存使用的全局函数指针/类成员函数指针/指针对象
    /// @param memClose 释放外部内存使用的全局函数指针/类成员函数指针/指针对象
    /// @param memGranularity 外部内存的颗粒度,必须为2的幂,最小为64k
    /// @param object 类成员函数指针的类对象
    static size_t init(MEM_OPEN_FUN memOpen, MEM_CLOSE_FUN memClose, size_t memGranularity);
    static size_t init(const CFunction<void*, size_t>& memOpen, const CFunction<void, void*>& memClose, size_t memGranularity);
    template<class NAME>
    static size_t init(NAME* object, void*(NAME::* memOpen)(size_t), void(NAME::* memClose)(void*), size_t memGranularity)
    {
        return init(CFunctionObject<void*, NAME, size_t>(memOpen, object), CFunctionObject<void, NAME, void*>(memClose, object), memGranularity);
    }

    /// @brief 初始化内存管理模块,用于crash后恢复
    /// @return size_t 0成功,非0失败
    /// @param memOpen 创建外部内存使用的全局函数指针/类成员函数指针/指针对象
    /// @param memClose 释放外部内存使用的全局函数指针/类成员函数指针/指针对象
    /// @param ppOldMem 外部内存的旧起始地址数组(只需要提供数组,init内部会填充起始地址)
    /// @param ppNewMem 外部内存的新起始地址数组
    /// @param pSize 外部内存的大小数组
    /// @param memCnt 外部内存的个数
    /// @param object 类成员函数指针的类对象
    static size_t init(MEM_OPEN_FUN memOpen, MEM_CLOSE_FUN memClose, void** ppOldMem, void** ppNewMem, size_t* pSize, size_t memCnt);
    static size_t init(const CFunction<void*, size_t>& memOpen, const CFunction<void, void*>& memClose, void** ppOldMem, void** ppNewMem, size_t* pSize, size_t memCnt);
    template<class NAME>
    static size_t init(NAME* object, void*(NAME::* memOpen)(size_t), void(NAME::* memClose)(void*), void** ppOldMem, void** ppNewMem, size_t* pSize, size_t memCnt)
    {
        return init(CFunctionObject<void*, NAME, size_t>(memOpen, object), CFunctionObject<void, NAME, void*>(memClose, object), ppOldMem, ppNewMem, pSize, memCnt);
    }

    /// @brief 结束使用内存管理模块
    /// @return void
    /// @param clearMem true:清理已分配的内存, false:不清理
    static void final(bool clearMem);

    /// @brief 设置dummy指针,crash恢复后仍可取回,默认值为0
    /// @return void
    /// @param p dummy指针
    static void setDummy(void* p);

    /// @brief 获取dummy指针
    /// @return void* dummy指针
    static void* getDummy();

    /// @brief 将旧的地址转换为新的地址(crash后使用)
    /// @return void* 转换后的新地址,失败返回0
    /// @param p 需要转换的地址
    /// @param ppOldMem 外部内存的旧起始地址数组(调用init后获取)
    /// @param ppNewMem 外部内存的新起始地址数组
    /// @param pSize 外部内存的大小数组
    /// @param memCnt 外部内存的个数
    static void* addrConvert(void* p, void** ppOldMem, void** ppNewMem, size_t* pSize, size_t memCnt);

    /// @brief 分配内存块
    /// @return void* 分配内存块的地址,失败返回0
    /// @param n 内存块大小,0表示可分配的最小内存块(32位系统为16字节,64位系统为32字节)
    static void* malloc(size_t n);

    /// @brief 释放内存块
    /// @return void
    /// @param p 释放内存块的地址,如果地址不是已分配的,会导致异常中断退出
    static void free(void* p);

    /// @brief 外部内存使用量
    /// @return size_t 外部内存使用量
    /// @param
    static size_t footprint(void);

    /// @brief 外部内存使用量的峰值
    /// @return size_t 外部内存使用量的峰值
    /// @param
    static size_t footprintMax(void);

    /// @brief 外部内存使用量的上限
    /// @return size_t 外部内存使用量的上限,默认为size_t类型的最大值
    /// @param
    static size_t footprintLimit(void);

    /// @brief 设置外部内存使用量的上限
    /// @return size_t 设置后的外部内存使用量的上限
    /// @param bytes 外部内存使用量的上限,会自动按外部内存的颗粒度取上界对齐
    static size_t setFootprintLimit(size_t bytes);
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_MEMORY_MEMMALLOC_H__
