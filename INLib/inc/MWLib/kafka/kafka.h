//////////////////////////////////////////////////////////////////////
//  created:    2018/10/29
//  filename:   MWLib/kafka/kafka.h
//  author:     League of Perfect
/// @brief      libKafka_9md -> kafkabridge -> librdkafka -> zlib openssl..
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_KAFKA_KAFKA_H__
#define __MWLIB_KAFKA_KAFKA_H__

#include <MWLib/utility/logFile.h>
#include <BCLib/utility/io/xmlFile.h>

#ifdef MWLIB_KAFKA_EXPORTS
#    define MWLIB_KAFKA_API _declspec(dllexport)
#elif defined(MWLIB_KAFKA_IMPORTS)
#    define MWLIB_KAFKA_API _declspec(dllimport)
#else
#    define MWLIB_KAFKA_API
#endif

namespace MWLib
{
namespace Kafka
{
typedef BCLib::Utility::HXmlNode XmlNode;
typedef BCLib::Utility::CXmlFile XmlFile;
typedef std::map <std::string, std::string > MapNameValue;

//解析所有子结点
extern MWLIB_KAFKA_API void getXmlChildNodeNameValue(const XmlNode &node, MapNameValue& map);
extern MWLIB_KAFKA_API void kafkaLogger(int level, const char*msg);
}//Kafka
}//MWLib

#endif//__MWLIB_KAFKA_KAFKA_H__
