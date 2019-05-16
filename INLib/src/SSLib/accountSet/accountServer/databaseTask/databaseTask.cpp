//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/databaseTask/databaseTask.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////

#include <BCLib/utility/logFile.h>
#include <BCLib/security/hash.h>
#include "databaseTask.h"
#include "../databaseMgr/databaseMgr.h"

namespace SSLib
{
namespace AccountServer
{

CDatabaseTask::CDatabaseTask()
:m_stubID(BCLib::Network::INVALID_TCPSTUBID)
,m_echoID(0)
{
}

CDatabaseTask::~CDatabaseTask()
{
}

bool CDatabaseTask::HashIndex(BCLib::uint64 uHash, BCLib::Utility::CStringA& strDBIndex, BCLib::Utility::CStringA& strTBIndex)
{
	BCLib::uint32 dbIndex = CDatabaseMgr::singleton().getDBIndex(uHash);
	BCLib::uint32 tbIndex = CDatabaseMgr::singleton().getTBIndex(uHash);
	strDBIndex.format("%03d",dbIndex+1);
	strTBIndex.format("%03d",tbIndex+1);
	return true;
}

bool CDatabaseTask::HashIndex(BCLib::Utility::CStringA& strHashKey, BCLib::Utility::CStringA& strDBIndex, BCLib::Utility::CStringA& strTBIndex)
{
	strHashKey.toLower();
	BCLib::uint32 hashID = BCLib::Security::CHash::getBKDRHash(strHashKey.c_str());
	return HashIndex(hashID, strDBIndex, strTBIndex);
}

BCLib::Database::CConnection* CDatabaseTask::GetConnection(BCLib::Database::CConnectionMap & mapConnections, std::string strDBIndex, std::string& strDBName)
{
	std::string strNum;
	for (BCLib::Database::CConnectionMap::iterator it = mapConnections.begin(); it != mapConnections.end(); ++it)
	{
		strNum = it->first;
		if (strNum.substr(strNum.length()-3) == strDBIndex.c_str())
		{
			strDBName = it->first;
			return it->second;
		}
	}

	return NULL;
}

}//AccountServer
}//SSLib