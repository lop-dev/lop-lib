#ifndef QBUS_QBUS_CONSUMER_H_
#define QBUS_QBUS_CONSUMER_H_

#include <string>
#include <vector>
#include "qbus_logger.h"

//-------------------------------------------------------------
//struct rd_kafka_message_s;

namespace qbus {

	class QbusConsumerImp;
	class QbusConfigLoader;

	typedef void (*RebalanceCallback)(void *arg);
	struct QbusMsgContentInfo {
		//std::string topic;
		const char* msg;
		int msg_len;
		void *rd_message;
	};
	
	class QbusConsumer {
	public:
		QbusConsumer();
		~QbusConsumer();

	public:
		bool init(const QbusConfigLoader& config_loader_);
		bool start();
		void stop();

		bool consume(QbusMsgContentInfo& msg_content_QBUS_INFO);
		bool commitOffset(const QbusMsgContentInfo& qbusMsgContentInfo);
		bool destroy(const QbusMsgContentInfo& qbusMsgContentInfo);
		bool setRebalanceCallback(RebalanceCallback cb, void* arg);
		//void RebalanceAssign();
	private:
		QbusConsumerImp* qbus_consumer_imp_;

	private:
		QbusConsumer(const QbusConsumer&);
		QbusConsumer& operator=(const QbusConsumer&);
	};
} //namespace qbus
#endif //#ifndef QBUS_QBUS_CONSUMER_H_
