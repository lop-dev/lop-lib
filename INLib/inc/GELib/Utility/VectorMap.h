//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/VectorMap.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_VECTORMAP_H__
#define __GELIB_UTILITY_VECTORMAP_H__

#include <vector>

namespace GELib
{


template<typename KeyType, typename ValueType>
class VectorMap
{
public:

	class Pair
	{
	public:
		Pair()
		{
		}

		Pair(const KeyType &key, const ValueType &value) : m_Key(key), m_Value(value)
		{
		}

		KeyType m_Key;
		ValueType m_Value;
	};

	// if key exist, old value is replaced
	// return insert index
	int Insert(const KeyType &key, const ValueType &value)
	{
		int i0 = 0;
		int i1 = (int)m_Vector.size() - 1;
		while (i1 >= i0)
		{
			int i = (i0 + i1) / 2;
			Pair &p = m_Vector[i];
			if (key < p.m_Key)
				i1 = i - 1;
			else if (p.m_Key < key)
				i0 = i + 1;
			else
			{
				p.m_Value = value;
				return i;
			}
		}

		m_Vector.insert(m_Vector.begin() + i0, Pair(key, value));
		return i0;
	}

	// return remove index
	int Remove(const KeyType &key)
	{
		int i = GetIndex(key);
		if (i >= 0)
			RemoveByIndex(i);
		return i;
	}

	void RemoveByIndex(int index)
	{
		m_Vector.erase(m_Vector.begin() + index);
	}

	// return index
	int Get(const KeyType &key, ValueType &value) const
	{
		int i = GetIndex(key);
		if (i >= 0)
			value = m_Vector[i].m_Value;
		return i;
	}

	ValueType* Get(const KeyType &key)
	{
		int i = GetIndex(key);
		if (i >= 0)
			return &m_Vector[i].m_Value;
		return 0;
	}

	ValueType& GetByIndex(int index)
	{
		return m_Vector[index].m_Value;
	}

	const ValueType& GetByIndex(int index) const
	{
		return m_Vector[index].m_Value;
	}

	const KeyType& GetKeyByIndex(int index) const
	{
		return m_Vector[index].m_Key;
	}

	int GetIndex(const KeyType &key) const
	{
		int i0 = 0;
		int i1 = (int)m_Vector.size() - 1;
		while (i1 >= i0)
		{
			int i = (i0 + i1) / 2;
			const Pair &p = m_Vector[i];
			if (key < p.m_Key)
				i1 = i - 1;
			else if (p.m_Key < key)
				i0 = i + 1;
			else
				return i;
		}

		return -1;
	}

	int GetSize() const
	{
		return (int)m_Vector.size();
	}

	void Clear()
	{
		m_Vector.clear();
	}

	bool IsEmpty() const
	{
		return m_Vector.empty();
	}

	void Reserve(int s)
	{
		m_Vector.reserve(s);
	}

private:
	std::vector<Pair> m_Vector;
};


}//GELib


#endif//__GELIB_UTILITY_VECTORMAP_H__
