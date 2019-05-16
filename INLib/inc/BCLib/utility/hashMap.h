//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/hashMap.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_HASHMAP_H__
#define __BCLIB_UTILITY_HASHMAP_H__

#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#include <unordered_map>
#include <BCLib/utility/string.h>
#include <BCLib/utility/function.h>

#ifdef __GNUC__
namespace __gnu_cxx
{
template<>
struct hash< BCLib::int64 >
{
    size_t operator()(const BCLib::int64& n) const
    {
        return n;
    }
};

template<>
struct hash< BCLib::uint64 >
{
    size_t operator()(const BCLib::uint64& n) const
    {
        return n;
    }
};

template<>
struct hash< std::string >
{
    size_t operator()(const std::string& s) const
    {
        return hash<const char*>()(s.c_str());
    }
};
}//__gnu_cxx
#endif

namespace BCLib
{
namespace Utility
{
template<class KEY>
class CHashCompare : public std::binary_function<KEY, KEY, bool>
{
public:
    inline bool operator()(const KEY& key1, const KEY& key2) const;
};

template<>
inline bool CHashCompare< std::string >::operator()(const std::string& key1, const std::string& key2) const
{
#ifdef __GNUC__
    return strcmp(key1.c_str(), key2.c_str()) == 0;
#else
    return strcmp(key1.c_str(), key2.c_str()) != 0;
#endif
}

template<>
inline bool CHashCompare< CStringA >::operator()(const CStringA& key1, const CStringA& key2) const
{
#ifdef __GNUC__
    return strcmp(key1.c_str(), key2.c_str()) == 0;
#else
    return strcmp(key1.c_str(), key2.c_str()) != 0;
#endif
}

template<>
inline bool CHashCompare< char* >::operator()(char* const & key1, char* const & key2) const
{
#ifdef __GNUC__
    return strcmp(key1, key2) == 0;
#else
    return strcmp(key1, key2) != 0;
#endif
}

template<>
inline bool CHashCompare< int8 >::operator()(const int8& key1, const int8& key2) const
{
#ifdef __GNUC__
    return key1 == key2;
#else
    return key1 != key2;
#endif
}

template<>
inline bool CHashCompare< uint8 >::operator()(const uint8& key1, const uint8& key2) const
{
#ifdef __GNUC__
    return key1 == key2;
#else
    return key1 != key2;
#endif
}

template<>
inline bool CHashCompare< int16 >::operator()(const int16& key1, const int16& key2) const
{
#ifdef __GNUC__
    return key1 == key2;
#else
    return key1 != key2;
#endif
}

template<>
inline bool CHashCompare< uint16 >::operator()(const uint16& key1, const uint16& key2) const
{
#ifdef __GNUC__
    return key1 == key2;
#else
    return key1 != key2;
#endif
}

template<>
inline bool CHashCompare< int32 >::operator()(const int32& key1, const int32& key2) const
{
#ifdef __GNUC__
    return key1 == key2;
#else
    return key1 != key2;
#endif
}

template<>
inline bool CHashCompare< uint32 >::operator()(const uint32& key1, const uint32& key2) const
{
#ifdef __GNUC__
    return key1 == key2;
#else
    return key1 != key2;
#endif
}

template<>
inline bool CHashCompare< int64 >::operator()(const int64& key1, const int64& key2) const
{
#ifdef __GNUC__
    return key1 == key2;
#else
    return key1 != key2;
#endif
}

template<>
inline bool CHashCompare< uint64 >::operator()(const uint64& key1, const uint64& key2) const
{
#ifdef __GNUC__
    return key1 == key2;
#else
    return key1 != key2;
#endif
}

//////////////////////////////////////////////////////////////////////////
/// @brief HashMap 对windows/linux统一接口
//////////////////////////////////////////////////////////////////////////
template<class KEY, class VALUE>
class CHashMap
{
public:
    class CCallback
    {
    public:
        virtual bool exec(const KEY& _key, VALUE& _value) = 0;
    };

private:

#ifdef __GNUC__
    //typedef __gnu_cxx::hash_map<KEY, VALUE, __gnu_cxx::hash<KEY>, CHashCompare<KEY> > HashMapType;
    typedef std::unordered_map<KEY, VALUE, __gnu_cxx::hash<KEY>, CHashCompare<KEY> > HashMapType;
#else
    //typedef stdext::hash_map<KEY, VALUE, stdext::hash_compare<KEY, CHashCompare<KEY> > > HashMapType;
    typedef std::unordered_map<KEY, VALUE, stdext::hash_compare<KEY, CHashCompare<KEY> > > HashMapType;
#endif

public:
    typedef typename HashMapType::iterator iterator;
    typedef typename HashMapType::const_iterator const_iterator;

private:
    HashMapType m_hashMap;

public:
    iterator begin()
    {
        return m_hashMap.begin();
    }

    const_iterator begin() const
    {
        return m_hashMap.begin();
    }

    iterator end()
    {
        return m_hashMap.end();
    }

    const_iterator end() const
    {
        return m_hashMap.end();
    }

    iterator erase(iterator it)
    {
#if defined(_WIN32)
        return m_hashMap.erase(it);
#elif defined(_LINUX)
        iterator it1 = it;
        ++it1;
        m_hashMap.erase(it);
        return it1;
#endif

    }

    VALUE& getValue(const KEY& key)
    {
        return m_hashMap[key];
    }

    bool getValue(const KEY& key, VALUE& value) const
    {
        const_iterator it = m_hashMap.find(key);
        if(it == m_hashMap.end())
        {
            return false;
        }
        value = it->second;
        return true;
    }

    void setValue(const KEY& key, const VALUE& value)
    {
        m_hashMap[key] = value;
    }

    bool remove(const KEY& key)
    {
        iterator it = m_hashMap.find(key);
        if(it == m_hashMap.end())
        {
            return false;
        }
        m_hashMap.erase(it);
        return true;
    }

    iterator find(const KEY& key)
    {
        return m_hashMap.find(key);
    }

    const_iterator find(const KEY& key) const
    {
        return m_hashMap.find(key);
    }

    void clear()
    {
        m_hashMap.clear();
    }

    unsigned int size() const
    {
        return (unsigned int)m_hashMap.size();
    }

    void traversal(CCallback& cb)
    {
        for(iterator it = m_hashMap.begin(); it != m_hashMap.end(); ++it)
        {
            if(!cb.exec(it->first, it->second))
            {
                break;
            }
        }
    }

    void traversal(CFunction<bool, const KEY&, VALUE&>& fun)
    {
        for(iterator it = m_hashMap.begin(); it != m_hashMap.end(); ++it)
        {
            if(!fun(it->first, it->second))
            {
                break;
            }
        }
    }

    template<class P1>
    void traversal(CFunction<bool, const KEY&, VALUE&, P1>& fun, P1 p1)
    {
        for(iterator it = m_hashMap.begin(); it != m_hashMap.end(); ++it)
        {
            if(!fun(it->first, it->second, p1))
            {
                break;
            }
        }
    }
    template<class P1, class P2>
    void traversal(CFunction<bool, const KEY&, VALUE&, P1, P2>& fun, P1 p1, P2 p2)
    {
        for(iterator it = m_hashMap.begin(); it != m_hashMap.end(); ++it)
        {
            if(!fun(it->first, it->second, p1, p2))
            {
                break;
            }
        }
    }

    template<class P1, class P2, class P3>
    void traversal(CFunction<bool, const KEY&, VALUE&, P1, P2, P3>& fun, P1 p1, P2 p2, P3 p3)
    {
        for(iterator it = m_hashMap.begin(); it != m_hashMap.end(); ++it)
        {
            if(!fun(it->first, it->second, p1, p2, p3))
            {
                break;
            }
        }
    }

    template<class P1, class P2, class P3, class P4>
    void traversal(CFunction<bool, const KEY&, VALUE&, P1, P2, P3, P4>& fun, P1 p1, P2 p2, P3 p3, P4 p4)
    {
        for(iterator it = m_hashMap.begin(); it != m_hashMap.end(); ++it)
        {
            if(!fun(it->first, it->second, p1, p2, p3, p4))
            {
                break;
            }
        }
    }

    template<class P1, class P2, class P3, class P4, class P5>
    void traversal(CFunction<bool, const KEY&, VALUE&, P1, P2, P3, P4, P5>& fun, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
    {
        for(iterator it = m_hashMap.begin(); it != m_hashMap.end(); ++it)
        {
            if(!fun(it->first, it->second, p1, p2, p3, p4, p5))
            {
                break;
            }
        }
    }
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_HASHMAP_H__
