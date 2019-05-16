//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/CircularBuffer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_CIRCULARBUFFER_H__
#define __GELIB_UTILITY_CIRCULARBUFFER_H__

#include <cassert>
#include <memory>

namespace GELib
{


template<class T> class CircularBuffer
{
public:
	CircularBuffer()
	{
		m_pBuffer = 0;
		m_iCapacity = 0;
		m_iBegin = m_iEnd = 0;
	}

	~CircularBuffer()
	{
		Clear();
	}

	void Clear()
	{
		// call destructor, delete buffer
		for (int i=0; i<m_iCapacity; ++i)
			m_pBuffer[i].~T();
		delete[] (char*)m_pBuffer;
		m_pBuffer = 0;
		m_iCapacity = 0;
		m_iBegin = m_iEnd = 0;
	}

	void Reserve(int size)
	{
		if (size < m_iCapacity)
			return;

		int cap;
		if (size < 16)
		{
			cap = 16;
		}
		else
		{
			cap = m_iCapacity + m_iCapacity / 2;
			if (size >= cap)
				cap = size + 1;
		}

		// allocate new buffer
		T *buf = (T*)new char[sizeof(T) * cap];

		// copy elements to new buffer
		if (m_iCapacity > 0)
		{
			int s0 = m_iCapacity - m_iBegin;
			memcpy(buf, m_pBuffer + m_iBegin, sizeof(T) * s0);
			if (m_iBegin > 0)
				memcpy(buf + s0, m_pBuffer, sizeof(T) * m_iBegin);

			// delete old buffer
			delete[] (char*)m_pBuffer;
		}

		// initialize new elements
		for (int i=m_iCapacity; i<cap; ++i)
			new(buf + i) T;

		int osize = GetSize();
		m_pBuffer = buf;
		m_iCapacity = cap;
		m_iBegin = 0;
		m_iEnd = osize;
	}

	void Resize(int size)
	{
		Reserve(size);
		m_iEnd = (m_iBegin + size) % m_iCapacity;
	}

	int GetSize() const
	{
		if (m_iEnd >= m_iBegin)
			return m_iEnd - m_iBegin;
		else
			return m_iEnd + m_iCapacity - m_iBegin;
	}

	bool IsEmpty() const
	{
		return m_iBegin == m_iEnd;
	}

	void PushFront(const T &v)
	{
		Reserve(GetSize() + 1);
		if (--m_iBegin < 0)
			m_iBegin += m_iCapacity;
		m_pBuffer[m_iBegin] = v;
	}

	void PushBack(const T &v)
	{
		Reserve(GetSize() + 1);
		m_pBuffer[m_iEnd] = v;
		m_iEnd = (m_iEnd + 1) % m_iCapacity;
	}

	void PopFront()
	{
		assert(!IsEmpty());
		m_iBegin = (m_iBegin + 1) % m_iCapacity;
	}

	void PopBack()
	{
		assert(!IsEmpty());
		if (--m_iEnd < 0)
			m_iEnd += m_iCapacity;
	}

	T& GetFront() const
	{
		assert(!IsEmpty());
		return m_pBuffer[m_iBegin];
	}

	T& GetBack() const
	{
		assert(!IsEmpty());
		int e = m_iEnd - 1;
		if (e < 0)
			e += m_iCapacity;
		return m_pBuffer[e];
	}

	T& Get(int index) const
	{
		assert(index >= 0 && index < GetSize());
		return m_pBuffer[(m_iBegin + index) % m_iCapacity];
	}

	T& operator[](int index) const
	{
		return Get(index);
	}

private:

	T *m_pBuffer;
	int m_iCapacity;
	int m_iBegin;
	int m_iEnd;
};



}//GELib

#endif//__GELIB_UTILITY_CIRCULARBUFFER_H__
