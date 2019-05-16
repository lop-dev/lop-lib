//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/PhysicsObjectAttachable.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_PHYSICSOBJECTATTACHABLE_H__
#define __GELIB_EXT_PHYSICSOBJECTATTACHABLE_H__

#include "IAttachable.h"

namespace GELib
{


class CPhysicsObject;


class CPhysicsObjectAttachable : public IAttachable
{
public:
    CPhysicsObjectAttachable();
    virtual ~CPhysicsObjectAttachable();

	void SetPhysicsObject(CPhysicsObject *pobj);

    virtual int GetAttachPoints(std::vector<GELib::IAttachPoint*> &points);
    virtual IAttachPoint* GetAttachPoint(const char *name);

    class CPhysicsObjectAttachPoint : public IAttachPoint
    {
    public:
        virtual const char * GetName();
        virtual void GetMatrix(GELib::CMatrix &matrix);
        int m_iElementIndex;
        CPhysicsObjectAttachable *m_pAttachable;
    };

    GELib::CPhysicsObject *m_pPhysicsObject;
    std::vector<CPhysicsObjectAttachPoint> m_AttachPoints;
};


}//GELib

#endif//__GELIB_EXT_PHYSICSOBJECTATTACHABLE_H__
