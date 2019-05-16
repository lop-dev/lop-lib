#include "qbus_consumer.h"


#include "qbus_consumer_imp.h"
#include "qbus_config.h"
#include "qbus_constant.h"
//------------------------------------------------------------
namespace qbus {

QbusConsumer::QbusConsumer():
  qbus_consumer_imp_(NULL) {
}

QbusConsumer::~QbusConsumer() {
  if (NULL != qbus_consumer_imp_) {
    delete qbus_consumer_imp_;
    qbus_consumer_imp_ = NULL;
  }
}

bool QbusConsumer::init(const QbusConfigLoader& config_loader_
//#ifndef NOT_USE_CONSUMER_CALLBACK
//    ,const QbusConsumerCallback& callback
//#endif
    ) {
  bool rt = false;


  //QBUS_INFO(__FUNCTION__ << " | Start init | qbus cluster: " << broker_list
  //    << " | topic: " << topic_name
  //    << " | config: " << config_path);
  //std::string broker_list = config_loader_.GetSdkConfig(RD_KAFKA_SDK_CONFIG_BROKER_LIST, "");


  qbus_consumer_imp_ = new QbusConsumerImp();
  if (NULL != qbus_consumer_imp_) {
    rt = qbus_consumer_imp_->Init(config_loader_);
  }

  if (rt) {
	  QBUS_INFO(__FUNCTION__ << " | QbusConsumer::init is OK");
  } else {
	  QBUS_ERROR(__FUNCTION__ << " | Failed to QbusConsumer::init");
  }

  return rt;
}

//void QbusConsumer::setConsumeCallback(ConsumeCallback cb)
//{
//	qbus_consumer_imp_->SetConsumeCallback(cb);
//}


bool QbusConsumer::start() {
  bool rt = false;
  if (NULL != qbus_consumer_imp_) {
    rt = qbus_consumer_imp_->Start();
  }

  if (rt) {
	  QBUS_INFO(__FUNCTION__ << " | QbusConsumer::start OK");
  } else {
	  QBUS_ERROR(__FUNCTION__ << " | Failed to QbusConsumer::start");
  }

  return rt;
}

void QbusConsumer::stop() {
  if (NULL != qbus_consumer_imp_) {
    qbus_consumer_imp_->Stop();
  }
}

bool QbusConsumer::commitOffset(const QbusMsgContentInfo& qbusMsgContentInfo) {
	if (NULL != qbus_consumer_imp_) {
		return qbus_consumer_imp_->CommitOffset(qbusMsgContentInfo);
	}
	return false;
}

bool QbusConsumer::destroy(const QbusMsgContentInfo& qbusMsgContentInfo)
{
	if (NULL != qbus_consumer_imp_) {
		return qbus_consumer_imp_->Destroy(qbusMsgContentInfo);
	}
	return false;
}

bool QbusConsumer::setRebalanceCallback(RebalanceCallback cb, void* arg)
{
	if (NULL != qbus_consumer_imp_) {
		qbus_consumer_imp_->SetRebalanceCallback(cb, arg);
		return true;
	}
	return false;
}
//
//void QbusConsumer::RebalanceAssign()
//{
//	if (NULL != qbus_consumer_imp_)
//	{
//		qbus_consumer_imp_->RebalanceAssign();
//	}
//}

//#ifdef NOT_USE_CONSUMER_CALLBACK
bool QbusConsumer::consume(QbusMsgContentInfo& msg_content_QBUS_INFO) {
  bool rt = false;

  if (NULL != qbus_consumer_imp_) {
    rt = qbus_consumer_imp_->Consume(msg_content_QBUS_INFO);
  }

  return rt;
}
//#endif

}//namespace qbus 
