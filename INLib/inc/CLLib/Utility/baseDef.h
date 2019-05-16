//////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   CLLib\utility\baseDef.h
//    author:     League of Perfect
///   @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_UTILITY_BASEDEF_H__
#define __CLLIB_UTILITY_BASEDEF_H__

#ifndef CLLIB_SAFE_DELETE
#define CLLIB_SAFE_DELETE(x)     if((x)!=NULL){delete (x); (x) = NULL;} 
#endif // CLLIB_SAFE_DELETE

#ifndef CLLIB_SAFE_DELETE_ARRAY
#define CLLIB_SAFE_DELETE_ARRAY(x)  if((x)!=NULL){delete [] x; x = NULL;}
#endif // CLLIB_SAFE_DELETE_ARRAY  

#endif //__CLLIB_UTILITY_BASEDEF_H__



