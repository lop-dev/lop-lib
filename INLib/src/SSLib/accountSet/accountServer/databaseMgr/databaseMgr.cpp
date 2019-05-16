//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   SSLib/accountSet/accountServer/databaseMgr/databaseMgr.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <sstream>
#include <BCLib/utility/baseDec.h>
#include <BCLib/utility/string.h>
#include <BCLib/network/sockAddr.h>
#include <BCLib/utility/logFile.h>
#include <BCLib/network/sockAddr.h>
#include <BCLib/security/hash.h>
#include <BCLib/utility/io/xmlFile.h>
#include "databaseMgr.h"
#include "../databaseTask/databaseTaskMgr.h"

namespace SSLib
{
namespace AccountServer
{

BCLIB_SINGLETON_DEFINE(CDatabaseMgr);

CDatabaseMgr::CDatabaseMgr()
:m_bInit(false)
,m_dbMaxNum(0)
,m_tbMaxNum(0)

{}

CDatabaseMgr::~CDatabaseMgr()
{
}

bool CDatabaseMgr::init(std::string strDbFile)
{
	if (m_bInit)
	{
		return false;
	}

	BCLib::Utility::CXmlFile dbSetFile;
	if (!dbSetFile.loadFile(strDbFile))
	{
		return false;
	}

	std::string strDBName;
	std::string strDBType;
	std::string strDBHost;
	std::string strDBPort;
	std::string strUserName;
	std::string strPassword;

	BCLib::int32 dbCount = 0;
	BCLib::int32 tbCount = 0;

	BCLib::Utility::HXmlNode root = dbSetFile.getRootNode("root");
	BCLib::Utility::CXmlFile::getNodeAttrValue(root,"TableAmount",tbCount);
	BCLib::Utility::HXmlNode node = BCLib::Utility::CXmlFile::getChildNode(root);
	while (node)
	{
		BCLib::Database::CDBInfo* pDBInfo = new BCLib::Database::CDBInfo();
		BCLib::Utility::CXmlFile::getNodeAttrValue(node,"DBName",strDBName);
		pDBInfo->setDBName(strDBName);
		BCLib::Utility::CXmlFile::getNodeAttrValue(node,"DBHost",strDBHost);
		BCLib::Utility::CXmlFile::getNodeAttrValue(node,"DBPort",strDBPort);
		pDBInfo->setDBServer("tcp://" + strDBHost + ":" + strDBPort);
		BCLib::Utility::CXmlFile::getNodeAttrValue(node,"DBUser",strUserName);
		pDBInfo->setUserName(strUserName);
		BCLib::Utility::CXmlFile::getNodeAttrValue(node,"DBPassword",strPassword);
		pDBInfo->setPassword(strPassword);
		BCLib::Utility::CXmlFile::getNodeAttrValue(node,"DBType",strDBType);
		pDBInfo->setDBType(strDBType);
		m_vecDBInfo.push_back(pDBInfo);
		node = BCLib::Utility::CXmlFile::getNextNode(node);
		++dbCount;
	}

	if (m_vecDBInfo.size() == 0)
	{
		BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "no db connect!!!!");
		return false;
	}

	if (!CDatabaseTaskMgr::singleton().init(m_vecDBInfo))
	{
		BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "CDatabaseTaskMgr init Failed!");
		return false;
	}

	BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "LocalBD init Succeed");

	m_dbMaxNum = dbCount;
	m_tbMaxNum = tbCount;

	m_bInit = true;
	return true;
}

}//AccountServer
}//SSLib