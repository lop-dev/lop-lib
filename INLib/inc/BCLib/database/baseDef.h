//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/baseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_BASEDEF_H__
#define __BCLIB_DATABASE_BASEDEF_H__

#include <BCLib/utility/baseDef.h>

#ifdef BCLIB_DATABASE_EXPORTS
#    define BCLIB_DATABASE_API _declspec(dllexport)
#elif defined(BCLIB_DATABASE_IMPORTS)
#    define BCLIB_DATABASE_API _declspec(dllimport)
#else
#    define BCLIB_DATABASE_API
#endif

namespace BCLib
{
    namespace Database
    {
        enum EDBActID
        {
            EDB_ACTID_UNKNOW,
            EDB_ACTID_INSERT,
            EDB_ACTID_SELECT,
            EDB_ACTID_UPDATE,
            EDB_ACTID_DELETE,
        };
    }
}

#endif//__BCLIB_DATABASE_BASEDEF_H__
