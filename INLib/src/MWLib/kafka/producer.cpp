//////////////////////////////////////////////////////////////////////
//  created:    2018/10/29
//  filename:   MWLib/kafka/PRODUCER.cpp
//  author:     xums
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <MWLib/kafka/producer.h>

#include <qbus_config.h>
//#include <qbus_constant.h>
#include <qbus_producer.h>

namespace MWLib
{
	namespace Kafka
	{
		
		CKafkaProducer::CKafkaProducer()
			: m_producer(NULL)
		{

		}

		CKafkaProducer::~CKafkaProducer()
		{
			if (m_producer) {
				delete m_producer;
			}
		}


		bool CKafkaProducer::init(const char* configXmlPath)
		{
			auto logLevel = BCLIB_LOG_GET_MODULE_LEVEL(ELOGMODULE_MWLIB_KAFKA);
			qbus::Logger::init(logLevel, kafkaLogger);
			if (NULL != m_producer) {
				return false;
			}
			BCLib::Utility::CXmlFile xmlFile;
			if (!xmlFile.loadFile(configXmlPath)) {
				BCLIB_LOG_ERROR(ELOGMODULE_MWLIB_KAFKA, "load producer xml config file fail");
				return false;
			}
			XmlNode rootNode = xmlFile.getRootNode();
			if (!rootNode) {
				BCLIB_LOG_ERROR(ELOGMODULE_MWLIB_KAFKA, "parse producer xml config file fail");
				return false;
			}

			//qbus::QbusConfigLoader configLoader;

			XmlNode globalNode = BCLib::Utility::CXmlFile::getChildNode(rootNode, "global");
			MapNameValue globalConfig;
			getXmlChildNodeNameValue(globalNode, globalConfig);
			qbus::QbusConfigLoader configLoder;
			configLoder.setGlobalConfig(globalConfig);

			XmlNode topicNode = BCLib::Utility::CXmlFile::getChildNode(rootNode, "topic");
			MapNameValue topicConfig;
			getXmlChildNodeNameValue(topicNode, topicConfig);
			configLoder.setTopicConfig(topicConfig);

			XmlNode sdkNode = BCLib::Utility::CXmlFile::getChildNode(rootNode, "sdk");
			MapNameValue sdkConfig;
			getXmlChildNodeNameValue(sdkNode, sdkConfig);
			configLoder.setSdkConfig(sdkConfig);

			BCLIB_LOG_INFOR(ELOGMODULE_MWLIB_KAFKA, "begin init producer");

			m_producer = new qbus::QbusProducer();
			return m_producer->init(configLoder);
		}

		bool CKafkaProducer::produce(const char* msg, size_t len, const std::string&key)
		{
			if (NULL == m_producer) {
				return false;
			}

			return m_producer->produce(msg, len, key);

		}

		void CKafkaProducer::setProduceCallback(ProduceCallback cb)
		{
			if (NULL != m_producer) {
				m_producer->setProduceCallback(cb);
			}
		}

		void CKafkaProducer::close()
		{
			if (NULL == m_producer) {
				return;
			}
			m_producer->uninit();
		}


	}//Kafka
}//MWLib