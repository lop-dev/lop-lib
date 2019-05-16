//////////////////////////////////////////////////////////////////////
//  created:    2018/10/29
//  filename:   MWLib/kafka/consumer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_KAFKA_CONSUMER_H__
#define __MWLIB_KAFKA_CONSUMER_H__

#include <BCLib/utility/noncopyable.h>
#include "kafka.h"

namespace qbus
{
	class QbusConsumer;
	struct QbusMsgContentInfo;
}
namespace MWLib
{
	namespace Kafka
	{
		typedef void(*RebalanceCallback)(void *arg);

		class ConsumeMsg
		{
		public:
			const char* m_buf;
			int  m_len;
			void* m_arg;
		};

		class MWLIB_KAFKA_API CKafkaConsumer :public BCLib::Utility::CNoncopyable
		{
		public:
			CKafkaConsumer();
			virtual ~CKafkaConsumer();

			bool init(const char* configXmlPath);

			bool start();
			void stop();
			bool consume(ConsumeMsg& msg);
			bool commit(const ConsumeMsg& msg);
			bool destroy(const ConsumeMsg& msg);
			//void rebalanceAssign();
			void setRebalanceCallback(RebalanceCallback cb, void *arg);
		private:
			qbus::QbusConsumer* m_consumer;
			qbus::QbusMsgContentInfo* m_msgContent;
		};
	}//Kafka
}//MWLib

#endif//__MWLIB_KAFKA_KAFKA_H__
