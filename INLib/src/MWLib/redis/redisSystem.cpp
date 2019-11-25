//////////////////////////////////////////////////////////////////////
//  created:    2014/07/18
//  filename:   MWLib/redis/redisSystem.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <MWLib/redis/redisSystem.h>
#include <MWLib/redis/redisClient.h>
#include <BCLib/utility/string.h>
#include <BCLib/utility/convert.h>
#include <BCLib/utility/logFile.h>
#include <BCLib/utility/baseDef.h>

#include <sstream>
namespace MWLib
{
	namespace Redis
	{
		BCLIB_SINGLETON_DEFINE_WITH_LEVELID(CRedisSystem, 100) // 原来是 SHLib::E_SINGLETON_LEVEL_REDIS 变量，临时这么处理

		CRedisSystem::CRedisSystem()
			:m_eAccessRight(E_REDIS_READ_AND_WRITE)
		{
			m_redisClientMap.clear();
		}
		CRedisSystem:: ~CRedisSystem()
		{
			for (auto it = m_redisClientMap.begin(); it != m_redisClientMap.end(); ++it)
			{
				BCLIB_SAFE_DELETE(it->second);
			}
			m_redisClientMap.clear();
		}

		bool CRedisSystem::init(EREDIS_ACCESS_RIGHT_TYPE type)
		{
			m_eAccessRight = type;
			return true;
		}
		bool CRedisSystem::isValid(EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->isValid( type );
			}
			BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "初始化RedisClient CRedisSystem::isValid = false");
			return false;
		}
		void CRedisSystem::setInfo(std::string& host, int port, std::string& passwd, EREDIS_CONTEXT_TYPE type)
		{
			std::unordered_map<BCLib::uint16, REDIS_CONNECT_INFO>::iterator it = m_redisConnectInfoMap.find(type);
			if (it == m_redisConnectInfoMap.end())
			{
				REDIS_CONNECT_INFO info;
				info.m_host = host;
				info.m_port = port;
				info.m_passwd = passwd;
				m_redisConnectInfoMap[type] = info;
			}
			return;
		}
		CRedisClient* CRedisSystem::getRedisClient()
		{
			BCLib::Utility::CMutexFun fun(&m_mutex);
			std::thread::id tid = std::this_thread::get_id();
			auto it = m_redisClientMap.find(tid);
			if (it != m_redisClientMap.end())
			{
				return it->second;
			}
			return _createRedisClient(tid);		
		}
		CRedisClient* CRedisSystem::_createRedisClient(std::thread::id tid)
		{
			auto it = m_redisClientMap.find(tid);
			if (it != m_redisClientMap.end())
			{
				return it->second;
			}
			CRedisClient* pRedisClient = new CRedisClient();
			if (pRedisClient == NULL)
			{
				return NULL;
			}
			BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "初始化RedisClient m_redisConnectInfoMap = [%llu].size = [%d]... this = [%llu]", (BCLib::uint64)&m_redisConnectInfoMap, m_redisConnectInfoMap.size(), (BCLib::uint64)this);
			for (auto iter = m_redisConnectInfoMap.begin(); iter != m_redisConnectInfoMap.end(); ++iter)
			{
				BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "初始化RedisClient host = [%s]... port = [%d]... type[%d]", iter->second.m_host.c_str(), iter->second.m_port, iter->first);
				pRedisClient->setInfo(iter->second.m_host, iter->second.m_port, iter->second.m_passwd, (EREDIS_CONTEXT_TYPE)iter->first);
			}
			
			pRedisClient->init(m_eAccessRight);
			m_redisClientMap[tid] = pRedisClient;
			BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "初始化RedisClient threadID = [%d]... m_redisClientMap.size() = %d", tid, m_redisClientMap.size());
			return pRedisClient;
		}
		void CRedisSystem::removeRedisClient()
		{
			BCLib::Utility::CMutexFun fun(&m_mutex);
			std::thread::id tid = std::this_thread::get_id();
			auto it = m_redisClientMap.find(tid);
			if (it != m_redisClientMap.end())
			{
				BCLIB_SAFE_DELETE(it->second);
				it = m_redisClientMap.erase(it);
			}
		}

		bool CRedisSystem::savePTBuf(std::string &key, BCLib::uint64 uniqueid, std::string & strPTbufName, const ::google::protobuf::MessageLite *pPtbuf, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->savePTBuf(key, uniqueid, strPTbufName, pPtbuf, type);
			}
			return false;
		}
		BCLib::uint32 CRedisSystem::loadPTBuf(std::string &key, BCLib::uint64 uniqueid, std::string & strPTbufName, ::google::protobuf::MessageLite *pPtbuf, BCLib::uint32 readBufSize, EREDIS_CONTEXT_TYPE type)
		{
			
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->loadPTBuf(key, uniqueid, strPTbufName, pPtbuf, readBufSize, type);
			}	
			return false;
		}

		void  CRedisSystem::disconnect(EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				pRedisClient->disconnect(type);
			}
		}

		void  CRedisSystem::disconnectAll()
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				pRedisClient->disconnectAll();
			}
		}

		bool  CRedisSystem::exec(const char *cmd, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->exec(cmd, type);
			}
			return false;
		}

		bool CRedisSystem::setCommandTimeout(const struct timeval tv, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->setCommandTimeout(tv, type);
			}
			return false;
		}
		bool CRedisSystem::checkStatus(EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->checkStatus(type);
			}
			return false;
		}

		bool CRedisSystem::clear(EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->clear(type);
			}
			return false;
		}

		bool CRedisSystem::setString(const char *key, const char *value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->setString(key, value, type);
			}
			return false;
		}
		bool CRedisSystem::setString(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->setString(key, uniqueid, subkey, value, type);
			}
			return false;
		}

		std::string CRedisSystem::getString(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->getString(key, type);
			}
			return "";
		}
		std::string CRedisSystem::getString(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->getString(key, uniqueid, subkey, type);
			}
			return "";
		}

		bool CRedisSystem::setUint64(const char *key, BCLib::uint64 value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->setUint64(key, value, type);
			}
			return false;
		}
		bool CRedisSystem::setUint64(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::uint64 value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->setUint64(key, uniqueid, subkey, value, type);
			}
			return false;
		}

		BCLib::uint64 CRedisSystem::getUint64(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->getUint64(key, type);
			}
			return 0;
		}
		BCLib::uint64 CRedisSystem::getUint64(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->getUint64(key, uniqueid, subkey, type);
			}
			return 0;
		}

		bool CRedisSystem::setBin(const char *key, const char *value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->setBin(key, value, len, type);
			}
			return false;
		}
		bool CRedisSystem::setBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->setBin(key, uniqueid, subkey, value, len, type);
			}
			return false;
		}

		BCLib::uint32 CRedisSystem::getBin(const char *key, char *value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->getBin(key, value, len, type);
			}
			return 0;
		}
		BCLib::uint32 CRedisSystem::getBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, char *value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->getBin(key, uniqueid, subkey, value, len, type);
			}
			return 0;
		}
		bool CRedisSystem::delKey(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->delKey(key, type);
			}
			return false;
		}
		bool CRedisSystem::delKey(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->delKey(key, uniqueid, subkey, type);
			}
			return false;
		}

		//设置消亡时间 单位s -1 代表永久
		bool CRedisSystem::expireSecond(const char *key, BCLib::int32 second, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->expireSecond(key, second, type);
			}
			return false;
		}
		bool CRedisSystem::expireSecond(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 second, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->expireSecond(key, uniqueid, subkey, second, type);
			}
			return false;
		}


		bool CRedisSystem::persist(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->persist(key, type);
			}
			return false;
		}
		bool CRedisSystem::persist(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->persist(key, uniqueid, subkey, type);
			}
			return false;
		}

		
		bool CRedisSystem::keys(const char *key, std::vector<std::string> &values, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->keys(key, values, type);
			}
			return false;
		}

		
		BCLib::uint64 CRedisSystem::scan(const char *matchKey, std::vector<std::string> &values, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->scan(matchKey, values, start, count, type);
			}
			return 0;
		}

		
		bool CRedisSystem::hsetString(const char*key, const char*field, const char*value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hsetString(key, field, value, type);
			}
			return false;
		}

		
		bool CRedisSystem::hsetString(const char*key, const char*field, const std::string &value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hsetString(key, field, value, type);
			}
			return false;
		}

		
		bool CRedisSystem::hsetBin(const char *key, const char *field, const char *value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hsetBin(key, field, value, len, type);
			}
			return false;
		}

		
		bool CRedisSystem::hsetString(const char*key, BCLib::uint64 uniqueid, const char*subkey, const char*field, const char*value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hsetString(key, uniqueid, subkey, field, value, type);
			}
			return false;
		}

		bool CRedisSystem::hsetString(const char*key, BCLib::uint64 uniqueid, const char*subkey, const char*field, const std::string &value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hsetString(key, uniqueid, subkey, field, value, type);
			}
			return false;
		}

		
		bool CRedisSystem::hsetBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *field, const char *value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hsetBin(key, uniqueid, subkey, field, value, len, type);
			}
			return false;
		}

		
		std::string CRedisSystem::hgetString(const char*key, const char*field, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hgetString(key, field, type);
			}
			return "";
		}
		
		BCLib::uint32 CRedisSystem::hgetBin(const char *key, const char *field, char *value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hgetBin(key, field, value, len, type);
			}
			return 0;
		}

		
		std::string CRedisSystem::hgetString(const char*key, BCLib::uint64 uniqueid, const char*subkey, const char*field, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hgetString(key, uniqueid, subkey, field, type);
			}
			return "";
		}

		
		BCLib::uint32 CRedisSystem::hgetBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *field, char *value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hgetBin(key, uniqueid, subkey, field, value, len, type);
			}
			return 0;
		}

		
		void CRedisSystem::hgetall(const char *key, std::map<std::string, std::string>& field_values, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hgetall(key, field_values, type);
			}
			return;
		}

		
		const std::map<std::string, std::pair<char*, BCLib::uint32>*>* CRedisSystem::hgetallBin(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hgetallBin(key, type);
			}
			return NULL;
		}

		
		void CRedisSystem::hgetall(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::map<std::string, std::string>& field_values, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hgetall(key, uniqueid, subkey, field_values, type);
			}
			return;
		}

		
		const std::map<std::string, std::pair<char*, BCLib::uint32>*>* CRedisSystem::hgetallBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hgetallBin(key, uniqueid, subkey, type);
			}
			return NULL;
		}

		
		void CRedisSystem::hmset(const char *key, EREDIS_CONTEXT_TYPE type, std::string format, ...)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				va_list ap;
				va_start(ap, format);
				pRedisClient->hmset(key, type, format, ap);
				va_end(ap);
			  
			}
			return;
		}

		
		void CRedisSystem::hmset(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type, std::string format, ...)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				va_list ap;
				va_start(ap, format);
				pRedisClient->hmset(key, uniqueid, subkey, type, format, ap);
				va_end(ap);

			}
			return;
		}

		
		bool CRedisSystem::herase(const char *key, const char *field, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->herase(key, field, type);
			}
			return false;
		}

		
		bool CRedisSystem::herase(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *field, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->herase(key, uniqueid, subkey, field, type);
			}
			return false;
		}

		
		BCLib::uint64 CRedisSystem::hscan(const char *key, const char *matchKey, std::map<std::string, std::string>& field_values, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hscan(key, matchKey, field_values, start, count, type);
			}
			return 0;
		}

		
		BCLib::uint64 CRedisSystem::hscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, std::map<std::string, std::string>& field_values, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hscan(key, uniqueid, subkey, matchKey, field_values, start, count, type);
			}
			return 0;
		}

		
		const std::map<std::string, std::pair<char*, BCLib::uint32>*>* CRedisSystem::hscan(const char *key, const char *matchKey, BCLib::uint64 &end, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hscan(key, matchKey, end, start, count, type);
			}
			return NULL;
		}
		
		const std::map<std::string, std::pair<char*, BCLib::uint32>*>* CRedisSystem::hscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, BCLib::uint64 &end, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hscan(key, uniqueid, subkey, matchKey, end, start, count, type);
			}
			return NULL;
		}

		BCLib::uint32 CRedisSystem::hlen(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hlen(key, type);
			}
			return 0;
		}

		BCLib::uint32 CRedisSystem::hlen(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->hlen(key, uniqueid, subkey, type);
			}
			return 0;
		}

		bool CRedisSystem::lpushString(const char *key, const char *value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lpushString(key, value, type);
			}
			return false;
		}

		
		bool CRedisSystem::lpushBin(const char *key, const char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lpushBin(key, value, len, type);
			}
			return false;
		}

		
		bool CRedisSystem::lpushString(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lpushString(key, uniqueid, subkey, value, type);
			}
			return false;
		}

		
		bool CRedisSystem::lpushBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lpushBin(key, uniqueid, subkey, value, len, type);
			}
			return false;
		}
		
		std::string CRedisSystem::lreadString(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lreadString(key, type);
			}
			return "";
		}

		
		BCLib::uint32 CRedisSystem::lreadBin(const char *key, char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lreadBin(key, value, len, type);
			}
			return 0;
		}

		
		std::string CRedisSystem::lreadString(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lreadString(key, uniqueid, subkey, type);
			}
			return "";
		}

		
		BCLib::uint32 CRedisSystem::lreadBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lreadBin(key, uniqueid, subkey, value, len, type);
			}
			return 0;
		}

		
		bool CRedisSystem::rpushString(const char *key, const char *value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rpushString(key, value, type);
			}
			return false;
		}

		
		bool CRedisSystem::rpushBin(const char *key, const char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rpushBin(key, value, len, type);
			}
			return false;
		}

		
		bool CRedisSystem::rpushString(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rpushString(key, uniqueid, subkey, value, type);
			}
			return false;
		}

		bool CRedisSystem::rpushBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rpushBin(key, uniqueid, subkey, value, len, type);
			}
			return false;
		}

		
		std::string CRedisSystem::rreadString(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rreadString(key, type);
			}
			return "";
		}

		
		BCLib::uint32 CRedisSystem::rreadBin(const char *key, char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rreadBin(key, value, len, type);
			}
			return 0;
		}

		
		std::string CRedisSystem::rreadString(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rreadString(key, uniqueid, subkey, type);
			}
			return "";
		}

		
		BCLib::uint32 CRedisSystem::rreadBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rreadBin(key, uniqueid, subkey, value, len, type);
			}
			return 0;
		}

		
		BCLib::int32 CRedisSystem::llen(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->llen(key, type);
			}
			return 0;
		}

		
		BCLib::int32 CRedisSystem::llen(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->llen(key, uniqueid, subkey, type);
			}
			return 0;
		}
		
		std::string CRedisSystem::lpopString(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lpopString(key, type);
			}
			return "";
		}

		
		BCLib::uint32  CRedisSystem::lpopBin(const char *key, char*value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lpopBin(key, value, len, type);
			}
			return 0;
		}

		
		std::string CRedisSystem::lpopString(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lpopString(key, uniqueid, subkey, type);
			}
			return "";
		}

		
		BCLib::uint32 CRedisSystem::lpopBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, char*value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lpopBin(key, uniqueid, subkey, value, len, type);
			}
			return 0;
		}

		
		std::string CRedisSystem::rpopString(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rpopString(key, type);
			}
			return "";
		}

		
		BCLib::uint32 CRedisSystem::rpopBin(const char *key, char*value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rpopBin(key, value, len, type);
			}
			return 0;
		}

		
		std::string CRedisSystem::rpopString(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rpopString(key, uniqueid, subkey, type);
			}
			return "";
		}

		
		BCLib::uint32 CRedisSystem::rpopBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, char*value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rpopBin(key, uniqueid, subkey, value, len, type);
			}
			return 0;
		}

		
		bool CRedisSystem::lsetString(const char *key, BCLib::int32 index, const char *value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lsetString(key, index, value, type);
			}
			return false;
		}

		
		bool CRedisSystem::lsetBin(const char *key, BCLib::int32 index, char*value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lsetBin(key, index, value, len, type);
			}
			return false;
		}

		
		bool CRedisSystem::lsetString(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 index, const char *value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lsetString(key, uniqueid, subkey, index, value, type);
			}
			return false;
		}

		
		bool CRedisSystem::lsetBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 index, char*value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lsetBin(key, uniqueid, subkey, index, value, len, type);
			}
			return false;
		}

		
		bool CRedisSystem::rpoplpush(const char *keyTar, const char *keySrc, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rpoplpush(keyTar, keySrc, type);
			}
			return false;
		}

		
		bool CRedisSystem::rpoplpush(const char *keyTar, BCLib::uint64 uniqueidTar, const char *subkeyTar, const char *keySrc, BCLib::uint64 uniqueidSrc, const char *subkeySrc, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->rpoplpush(keyTar, uniqueidTar, subkeyTar, keySrc, uniqueidSrc, subkeySrc, type);
			}
			return false;
		}

		
		bool CRedisSystem::lgetAllElement(const char *key, std::list<std::string>& mylist, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lgetAllElement(key, mylist, type);
			}
			return false;
		}

		
		const std::list<std::pair<char*, BCLib::uint32>*>* CRedisSystem::lgetAllElement(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lgetAllElement(key, type);
			}
			return NULL;
		}


		bool CRedisSystem::lgetAllElement(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::list<std::string>& mylist, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lgetAllElement(key, uniqueid, subkey, mylist, type);
			}
			return false;
		}

		
		const std::list<std::pair<char*, BCLib::uint32>*>*  CRedisSystem::lgetAllElement(const char *key, BCLib::uint64 uniqueid, const char*subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lgetAllElement(key, uniqueid, subkey, type);
			}
			return NULL;
		}

		
		bool CRedisSystem::lrem(const char *key, BCLib::int32 count, char*value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lrem(key, count, value, type);
			}
			return false;
		}

		bool CRedisSystem::lrem(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 count, char *value, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lrem(key, uniqueid, subkey, count, value, type);
			}
			return false;
		}

		
		bool CRedisSystem::lrem(const char *key, BCLib::int32 count, char *value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lrem(key, count, value, len, type);
			}
			return false;
		}

		
		bool CRedisSystem::lrem(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 count, char *value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->lrem(key, uniqueid, subkey, count, value, len, type);
			}
			return false;
		}

		
		bool CRedisSystem::sadd(const char *key, EREDIS_CONTEXT_TYPE type, std::string members, ...)
		{
			bool ret = false;
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				va_list ap;
				va_start(ap, members);
				ret = pRedisClient->sadd(key, type, members, ap);
				va_end(ap);
			}
			return ret;
		}

		
		bool CRedisSystem::sadd(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type, std::string members, ...)
		{
			bool ret = false;
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				va_list ap;
				va_start(ap, members);
				ret = pRedisClient->sadd(key, uniqueid,subkey, type, members, ap);
				va_end(ap);
			}
			return ret;
		}

		
		BCLib::uint32 CRedisSystem::scard(const char *key, EREDIS_CONTEXT_TYPE type)
		{
		
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->scard(key, type);
			}
			return 0;
		}

		
		BCLib::uint32 CRedisSystem::scard(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->scard(key, uniqueid, subkey, type);
			}
			return 0;
		}

		
		bool CRedisSystem::sismember(const char *key, const char *member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->sismember(key, member, type);
			}
			return false;
		}

		
		bool CRedisSystem::sismember(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->sismember(key, uniqueid, subkey, member, type);
			}
			return false;
		}

		
		bool CRedisSystem::sgetAllMembers(const char *key, std::set<std::string> &mySet, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->sgetAllMembers(key, mySet, type);
			}
			return false;
		}

		
		bool CRedisSystem::sgetAllMembers(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::set<std::string> &mySet, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->sgetAllMembers(key, uniqueid, subkey, mySet, type);
			}
			return false;
		}

		
		bool CRedisSystem::spop(const char *key, std::string &member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->spop(key, member, type);
			}
			return false;
		}

		
		bool CRedisSystem::spop(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::string &member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->spop(key, uniqueid, subkey, member, type);
			}
			return false;
		}

		
		bool CRedisSystem::sremove(const char *key, const std::string members, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->sremove(key, members, type);
			}
			return false;
		}

		
		bool CRedisSystem::sremove(const char *key, BCLib::uint64 uniqueid, const char *subkey, const std::string members, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->sremove(key, uniqueid, subkey, members, type);
			}
			return false;
		}

		
		bool CRedisSystem::smove(const char *disKey, const char *srcKey, const std::string member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->smove(disKey, srcKey, member, type);
			}
			return false;
		}

		bool CRedisSystem::smove(const char *disKey, BCLib::uint64 disUniqueid, const char *disSubkey, const char *srcKey, BCLib::uint64 srcUniqueid, const char *srcSubkey, const std::string member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->smove(disKey, disUniqueid, disSubkey, srcKey, srcUniqueid, srcSubkey, member, type);
			}
			return false;
		}

		
		bool CRedisSystem::sinter(std::set<std::string> &set, EREDIS_CONTEXT_TYPE type, std::string keys, ...)
		{
			bool ret = false;
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				va_list ap;
				va_start(ap, keys);
				ret = pRedisClient->sinter(set, type, keys, ap);
				va_end(ap);
			}
			return ret;
		}
		
		bool CRedisSystem::sinter(std::set<std::string> &set, const std::vector<std::string> &keys, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->sinter(set, keys, type);
			}
			return false;
		}
		
		BCLib::uint64 CRedisSystem::sscan(const char *key, const char *matchKey, std::set<std::string> &mySet, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->sscan(key, matchKey, mySet, start, count, type);
			}
			return 0;
		}

		BCLib::uint64 CRedisSystem::sscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, std::set<std::string> &mySet, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->sscan(key, uniqueid, subkey, matchKey, mySet, start, count, type);
			}
			return 0;
		}

		
		bool CRedisSystem::zadd(const char *key, const std::vector<double> &scores, const std::vector<std::string> &members, BCLib::uint32 precision, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zadd(key, scores, members, precision, type);
			}
			return false;
		}

		
		bool CRedisSystem::zadd(const char *key, BCLib::uint64 uniqueid, const char *subkey, const std::vector<double> &scores, const std::vector<std::string> &members, BCLib::uint32 precision, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zadd(key, uniqueid, subkey, scores, members, precision, type);
			}
			return false;
		}

		
		BCLib::uint32  CRedisSystem::zcard(const char *key, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zcard(key, type);
			}
			return 0;
		}

		
		BCLib::uint32  CRedisSystem::zcard(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zcard(key, uniqueid, subkey, type);
			}
			return 0;
		}

		
		BCLib::int32 CRedisSystem::zrank(const char *key, const char *member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrank(key, member, type);
			}
			return 0;
		}

		
		BCLib::int32 CRedisSystem::zrank(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrank(key, uniqueid, subkey, member, type);
			}
			return 0;
		}

		
		BCLib::int32 CRedisSystem::zrevRank(const char *key, const char *member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrevRank(key, member, type);
			}
			return 0;
		}

		
		BCLib::int32 CRedisSystem::zrevRank(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrevRank(key, uniqueid, subkey, member, type);
			}
			return 0;
		}

		
		bool CRedisSystem::zrem(const char *key, std::vector<std::string> &members, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrem(key, members, type);
			}
			return false;
		}

		
		bool CRedisSystem::zrem(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::vector<std::string> &members, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrem(key, uniqueid, subkey, members, type);
			}
			return false;
		}

		
		bool CRedisSystem::zrange(const char *key, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrange(key, members, startIndex, stopIndex, type);
			}
			return false;
		}

		
		bool CRedisSystem::zrange(const char *key, const BCLib::uint64 uniqueid, const char *subkey, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrange(key, uniqueid, subkey, members, startIndex, stopIndex, type);
			}
			return false;
		}

		
		bool CRedisSystem::zrevRange(const char *key, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrevRange(key, members, startIndex, stopIndex, type);
			}
			return false;
		}

		bool CRedisSystem::zrevRange(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrevRange(key, uniqueid, subkey, members, startIndex, stopIndex, type);
			}
			return false;
		}

		
		double CRedisSystem::zscore(const char *key, const char *member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zscore(key, member, type);
			}
			return 0.0f;
		}

		double CRedisSystem::zscore(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *member, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zscore(key, uniqueid, subkey, member, type);
			}
			return 0.0f;
		}

		bool CRedisSystem::zremRangeByRank(const char *key, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zremRangeByRank(key, startIndex, stopIndex, type);
			}
			return false;
		}

		bool CRedisSystem::zremRangeByRank(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 startIndex, BCLib::int32 stopIndex, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zremRangeByRank(key, uniqueid, subkey, startIndex, stopIndex, type);
			}
			return false;
		}

		bool CRedisSystem::zincrby(const char *key, const char *member, double increment, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zincrby(key, member, increment, type);
			}
			return false;
		}

		bool CRedisSystem::zincrby(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *member, double increment, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zincrby(key, uniqueid, subkey, member, increment, type);
			}
			return false;
		}

		BCLib::int32 CRedisSystem::zcount(const char *key, double min, double max, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zcount(key, min, max, type);
			}
			return 0;
		}

		BCLib::int32 CRedisSystem::zcount(const char *key, BCLib::uint64 uniqueid, const char *subkey, double min, double max, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zcount(key, uniqueid, subkey, min, max, type);
			}
			return 0;
		}

		bool CRedisSystem::zrangeByScore(const char *key, std::vector<std::pair<std::string, double>> &members, const char *minStr, const char *maxStr, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrangeByScore(key, members, minStr, maxStr, type);
			}
			return false;
		}

		bool CRedisSystem::zrangeByScore(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::vector<std::pair<std::string, double>> &members, const char *minStr, const char *maxStr, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrangeByScore(key, uniqueid, subkey, members, minStr, maxStr, type);
			}
			return false;
		}

	
		bool CRedisSystem::zrevRangeByScore(const char *key, std::vector<std::pair<std::string, double>> &members, const char *maxStr, const char *minStr, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrevRangeByScore(key, members, maxStr, minStr, type);
			}
			return false;
		}

		
		bool CRedisSystem::zrevRangeByScore(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::vector<std::pair<std::string, double>> &members, const char *maxStr, const char *minStr, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zrevRangeByScore(key, uniqueid, subkey, members, maxStr, minStr, type);
			}
			return false;
		}

		
		BCLib::uint64 CRedisSystem::zscan(const char *key, const char *matchKey, std::vector<std::pair<std::string, double>> &members, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zscan(key, matchKey, members, start, count, type);
			}
			return 0;
		}

		
		BCLib::uint64 CRedisSystem::zscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, std::vector<std::pair<std::string, double>> &members, BCLib::uint64 start, BCLib::uint64 count, EREDIS_CONTEXT_TYPE type)
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->zscan(key, uniqueid, subkey, matchKey, members, start, count, type);
			}
			return 0;
		}

		CRedLock *CRedisSystem::getRedLock(EREDIS_CONTEXT_TYPE type )
		{
			CRedisClient* pRedisClient = getRedisClient();
			if (pRedisClient != NULL)
			{
				return pRedisClient->getRedLock(type);
			}
			return NULL;
		}
    }//Redis
}//MWLib
