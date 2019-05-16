//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/InterpolationValue.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_INTERPOLATIONVALUE_H__
#define __GELIB_MATH_INTERPOLATIONVALUE_H__

#include <vector>
#include <string>
#include <GELib/Math/GeMath.h>


namespace GELib
{

enum EInterpolationMode
{
	INTERPOLATION_NONE,
	INTERPOLATION_LINEAR,
	INTERPOLATION_SMOOTH,
	INTERPOLATION_SMOOTHER,
	INTERPOLATION_SQUARE,
	INTERPOLATION_INVSQUARE,
};

enum ECurveMode
{
	CURVE_NONE,
	CURVE_CATMULLROM
};

template<typename T> class CInterpolationValue
{
public:

    class Key
    {
    public:
        Key() {}
        Key(float time, const T &value, EInterpolationMode mode, ECurveMode curve) { m_fTime = time; m_Value = value; m_eMode = mode; m_eCurve = curve; }

        float m_fTime;
        T m_Value;
		EInterpolationMode m_eMode;
		ECurveMode m_eCurve;
    };

    int AddKey(float time, const T &value, EInterpolationMode mode, ECurveMode curve)
	{
		typename std::vector<Key>::iterator it = m_Keys.begin();
		typename std::vector<Key>::iterator end = m_Keys.end();
		int index = 0;
		while (it != end)
		{
			Key &key = (*it);
			if (key.m_fTime > time)
				break;
			++it;
			++index;
		}

		m_Keys.insert(it, Key(time, value, mode, curve));
		return index;
	}

    void RemoveKey(int index)
	{
		m_Keys.erase(m_Keys.begin() + index);
	}

	void RemoveAll()
	{
		m_Keys.clear();
	}

	void SetKey(int index, const Key &key)
	{
		m_Keys[index] = key;
	}

    void SetKey(int index, float time, const T &value, EInterpolationMode mode, ECurveMode curve)
	{
		Key &k = m_Keys[index];
		k.m_fTime = time;
		k.m_Value = value;
		k.m_eMode = mode;
		k.m_eCurve = curve;
	}

	Key& GetKey(int index)
	{
		return m_Keys[index];
	}

	const Key& GetKey(int index) const
	{
		return m_Keys[index];
	}

	void GetKey(int index, Key &key) const
	{
		key = m_Keys[index];
	}

    void GetKey(int index, float &time, T &value, EInterpolationMode &mode, ECurveMode &curve) const
	{
		const Key &k = m_Keys[index];
		time = k.m_fTime;
		value = k.m_Value;
		mode = k.m_eMode;
		curve = k.m_eCurve;
	}

    int GetKeyIndexByTime(float time, float epsilon=0) const    // return -1 if no match key
	{
		int s = (int)m_Keys.size();
		for (int i=0; i<s; ++i)
		{
			if (fabs(m_Keys[i].m_fTime - time) <= epsilon)
				return i;
		}
		return -1;
	}

    int ChangeKeyTime(float oldTime, float newTime, float epsilon=0)
	{
		int index = GetKeyIndexByTime(oldTime, epsilon);
		if (index >= 0)
		{
			Key k = m_Keys[index];
			RemoveKey(index);
			return AddKey(newTime, k.m_Value, k.m_eMode, k.m_eCurve);
		}
		return -1;
	}

    void SetNumKeys(int n)
	{
	    m_Keys.resize(n);
	}

    int GetNumKeys() const
	{
	    return (int)m_Keys.size();
	}

    void GetTimeRange(float &beginTime, float &endTime)
	{
		if (m_Keys.empty())
		{
			beginTime = endTime = 0;
			return;
		}

		beginTime = m_Keys.front().m_fTime;
		endTime = m_Keys.back().m_fTime;
	}

	void SetInterpolationMode(EInterpolationMode mode)
	{
		size_t s = m_Keys.size();
		for (size_t i=0; i<s; ++i)
			m_Keys[i].m_eMode = mode;
	}

	EInterpolationMode GetInterpolationMode(float time)
	{
		if (m_Keys.empty())
		{
			return INTERPOLATION_NONE;
		}

		typename std::vector<Key>::iterator it;
        it = m_Keys.begin();
		if (time <= (*it).m_fTime)
		{
			return (*it).m_eMode;
		}

		if (time >= m_Keys.back().m_fTime)
		{
			return m_Keys.back().m_eMode;
		}

		while (true)
		{
			Key &key0 = *it;
			++it;
			Key &key1 = *it;

			if (time < key1.m_fTime)
			{
				return key0.m_eMode;
			}
		}
	}

	void SetCurveMode(ECurveMode curve)
	{
		size_t s = m_Keys.size();
		for (size_t i=0; i<s; ++i)
			m_Keys[i].m_eCurve = curve;
	}

	ECurveMode GetCurveMode(float time)
	{
		if (m_Keys.empty())
		{
			return CURVE_NONE;
		}

		typename std::vector<Key>::iterator it;
        it = m_Keys.begin();
		if (time <= (*it).m_fTime)
		{
			return (*it).m_eCurve;
		}

		if (time >= m_Keys.back().m_fTime)
		{
			return m_Keys.back().m_eCurve;
		}

		while (true)
		{
			Key &key0 = *it;
			++it;
			Key &key1 = *it;

			if (time < key1.m_fTime)
			{
				return key0.m_eCurve;
			}
		}
	}

protected:
    std::vector<Key> m_Keys;
};

template<typename T> class CInterpolationValue2		// always INTERPOLATION_NONE
{
public:

    class Key
    {
    public:
        Key() {}
        Key(float time, const T &value) { m_fTime = time; m_Value = value; }
        float m_fTime;
        T m_Value;
    };

    int AddKey(float time, const T &value)
	{
		typename std::vector<Key>::iterator it = m_Keys.begin();
		typename std::vector<Key>::iterator end = m_Keys.end();
		int index = 0;
		while (it != end)
		{
			Key &key = (*it);
			if (key.m_fTime > time)
				break;
			++it;
			++index;
		}

		m_Keys.insert(it, Key(time, value));
		return index;
	}

    void RemoveKey(int index)
	{
		m_Keys.erase(m_Keys.begin() + index);
	}

	void RemoveAll()
	{
		m_Keys.clear();
	}

	void SetKey(int index, const Key &key)
	{
		m_Keys[index] = key;
	}

    void SetKey(int index, float time, const T &value)
	{
		Key &k = m_Keys[index];
		k.m_fTime = time;
		k.m_Value = value;
	}

	Key& GetKey(int index)
	{
		return m_Keys[index];
	}

	const Key& GetKey(int index) const
	{
		return m_Keys[index];
	}

	void GetKey(int index, Key &key) const
	{
		key = m_Keys[index];
	}

    void GetKey(int index, float &time, T &value) const
	{
		const Key &k = m_Keys[index];
		time = k.m_fTime;
		value = k.m_Value;
	}

    int GetKeyIndexByTime(float time, float epsilon=0) const    // return -1 if no match key
	{
		int s = (int)m_Keys.size();
		for (int i=0; i<s; ++i)
		{
			if (fabs(m_Keys[i].m_fTime - time) <= epsilon)
				return i;
		}
		return -1;
	}

    int ChangeKeyTime(float oldTime, float newTime, float epsilon=0)
	{
		int index = GetKeyIndexByTime(oldTime, epsilon);
		if (index >= 0)
		{
			T value = m_Keys[index].m_Value;
			RemoveKey(index);
			return AddKey(newTime, value);
		}
		return -1;
	}

    void SetNumKeys(int n)
	{
	    m_Keys.resize(n);
	}

    int GetNumKeys() const
	{
	    return (int)m_Keys.size();
	}

    void GetTimeRange(float &beginTime, float &endTime)
	{
		if (m_Keys.empty())
		{
			beginTime = endTime = 0;
			return;
		}

		beginTime = m_Keys.front().m_fTime;
		endTime = m_Keys.back().m_fTime;
	}


protected:

    std::vector<Key> m_Keys;
};

class CInterpolationFloat : public CInterpolationValue<float>
{
public:

    void GetValue(float time, float &value);
};

class CInterpolationVector2 : public CInterpolationValue<GELib::CVector2>
{
public:

    void GetValue(float time, CVector2 &value);
};

class CInterpolationVector3 : public CInterpolationValue<GELib::CVector3>
{
public:

    void GetValue(float time, CVector3 &value);
    void GetValues(float time, CVector3 &value0, CVector3 &value1, float &a);
};

class CInterpolationVector4 : public CInterpolationValue<GELib::CVector4>
{
public:

    void GetValue(float time, CVector4 &value);
    void GetValues(float time, CVector4 &value0, CVector4 &value1, float &a);
};

class CInterpolationColor : public CInterpolationValue<GELib::CRGBAColor>
{
public:

    void GetValue(float time, CRGBAColor &value);
};

class CInterpolationInt : public CInterpolationValue<int>
{
public:

    void GetValue(float time, int &value);
};

class CInterpolationBool : public CInterpolationValue2<bool>
{
public:

    void GetValue(float time, bool &value);
	bool GetValue(float time);
};

class CInterpolationString : public CInterpolationValue2<std::string>
{
public:

    void GetValue(float time, std::string &value);
	const std::string& GetValue(float time);
};

}//GELib

#endif//__GELIB_MATH_INTERPOLATIONVALUE_H__
