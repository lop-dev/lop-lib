//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/RGBAColor.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_RGBACOLOR_H__
#define __GELIB_MATH_RGBACOLOR_H__

#include <GELib/GeConfig.h>

namespace GELib
{

class CRGBAColor
{
public:

	CRGBAColor() { r = g = b = a = 0; }

	CRGBAColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	CRGBAColor(unsigned char r, unsigned char g, unsigned char b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 255;
	}

	bool operator==(const CRGBAColor &r) const
	{
		return m_i == r.m_i;
	}

	bool operator!=(const CRGBAColor &r) const
	{
		return m_i != r.m_i;
	}

    union
    {
        struct
        {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        };
        unsigned char m_v[4];
		unsigned int m_i;
    };
};

}//GELib

#endif//__GELIB_MATH_RGBACOLOR_H__
