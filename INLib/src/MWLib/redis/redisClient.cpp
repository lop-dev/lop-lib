//////////////////////////////////////////////////////////////////////
//  created:    2014/07/18
//  filename:   MWLib/redis/redisClient.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <MWLib/redis/redisClient.h>
#include <BCLib/utility/string.h>
#include <BCLib/utility/convert.h>
#include <BCLib/utility/logFile.h>
#include <sstream>
#include <codecvt>

#define MWLIB_PROCESS_REPLY_ERROR    \
    if(m_redisReply == NULL)    \
    {    \
        this->disconnect(type);    \
        return false;    \
    }    \
    if(m_redisReply->type == REDIS_REPLY_ERROR)    \
    {    \
        freeReplyObject(m_redisReply);    \
        m_redisReply = NULL;    \
        return false;    \
    }    \
    freeReplyObject(m_redisReply);    \
    m_redisReply = NULL;    \
    return true;

#define MWLIB_PROCESS_REPLY_RETURN    \
    if(m_redisReply == NULL)    \
    {    \
        this->disconnect(type);    \
        return;    \
    }    \
    if(m_redisReply->type == REDIS_REPLY_ERROR)    \
    {    \
        freeReplyObject(m_redisReply);    \
        m_redisReply = NULL;    \
        return;    \
    }    \
    freeReplyObject(m_redisReply);    \
    m_redisReply = NULL;    \
    return;

namespace MWLib
{
	namespace Redis
	{

		// BCLIB_SINGLETON_DEFINE_WITH_LEVELID(CRedisClient, 100) // 原来是 SHLib::E_SINGLETON_LEVEL_REDIS 变量，临时这么处理

		CRedisClient::CRedisClient()
			:m_redisContext(NULL)
			, m_redisReply(NULL)
			, m_pRedLock(NULL)
			, m_eAccessRight(E_REDIS_READ_AND_WRITE)
			, m_eOnOff(E_REDIS_TURN_ON)
		{
			m_pRedLock = new CRedLock();
			m_redisContextMap.clear();
			for (BCLib::uint16 i = E_REDIS_SERVERTYPE_BEGIN + 1; i < E_REDIS_SERVERTYPE_END; i++)
			{
				REDIS_NODE node;
				m_redisContextMap[i] = node;
			}
		}

		CRedisClient::~CRedisClient()
		{
			disconnectAll();
			destoryTempList();
			destoryTempMap();
			destoryRedLock();
			m_redisContextMap.clear();
		}
		bool CRedisClient::isValid(EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "isValid exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "CRedisClient::isValid type= %d", type);
				return false;
			}
			if (it->second.m_redisContext == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "CRedisClient::isValid m_redisContext=NULL type= %d", type);
				return false;
			}
			return true;
		}

		bool CRedisClient::savePTBuf(std::string &key, BCLib::uint64 uniqueid, std::string & strPTbufName, const ::google::protobuf::MessageLite *pPtbuf, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "savePTBuf exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key.empty() == true || strPTbufName.empty() == true || pPtbuf == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			//char writebuf[REDIS_READER_MAX_BUF] = { 0 };
			std::string str;
			pPtbuf->SerializeToString(&str);

			bool ret = setString(key.c_str(), uniqueid, strPTbufName.c_str(), str,  type);
			return ret;
		}
		BCLib::uint32 CRedisClient::loadPTBuf(std::string &key, BCLib::uint64 uniqueid, std::string & strPTbufName, ::google::protobuf::MessageLite *pPtbuf, BCLib::uint32 readBufSize, EREDIS_CONTEXT_TYPE type)
		{
			if (key.empty() == true || strPTbufName.empty() == true || pPtbuf == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			//char readbuf[REDIS_READER_MAX_BUF] = { 0 };
			//char *readbuf = new char[readBufSize];
			//pPtbuf->SerializeToArray(readbuf, REDIS_READER_MAX_BUF);
			std::string str = "";
			bool bRet = getString(key.c_str(), uniqueid, strPTbufName.c_str(), str, type);
			if (!bRet)
			{
				return 0;
			}
			BCLib::uint32 ret = str.length();
			pPtbuf->ParseFromString(str);
			//BCLIB_SAFE_DELETE_ARRAY(readbuf);
			return ret;
		}
		void CRedisClient::setInfo(std::string& host, int port, std::string& passwd, EREDIS_CONTEXT_TYPE type)
		{
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it != m_redisContextMap.end())
			{
				it->second.m_host = host;
				it->second.m_port = port;
				it->second.m_passwd = passwd;
			}
			return;
		}

		bool CRedisClient::init(EREDIS_ACCESS_RIGHT_TYPE type)
		{
			struct timeval timeout = { 1, 500000 }; // 1.5 seconds
			disconnectAll();
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.begin();
			while (it != m_redisContextMap.end())
			{
				if (it->second.m_host == "")
				{
					++it;
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis init 结束 m_redisContextMap.m_host = [空]!!!");
					continue;
				}
				bool  connectflag = connectWithTimeout(it->second.m_host, it->second.m_port, timeout, (EREDIS_CONTEXT_TYPE)it->first);
				if (!connectflag)
				{
					++it;
					continue;//return false;
				}

				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis Connect 连接 %s %d 成功", it->second.m_host.c_str(), it->second.m_port);

				std::string auth_cmd = "AUTH " + it->second.m_passwd;
				bool cmdflag = exec(auth_cmd.c_str(), (EREDIS_CONTEXT_TYPE)it->first);
				if (!cmdflag)
				{
					++it;
					continue;//return false;
				}

				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis 认证成功!!!");
				it++;
			}
			BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis init 结束 m_redisContextMap.m_host = [xxx]!!!");
			m_eAccessRight = type;
			return true;
		}

		bool  CRedisClient::exec(const char* cmd, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (cmd == NULL)
			{
				return false;
			}
			m_redisReply = NULL;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd);

			MWLIB_PROCESS_REPLY_ERROR
		}

		bool CRedisClient::clear(EREDIS_CONTEXT_TYPE type)
		{
			BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis Clear");
			return exec("FLUSHALL", type);
		}
		bool CRedisClient::checkConnect(EREDIS_CONTEXT_TYPE type)
		{
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis checkConnect error m_redisContextMap has no this context!!! type = %d", type);
				return false;
			}
			if (it->second.m_redisContext == NULL)
			{
				return reconnect(type);
			}
			return true;
		}
		bool CRedisClient::reconnect(EREDIS_CONTEXT_TYPE type)
		{
			struct timeval timeout = { 0, 200000 }; // 1.5 seconds
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			if (type == E_REDIS_SERVERTYPE_CACHE)
			{
				return false; //cache不发起重连
			}
			if (it->second.m_host == "")
			{		
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis reconnect error m_redisContextMap.m_host = [空]!!! type = %d", type);
				return false;
			}
			bool  connectflag = connectWithTimeout(it->second.m_host, it->second.m_port, timeout, (EREDIS_CONTEXT_TYPE)it->first);
			if (!connectflag)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis reconnect error m_redisContextMap.m_host = [%s] port[%d]!!! type = %d", it->second.m_host.c_str(), it->second.m_port, type);
				return false;
			}

			BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis Connect 连接 %s %d 成功", it->second.m_host.c_str(), it->second.m_port);

			std::string auth_cmd = "AUTH " + it->second.m_passwd;
			bool cmdflag = exec(auth_cmd.c_str(), (EREDIS_CONTEXT_TYPE)it->first);
			if (!cmdflag)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis reconnect error AUTH m_redisContextMap.m_host = [%s] port[%d] passwd[%s] type = %d", it->second.m_host.c_str(), it->second.m_port, it->second.m_passwd.c_str(), type);
				return false;
			}
			BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis 认证成功!!!");			
			return true;
		}
		bool CRedisClient::connect(std::string host, int port, EREDIS_CONTEXT_TYPE type)
		{
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}

			m_redisContext = redisConnect(host.c_str(), port);
			if (m_redisContext == NULL)
			{
				return false;
			}
			else if (m_redisContext->err)
			{
				printf("connect error: %s\n", m_redisContext->errstr);
				return false;
			}
			if (m_pRedLock == NULL)
			{
				return true;
			}
			m_pRedLock->AddServerContext(m_redisContext, (BCLib::uint16)type, this);
			it->second.m_redisContext = m_redisContext;
			m_redisContext = NULL;
			return true;
		}

		bool CRedisClient::connectWithTimeout(std::string host, int port, const struct timeval tv, EREDIS_CONTEXT_TYPE type)
		{
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = redisConnectWithTimeout(host.c_str(), port, tv);
			if (m_redisContext == NULL || m_redisContext->err != 0)
			{
				return false;
			}
			if (m_pRedLock == NULL)
			{
				return true;
			}
			m_pRedLock->AddServerContext(m_redisContext, (BCLib::uint16)type, this);
			it->second.m_redisContext = m_redisContext;
			m_redisContext = NULL;
			//setCommandTimeout(tv, type);
			return true;
		}

		void  CRedisClient::disconnect(EREDIS_CONTEXT_TYPE type)
		{
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return;
			}
			m_redisContext = it->second.m_redisContext;
			it->second.m_redisContext = NULL;

			if (m_redisContext == NULL)
			{
				return;
			}

			if (m_pRedLock != NULL)
			{
				m_pRedLock->delServerContext(m_redisContext, (BCLib::uint16) type);
			}

			if (m_redisReply)
			{
				freeReplyObject(m_redisReply);
				m_redisReply = NULL;
			}

			redisFree(m_redisContext);
			m_redisContext = NULL;

			//m_redisContextMap.erase(it);
		}
		void  CRedisClient::disconnectAll()
		{
			for (BCLib::uint16 i = E_REDIS_SERVERTYPE_BEGIN; i < E_REDIS_SERVERTYPE_END; i++)
			{
				disconnect((EREDIS_CONTEXT_TYPE)i);
			}
		}
		bool CRedisClient::setCommandTimeout(const struct timeval tv, EREDIS_CONTEXT_TYPE type)
		{
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			//struct timeval tv = {0};
			//tv.tv_sec = seconds;
			if (m_redisContext)
			{
				int ret = redisSetTimeout(m_redisContext, tv);
				if (ret == 0) return true;
			}
			return false;
		}

		bool CRedisClient::checkStatus(EREDIS_CONTEXT_TYPE type)
		{
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "ping");
			if (m_redisReply == NULL)
			{
				this->disconnect(type);
				return false;
			}

			BCLib::Utility::CStringA replyStr = (std::string)m_redisReply->str;
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			if (!replyStr.compareNoCase("PONG"))
			{
				return false;
			}

			return true;
		}
		bool CRedisClient::setNxString(const char *key, const char *value, EREDIS_CONTEXT_TYPE type )
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "setString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}

			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "SETNX %s %s", key, value);
			if (m_redisReply == NULL)
			{
				this->disconnect(type);
				return false;
			}
			if (m_redisReply->type == REDIS_REPLY_ERROR)
			{    
				freeReplyObject(m_redisReply);
				m_redisReply = NULL;
				return false;
			} 
			if (m_redisReply->type == REDIS_REPLY_INTEGER)
			{
				BCLib::uint32 ret = m_redisReply->integer;
				if (ret == 1)
				{
					freeReplyObject(m_redisReply);
					m_redisReply = NULL;
					return true;
				}
				else
				{
					freeReplyObject(m_redisReply);
					m_redisReply = NULL;
					return false;
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::setNxString(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "hsetString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return setNxString(strKey, value, type);
		}
		bool CRedisClient::setString(const char* key, const char* value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "setString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "SET %s %s", key, value);

			MWLIB_PROCESS_REPLY_ERROR
		}

		std::string CRedisClient::getString(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return "";
			}
			if (!checkConnect(type))
			{
				return "";
			}
			std::string retStr = "";
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return retStr;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return retStr;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "GET %s", key);
			if (m_redisReply == NULL)
			{
				this->disconnect(type);
				return retStr;
			}

			if (m_redisReply->type == REDIS_REPLY_STRING)
			{
				//retStr = m_redisReply->str;
				retStr.append(m_redisReply->str, m_redisReply->len);
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return retStr;
		}

		bool CRedisClient::setString(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "setString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "SET %s:[%llu]:%s %s", key, uniqueid, subkey, value);

			MWLIB_PROCESS_REPLY_ERROR
		}

		std::string CRedisClient::getString(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return "";
			}
			if (!checkConnect(type))
			{
				return "";
			}
			std::string retStr = "";
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return retStr;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return retStr;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "GET %s:[%llu]:%s", key, uniqueid, subkey);
			if (m_redisReply == NULL || subkey == NULL)
			{
				this->disconnect(type);
				return retStr;
			}

			if (m_redisReply->type == REDIS_REPLY_STRING)
			{
				//retStr = m_redisReply->str;
				retStr.append(m_redisReply->str, m_redisReply->len);
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return retStr;
		}
		bool CRedisClient::getString(const char *key, std::string &str, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::string retStr = "";
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "GET %s", key);
			if (m_redisReply == NULL)
			{
				this->disconnect(type);
				return false;
			}

			if (m_redisReply->type == REDIS_REPLY_STRING)
			{
				//retStr = m_redisReply->str;
				str.append(m_redisReply->str, m_redisReply->len);
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return true;
		}
		bool CRedisClient::getString(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::string &str, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "hsetString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return getString(strKey, str, type);
		}
		bool CRedisClient::setString(const char *key, std::string &value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "hsetString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			return setBin(key, value.c_str(), value.size(), type);
		}
		bool CRedisClient::setString(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::string &value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "hsetString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return setString(strKey, value, type);
		}
		bool CRedisClient::setUint64(const char* key, BCLib::uint64 value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "setUint64 exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "SET %s %llu", key, value);

			MWLIB_PROCESS_REPLY_ERROR
		}

		BCLib::uint64 CRedisClient::getUint64(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			BCLib::uint64 retUint64 = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return retUint64;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return retUint64;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "GET %s", key);

			if (m_redisReply == NULL)
			{
				this->disconnect(type);
				return retUint64;
			}

			if (m_redisReply->type == REDIS_REPLY_STRING)
			{
				BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
				retUint64 = BCLib::Utility::CConvert::toUint64(str);
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;

			return retUint64;
		}

		bool CRedisClient::setUint64(const char* key, BCLib::uint64 uniqueid, const char* subkey, BCLib::uint64 value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "setUint64 exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "SET %s:[%llu]:%s %llu", key, uniqueid, subkey, value);

			MWLIB_PROCESS_REPLY_ERROR
		}

		BCLib::uint64 CRedisClient::getUint64(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			BCLib::uint64 retUint64 = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return retUint64;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return retUint64;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "GET %s:[%llu]:%s", key, uniqueid, subkey);

			if (m_redisReply == NULL)
			{
				this->disconnect(type);
				return retUint64;
			}

			if (m_redisReply->type == REDIS_REPLY_STRING)
			{
				BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
				retUint64 = BCLib::Utility::CConvert::toUint64(str);
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;

			return retUint64;
		}

		/* Format a command according to the Redis protocol. This function
		* takes a format similar to printf:
		*
		* %s represents a C null terminated string you want to interpolate
		* %b represents a binary safe string
		*
		* When using %b you need to provide both the pointer to the string
		* and the length in bytes as a size_t. Examples:
		*
		* len = redisFormatCommand(target, "GET %s", mykey);
		* len = redisFormatCommand(target, "SET %s %b", mykey, myval, myvallen);
		*/
		bool CRedisClient::setBin(const char* key, const char* value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "setBin exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "set  %s %b", key, value, len);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate setBin m_redisReply = null error set key= %s len=%d error", key, len);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate setBin error set key= %s len=%d error", key, len);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}

		BCLib::uint32 CRedisClient::getBin(const char* key, char* value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			BCLib::uint32 retLen = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return retLen;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return retLen;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "GET %s", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate getBin m_redisReply = null error get key= %s error", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate getBin error get key= %s error", key);
				}
			}
			if (m_redisReply == NULL)
			{
				this->disconnect(type);
				return retLen;
			}

			if (m_redisReply->type == REDIS_REPLY_STRING)
			{

				if (len > (BCLib::uint32)m_redisReply->len)
				{
					memcpy(value, m_redisReply->str, m_redisReply->len);
				}
				else
				{
					memcpy(value, m_redisReply->str, len);
				}

				retLen = m_redisReply->len;
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;

			return retLen;
		}

		/* Format a command according to the Redis protocol. This function
		* takes a format similar to printf:
		*
		* %s represents a C null terminated string you want to interpolate
		* %b represents a binary safe string
		*
		* When using %b you need to provide both the pointer to the string
		* and the length in bytes as a size_t. Examples:
		*
		* len = redisFormatCommand(target, "GET %s", mykey);
		* len = redisFormatCommand(target, "SET %s %b", mykey, myval, myvallen);
		*/
		bool CRedisClient::setBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "setBin exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "set  %s %b", tmp, value, len);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate setBin m_redisReply = null error set key= %s error len=%d", key, len);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate setBin error set key= %s error len=%d", key, len);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}

		BCLib::uint32 CRedisClient::getBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, char* value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			BCLib::uint32 retLen = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return retLen;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return retLen;
			}

			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "GET %s", tmp);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate getBin m_redisReply = null error get key= %s error", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate getBin error get key= %s error", key);
				}
			}
			if (m_redisReply == NULL)
			{
				this->disconnect(type);
				return retLen;
			}

			if (m_redisReply->type == REDIS_REPLY_STRING)
			{
				if (len > (BCLib::uint32)m_redisReply->len)
				{
					memcpy(value, m_redisReply->str, m_redisReply->len);
					retLen = m_redisReply->len;
				}
				else
				{
					memcpy(value, m_redisReply->str, len);
					retLen = len;
				}

				
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;

			return retLen;
		}
		bool CRedisClient::delKey(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "delKey exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "DEL %s", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate m_redisReply = null error delKey key= %s error", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate  error delKey key= %s error", key);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}

		bool CRedisClient::delKey(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "delKey exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "DEL %s", tmp);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error m_redisReply = null delKey key= %s error", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error delKey key= %s error", key);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::keys(const char *key, std::vector<std::string> &values, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "KEYS %s", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error m_redisReply = null KEYS key= %s error", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error KEYS key= %s error", key);
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i++)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{
						std::string value = "";
						values.push_back(value);
						auto it = values.end() - 1;
						it->append(m_redisReply->element[i]->str, m_redisReply->element[i]->len);
					}
				}
			}

			MWLIB_PROCESS_REPLY_ERROR
		}
		BCLib::uint64 CRedisClient::scan(const char *matchKey, std::vector<std::string> &values, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			if (matchKey == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint64 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			std::string match = "";
			if (matchKey != NULL)
			{
				match = "MATCH " + std::string(matchKey) + " ";
			}
			replace(match.begin(), match.end(), '%', '*');

			std::string cmd = "SCAN " + BCLib::Utility::CConvert::toStringA(start) + " " + match + "COUNT " + BCLib::Utility::CConvert::toStringA(count);
			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate SCAN error m_redisReply = null %s", cmd.c_str());
				//this->disconnect(type);
				return ret;
			}

			if (m_redisReply->type == REDIS_REPLY_ERROR)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate SCAN error  %s", cmd.c_str());
			}
			for (BCLib::uint32 i = 0; i < m_redisReply->elements && m_redisReply->elements == 2; i++)
			{
				if (i == 0)
				{
					ret = BCLib::Utility::CConvert::toUint64(m_redisReply->element[i]->str);
				}
				else
				{
					if (m_redisReply->element[i] != NULL)
					{
						redisReply *tempRedisReply = m_redisReply->element[i];
						for (BCLib::uint64 j = 0; j < tempRedisReply->elements; j++)
						{
							if (tempRedisReply->element[j] != NULL && tempRedisReply->element[j]->len > 0)
							{
								std::string value = "";
								values.push_back(value);
								auto it = values.end() - 1;
								it->append(tempRedisReply->element[j]->str, tempRedisReply->element[j]->len);
							}
						}
					}
				}
			}
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}

		bool CRedisClient::expireSecond(const char* key, BCLib::int32 second, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "delKey exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "EXPIRE %s %d", key, second);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis ExpireSecond m_redisReply = null %s %d error", key, second);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis ExpireSecond %s %d error", key, second);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::expireSecond(const char* key, BCLib::uint64 uniqueid, const char* subkey, BCLib::int32 second, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);

			return expireSecond(tmp, second, type);
		}
		bool CRedisClient::persist(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "delKey exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "PERSIST %s", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error m_redisReply = null PERSIST key= %s error", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error PERSIST key= %s error", key);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}

		bool CRedisClient::persist(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return persist(tmp, type);

		}

		bool CRedisClient::hsetString(const char* key, const char* field, const char* value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "hsetString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "HSET %s %s %s", key, field, value);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error m_redisReply = null HSET%s %s %s", key, field, value);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error %s %s %s", key, field, value);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::hsetString(const char*key, const char*field, const std::string &value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "hsetString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			return hsetBin(key, field, value.c_str(), value.size(), type);
		}
		bool CRedisClient::hsetBin(const char* key, const char* field, const char* value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "hsetBin exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "HSET %s %s %b ", key, field, value, len);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error m_redisReply = null hsetBin %s %s ", key, field);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error hsetBin %s %s", key, field);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::hsetString(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* field, const char* value, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			//std::string strKey = key + ":[" + BCLib::Utility::CConvert::toStringA(uniqueid) + "]" + ":" + subkey;
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return hsetString(strKey, field, value, type);
		}
		bool CRedisClient::hsetString(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* field, const std::string &value, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			//std::string strKey = key + ":[" + BCLib::Utility::CConvert::toStringA(uniqueid) + "]" + ":" + subkey;
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return hsetString(strKey, field, value, type);
		}
		bool CRedisClient::hsetBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* field, const char* value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return hsetBin(tmp, field, value, len, type);
		}

		std::string CRedisClient::hgetString(const char* key, const char* field, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return "";
			}
			if (!checkConnect(type))
			{
				return "";
			}
			std::string str = "";
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return str;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;

			if (m_redisContext == NULL)
			{
				return str;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "HGET %s %s", key, field);
			if (m_redisReply == NULL)
			{
				this->disconnect(type);
				return str;
			}
			if (m_redisReply->type == REDIS_REPLY_STRING)
			{
				//str = m_redisReply->str;
				str.append(m_redisReply->str, m_redisReply->len);
			}
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return str;
		}
		BCLib::uint32 CRedisClient::hgetBin(const char* key, const char* field, char* value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint32 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "HGET %s %s", key, field);
			if (m_redisReply == NULL)
			{
				this->disconnect(type);
				return ret;
			}
			//std::string str = m_redisReply->str;
			if (m_redisReply->type == REDIS_REPLY_STRING)
			{
				if (len >= static_cast<unsigned int>(m_redisReply->len))
				{
					memcpy(value, m_redisReply->str, m_redisReply->len);
					ret = m_redisReply->len;
				}
				else
				{
					memcpy(value, m_redisReply->str, len);
					ret = len;
				}
			}
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}

		std::string CRedisClient::hgetString(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* field, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return "";
			}
			//std::string strKey = key + ":[" + BCLib::Utility::CConvert::toStringA(uniqueid) + "]" + ":" + subkey;
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return hgetString(strKey, field, type);
		}

		BCLib::uint32 CRedisClient::hgetBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* field, char* value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return 0;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return hgetBin(tmp, field, value, len, type);
		}

		void CRedisClient::hgetall(const char* key, std::map<std::string, std::string>& field_values, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return;
			}
			if (!checkConnect(type))
			{
				return;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "HGETALL %s", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error m_redisReply = null HGETALL %s ", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error HGETALL %s ", key);
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i = i + 2)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i + 1] && m_redisReply->element[i + 1]->len > 0)
					{
						std::string str;
						str.append(m_redisReply->element[i + 1]->str, m_redisReply->element[i + 1]->len);
						field_values.insert(std::pair<std::string, std::string>(m_redisReply->element[i]->str, str));
					}
				}
			}

			MWLIB_PROCESS_REPLY_RETURN
		}
		const std::map<std::string, std::pair<char*, BCLib::uint32>*>* CRedisClient::hgetallBin(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return NULL;
			}
			if (!checkConnect(type))
			{
				return NULL;
			}
			destoryTempMap();
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return NULL;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return NULL;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "HGETALL %s", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate hgetallBin error m_redisReply = null HGETALL %s ", key);
				this->disconnect(type);
				return NULL;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate hgetallBin error HGETALL %s ", key);
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i = i + 2)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i + 1] && m_redisReply->element[i + 1]->len > 0)
					{
						std::pair<char*, BCLib::uint32>* pTempPair = new std::pair<char*, BCLib::uint32>();
						char * pChar = new char[m_redisReply->element[i + 1]->len];

						memcpy(pChar, m_redisReply->element[i + 1]->str, m_redisReply->element[i + 1]->len);

						pTempPair->first = pChar;
						pTempPair->second = m_redisReply->element[i + 1]->len;
						m_map.insert(std::pair<std::string, std::pair<char*, BCLib::uint32>*>(m_redisReply->element[i]->str, pTempPair));
					}
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return &m_map;
		}
		void CRedisClient::hgetall(const char* key, BCLib::uint64 uniqueid, const char* subkey, std::map<std::string, std::string>& field_values, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);

			hgetall(strKey, field_values, type);
			return;
		}
		const std::map<std::string, std::pair<char*, BCLib::uint32>*>* CRedisClient::hgetallBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return NULL;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return hgetallBin(tmp, type);
		}
		void CRedisClient::hmset(const char* key, EREDIS_CONTEXT_TYPE type, std::string format, ...)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "hmset exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return;
			}
			if (key == NULL)
			{
				return;
			}
			if (!checkConnect(type))
			{
				return;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return;
			}
			std::string set_str = "HMSET " + std::string(key) + " " + format;

			va_list ap;
			va_start(ap, format);
			m_redisReply = (redisReply*)redisvCommand(m_redisContext, set_str.c_str(), ap);
			va_end(ap);

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate hmset error m_redisReply = null HMSET %s ", set_str.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate hmset error HMSET %s ", set_str.c_str());
				}
			}
			MWLIB_PROCESS_REPLY_RETURN
		}
		void CRedisClient::hmset(const char *key, EREDIS_CONTEXT_TYPE type, std::string format, va_list ap)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "hmset exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return;
			}
			if (key == NULL)
			{
				return;
			}
			if (!checkConnect(type))
			{
				return;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return;
			}
			std::string set_str = "HMSET " + std::string(key) + " " + format;

			m_redisReply = (redisReply*)redisvCommand(m_redisContext, set_str.c_str(), ap);

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate hmset error m_redisReply = null HMSET %s ", set_str.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate hmset error HMSET %s ", set_str.c_str());
				}
			}
			MWLIB_PROCESS_REPLY_RETURN
		}
		void CRedisClient::hmset(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type, std::string format, va_list ap)
		{
			if (key == NULL || subkey == NULL)
			{
				return;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);

			hmset(strKey, type, format, ap);

			return;
		}

		void CRedisClient::hmset(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type, std::string format, ...)
		{
			if (key == NULL || subkey == NULL)
			{
				return;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			va_list ap;
			va_start(ap, format);
			hmset(strKey, type, format, ap);
			va_end(ap);
			return;
		}
		void CRedisClient::hmget(const char *key, EREDIS_CONTEXT_TYPE type, const std::set<std::string> &field_set, std::map<std::string, std::string>& field_values)
		{
			if (key == NULL)
			{
				return;
			}
			if (!checkConnect(type))
			{
				return;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return;
			}
			if (field_set.empty())
			{
				return;
			}
			std::string field = "";
			for (auto it = field_set.begin(); it != field_set.end(); ++it)
			{
				if (field == "")
				{
					field = *it;
				}
				else
				{
					field = field + " " + *it;
				}
			}
			std::string cmd = "HMGET " + std::string(key) + " " + field;
			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error m_redisReply = null %s ", cmd.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate error HMGET %s ", cmd.c_str());
				}
				auto iter = field_set.begin();
				for ( unsigned int i = 0 ; i < m_redisReply->elements && iter != field_set.end(); i = i + 1, ++iter)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{
						std::string &str = field_values[*iter];
						str.append(m_redisReply->element[i]->str, m_redisReply->element[i]->len);
					}
				}
			}

			MWLIB_PROCESS_REPLY_RETURN
			return;
		}
		void CRedisClient::hmget(const char *key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type, const std::set<std::string> &field_set, std::map<std::string, std::string>& field_values)
		{
			if (key == NULL || subkey == NULL)
			{
				return;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			hmget(strKey, type, field_set, field_values);
			return;
		}
		//void CRedisClient::hmsetBin(const char* key, const char* filed, const char*  value, BCLib::uint32 len)
		//{
		//	m_redisReply = NULL;
		//	if (m_redisContext == NULL)
		//	{
		//		return;
		//	}
		//	m_redisReply = (redisReply*)redisCommand(m_redisContext, "HMSET %s %s %b", key, filed, value, len);
		//	
		//	return;
		//}
		//void CRedisClient::hmsetBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char*  filed, const char*  value, BCLib::uint32 len)
		//{
		//	char tmp[1024] = { 0 };
		//	snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
		//	hmsetBin(tmp, filed, value, len);
		//	return;
		//}

		bool CRedisClient::herase(const char* key, const char* field, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "hsetString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string del_str = "HDEL " + std::string(key) + " " + field;
			m_redisReply = (redisReply*)redisCommand(m_redisContext, del_str.c_str());

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate herase error m_redisReply = null  %s ", del_str.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate herase error  %s ", del_str.c_str());
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}

		bool CRedisClient::herase(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* field, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			//std::string strKey = key + ":[" + BCLib::Utility::CConvert::toStringA(uniqueid) + "]" + ":" + subkey;
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return herase(strKey, field, type);
		}
		BCLib::uint64 CRedisClient::hscan(const char *key, const char *matchKey, std::map<std::string, std::string>& field_values, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint64 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			std::string match = "";
			if (matchKey != NULL)
			{
				match = "MATCH " + std::string(matchKey) + " ";
			}
			replace(match.begin(), match.end(), '%', '*');

			std::string cmd = "HSCAN " + std::string(key) + " " + BCLib::Utility::CConvert::toStringA(start) + " " + match + "COUNT " + BCLib::Utility::CConvert::toStringA(count);
			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate HSCAN error m_redisReply = null %s", cmd.c_str());
				//this->disconnect(type);
				return ret;
			}

			if (m_redisReply->type == REDIS_REPLY_ERROR)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate HSCAN error  %s", cmd.c_str());
			}
			for (BCLib::uint32 i = 0; i < m_redisReply->elements && m_redisReply->elements == 2; i++)
			{
				if (i == 0)
				{
					ret = BCLib::Utility::CConvert::toUint64(m_redisReply->element[i]->str);
				}
				else
				{
					if (m_redisReply->element[i] != NULL)
					{
						redisReply *tempRedisReply = m_redisReply->element[i];
						for (BCLib::uint64 j = 0; j < tempRedisReply->elements; j = j + 2)
						{
							if (tempRedisReply->element[j] != NULL&& tempRedisReply->element[j + 1] != NULL
								&& tempRedisReply->element[j]->len > 0 && tempRedisReply->element[j + 1]->len > 0)
							{
								std::string str;
								str.append(tempRedisReply->element[j + 1]->str, tempRedisReply->element[j + 1]->len);
								field_values.insert(std::pair<std::string, std::string>(tempRedisReply->element[j]->str, str));
							}
						}

					}
				}
			}
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		BCLib::uint64 CRedisClient::hscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, std::map<std::string, std::string>& field_values, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			//std::string strKey = key + ":[" + BCLib::Utility::CConvert::toStringA(uniqueid) + "]" + ":" + subkey;
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return hscan(strKey, matchKey, field_values, start, count, type);
		}
		const std::map<std::string, std::pair<char*, BCLib::uint32>*>* CRedisClient::hscan(const char *key, const char *matchKey, BCLib::uint64 &end, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return NULL;
			}
			if (!checkConnect(type))
			{
				return NULL;
			}
			destoryTempMap();
			end = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return NULL;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return NULL;
			}
			std::string match = "";
			if (matchKey != NULL)
			{
				match = "MATCH " + std::string(matchKey) + " ";
			}
			replace(match.begin(), match.end(), '%', '*');

			std::string cmd = "HSCAN " + std::string(key) + " " + BCLib::Utility::CConvert::toStringA(start) + " " + match + "COUNT " + BCLib::Utility::CConvert::toStringA(count);
			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate HSCAN error m_redisReply = null %s", cmd.c_str());
				//this->disconnect(type);
				return NULL;
			}

			if (m_redisReply->type == REDIS_REPLY_ERROR)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate HSCAN error %s", cmd.c_str());
			}
			for (BCLib::uint32 i = 0; i < m_redisReply->elements && m_redisReply->elements == 2; i++)
			{
				if (i == 0)
				{
					end = BCLib::Utility::CConvert::toUint64(m_redisReply->element[i]->str);
				}
				else
				{
					if (m_redisReply->element[i] != NULL)
					{
						redisReply *tempRedisReply = m_redisReply->element[i];
						for (BCLib::uint64 j = 0; j < tempRedisReply->elements; j = j + 2)
						{
							if (tempRedisReply->element[j] != NULL&& tempRedisReply->element[j + 1] != NULL
								&& tempRedisReply->element[j]->len > 0 && tempRedisReply->element[j + 1]->len > 0)
							{
								std::pair<char*, BCLib::uint32>* pTempPair = new std::pair<char*, BCLib::uint32>();
								char * pChar = new char[tempRedisReply->element[j + 1]->len];

								memcpy(pChar, tempRedisReply->element[j + 1]->str, tempRedisReply->element[j + 1]->len);

								pTempPair->first = pChar;
								pTempPair->second = tempRedisReply->element[j + 1]->len;
								m_map.insert(std::pair<std::string, std::pair<char*, BCLib::uint32>*>(tempRedisReply->element[j]->str, pTempPair));
							}
						}
					}
				}
			}
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return &m_map;
		}

		const std::map<std::string, std::pair<char*, BCLib::uint32>*>* CRedisClient::hscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, BCLib::uint64 &end, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return NULL;
			}
			//std::string strKey = key + ":[" + BCLib::Utility::CConvert::toStringA(uniqueid) + "]" + ":" + subkey;
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return hscan(strKey, matchKey, end, start, count, type);
		}

		BCLib::uint32 CRedisClient::hlen(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			BCLib::uint32 ret = 0;
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "lpushString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return ret;
			}
			if (key == NULL)
			{
				return ret;
			}
			if (!checkConnect(type))
			{
				return ret;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "HLEN %s ", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate hlen error m_redisReply = null HLEN %s ", key);
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate hlen error HLEN %s", key);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
					ret = BCLib::Utility::CConvert::toUint32(str);
				}
				if (m_redisReply->type == REDIS_REPLY_INTEGER)
				{
					ret = (BCLib::uint32)m_redisReply->integer;
				}		
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}

		BCLib::uint32 CRedisClient::hlen(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return 0;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return hlen(strKey, type);
		}

		bool CRedisClient::lpushString(const char* key, const char* value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "lpushString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LPUSH %s %s", key, value);

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lpushString error m_redisReply = null  LPUSH %s %s", key, value);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lpushString error LPUSH %s %s", key, value);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::lpushBin(const char* key, const char* value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "lpushBin exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LPUSH %s %b", key, value, len);

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lpushBin error m_redisReply = null  LPUSH %s", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lpushBin error LPUSH %s", key);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}

		bool CRedisClient::lpushString(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* value, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			//std::string strKey = key + ":[" + BCLib::Utility::CConvert::toStringA(uniqueid) + "]" + ":" + subkey;
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lpushString(strKey, value, type);
		}
		bool CRedisClient::lpushBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lpushBin(tmp, value, len, type);
		}

		std::string CRedisClient::lreadString(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return "";
			}
			if (!checkConnect(type))
			{
				return "";
			}
			std::string value = "";
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return value;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;

			if (m_redisContext == NULL)
			{
				return value;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LRANGE %s 0 0", key);

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lreadString error m_redisReply = null LRANGE %s 0 0", key);
				this->disconnect(type);
				return value;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lreadString error LRANGE %s 0 0", key);
				}

				for (unsigned int i = 0; i < m_redisReply->elements; i++ && i == 0)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{
						//value = m_redisReply->element[i]->str;
						value.append(m_redisReply->element[i]->str, m_redisReply->element[i]->len);
					}
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return value;
		}
		BCLib::uint32 CRedisClient::lreadBin(const char* key, char* value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint32 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;

			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LRANGE %s 0 0", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lreadBin error m_redisReply = null LRANGE %s 0 0", key);
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lreadBin error LRANGE %s 0 0", key);
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i++)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0 && i == 0)
					{
						//value = m_redisReply->element[i]->str;
						if (len >= m_redisReply->element[i]->len)
						{
							memcpy(value, m_redisReply->element[i]->str, m_redisReply->element[i]->len);
							ret = m_redisReply->element[i]->len;
						}
						else
						{
							memcpy(value, m_redisReply->element[i]->str, len);
							ret = len;
						}
					}
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		std::string CRedisClient::lreadString(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return "";
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lreadString(strKey, type);
		}
		BCLib::uint32 CRedisClient::lreadBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, char* value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return 0;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lreadBin(tmp, value, len, type);
		}
		bool CRedisClient::rpushString(const char* key, const char* value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "rpushString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "RPUSH %s %s", key, value);

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rpushString error m_redisReply = null RPUSH %s %s", key, value);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rpushString error RPUSH %s %s", key, value);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::rpushBin(const char* key, const char* value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "rpushBin exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "RPUSH %s %b", key, value, len);

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rpushBin error m_redisReply = null RPUSH %s", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rpushBin error RPUSH %s", key);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::rpushString(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* value, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			//std::string strKey = key + ":[" + BCLib::Utility::CConvert::toStringA(uniqueid) + "]:" + subkey;
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return rpushString(strKey, value, type);
		}

		bool CRedisClient::rpushBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return rpushBin(tmp, value, len, type);
		}

		std::string CRedisClient::rreadString(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return "";
			}
			if (!checkConnect(type))
			{
				return "";
			}
			std::string value = "";
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return value;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return value;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LRANGE %s -1 -1", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rreadString error m_redisReply = null LRANGE %s -1 -1", key);
				this->disconnect(type);
				return value;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rreadString error LRANGE %s -1 -1", key);
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i++)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{
						//value = m_redisReply->element[i]->str;
						value.append(m_redisReply->element[i]->str, m_redisReply->element[i]->len);
					}
				}
			}
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return value;
		}
		BCLib::uint32 CRedisClient::rreadBin(const char* key, char* value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint32 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;

			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LRANGE %s -1 -1", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rreadBin error m_redisReply = null LRANGE %s -1 -1", key);
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rreadBin error LRANGE %s -1 -1", key);
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i++)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{

						if (len >= m_redisReply->len)
						{
							memcpy(value, m_redisReply->element[i]->str, m_redisReply->element[i]->len);
							ret = m_redisReply->element[i]->len;
						}
						else
						{
							memcpy(value, m_redisReply->element[i]->str, len);
							ret = len;
						}
					}
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		std::string CRedisClient::rreadString(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return "";
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return rreadString(strKey, type);
		}
		BCLib::uint32 CRedisClient::rreadBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, char* value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return 0;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return rreadBin(tmp, value, len, type);
		}
		BCLib::int32 CRedisClient::llen(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return -1;
			}
			if (!checkConnect(type))
			{
				return -1;
			}
			BCLib::uint32 ret = -1;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LLEN %s ", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate llen error m_redisReply = null LLEN %s ", key);
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate llen error LLEN %s ", key);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
					ret = BCLib::Utility::CConvert::toUint32(str);
				}
				if (m_redisReply->type == REDIS_REPLY_INTEGER)
				{
					ret = (BCLib::uint32)m_redisReply->integer;
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		BCLib::int32 CRedisClient::llen(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return -1;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return llen(strKey, type);
		}

		std::string CRedisClient::lpopString(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "lpopString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return "";
			}
			if (key == NULL)
			{
				return "";
			}
			if (!checkConnect(type))
			{
				return "";
			}
			std::string value = "";
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return value;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return value;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LPOP %s ", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lpopString error m_redisReply = null LPOP %s ", key);
				this->disconnect(type);
				return value;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lpopString error LPOP %s ", key);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					//value = m_redisReply->str;
					value.append(m_redisReply->str, m_redisReply->len);
				}
			}
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return value;
		}
		BCLib::uint32 CRedisClient::lpopBin(const char* key, char*value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "lpopBin exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return 0;
			}
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint32 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LPOP %s ", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lpopBin error m_redisReply = null LPOP %s ", key);
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lpopBin error LPOP %s ", key);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					//value = m_redisReply->str;
					if (len >= m_redisReply->len)
					{
						memcpy(value, m_redisReply->str, m_redisReply->len);
						ret = m_redisReply->len;
					}
					else
					{
						memcpy(value, m_redisReply->str, len);
						ret = len;
					}
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		std::string CRedisClient::lpopString(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return "";
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lpopString(strKey, type);
		}

		BCLib::uint32 CRedisClient::lpopBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, char*value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return 0;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lpopBin(tmp, value, len, type);
		}

		std::string CRedisClient::rpopString(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "rpopString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return "";
			}
			if (key == NULL)
			{
				return "";
			}
			if (!checkConnect(type))
			{
				return "";
			}
			std::string value = "";
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return value;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return value;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "RPOP %s ", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rpopString error m_redisReply = null RPOP %s ", key);
				this->disconnect(type);
				return value;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rpopString error RPOP %s ", key);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					//value = m_redisReply->str;
					value.append(m_redisReply->str, m_redisReply->len);
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return value;
		}
		BCLib::uint32 CRedisClient::rpopBin(const char* key, char*value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "rpopBin exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint32 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "RPOP %s ", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rpopBin error m_redisReply = null RPOP %s ", key);
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rpopBin error RPOP %s ", key);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					if (len >= m_redisReply->len)
					{
						memcpy(value, m_redisReply->str, m_redisReply->len);
						ret = m_redisReply->len;
					}
					else
					{
						memcpy(value, m_redisReply->str, len);
						ret = len;
					}
				}
			}
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		std::string CRedisClient::rpopString(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return "";
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return rpopString(strKey, type);
		}
		BCLib::uint32 CRedisClient::rpopBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, char*value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return 0;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return rpopBin(tmp, value, len, type);
		}
		bool CRedisClient::lsetString(const char* key, BCLib::int32 index, const char* value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "lsetString exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LSET %s %d %s", key, index, value);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lsetString error m_redisReply = null LSET %s %d %s", key, index, value);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lsetString error LSET %s %d %s", key, index, value);
				}
			}

			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::lsetBin(const char* key, BCLib::int32 index, char*value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "lsetBin exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LSET %s %d %b", key, index, value, len);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lsetBin error m_redisReply = null LSET %s %d", key, index);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lsetBin error LSET %s %d", key, index);
				}
			}

			MWLIB_PROCESS_REPLY_ERROR
		}

		bool CRedisClient::lsetString(const char* key, BCLib::uint64 uniqueid, const char* subkey, BCLib::int32 index, const char* value, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lsetString(strKey, index, value, type);
		}

		bool CRedisClient::lsetBin(const char* key, BCLib::uint64 uniqueid, const char* subkey, BCLib::int32 index, char*value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lsetBin(tmp, index, value, len, type);
		}

		bool CRedisClient::rpoplpush(const char* keyTar, const char* keySrc, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "rpoplpush exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (keyTar == NULL || keySrc == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "RPOPLPUSH %s %s ", keySrc, keyTar);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rpoplpush error m_redisReply = null RPOPLPUSH %s %s ", keySrc, keyTar);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate rpoplpush error RPOPLPUSH %s %s ", keySrc, keyTar);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::rpoplpush(const char* keyTar, BCLib::uint64 uniqueidTar, const char* subkeyTar, const char* keySrc, BCLib::uint64 uniqueidSrc, const char* subkeySrc, EREDIS_CONTEXT_TYPE type)
		{
			if (keyTar == NULL || keySrc == NULL)
			{
				return false;
			}
			char strKeyTar[1024] = { 0 };
			snprintf(strKeyTar, 1024, "%s:[%llu]:%s", keyTar, uniqueidTar, subkeyTar);

			char strKeySrc[1024] = { 0 };
			snprintf(strKeySrc, 1024, "%s:[%llu]:%s", keySrc, uniqueidSrc, subkeySrc);

			return rpoplpush(strKeyTar, strKeySrc, type);
		}

		bool CRedisClient::lgetAllElement(const char* key, std::list<std::string>& mylist, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LRANGE %s 0 -1", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lgetAllElement error m_redisReply = null LRANGE %s 0 -1", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lgetAllElement error LRANGE %s 0 -1", key);
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i++)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{
						std::string str;// = m_redisReply->element[i]->str;
						str.append(m_redisReply->element[i]->str, m_redisReply->element[i]->len);
						mylist.push_back(str);
					}
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		const std::list<std::pair<char*, BCLib::uint32>*> * CRedisClient::lgetAllElement(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return NULL;
			}
			if (!checkConnect(type))
			{
				return NULL;
			}
			destoryTempList();
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return NULL;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return NULL;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LRANGE %s 0 -1", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lgetAllElement error m_redisReply = null LRANGE %s 0 -1", key);
				this->disconnect(type);
				return NULL;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lgetAllElement error LRANGE %s 0 -1", key);
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i++)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{
						//std::string str = m_redisReply->element[i]->str;
						char *p = new char[m_redisReply->element[i]->len];
						memcpy(p, m_redisReply->element[i]->str, m_redisReply->element[i]->len);
						std::pair<char *, BCLib::uint32>* pTempPair = new std::pair<char *, BCLib::uint32>();
						pTempPair->first = p;
						pTempPair->second = m_redisReply->element[i]->len;

						m_list.push_back(pTempPair);
					}
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return &m_list;
		}
		bool CRedisClient::lgetAllElement(const char* key, BCLib::uint64 uniqueid, const char* subkey, std::list<std::string>& mylist, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			//std::string strKey = key + ":[" + BCLib::Utility::CConvert::toStringA(uniqueid) + "]:" + subkey;
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lgetAllElement(strKey, mylist, type);
		}

		const std::list<std::pair<char*, BCLib::uint32>*> *  CRedisClient::lgetAllElement(const char* key, BCLib::uint64 uniqueid, const char*subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return NULL;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lgetAllElement(tmp, type);
		}
		bool CRedisClient::lrem(const char* key, BCLib::int32 count, char *value, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "lrem exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LREM %s %d %s", key, count, value);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lrem error m_redisReply = null LREM %s %d %s", key, count, value);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lrem error LREM %s %d %s", key, count, value);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::lrem(const char* key, BCLib::uint64 uniqueid, const char*subkey, BCLib::int32 count, char *value, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lrem(tmp, count, value, type);
		}
		bool CRedisClient::lrem(const char* key, BCLib::int32 count, char *value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "lrem exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "LREM %s %d %b", key, count, value, len);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lrem error m_redisReply = null LREM %s %d bin[%d]", key, count, len);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate lrem error LREM %s %d bin[%d]", key, count, len);
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}

		bool CRedisClient::lrem(const char* key, BCLib::uint64 uniqueid, const char*subkey, BCLib::int32 count, char *value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char tmp[1024] = { 0 };
			snprintf(tmp, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return lrem(tmp, count, value, len, type);
		}

		bool CRedisClient::sadd(const char* key, EREDIS_CONTEXT_TYPE type, std::string members, ...)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "sadd exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string set_str = "SADD " + std::string(key) + " " + members;
			va_list ap;
			va_start(ap, members);
			m_redisReply = (redisReply*)redisvCommand(m_redisContext, set_str.c_str(), ap);
			va_end(ap);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sadd error m_redisReply = null  %s ", set_str.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sadd error  %s ", set_str.c_str());
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}

		bool CRedisClient::sadd(const char* key, EREDIS_CONTEXT_TYPE type, std::string members, va_list ap)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "sadd exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string set_str = "SADD " + std::string(key) + " " + members;

			m_redisReply = (redisReply*)redisvCommand(m_redisContext, set_str.c_str(), ap);

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sadd error m_redisReply = null  %s ", set_str.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sadd error  %s ", set_str.c_str());
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}

		bool CRedisClient::sadd(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type, std::string members, ...)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			va_list ap;
			va_start(ap, members);
			bool ret = sadd(strKey, type, members, ap);
			va_end(ap);
			return ret;
		}

		bool CRedisClient::sadd(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type, std::string members, va_list ap)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return sadd(strKey, type, members, ap);
		}
		BCLib::uint32 CRedisClient::scard(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint32 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "SCARD %s ", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sadd error m_redisReply = null  SCARD %s ", key);
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sadd error  SCARD %s ", key);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
					ret = BCLib::Utility::CConvert::toUint32(str);
				}
				if (m_redisReply->type == REDIS_REPLY_INTEGER)
				{
					ret = (BCLib::uint32)m_redisReply->integer;
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		BCLib::uint32 CRedisClient::scard(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return 0;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return scard(strKey, type);
		}
		bool CRedisClient::sismember(const char* key, const char* member, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			BCLib::uint32 ret = 0;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "SISMEMBER %s %s", key, member);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sismember error m_redisReply = null  SISMEMBER %s %s", key, member);
				this->disconnect(type);
				return false;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sismember error  SISMEMBER %s %s", key, member);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
					ret = BCLib::Utility::CConvert::toUint32(str);
				}
				if (m_redisReply->type == REDIS_REPLY_INTEGER)
				{
					ret = (BCLib::uint32)m_redisReply->integer;
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			if (ret == 0)
			{
				return false;
			}
			return true;
		}
		bool CRedisClient::sismember(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* member, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return sismember(strKey, member, type);
		}
		bool CRedisClient::sgetAllMembers(const char* key, std::set<std::string> &mySet, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "SMEMBERS %s ", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sgetAllMembers error m_redisReply = null  SMEMBERS %s ", key);
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sgetAllMembers error  SMEMBERS %s ", key);
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i++)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{
						std::string str = m_redisReply->element[i]->str;
						mySet.insert(str);
					}
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::sgetAllMembers(const char* key, BCLib::uint64 uniqueid, const char* subkey, std::set<std::string> &mySet, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return sgetAllMembers(strKey, mySet, type);
		}
		bool CRedisClient::spop(const char* key, std::string &member, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "spop exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, "SPOP %s ", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate spop error m_redisReply = null  SPOP %s ", key);

			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate spop error  SPOP %s ", key);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					member = (std::string)m_redisReply->str;
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::spop(const char* key, BCLib::uint64 uniqueid, const char* subkey, std::string &member, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return spop(strKey, member, type);
		}
		bool CRedisClient::sremove(const char* key, const std::string members, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "sremove exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			//SREM set_test abc hijk
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string set_str = "SREM " + std::string(key) + " " + members;
			m_redisReply = (redisReply*)redisCommand(m_redisContext, set_str.c_str());

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sremove error m_redisReply = null %s ", set_str.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sremove error  %s ", set_str.c_str());
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::sremove(const char* key, BCLib::uint64 uniqueid, const char* subkey, const std::string members, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return sremove(strKey, members, type);
		}

		bool CRedisClient::smove(const char * disKey, const char * srcKey, const std::string member, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "smove exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (disKey == NULL || srcKey == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string set_str = "SMOVE " + std::string(srcKey) + " " + std::string(disKey) + " " + member;
			m_redisReply = (redisReply*)redisCommand(m_redisContext, set_str.c_str());

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate smove error m_redisReply = null %s ", set_str.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate smove error  %s ", set_str.c_str());
				}
				if (m_redisReply->type == REDIS_REPLY_INTEGER)
				{
					if (m_redisReply->integer == 0)
					{
						freeReplyObject(m_redisReply);
						m_redisReply = NULL;
						return false;
					}
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}

		bool CRedisClient::smove(const char* disKey, BCLib::uint64 disUniqueid, const char* disSubkey, const char* srcKey, BCLib::uint64 srcUniqueid, const char* srcSubkey, const std::string member, EREDIS_CONTEXT_TYPE type)
		{
			if (disKey == NULL || srcKey == NULL)
			{
				return false;
			}
			char strDisKey[1024] = { 0 };
			snprintf(strDisKey, 1024, "%s:[%llu]:%s", disKey, disUniqueid, disSubkey);
			char strSrcKey[1024] = { 0 };
			snprintf(strSrcKey, 1024, "%s:[%llu]:%s", srcKey, srcUniqueid, srcSubkey);
			return smove(strDisKey, strSrcKey, member, type);
		}
		bool CRedisClient::sinter(std::set<std::string> &set, EREDIS_CONTEXT_TYPE type, std::string keys, ...)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "sinter exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string set_str = "SINTER " + keys;
			va_list ap;
			va_start(ap, keys);
			m_redisReply = (redisReply*)redisvCommand(m_redisContext, set_str.c_str(), ap);
			va_end(ap);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sinter error m_redisReply = null  %s ", set_str.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sinter error  %s ", set_str.c_str());
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i++)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{
						std::string str = m_redisReply->element[i]->str;
						set.insert(str);
					}
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::sinter(std::set<std::string> &set, EREDIS_CONTEXT_TYPE type, std::string keys, va_list ap)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "sinter exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string set_str = "SINTER " + keys;

			m_redisReply = (redisReply*)redisvCommand(m_redisContext, set_str.c_str(), ap);

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sinter error m_redisReply = null  %s ", set_str.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sinter error  %s ", set_str.c_str());
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i++)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{
						std::string str = m_redisReply->element[i]->str;
						set.insert(str);
					}
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::sinter(std::set<std::string> &set, const std::vector<std::string> &keys, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "sinter exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			if (keys.size() == 0)
			{
				freeReplyObject(m_redisReply);
				m_redisReply = NULL;
				return false;
			}
			std::string set_str = "SINTER ";

			for (auto it = keys.begin(); it != keys.end(); it++)
			{
				set_str += " " + (*it);
			}

			m_redisReply = (redisReply*)redisCommand(m_redisContext, set_str.c_str());

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sinter error m_redisReply = null  %s ", set_str.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate sinter error  %s ", set_str.c_str());
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i++)
				{
					if (m_redisReply->element[i] && m_redisReply->element[i]->len > 0)
					{
						std::string str = m_redisReply->element[i]->str;
						set.insert(str);
					}
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		BCLib::uint64 CRedisClient::sscan(const char *key, const char *matchKey, std::set<std::string> &mySet, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint64 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			std::string match = "";
			if (matchKey != NULL)
			{
				match = "MATCH " + std::string(matchKey) + " ";
			}
			replace(match.begin(), match.end(), '%', '*');

			std::string cmd = "SSCAN " + std::string(key) + " " + BCLib::Utility::CConvert::toStringA(start) + " " + match + "COUNT " + BCLib::Utility::CConvert::toStringA(count);
			try
			{
				m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			}
			catch (...)
			{
				BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "Redis operate SSCAN error CException %s", cmd.c_str());
				return ret;
			}
			
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate SSCAN error m_redisReply = null %s", cmd.c_str());
				//this->disconnect(type);
				return ret;
			}

			if (m_redisReply->type == REDIS_REPLY_ERROR)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate SSCAN error  %s", cmd.c_str());
			}
			for (BCLib::uint32 i = 0; i < m_redisReply->elements && m_redisReply->elements == 2; i++)
			{
				if (i == 0)
				{
					ret = BCLib::Utility::CConvert::toUint64(m_redisReply->element[i]->str);
				}
				else
				{
					if (m_redisReply->element[i] != NULL)
					{
						redisReply *tempRedisReply = m_redisReply->element[i];
						for (BCLib::uint64 j = 0; j < tempRedisReply->elements; j++)
						{
							if (tempRedisReply->element[j] != NULL && tempRedisReply->element[j]->len > 0)
							{
								std::string str = tempRedisReply->element[j]->str;
								mySet.insert(str);
							}
						}
					}
				}
			}
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}

		BCLib::uint64 CRedisClient::sscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, std::set<std::string> &mySet, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return sscan(strKey, matchKey, mySet, start, count, type);
		}

		bool CRedisClient::zadd(const char* key, const std::vector<double> &scores, const std::vector<std::string> &members, BCLib::uint32 precision, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "zadd exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL || members.empty() == true || scores.size() != members.size())
			{
				return false;
			}
			std::string memTemp = "";
			for (BCLib::uint32 i = 0; i < members.size(); i++)
			{
				memTemp = memTemp + " " + DoubleToString(scores[i], precision) + " " + members[i];
			}

			std::string cmd = "ZADD " + std::string(key) + " " + memTemp;

			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());

			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zadd error m_redisReply = null %s ", cmd.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zadd error  %s ", cmd.c_str());
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::zadd(const char* key, BCLib::uint64 uniqueid, const char* subkey, const std::vector<double> &scores, const std::vector<std::string> &members, BCLib::uint32 precision, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zadd(strKey, scores, members, precision, type);
		}
		BCLib::uint32  CRedisClient::zcard(const char* key, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint32 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "ZCARD %s ", key);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zcard error m_redisReply = null ZCARD %s ", key);
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zcard error ZCARD %s ", key);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
					ret = BCLib::Utility::CConvert::toUint32(str);
				}
				if (m_redisReply->type == REDIS_REPLY_INTEGER)
				{
					ret = (BCLib::uint32)m_redisReply->integer;
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		BCLib::uint32  CRedisClient::zcard(const char* key, BCLib::uint64 uniqueid, const char* subkey, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return 0;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zcard(strKey, type);
		}
		BCLib::int32 CRedisClient::zrank(const char* key, const char* member, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return -1;
			}
			if (!checkConnect(type))
			{
				return -1;
			}
			BCLib::int32 ret = -1;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "ZRANK %s %s", key, member);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrank error m_redisReply = null ZRANK %s %s", key, member);
				this->disconnect(type);
				return ret;

			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrank error ZRANK %s %s", key, member);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
					ret = BCLib::Utility::CConvert::toUint32(str);
				}
				if (m_redisReply->type == REDIS_REPLY_INTEGER)
				{
					ret = (BCLib::uint32)m_redisReply->integer;
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		BCLib::int32 CRedisClient::zrank(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* member, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return -1;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zrank(strKey, member, type);
		}
		BCLib::int32 CRedisClient::zrevRank(const char* key, const char* member, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return -1;
			}
			if (!checkConnect(type))
			{
				return -1;
			}
			BCLib::int32 ret = -1;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "ZREVRANK %s %s", key, member);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrevRank error m_redisReply = null ZREVRANK %s %s", key, member);
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrevRank error ZREVRANK %s %s", key, member);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
					ret = BCLib::Utility::CConvert::toUint32(str);
				}
				if (m_redisReply->type == REDIS_REPLY_INTEGER)
				{
					ret = (BCLib::uint32)m_redisReply->integer;
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		BCLib::int32 CRedisClient::zrevRank(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* member, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return -1;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zrevRank(strKey, member, type);
		}
		//删除
		bool CRedisClient::zrem(const char* key, std::vector<std::string> &members, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "zrem exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL || members.empty() == true)
			{
				return false;
			}
			std::string memTemp = "";
			for (BCLib::uint32 i = 0; i < members.size(); i++)
			{
				memTemp = memTemp + " " + members[i];
			}
			std::string cmd = "ZREM " + std::string(key) + " " + memTemp;
			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrevRank error m_redisReply = null %s", cmd.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrevRank error  %s", cmd.c_str());
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::zrem(const char* key, BCLib::uint64 uniqueid, const char* subkey, std::vector<std::string> &members, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zrem(strKey, members, type);
		}
		bool CRedisClient::zrange(const char* key, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			// ZRANGE salary 0 - 1 WITHSCORES
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string cmd = "ZRANGE " + std::string(key) + " " + BCLib::Utility::CConvert::toStringA(startIndex) + " " + BCLib::Utility::CConvert::toStringA(stopIndex) + " WITHSCORES ";
			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrange error m_redisReply = null %s", cmd.c_str());

			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrange error  %s", cmd.c_str());
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i = i + 2)
				{
					if (m_redisReply->element[i] != NULL&& m_redisReply->element[i + 1] != NULL
						&& m_redisReply->element[i]->len > 0 && m_redisReply->element[i + 1]->len > 0)
					{
						std::string str = m_redisReply->element[i]->str;
						std::string strScore = m_redisReply->element[i + 1]->str;
						double score = BCLib::Utility::CConvert::toDouble(strScore);
						std::pair<std::string, double> mPair;
						mPair.first = str;
						mPair.second = score;
						members.push_back(mPair);
					}
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::zrange(const char* key, BCLib::uint64 uniqueid, const char* subkey, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zrange(strKey, members, startIndex, stopIndex, type);
		}
		bool CRedisClient::zrevRange(const char* key, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			// ZREVRANGE salary 0 - 1 WITHSCORES
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string cmd = "ZREVRANGE " + std::string(key) + " " + BCLib::Utility::CConvert::toStringA(startIndex) + " " + BCLib::Utility::CConvert::toStringA(stopIndex) + " WITHSCORES ";
			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrevRange error m_redisReply = null %s", cmd.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrevRange error  %s", cmd.c_str());
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i = i + 2)
				{
					if (m_redisReply->element[i] != NULL&& m_redisReply->element[i + 1] != NULL
						&& m_redisReply->element[i]->len > 0 && m_redisReply->element[i + 1]->len > 0)
					{
						std::string str = m_redisReply->element[i]->str;
						std::string strScore = m_redisReply->element[i + 1]->str;
						double score = BCLib::Utility::CConvert::toDouble(strScore);
						std::pair<std::string, double> mPair;
						mPair.first = str;
						mPair.second = score;
						members.push_back(mPair);
					}
				}
			}

			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::zrevRange(const char* key, BCLib::uint64 uniqueid, const char* subkey, std::vector<std::pair<std::string, double>> &members, const BCLib::int32 startIndex, const BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL ||subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zrevRange(strKey, members, startIndex, stopIndex, type);
		}
		double CRedisClient::zscore(const char* key, const char* member, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			double ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			m_redisReply = (redisReply*)redisCommand(m_redisContext, "ZSCORE %s %s", key, member);
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zscore error m_redisReply = null ZSCORE %s %s", key, member);
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zscore error ZSCORE %s %s", key, member);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
					ret = BCLib::Utility::CConvert::toDouble(str);
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;

		}
		double CRedisClient::zscore(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* member, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return 0;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zscore(strKey, member, type);
		}
		bool CRedisClient::zremRangeByRank(const char* key, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "zremRangeByRank exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string cmd = "ZREMRANGEBYRANK " + std::string(key) + " " + BCLib::Utility::CConvert::toStringA(startIndex) + " " + BCLib::Utility::CConvert::toStringA(stopIndex);

			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zscore error m_redisReply = null %s", cmd.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zscore error %s", key, cmd.c_str());
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::zremRangeByRank(const char* key, BCLib::uint64 uniqueid, const char* subkey, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zremRangeByRank(strKey, startIndex, stopIndex, type);
		}

		bool CRedisClient::zincrby(const char* key, const char* member, double increment, EREDIS_CONTEXT_TYPE type)
		{
			if (m_eAccessRight != E_REDIS_READ_AND_WRITE)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "zincrby exec 权限非法 m_eAccessRight= %d", m_eAccessRight);
				return false;
			}
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string cmd = "ZINCRBY " + std::string(key) + " " + BCLib::Utility::CConvert::toStringA(increment) + " " + std::string(member);

			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate ZINCRBY error m_redisReply = null %s", cmd.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate ZINCRBY error %s", key, cmd.c_str());
				}
			}
			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::zincrby(const char* key, BCLib::uint64 uniqueid, const char* subkey, const char* member, double increment, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zincrby(strKey, member, increment, type);
		}
		BCLib::int32 CRedisClient::zcount(const char* key, double min, double max, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return -1;
			}
			if (!checkConnect(type))
			{
				return -1;
			}
			BCLib::int32 ret = -1;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			std::string cmd = "ZCOUNT " + std::string(key) + " " + BCLib::Utility::CConvert::toStringA(min) + " " + BCLib::Utility::CConvert::toStringA(max);

			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zcount error m_redisReply = null  ZCOUNT %s ", cmd.c_str());
				this->disconnect(type);
				return ret;
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zcount error  ZCOUNT %s ", key);
				}
				if (m_redisReply->type == REDIS_REPLY_STRING)
				{
					BCLib::Utility::CStringA str = (std::string)m_redisReply->str;
					ret = BCLib::Utility::CConvert::toInt32(str);
				}
				if (m_redisReply->type == REDIS_REPLY_INTEGER)
				{
					ret = (BCLib::int32)m_redisReply->integer;
				}
			}

			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		BCLib::int32 CRedisClient::zcount(const char* key, BCLib::uint64 uniqueid, const char* subkey, double min, double max, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return -1;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zcount(strKey, min, max, type);
		}
		bool CRedisClient::zrangeByScore(const char* key, std::vector<std::pair<std::string, double>> &members, const char *minStr, const char *maxStr, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string cmd = "ZRANGEBYSCORE " + std::string(key) + " " + std::string(minStr) + " " + std::string(maxStr) + " WITHSCORES ";
			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrangeByScore error m_redisReply = null %s", cmd.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrangeByScore error  %s", cmd.c_str());
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i = i + 2)
				{
					if (m_redisReply->element[i] != NULL&& m_redisReply->element[i + 1] != NULL
						&& m_redisReply->element[i]->len > 0 && m_redisReply->element[i + 1]->len > 0)
					{
						std::string str = m_redisReply->element[i]->str;
						std::string strScore = m_redisReply->element[i + 1]->str;
						double score = BCLib::Utility::CConvert::toDouble(strScore);
						std::pair<std::string, double> mPair;
						mPair.first = str;
						mPair.second = score;
						members.push_back(mPair);
					}
				}
			}

			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::zrangeByScore(const char* key, BCLib::uint64 uniqueid, const char* subkey, std::vector<std::pair<std::string, double>> &members, const char *minStr, const char *maxStr, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zrangeByScore(strKey, members, minStr, maxStr, type);
		}
		bool CRedisClient::zrevRangeByScore(const char* key, std::vector<std::pair<std::string, double>> &members, const char *maxStr, const char *minStr, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return false;
			}
			if (!checkConnect(type))
			{
				return false;
			}
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return false;
			}
			m_redisContext = it->second.m_redisContext;
			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return false;
			}
			std::string cmd = "ZREVRANGEBYSCORE " + std::string(key) + " " + std::string(maxStr) + " " + std::string(minStr) + " WITHSCORES ";
			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrevRangeByScore error m_redisReply = null %s", cmd.c_str());
			}
			else
			{
				if (m_redisReply->type == REDIS_REPLY_ERROR)
				{
					BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate zrevRangeByScore error  %s", cmd.c_str());
				}
				for (unsigned int i = 0; i < m_redisReply->elements; i = i + 2)
				{
					if (m_redisReply->element[i] != NULL&& m_redisReply->element[i + 1] != NULL
						&& m_redisReply->element[i]->len > 0 && m_redisReply->element[i + 1]->len > 0)
					{
						std::string str = m_redisReply->element[i]->str;
						std::string strScore = m_redisReply->element[i + 1]->str;
						double score = BCLib::Utility::CConvert::toDouble(strScore);
						std::pair<std::string, double> mPair;
						mPair.first = str;
						mPair.second = score;
						members.push_back(mPair);
					}
				}
			}

			MWLIB_PROCESS_REPLY_ERROR
		}
		bool CRedisClient::zrevRangeByScore(const char* key, BCLib::uint64 uniqueid, const char* subkey, std::vector<std::pair<std::string, double>> &members, const char *maxStr, const char *minStr, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return false;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zrevRangeByScore(strKey, members, maxStr, minStr, type);
		}
		BCLib::uint64 CRedisClient::zscan(const char *key, const char *matchKey, std::vector<std::pair<std::string, double>> &members, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL)
			{
				return 0;
			}
			if (!checkConnect(type))
			{
				return 0;
			}
			BCLib::uint64 ret = 0;
			std::unordered_map<BCLib::uint16, REDIS_NODE>::iterator it = m_redisContextMap.find(type);
			if (it == m_redisContextMap.end())
			{
				return ret;
			}
			m_redisContext = it->second.m_redisContext;

			m_redisReply = NULL;
			if (m_redisContext == NULL)
			{
				return ret;
			}
			std::string match = "";
			if (matchKey != NULL)
			{		
				match = "MATCH " + std::string(matchKey) + " ";
			}
			replace(match.begin(), match.end(), '%', '*');
			
			std::string cmd = "ZSCAN " + std::string(key) + " " + BCLib::Utility::CConvert::toStringA(start) + " " + match + "COUNT " + BCLib::Utility::CConvert::toStringA(count);
			m_redisReply = (redisReply*)redisCommand(m_redisContext, cmd.c_str());
			if (m_redisReply == NULL)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate ZSCAN error m_redisReply = null %s", cmd.c_str());
				//this->disconnect(type);
				return ret;
			}

			if (m_redisReply->type == REDIS_REPLY_ERROR)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "Redis operate ZSCAN error  %s", cmd.c_str());
			}
			for (BCLib::uint32 i = 0; i < m_redisReply->elements && m_redisReply->elements == 2; i++)
			{
				if (i == 0)
				{
					ret = BCLib::Utility::CConvert::toUint64(m_redisReply->element[i]->str);
				}
				else
				{
					if (m_redisReply->element[i] != NULL)
					{
						redisReply *tempRedisReply = m_redisReply->element[i];
						for (BCLib::uint64 j = 0; j < tempRedisReply->elements; j = j + 2)
						{
							if (tempRedisReply->element[j] != NULL&& tempRedisReply->element[j + 1] != NULL
								&& tempRedisReply->element[j]->len > 0 && tempRedisReply->element[j + 1]->len > 0)
							{
								std::string str = tempRedisReply->element[j]->str;
								std::string strScore = tempRedisReply->element[j + 1]->str;
								double score = BCLib::Utility::CConvert::toDouble(strScore);
								std::pair<std::string, double> mPair;
								mPair.first = str;
								mPair.second = score;
								members.push_back(mPair);
							}
						}
					}
				}
			}
			freeReplyObject(m_redisReply);
			m_redisReply = NULL;
			return ret;
		}
		BCLib::uint64 CRedisClient::zscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, std::vector<std::pair<std::string, double>> &members, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			if (key == NULL || subkey == NULL)
			{
				return 0;
			}
			char strKey[1024] = { 0 };
			snprintf(strKey, 1024, "%s:[%llu]:%s", key, uniqueid, subkey);
			return zscan(strKey, matchKey, members, start, count, type);
		}
		CRedLock *CRedisClient::getRedLock(EREDIS_CONTEXT_TYPE type)
		{
			if (!checkConnect(type))
			{
				return NULL;
			}
			return m_pRedLock;
		}
		std::string CRedisClient::DoubleToString(const double value, BCLib::uint32 precisionAfterPoint/* = 6*/)
		{
			std::ostringstream out;
			// 清除默认精度
			out.precision(std::numeric_limits<double>::digits10);
			out << value;

			std::string res = std::move(out.str());
			auto pos = res.find('.');
			if (pos == std::string::npos)
				return res;

			auto splitLen = pos + 1 + precisionAfterPoint;
			if (res.size() <= splitLen)
				return res;

			return res.substr(0, splitLen);
		}
		
	}//Redis
}//MWLib