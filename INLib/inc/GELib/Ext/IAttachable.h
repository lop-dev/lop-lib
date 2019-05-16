//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/IAttachable.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_IATTACHABLE_H__
#define __GELIB_EXT_IATTACHABLE_H__


#include <GELib/Math/GeMath.h>
#include <string>
#include <vector>


namespace GELib
{

// attach interface used by fx

class IAttachPoint
{
public:
    IAttachPoint() {}
    virtual ~IAttachPoint() {}

    virtual const char * GetName() = 0;
    virtual void GetMatrix(CMatrix &matrix) = 0;
};

class IAttachable
{
public:
    IAttachable() {}
    virtual ~IAttachable() {}

    virtual int GetAttachPoints(std::vector<IAttachPoint*> &points) = 0;
    virtual IAttachPoint* GetAttachPoint(const char *name) = 0;
};



}//GELib

#endif//__GELIB_EXT_IATTACHABLE_H__
