//////////////////////////////////////////////////////////////////////
//  created:    2018/10/29
//  filename:   MWLib/kafka/kafka.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <MWLib/kafka/kafka.h>
#include <qbus_logger.h>

namespace MWLib
{
namespace Kafka
{
//日志回调
void kafkaLogger(int level, const char*msg)
{
	switch (level)
	{
	case qbus::Logger::LL_DEBUG:
		BCLIB_LOG_DEBUG(ELOGMODULE_MWLIB_KAFKA, msg);
		break;
	case qbus::Logger::LL_TRACE:
		BCLIB_LOG_TRACE(ELOGMODULE_MWLIB_KAFKA, msg);
		break;
	case qbus::Logger::LL_INFO:
		BCLIB_LOG_INFOR(ELOGMODULE_MWLIB_KAFKA, msg);
		break;
	case qbus::Logger::LL_WARNING:
		BCLIB_LOG_WARNING(ELOGMODULE_MWLIB_KAFKA, msg);
		break;
	case qbus::Logger::LL_ERROR:
		BCLIB_LOG_ERROR(ELOGMODULE_MWLIB_KAFKA, msg);
		break;
	case qbus::Logger::LL_END:
	default:
		break;
	}
}

void getXmlChildNodeNameValue(const XmlNode &node, MapNameValue& map)
{
	XmlNode childNode = XmlFile::getChildNode(node);
	while (childNode)
	{
		std::string name;
		std::string value;
		bool ret = true;
		ret &= XmlFile::getNodeName(childNode, name);
		ret &= XmlFile::getNodeValue(childNode, value);
		if (ret)
		{
			map[name] = value;
		}
		childNode = BCLib::Utility::CXmlFile::getNextNode(childNode);
	}
}
}//Kafka
}//MWLib