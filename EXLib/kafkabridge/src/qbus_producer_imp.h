#ifndef QBUS_PRODUCER_IMP_H_
#define QBUS_PRODUCER_IMP_H_

#include <string>

#include "qbus_rdkafka_def.h"


//-----------------------------------------------------
namespace qbus {

	class QbusConfigLoader;

	class QbusProducerImp {
	public:

		QbusProducerImp();
		~QbusProducerImp();

		bool Init(const QbusConfigLoader& config_loader_);
		void SetProduceCallback(ProduceCallback cb);
		void Uninit();

		bool Produce(const char* data, size_t data_len, const std::string& key);

	private:
		static void MsgDeliveredCallback(rd_kafka_s *rk,
			const rd_kafka_message_s *rkmessage, void *opaque);
		static int32_t PartitionHashFunc(const rd_kafka_topic_s *rkt,
			const void *keydata,
			size_t keylen,
			int32_t partition_cnt,
			void *rkt_opaque,
			void *msg_opaque);

		bool InitRdKafkaConfig(const QbusConfigLoader& config_loader_);
		bool InitRdKafkaHandle(const std::string& topic_name);

		bool InternalProduce(const char* data,
			size_t data_len,
			const std::string& key,
			void *opaque);

	private:
		ProduceCallback produce_callback_;
		rd_kafka_conf_s* rd_kafka_conf_;
		rd_kafka_topic_conf_s* rd_kafka_topic_conf_;
		rd_kafka_topic_s* rd_kafka_topic_;
		rd_kafka_s* rd_kafka_handle_;
		int sync_send_err_;

		std::string broker_list_;

		//QbusConfigLoader config_loader_;

		bool is_sync_send_;
		bool is_init_;
		bool is_record_msg_for_send_failed_;
		//bool is_speedup_terminate_;
		//bool fast_exit_;
	};



}
#endif//#define QBUS_PRODUCER_IMP_H_
