#include "qbus_consumer_imp.h"

//#include <strings.h>
#include <string>
#include <sstream>
#include <errno.h>
#include <algorithm>
#include <iostream>


//#include <pthread.h>

//from rdkafka_int.h, avoid to redefine
#define RD_KAFKA_OFFSET_QBUS_ERROR -1001


//#include "qbus_consumer_callback.h"
#include "qbus_constant.h"
#include "qbus_helper.h"
//------------------------------------------------------------
namespace qbus {

	static int kKafkaDestroyTime = 5;

	QbusConsumerImp::QbusConsumerImp() 
		:rd_kafka_conf_(NULL),
		rd_kafka_topic_conf_(NULL),
		rd_kafka_handle_(NULL),
		//cluster_name_(""),
		start_flag_(false),
		//enable_rdkafka_logger_(false),
		is_auto_commit_offset_(true),
		//consumeCallback_(NULL),
		//is_user_manual_commit_offset_(false),
		//is_force_destroy_(false),
		//rebalance_cb_(NULL),
		//rebalance_arg_(NULL), 
		consumer_poll_ms_(RD_KAFKA_CONSUMER_POLL_TIMEOUT_MS)
	{
	}

	QbusConsumerImp::~QbusConsumerImp()
	{
	}

	bool QbusConsumerImp::Init(const QbusConfigLoader &config_loader_) {
		//QbusHelper::InitLog(cb);
		//config_loader_.LoadConfig(config_path);
		//QbusHelper::InitLog(config_loader_.GetSdkConfig(RD_KAFKA_SDK_CONFIG_LOG_LEVEL,
		//      RD_KAFKA_SDK_CONFIG_LOG_LEVEL_DEFAULT),
		//    log_path);

		//std::string logLevel = config_loader_.GetSdkConfig(RD_KAFKA_SDK_CONFIG_LOG_LEVEL,
		//	RD_KAFKA_SDK_CONFIG_LOG_LEVEL_DEFAULT);
		//std::string logPath = config_loader_.GetSdkConfig(RD_KAFKA_SDK_CONFIG_LOG_PATH,
		//	RD_KAFKA_SDK_CONFIG_LOG_PATH_DEFAULT);
		////std::string configLogLevel = 
		//QbusHelper::InitLog(logLevel, logPath);


		//int status = pthread_mutex_init(&wait_commit_msgs_mutex_, NULL);
		//if (0 != status) {
		//  QBUS_ERROR(__FUNCTION__ << " | Faile to pthread_mutex_init for wait_commit_msgs_mutex_"
		//      << " | QBUS_ERROR code:" << status);
		//  return false;
		//}

		group_ = config_loader_.GetSdkConfig(RD_KAFKA_SDK_CONFIG_CONSUME_GROUP, "");
		if (group_.empty()) {
			QBUS_ERROR("parse consumer config, not find consume group");
			return false;
		}
		topic_ = config_loader_.GetSdkConfig(RD_KAFKA_SDK_CONFIG_TOPIC_NAME, "");
		if (topic_.empty()) {
			QBUS_ERROR("parse consumer config, not find topic");
			return false;
		}
		broker_list_ = config_loader_.GetSdkConfig(RD_KAFKA_SDK_CONFIG_BROKER_LIST, "");
		if (broker_list_.empty())
		{
			QBUS_ERROR("parse consumer config, not find broker_list_");
			return false;
		}
		QBUS_INFO("QbusConsumerImp Init, topic_=" << topic_ << "broker_list_=" <<  broker_list_);
		return InitRdKafkaConfig(config_loader_) && InitRdKafkaHandle();
	}

	bool QbusConsumerImp::InitRdKafkaConfig(const QbusConfigLoader &config_loader_) {

		rd_kafka_conf_ = rd_kafka_conf_new();

		rd_kafka_conf_set_opaque(rd_kafka_conf_, static_cast<void*>(this));
		rd_kafka_conf_set_rebalance_cb(rd_kafka_conf_, &QbusConsumerImp::rdkafka_rebalance_cb);

		rd_kafka_topic_conf_ = rd_kafka_topic_conf_new();

		if(!config_loader_.LoadRdkafkaConfig(rd_kafka_conf_, rd_kafka_topic_conf_))
		{
			QBUS_ERROR("LoadRdkafkaConfig fail");
			return false;
		}

		//set client.id
		QbusHelper::SetClientId(rd_kafka_conf_);

		//set stored offset into zk or broker
		if (!config_loader_.IsSetConfig(RD_KAFKA_TOPIC_CONFIG_OFFSET_STORED_METHOD, true)) {
			if (!QbusHelper::SetRdKafkaTopicConfig(rd_kafka_topic_conf_,
				RD_KAFKA_TOPIC_CONFIG_OFFSET_STORED_METHOD,
				RD_KAFKA_TOPIC_CONFIG_OFFSET_STORED_METHOD_BROKER))
			{
				return false;
			}
		}


		//if (!rt) {
		//	QBUS_ERROR(__FUNCTION__ << " | set topic config[" << RD_KAFKA_TOPIC_CONFIG_OFFSET_STORED_METHOD << "] failed");
		//}
		//else {
		//	rd_kafka_conf_set_default_topic_conf(rd_kafka_conf_, rd_kafka_topic_conf_);
		//}
		rd_kafka_conf_set_default_topic_conf(rd_kafka_conf_, rd_kafka_topic_conf_);
		//set whether auto reset offset when current offset is invalid, default earliest 
		if (!config_loader_.IsSetConfig(RD_KAFKA_TOPIC_CONFIG_AUTO_OFFSET_RESET, true)) {
			if (!QbusHelper::SetRdKafkaTopicConfig(rd_kafka_topic_conf_,
				RD_KAFKA_TOPIC_CONFIG_AUTO_OFFSET_RESET,
				RD_KAFKA_TOPIC_CONFIG_AUTO_OFFSET_RESET_EARLIEST))
			{
				return false;
			}
		}


		char auto_reset_offset[20] = { 0 };
		size_t auto_reset_offset_size = 0;
		rd_kafka_conf_res_t get_auto_reset_offset_rt = rd_kafka_topic_conf_get(rd_kafka_topic_conf_,
			RD_KAFKA_TOPIC_CONFIG_AUTO_OFFSET_RESET,
			auto_reset_offset,
			&auto_reset_offset_size);
		if (RD_KAFKA_CONF_OK == get_auto_reset_offset_rt) {
			QBUS_INFO(__FUNCTION__ << " | Reset offset to " << auto_reset_offset << " if the current offset is invalid");
		}
		else {
			QBUS_INFO(__FUNCTION__ << " | Reset offset if the current offset is invalid");
		}
		consumer_poll_ms_ = std::stoi(config_loader_.GetSdkConfig(RD_KAFKA_SDK_CONSUMER_POLL_TIME, RD_KAFKA_SDK_CONSUMER_POLL_TIME_DEFAULT_MS));
		//manual_commit_ms_ = std::stoi(config_loader_.GetSdkConfig(RD_KAFKA_SDK_CONSUMER_MANUAL_COMMIT_TIME, RD_KAFKA_SDK_MANUAL_COMMIT_TIME_DEFAULT_MS_STR));

		//get whether auto commit offset
		char is_auto_commit_offset[10];
		size_t is_auto_commit_offset_size = 0;
		rd_kafka_conf_res_t get_auto_commit_rt = rd_kafka_conf_get(rd_kafka_conf_,
			RD_KAFKA_CONFIG_ENABLE_AUTO_COMMIT,
			is_auto_commit_offset,
			&is_auto_commit_offset_size);
		if (RD_KAFKA_CONF_OK == get_auto_commit_rt) {

			is_auto_commit_offset[is_auto_commit_offset_size] = '\0';
			std::string str(is_auto_commit_offset);

			is_auto_commit_offset_ = (0 == str.compare("true"));
			//is_auto_commit_offset_ = (0 == strncasecmp(is_auto_commit_offset, "true", is_auto_commit_offset_size));
		}
		else {
			QBUS_ERROR(__FUNCTION__ << " | Failed to rd_kafka_conf_get for " << RD_KAFKA_CONFIG_ENABLE_AUTO_COMMIT);
			return false;
		}

		QBUS_INFO(__FUNCTION__ << " | is_auto_commit_offset_: " << is_auto_commit_offset_ );
		if (!QbusHelper::SetRdKafkaConfig(rd_kafka_conf_,
			RD_KAFKA_CONFIG_GROUP_ID,
			group_.c_str()))
		{
			QBUS_ERROR(__FUNCTION__ << " | Failed to set group: " << group_);
			return false;
		}
		QBUS_INFO("Set config group: " << group_);
		//get whether user manual commit offset and if use use manual commit offset, then set is_auto_commit_offset to false
		//std::string user_manual_commit_offset = config_loader_.GetSdkConfig(RD_KAFKA_SDK_USER_MANUAL_COMMIT_OFFSET,
		//	RD_KAFKA_SDK_USER_MANUAL_COMMIT_OFFSET_DEFAULT);
		//if (0 == user_manual_commit_offset.compare(RD_KAFKA_SDK_USER_MANUAL_COMMIT_OFFSET_DEFAULT)) {
		//	//if (0 == strncasecmp(user_manual_commit_offset.c_str(),
		//	//      RD_KAFKA_SDK_USER_MANUAL_COMMIT_OFFSET_DEFAULT,
		//	//      user_manual_commit_offset.length())) {
		//	is_user_manual_commit_offset_ = false;
		//}
		//else {
		//	QBUS_DEBUG(__FUNCTION__ << " | use user_manual_commit_offset option");
		//	is_user_manual_commit_offset_ = true;
		//	if (is_auto_commit_offset_) {
		//		is_auto_commit_offset_ = false;
		//		QbusHelper::SetRdKafkaConfig(rd_kafka_conf_,
		//			RD_KAFKA_CONFIG_ENABLE_AUTO_COMMIT,
		//			"false");
		//	}
		//}

		return true ;
	}

	void QbusConsumerImp::OnRebalance()
	{
		if (rebalance_cb_)
		{
			rebalance_cb_(rebalance_arg_);
		}
		//if (!is_auto_commit_offset_)
		//{
		//	ManualCommitAll();
		//}
		//ClearWaitDestroyMsgs();
	}

	bool QbusConsumerImp::InitRdKafkaHandle() {
		bool rt = false;

		if (NULL != rd_kafka_handle_) {
			rt = true;
		}
		else {
			char err_str[512] = { 0 };
			rd_kafka_handle_ = rd_kafka_new(RD_KAFKA_CONSUMER,
				rd_kafka_conf_,
				err_str,
				sizeof(err_str));
			if (NULL == rd_kafka_handle_) {
				QBUS_ERROR(__FUNCTION__ << "Failed to create new consumer | QBUS_ERROR msg:" << err_str);
			}
			else if (0 == rd_kafka_brokers_add(rd_kafka_handle_, broker_list_.c_str())) {
				QBUS_ERROR(__FUNCTION__ << " | Failed to rd_kafka_broker_add | broker list:" << broker_list_);
			}
			else {
				rd_kafka_resp_err_t resp_rs = rd_kafka_poll_set_consumer(rd_kafka_handle_);
				if (RD_KAFKA_RESP_ERR_NO_ERROR != resp_rs) {
					QBUS_ERROR(__FUNCTION__ << " | Failed to rd_kafka_poll_set_consumer | err msg:" << rd_kafka_err2str(resp_rs));
				}
				else {
					rt = true;
				}
			}
		}

		return rt;
	}


	bool QbusConsumerImp::Start() {
		QBUS_INFO(__FUNCTION__ << " | Startting consume...")

		bool rt = true;

		if (!start_flag_ && !topic_.empty()) {
			start_flag_ = true;

			rd_kafka_topic_partition_list_t* rd_kafka_topic_list = rd_kafka_topic_partition_list_new(1);

			//for (size_t i = 0; i < topics.size(); ++i) {
			rd_kafka_topic_partition_t* res = rd_kafka_topic_partition_list_add(rd_kafka_topic_list,
				topic_.c_str(),
				-1);//-1 mean consumed all partitions

			if (NULL == res) {
				QBUS_ERROR(__FUNCTION__ << " | Failed to rd_kafka_topic_partition_list_add | group:" << group_<< " | topic: " << topic_);
				rt = false;
				//break;
			}

			QBUS_INFO("subcribe topic:" << topic_);
			if (rt) {
				rd_kafka_resp_err_t err = rd_kafka_subscribe(rd_kafka_handle_,
					rd_kafka_topic_list);
				if (RD_KAFKA_RESP_ERR_NO_ERROR != err ) {
					rt = false;
					QBUS_ERROR(__FUNCTION__ << " | Failed to rd_kafka_subscribe | err msg:" << rd_kafka_err2str(err));
				}		
			}
			rd_kafka_topic_partition_list_destroy(rd_kafka_topic_list);
		}

		if (!rt)
		{
			start_flag_ = false;
		}
		return rt;
	}

	void QbusConsumerImp::Stop() {
		QBUS_INFO(__FUNCTION__ << " | Startting stop consumer...");
			//<< " | is_auto_commit_offset:" << is_auto_commit_offset_
			//<< " | is_user_manual_commit_offset:" << is_user_manual_commit_offset_
			//<< " | is_force_destroy:" << is_force_destroy_
			//<< " | wait destroy msgs:" << wait_destroy_msgs_.size());

		if (start_flag_) {
			start_flag_ = false;

			//同步方式停止后表名应用层不再收取消息了， 不再可以提交了
			//if (!is_auto_commit_offset_) {
			//	ManualCommitAll();
			//}
			//ClearWaitDestroyMsgs();
			//if (is_user_manual_commit_offset_) {
			//	ClearWaitDestroyMsgs();
			//}
		}

		QBUS_INFO(__FUNCTION__ << " | Startting clean rdkafka...");

		if (NULL != rd_kafka_handle_) {
			QBUS_INFO(__FUNCTION__ << " | Startting consumer close...");
			rd_kafka_resp_err_t err = rd_kafka_consumer_close(rd_kafka_handle_);
			if (RD_KAFKA_RESP_ERR_NO_ERROR != err) {
				QBUS_ERROR(__FUNCTION__ << " | Failed to close consumer | err msg: " << rd_kafka_err2str(err));
			}
		}

		QBUS_INFO(__FUNCTION__ << " | Startting destory rdkafka...");

		if (NULL != rd_kafka_handle_) {
			//if (is_user_manual_commit_offset_ &&
			//	is_force_destroy_) {
			//}
			//else {
				rd_kafka_destroy(rd_kafka_handle_);
			//}
			rd_kafka_handle_ = NULL;
		}

		QBUS_INFO(__FUNCTION__ << " | Startting wait destoryed rdkafka...");
		/* Let background threads clean up and terminate cleanly. */
		int run = kKafkaDestroyTime;
		while (run-- > 0 && rd_kafka_wait_destroyed(1000) == -1) {
			QBUS_DEBUG(__FUNCTION__ << " | Waiting for librdkafka to decommission");
		}

		QBUS_INFO(__FUNCTION__ << " | Consumer clean up done!");
	}

	bool QbusConsumerImp::CheckMsg(rd_kafka_message_s* rkmessage) {
		bool rt = false;

		if (RD_KAFKA_RESP_ERR_NO_ERROR != rkmessage->err) {
			if (RD_KAFKA_RESP_ERR__PARTITION_EOF == rkmessage->err) {
				const char* topicName = NULL;
				if (NULL != rkmessage->rkt)
				{
					topicName = rd_kafka_topic_name(rkmessage->rkt);
				}
				QBUS_DEBUG(__FUNCTION__ << " | Consumer reached end of " << topicName
					<< "[" << rkmessage->partition << "]" << " | offset: " << rkmessage->offset);
			}
			else if (NULL != rkmessage->rkt) {
				QBUS_ERROR(__FUNCTION__ << " | Consumer QBUS_ERROR for" << rd_kafka_topic_name(rkmessage->rkt)
					<< "[" << rkmessage->partition << "]" << " | offset: " << rkmessage->offset
					<< " | err msg: " << rd_kafka_message_errstr(rkmessage));
			}
			else {
				QBUS_ERROR(__FUNCTION__ << " | Consumer QBUS_ERROR | err msg: " << rkmessage->err
					<< " | rd_kafka_message's err msg: " << rd_kafka_message_errstr(rkmessage));
			}
		}
		else {
			rt = true;
		}

		return rt;
	}

//#ifdef NOT_USE_CONSUMER_CALLBACK
	bool QbusConsumerImp::Consume(QbusMsgContentInfo& msg_content_info) {

		if (!start_flag_) {
			QBUS_ERROR(__FUNCTION__ << " not start for consume");
			return false;
		}
		bool rt = false;
		rd_kafka_message_s* rkmessage = NULL;
		rkmessage = rd_kafka_consumer_poll(rd_kafka_handle_, consumer_poll_ms_);
		if (NULL != rkmessage &&
			CheckMsg(rkmessage) &&
			NULL != rkmessage->payload) {
			//std::string topic_name(NULL != rd_kafka_topic_name(rkmessage->rkt) ? rd_kafka_topic_name(rkmessage->rkt) : "");
			QBUS_DEBUG(__FUNCTION__ << " | Successed consumed msg of " << rd_kafka_topic_name(rkmessage->rkt)
				<< "[" << rkmessage->partition << "]" << " | offset: " << rkmessage->offset
				<< " | msg len: " << (int)rkmessage->len);

			//msg_content_info.topic = topic_name;
			msg_content_info.msg = static_cast<const char*>(rkmessage->payload);
			msg_content_info.msg_len = rkmessage->len;
			msg_content_info.rd_message = rkmessage;

			//if (!is_auto_commit_offset_) {
			//	AddWaitCommit(rkmessage);
			//}

			//AddWaitDestroyMsg(rkmessage);
			rt = true;
		}
		else if (NULL != rkmessage) {
			rd_kafka_message_destroy(rkmessage);
			rkmessage = NULL;
		}
	

		return rt;
	}
//#endif

	bool QbusConsumerImp::CommitOffset(const QbusMsgContentInfo& qbusMsgContentInfo) {
		auto rkm = static_cast<rd_kafka_message_s*>(qbusMsgContentInfo.rd_message);
		if (!rkm)
		{
			QBUS_ERROR(__FUNCTION__ << "cast rd_kafka_message_s error");
			return false;
		}
		bool rt = true;
		if (!is_auto_commit_offset_) {
			rt = ManualCommit(rkm);
		}
		return rt && DestroyMsg(rkm);
	}



	bool QbusConsumerImp::Destroy(const QbusMsgContentInfo& qbusMsgContentInfo)
	{
		auto rkm = static_cast<rd_kafka_message_s*>(qbusMsgContentInfo.rd_message);
		if (!rkm)
		{
			QBUS_ERROR(__FUNCTION__ << "cast rd_kafka_message_s error");
			return false;
		}
		DestroyMsg(rkm);
		return true;
	}

	//std::string QbusConsumerImp::GetWaitOffsetKey(const rd_kafka_message_s *msg) {
	//	std::stringstream ss;
	//	const char *name = rd_kafka_topic_name(msg->rkt);
	//	ss << (NULL != name ? name : "") << msg->partition;
	//	std::string key = ss.str();
	//	ss.str("");
	//	return key;
	//}

	//void QbusConsumerImp::ClearWaitDestroyMsgs()
	//{
	//	for (auto i = wait_destroy_msgs_.begin(), e = wait_destroy_msgs_.end();
	//		i != e; ++i) {
	//		MsgList& list = i->second;
	//		for (auto itList = list.begin(); itList != list.end(); itList++)
	//		{
	//			rd_kafka_message_s *rdmsg = *itList;
	//			rd_kafka_resp_err_t rt = rd_kafka_commit_message(rd_kafka_handle_,
	//				rdmsg,
	//				0);
	//			if (RD_KAFKA_RESP_ERR_NO_ERROR != rt) {
	//				QBUS_ERROR(__FUNCTION__ << " | Failed to rd_kafka_commit_message | QBUS_ERROR msg: " << rd_kafka_err2str(rt));
	//			}
	//		}
	//	}
	//	wait_destroy_msgs_.clear();
	//}

	//void QbusConsumerImp::AddWaitDestroyMsg(rd_kafka_message_s *rkm) {
	//	auto found = wait_destroy_msgs_.find(rkm->partition);
	//	if (wait_destroy_msgs_.end() != found) {
	//		MsgList& msgs = found->second;
	//		msgs.push_back(rkm);
	//	}
	//	else {
	//		MsgList msgs;
	//		msgs.push_back(rkm);
	//		wait_destroy_msgs_.insert(std::map<int, MsgList>::value_type(
	//			rkm->partition,
	//			msgs));
	//	}
	//}



	bool QbusConsumerImp::DestroyMsg(rd_kafka_message_s *rkm) {
		rd_kafka_message_destroy(rkm);
		return true;
		//auto found = wait_destroy_msgs_.find(rkm->partition);
		//if (wait_destroy_msgs_.end() != found) {
		//	MsgList& msgs = found->second;
		//	auto it = std::find(std::begin(msgs), std::end(msgs), rkm);
		//	if (it != msgs.end()) {
		//		msgs.erase(it);
		//		rd_kafka_message_destroy(rkm);
		//		return true;
		//	}
		//}
		//return false;
	}

	//void QbusConsumerImp::AddWaitCommit(rd_kafka_message_s *rkm) 
	//{
	//	auto it = wait_commit_msgs_.find(rkm->partition);
	//	if (it != wait_commit_msgs_.end() && it->second->offset > rkm->offset)
	//	{
	//		return;
	//	}
	//	wait_commit_msgs_[rkm->partition] = rkm;
	//}

////#ifndef NOT_USE_CONSUMER_CALLBACK
//	void* QbusConsumerImp::ConsumePollThread(void* arg) {
//		QbusConsumerImp* consumer = static_cast<QbusConsumerImp*>(arg);
//
//		if (NULL != consumer) {
//			rd_kafka_message_s* rdkafka_message = NULL;
//			while (consumer->start_flag_) {
//				auto timeout_ms = std::chrono::duration_cast<std::chrono::microseconds>(consumer->consumer_poll_time_).count();
//				rdkafka_message = rd_kafka_consumer_poll(consumer->rd_kafka_handle_,
//					int(timeout_ms));
//				if (NULL != rdkafka_message &&
//					consumer->CheckMsg(rdkafka_message)) {
//					if (!consumer->is_auto_commit_offset_) {
//						//if (!consumer->is_user_manual_commit_offset_) {
//							consumer->AddWaitCommitOffset(rdkafka_message);
//						//}
//						//else {
//						//	consumer->AddWatiDestroyMsgs(rdkafka_message);
//						//}
//						consumer->ReceivedConsumeMsg(rdkafka_message, NULL);
//					}
//					else {
//						consumer->ReceivedConsumeMsg(rdkafka_message, NULL);
//						rd_kafka_message_destroy(rdkafka_message);
//						rdkafka_message = NULL;
//					}
//				}
//				else if (NULL != rdkafka_message) {
//					rd_kafka_message_destroy(rdkafka_message);
//					rdkafka_message = NULL;
//				}
//
//				if (!consumer->is_auto_commit_offset_) {
//					consumer->ManualCommitWaitOffset(false);
//				}
//			}
//
//			if (!consumer->is_auto_commit_offset_) {
//				consumer->ManualCommitWaitOffset(false);
//			}
//		}
//
//		return (void*)(NULL);
//	}
////#endif

	//void  QbusConsumerImp::ManualCommitAll() {
	//	for (auto i = wait_commit_msgs_.begin(), e = wait_commit_msgs_.end();
	//		i != e; ++i) {
	//		rd_kafka_message_s *rdmsg = i->second;
	//		if (NULL != rdmsg) {
	//			rd_kafka_resp_err_t rt = rd_kafka_commit_message(rd_kafka_handle_,
	//				rdmsg,
	//				0);
	//			if (RD_KAFKA_RESP_ERR_NO_ERROR != rt) {
	//				QBUS_ERROR(__FUNCTION__ << " | Failed to rd_kafka_commit_message | QBUS_ERROR msg: " << rd_kafka_err2str(rt));
	//			}
	//		}
	//	}
	//	wait_commit_msgs_.clear();
	//}

	bool  QbusConsumerImp::ManualCommit(const rd_kafka_message_s* rkmessage) {
		//const rd_kafka_message_s* msg = rkmessage;

		//auto it = wait_commit_msgs_.find(msg->partition);
		//if (it != wait_commit_msgs_.end() && it->second->offset <= msg->offset)
		//{
		//	wait_commit_msgs_.erase(it);
		//}
		rd_kafka_resp_err_t rt = rd_kafka_commit_message(rd_kafka_handle_,
			rkmessage,
			1);
		if (RD_KAFKA_RESP_ERR_NO_ERROR != rt) {
			QBUS_ERROR(__FUNCTION__ << " | Failed to rd_kafka_commit_message | QBUS_ERROR msg: " << rd_kafka_err2str(rt));
			return false;
		}
		return true;
	}

	void QbusConsumerImp::SetRebalanceCallback(RebalanceCallback cb, void* arg)
	{
		rebalance_cb_ = cb;
		rebalance_arg_ = arg;
	}

//
////#ifndef NOT_USE_CONSUMER_CALLBACK
//	void QbusConsumerImp::ReceivedConsumeMsg(rd_kafka_message_s *rkmessage,
//		void *opaque) {
//		if (NULL == rkmessage) {
//			return;
//		}
//
//		//std::string topic_name(NULL != rd_kafka_topic_name(rkmessage->rkt) ? rd_kafka_topic_name(rkmessage->rkt) : "");
//		QBUS_DEBUG(__FUNCTION__ << " | Successed consumed msg of " << rd_kafka_topic_name(rkmessage->rkt)
//			<< "[" << rkmessage->partition << "]" << " | offset: " << rkmessage->offset
//			<< " | msg len: " << (int)rkmessage->len);
//		if (consumeCallback_) {
//			consumeCallback_(static_cast<const char*>(rkmessage->payload), rkmessage->len);
//		}	else {
//			QBUS_WARNING(__FUNCTION__ << " | not find consumer callback " << rd_kafka_topic_name(rkmessage->rkt)
//				<< "[" << rkmessage->partition << "]" << " | offset: " << rkmessage->offset
//				<< " | msg len: " << (int)rkmessage->len)
//		}
//		//if (!is_user_manual_commit_offset_) {
//		//	std::string copy_msg(static_cast<const char*>(rkmessage->payload), rkmessage->len);
//		//	qbus_consumer_callback_.deliveryMsg(topic_name,
//		//		copy_msg.c_str(),
//		//		rkmessage->len);
//		//}
//		//else {
//		//	//!LW! set user.manual.commit.offset=true into consumer config file and user need to call to commitOffset
//		//	QbusMsgContentInfo msg_content_QBUS_INFO;
//		//	msg_content_QBUS_INFO.topic = topic_name;
//		//	msg_content_QBUS_INFO.msg = std::string(static_cast<const char*>(
//		//		rkmessage->payload != NULL ? rkmessage->payload : ""),
//		//		rkmessage->len);
//		//	msg_content_QBUS_INFO.msg_len = rkmessage->len;
//		//	msg_content_QBUS_INFO.rd_message = rkmessage;
//		//	qbus_consumer_callback_.deliveryMsgForCommitOffset(msg_content_QBUS_INFO);
//		//}
//	}
////#endif

	void QbusConsumerImp::rdkafka_rebalance_cb(rd_kafka_s *rk,
		rd_kafka_resp_err_t err,
		rd_kafka_topic_partition_list_s *partitions,
		void *opaque) {
		switch (err) {
		case RD_KAFKA_RESP_ERR__ASSIGN_PARTITIONS:
		{
			QBUS_DEBUG(__FUNCTION__ << " | rebalnace result OK: " << QbusHelper::FormatTopicPartitionList(partitions));
			rd_kafka_assign(rk, partitions);
		}
		break;
		case RD_KAFKA_RESP_ERR__REVOKE_PARTITIONS:
		{
			QBUS_DEBUG(__FUNCTION__ << " | rebalnace result revoke | msg: " << rd_kafka_err2str(err) << " | " << QbusHelper::FormatTopicPartitionList(partitions));
			QbusConsumerImp* consumer_imp = static_cast<QbusConsumerImp*>(opaque);
			if (consumer_imp)
			{
				consumer_imp->OnRebalance();
			}
			rd_kafka_assign(rk, NULL);
		}
		break;
		default:
			QBUS_ERROR(__FUNCTION__ << " | Failed to rebalance | err msg: " << rd_kafka_err2str(err));
			rd_kafka_assign(rk, NULL);
			break;
		}
	}
}//namespace qbus
