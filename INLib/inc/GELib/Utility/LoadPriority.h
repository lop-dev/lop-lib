//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/LoadPriority.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_LOADPRIORITY_H__
#define __GELIB_UTILITY_LOADPRIORITY_H__


namespace GELib
{

enum ELoadPriority
{
    LOAD_IMMEDIATELY = 0,
    LOAD_BACKGROUND_LOW = 1,
    LOAD_BACKGROUND_NORMAL = 2,
    LOAD_BACKGROUND_HIGH = 3,
    LOAD_IMMEDIATELY_PARTIAL = 4,		// background loading internal flag, don't use
};


}//GELib

#endif//__GELIB_UTILITY_LOADPRIORITY_H__
