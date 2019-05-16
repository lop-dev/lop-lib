#include "qbus_helper.h"


#include <stdio.h>
#include <ctime>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <fstream>
#include <sstream>

#include <mutex>
#include <chrono>
#include <thread>



#include "qbus_constant.h"
#include "qbus_config.h"
#include "qbus_rdkafka_init.h"

//-------------------------------------------------
namespace qbus {

static const char kRdkafkaLog[] = "./rdkafka.log";




bool QbusHelper::kInitLog = false;

static std::mutex kInitLogMutex ;

void QbusHelper::InitLog(int level, LogCallback cb)
{
	kInitLogMutex.lock();
	//pthread_mutex_lock(&kInitLogMutex);

	if (!kInitLog ) {
		Logger::init(level, cb);
		kInitLog = true;
	}

	kInitLogMutex.unlock();
}

bool QbusHelper::GetQbusBrokerList(const QbusConfigLoader& config_loader,
            std::string* broker_list) {
    bool rt = false;

    if (NULL != broker_list && *broker_list == "") {
        *broker_list = config_loader.GetSdkConfig(RD_KAFKA_SDK_CONFIG_BROKER_LIST, "");
    }

    rt = ("" != *broker_list);
    return rt;
}

bool QbusHelper::SetRdKafkaConfig(rd_kafka_conf_s* rd_kafka_conf,
            const char* item,
            const char* value) {
   bool rt = false;

   if (NULL != rd_kafka_conf &&
               NULL != item &&
               0 != item[0] &&
               NULL != value &&
               0 != item[0]) {
       char err_str[512] = {0};

       if (RD_KAFKA_CONF_OK != rd_kafka_conf_set(rd_kafka_conf,
                       item,
                       value,
                       err_str,
                       sizeof(err_str))) {
       QBUS_ERROR(__FUNCTION__ << " | Failed to rd_kafka_conf_set | item: " << item
                   << " | value: " << value
                   << " | QBUS_ERROR msg: " << err_str);
       } else {
           rt = true;
       }
   } else {
        QBUS_ERROR(__FUNCTION__ << " | invailed parameter!" << " | item: " << item << " | value: " << value);
   }

   return rt;
}

bool QbusHelper::SetRdKafkaTopicConfig(rd_kafka_topic_conf_s* rd_kafka_topic_conf,
                    const char* item,
                    const char* value) {
   bool rt = false;

   if (NULL != rd_kafka_topic_conf &&
               NULL != item &&
               0 != item[0] &&
               NULL != value &&
               0 != item[0]) {
       char err_str[512] = {0};

       if (RD_KAFKA_CONF_OK != rd_kafka_topic_conf_set(rd_kafka_topic_conf,
                       item,
                       value,
                       err_str,
                       sizeof(err_str))) {
       QBUS_ERROR(__FUNCTION__ << " | Failed to rd_kafka_topic_conf_set | item: " << item
                   << " | value: " << value
                   << " | QBUS_ERROR msg: " << err_str);
       } else {
           rt = true;
       }
   } else {
        QBUS_ERROR(__FUNCTION__ << " | invailed parameter!");
   }

   return rt;
}

std::string QbusHelper::FormatTopicPartitionList(const rd_kafka_topic_partition_list_s* partitions) {
    std::string rt("");
    
    if (NULL != partitions) {
        std::stringstream ss;
        for (int i = 0 ; i < partitions->cnt; ++i) {
            ss << partitions->elems[i].topic << "[" << partitions->elems[i].partition << "] |";
        }

        rt = ss.str();
        ss.clear();
    }
    
    return rt;
}

void QbusHelper::RdKafkaLogger(const rd_kafka_s *rk,
            int level,
            const char *fac,
            const char *buf) {

	switch (level)
	{
	case LOG_EMERG:
	case LOG_ALERT:
	case LOG_CRIT:
	case LOG_ERR:
		QBUS_ERROR("RD_KAFKA_LOG fac: " << (NULL != fac ? fac : "") << " | msg: " << (NULL != buf ? buf : ""));
		break;
	case LOG_WARNING:
		QBUS_WARNING("RD_KAFKA_LOG fac: " << (NULL != fac ? fac : "") << " | msg: " << (NULL != buf ? buf : ""));
		break;
	case LOG_NOTICE:
	case LOG_INFO:
		QBUS_INFO("RD_KAFKA_LOG fac: " << (NULL != fac ? fac : "") << " | msg: " << (NULL != buf ? buf : ""));
		break;
	case LOG_DEBUG:
		QBUS_DEBUG("RD_KAFKA_LOG fac: " << (NULL != fac ? fac : "") << " | msg: " << (NULL != buf ? buf : ""));
		break;
	default:
		QBUS_WARNING("RD_KAFKA_LOG level[" << level << "] fac: " << (NULL != fac ? fac : "") << " | msg: " << (NULL != buf ? buf : ""));
		break;
	}
//	time_t result = time(NULL);
//#ifdef _MSC_VER
//	char str[26];
//	ctime_s(str, sizeof str, &result);
//#else
//	char *str = ctime(&result);
//#endif

	
    //std::ofstream out;  
    //out.open(kRdkafkaLog, std::ios::out|std::ios::app);
    //out << str
    //    << " | RD_KAFKA_LOG"
    //    << " | level: " << level
    //    << " | fac: " << (NULL != fac ? fac : "")
    //    << " | msg: " << (NULL != buf ? buf : "")
    //    << std::endl;
    //out.close();
}

void QbusHelper::SetClientId(rd_kafka_conf_s* rd_kafka_conf, bool isAppendThreadId) {
    if (isAppendThreadId) {
		
        char clientId[100];

	time_t tt = time(NULL);
#ifdef _MSC_VER
	tm t;
	localtime_s(&t, &tt);
	snprintf(clientId,
			sizeof(clientId),
			"%s_%d-%02d-%02d-%02d-%02d-%02d",
			RD_KAFKA_CONFIG_CLIENT_ID_VALUE,
			//GetCurrentThreadId(),
			t.tm_year + 1900,
			t.tm_mon + 1,
			t.tm_mday,
			t.tm_hour,
			t.tm_min,
			t.tm_sec);

#else
	tm *t = localtime(&tt);
	snprintf(clientId,
			sizeof(clientId),
			"%s_%d-%02d-%02d-%02d-%02d-%02d",
			RD_KAFKA_CONFIG_CLIENT_ID_VALUE,
			t->tm_year + 1900,
			t->tm_mon + 1,
			t->tm_mday,
			t->tm_hour,
			t->tm_min,
			t->tm_sec);
    //    } else {

#endif
    //    } else {
    //        snprintf(clientId, 
    //                    sizeof(clientId),
    //                    "%s_%ld",
    //                    RD_KAFKA_CONFIG_CLIENT_ID_VALUE,
				//GetCurrentThreadId());
    //    }
        SetRdKafkaConfig(rd_kafka_conf,
                    RD_KAFKA_CONFIG_CLIENT_ID,
                    clientId);
    } else {
        SetRdKafkaConfig(rd_kafka_conf,
                    RD_KAFKA_CONFIG_CLIENT_ID,
                    RD_KAFKA_CONFIG_CLIENT_ID_VALUE);
    }
}

//int QbusHelper::GetCurrentThreadId() {
//	std::stringstream ss;
//	ss << std::this_thread::get_id();
//	return std::stoi(ss.str());
//}

//long QbusHelper::GetCurrentTimeMs() {
//
//	struct timeval now_time;
//	std::timespec now_time;
//	gettimeofday(&now_time, NULL);
//	return ((long)now_time.tv_sec) * 1000 + (long)now_time.tv_usec / 1000;
//
//}

}//namespace qbus
