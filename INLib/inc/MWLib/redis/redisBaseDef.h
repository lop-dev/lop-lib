//////////////////////////////////////////////////////////////////////
//  created:    2014/07/18
//  filename:   MWLib/redis/redisBaseDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_REDIS_REDIS_BASE_DEF_H__
#define __MWLIB_REDIS_REDIS_BASE_DEF_H__
#include <string>

#define MWLIB_REDIS_DEFAULT_UNIQUEID 1000000
namespace MWLib
{
	namespace Redis
	{

		typedef struct Redis_Connect_Info
		{
			std::string m_host;
			int m_port;
			std::string m_passwd;
			Redis_Connect_Info()
			{
				m_host = "";
				m_port = 0;
				m_passwd = "";
			}
			~Redis_Connect_Info()
			{

			}
		} REDIS_CONNECT_INFO;

		enum EREDIS_CONTEXT_TYPE
		{
			E_REDIS_SERVERTYPE_BEGIN = 0,
			E_REDIS_SERVERTYPE_LOGIC,
			E_REDIS_SERVERTYPE_CACHE,
			E_REDIS_SERVERTYPE_EXTERNAL,
			E_REDIS_SERVERTYPE_END,

		};

		enum EREDIS_ACCESS_RIGHT_TYPE
		{
			E_REDIS_READ_AND_WRITE = 0,
			E_REDIS_READ_ONLY,
		};
	} //Redis
} //MWLib
#endif //__MWLIB_REDIS_REDIS_BASE_DEF_H__