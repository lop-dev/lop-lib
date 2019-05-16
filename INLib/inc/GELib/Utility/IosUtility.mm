#include "IosUtility.h"
#include <mach/mach_time.h>
#include <string.h>
#include <UIKit/UIDevice.h>

EIosDevice IosGetDeviceType()
{
	if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad)
		return IOS_DEVICE_IPAD;
	else 
		return IOS_DEVICE_IPHONE;
}

static bool g_bTimeInfoInitialized = false;
static mach_timebase_info_data_t g_TimeInfo;

double IosGetMilliseconds()
{
	if (!g_bTimeInfoInitialized)
	{
		mach_timebase_info(&g_TimeInfo);
		g_bTimeInfoInitialized = true;
	}
	
	uint64_t t = mach_absolute_time();
	
	// convert to nanoseconds
	t *= g_TimeInfo.numer;
	t /= g_TimeInfo.denom;
	
	// return in milliseconds
	return (double)t / 1000000.0;		
}


static char g_ResourcePath[256];
static char g_DocumentPath[256];
static char g_CachePath[256];
static bool g_bPathInitialized = false;

static void InitializePath()
{	
	if (g_bPathInitialized)
		return;
	
	NSString *path = [[NSBundle mainBundle] resourcePath];
	[path getCString:g_ResourcePath maxLength:255 encoding:NSUTF8StringEncoding];
	strcat(g_ResourcePath, "/");
	
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    path = [paths objectAtIndex:0];
	[path getCString:g_DocumentPath maxLength:255 encoding:NSUTF8StringEncoding];
	strcat(g_DocumentPath, "/");
	
    paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    path = [paths objectAtIndex:0];
	[path getCString:g_CachePath maxLength:255 encoding:NSUTF8StringEncoding];
    strcat(g_CachePath, "/");
				
	g_bPathInitialized = true;
}

const char * IosGetResourcePath()
{
	InitializePath();
	return g_ResourcePath;
}

const char * IosGetDocumentPath()
{
	InitializePath();
	return g_DocumentPath;
}

const char * IosGetCachePath()
{
	InitializePath();
	return g_CachePath;
}

