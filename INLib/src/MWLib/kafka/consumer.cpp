//////////////////////////////////////////////////////////////////////
//  created:    2018/10/29
//  filename:   MWLib/kafka/consumer.cpp
//  author:     xums
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <MWLib/kafka/consumer.h>

#include <qbus_config.h>
#include <qbus_constant.h>
#include <qbus_consumer.h>


namespace MWLib
{
	namespace Kafka
	{
		CKafkaConsumer::CKafkaConsumer()
			: m_consumer(NULL)
			, m_msgContent(NULL)
		{

		}

		CKafkaConsumer::~CKafkaConsumer()
		{
			if (m_consumer)
			{
				delete m_consumer;
				m_consumer = NULL;
			}
			if (m_msgContent)
			{
				delete m_msgContent;
				m_msgContent = NULL;
			}
		}


		bool CKafkaConsumer::init(const char* configXmlPath)
		{
			auto logLevel = BCLIB_LOG_GET_MODULE_LEVEL(ELOGMODULE_MWLIB_KAFKA);
			qbus::Logger::init(logLevel, kafkaLogger);
			if (NULL != m_consumer) {
				return false;
			}
			BCLib::Utility::CXmlFile xmlFile;
			if (!xmlFile.loadFile(configXmlPath)) {
				BCLIB_LOG_ERROR(ELOGMODULE_MWLIB_KAFKA, "load consumer xml config file fail");
				return false;
			}
			XmlNode rootNode = xmlFile.getRootNode();
			if (!rootNode) {
				BCLIB_LOG_ERROR(ELOGMODULE_MWLIB_KAFKA, "parse consumer xml config file fail");
				return false;
			}

			qbus::QbusConfigLoader configLoader;

			XmlNode globalNode = BCLib::Utility::CXmlFile::getChildNode(rootNode, "global");
			MapNameValue globalConfig;
			getXmlChildNodeNameValue(globalNode, globalConfig);
			configLoader.setGlobalConfig(globalConfig);

			XmlNode topicNode = BCLib::Utility::CXmlFile::getChildNode(rootNode, "topic");
			MapNameValue topicConfig;
			getXmlChildNodeNameValue(topicNode, topicConfig);
			configLoader.setTopicConfig(topicConfig);

			XmlNode sdkNode = BCLib::Utility::CXmlFile::getChildNode(rootNode, "sdk");
			MapNameValue sdkConfig;
			getXmlChildNodeNameValue(sdkNode, sdkConfig);
			configLoader.setSdkConfig(sdkConfig);


			BCLIB_LOG_INFOR(ELOGMODULE_MWLIB_KAFKA, "begin init consumer");

			m_consumer = new qbus::QbusConsumer();
			if (!m_consumer->init(configLoader)) {
				BCLIB_LOG_ERROR(ELOGMODULE_MWLIB_KAFKA, "consumer init fail~");
				return false;
			}
			m_msgContent = new qbus::QbusMsgContentInfo();
			return true;
		}

		bool CKafkaConsumer::start()
		{
			if (NULL == m_consumer) {
				return false;
			}
			return m_consumer->start();
		}

		void CKafkaConsumer::stop()
		{
			if (NULL == m_consumer) {
				return;
			}

			return m_consumer->stop();
		}


		bool CKafkaConsumer::consume(ConsumeMsg& msg)
		{
			if (m_consumer)
			{
				if (m_consumer->consume(*m_msgContent))
				{
					msg.m_buf = m_msgContent->msg;
					msg.m_len = m_msgContent->msg_len;
					msg.m_arg = m_msgContent->rd_message;
					return true;
				}
			}
			return false;
		}

		bool CKafkaConsumer::commit(const ConsumeMsg& msg)
		{
			if (m_consumer)
			{
				m_msgContent->msg = msg.m_buf;
				m_msgContent->msg_len = msg.m_len;
				m_msgContent->rd_message = msg.m_arg;

				return m_consumer->commitOffset(*m_msgContent);
			}
			return false;
		}



		bool CKafkaConsumer::destroy(const ConsumeMsg& msg)
		{
			if (m_consumer)
			{
				m_msgContent->msg = msg.m_buf;
				m_msgContent->msg_len = msg.m_len;
				m_msgContent->rd_message = msg.m_arg;

				return m_consumer->destroy(*m_msgContent);
			}
			return false;
		}

		//void CKafkaConsumer::rebalanceAssign()
		//{
		//	if (m_consumer)
		//	{
		//		m_consumer->RebalanceAssign();
		//	}
		//}

		void CKafkaConsumer::setRebalanceCallback(RebalanceCallback cb, void *arg)
		{
			if (m_consumer)
			{
				m_consumer->setRebalanceCallback(cb, arg);
			}
		}



	}//Kafka
}//MWLib

