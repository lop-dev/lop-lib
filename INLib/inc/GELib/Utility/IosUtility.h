//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/IosUtility.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_IOSUTILITY_H__
#define __GELIB_UTILITY_IOSUTILITY_H__

#include <GELib/GeConfig.h>

enum EIosDevice
{
	IOS_DEVICE_IPHONE, IOS_DEVICE_IPAD
};

EIosDevice IosGetDeviceType();
double IosGetMilliseconds();
const char * IosGetResourcePath();
const char * IosGetDocumentPath();
const char * IosGetCachePath();

#endif//__GELIB_UTILITY_IOSUTILITY_H__
