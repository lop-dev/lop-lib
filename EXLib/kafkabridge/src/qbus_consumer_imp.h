#ifndef QBUS_CONSUMER_IMP_H_
#define QBUS_CONSUMER_IMP_H_

#include <string>
#include <set>
#include <map>
#include <list>


#include "qbus_consumer.h"
#include "qbus_logger.h"
#include "qbus_rdkafka_init.h"
//------------------------------------------------------------
namespace qbus {
	class QbusConfigLoader;

	class QbusConsumerImp {
	public:
		QbusConsumerImp();
		~QbusConsumerImp();

	public:
		bool Init(const QbusConfigLoader &config_loader_);
		bool Start();
		void Stop();

		bool Consume(QbusMsgContentInfo& msg_content_QBUS_INFO);
		bool CommitOffset(const QbusMsgContentInfo& qbusMsgContentInfo);
		bool Destroy(const QbusMsgContentInfo& qbusMsgContentInfo);
		//void RebalanceAssign();
		//void ManualCommitAll();
		//void ClearWaitDestroyMsgs();
		void SetRebalanceCallback(RebalanceCallback cb, void* arg);
	private:
		static void rdkafka_rebalance_cb(rd_kafka_s *rk,
			rd_kafka_resp_err_t err,
			rd_kafka_topic_partition_list_s *partitions,
			void *opaque);

		bool InitRdKafkaHandle();
		bool InitRdKafkaConfig(const QbusConfigLoader &config_loader_);


		void OnRebalance();
		bool ManualCommit(const rd_kafka_message_s* rkmessage);
		bool CheckMsg(rd_kafka_message_s* rdkafka_massage);
		//void AddWaitCommit(rd_kafka_message_s *rd_kafka_message);
		//void AddWaitDestroyMsg(rd_kafka_message_s *rd_kafka_message);
		bool DestroyMsg(rd_kafka_message_s *rkm);
	private:
		rd_kafka_conf_s* rd_kafka_conf_;
		rd_kafka_topic_conf_s* rd_kafka_topic_conf_;
		rd_kafka_s* rd_kafka_handle_;

		//std::string cluster_name_;
		std::string broker_list_;
		//std::set<std::string> efficacious_topics_;

		bool start_flag_;
		//bool enable_rdkafka_logger_;
		bool is_auto_commit_offset_;
		
		void (*rebalance_cb_)(void *opaque);
		void *rebalance_arg_;
		int consumer_poll_ms_;

		//std::thread *thread_;
		//std::thread::id  poll_thread_id_;

		//QbusConfigLoader config_loader_;

//#ifndef NOT_USE_CONSUMER_CALLBACK
//		const QbusConsumerCallback& qbus_consumer_callback_;
//#endif

		std::string group_;
		std::string topic_;
		//std::vector<std::string> topics_;

		//std::mutex  wait_commit_msgs_mutex_;
		//std::map<std::string, rd_kafka_message_s*> wait_commit_msgs_;
		
		//std::map<int, rd_kafka_message_s*> wait_commit_msgs_;

		//typedef std::list<rd_kafka_message_s *> MsgList;
		//typedef std::map<int, MsgList> MsgListMap;
		//MsgListMap wait_destroy_msgs_;
	};
}//namespace qbus

#endif//QBUS_PRODUCER_IMP_H_
