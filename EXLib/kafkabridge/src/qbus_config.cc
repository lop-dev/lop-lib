#include "qbus_config.h"


#include <iostream>
#include <fstream>
#include <csignal>


#include "qbus_constant.h"
#include "qbus_helper.h"
#include "qbus_rdkafka_init.h"

namespace qbus {

static const char* GLOBAL_CONFIG = "global";
static const char* TOPIC_CONFIG = "topic";
static const char* SDK_CONFIG = "sdk";
static const char INI_CONFIG_KEY_VALUE_SPLIT = '|';

void QbusConfigLoader::setGlobalConfig(std::map<std::string, std::string> &configMap)
{
	this->set_global_config_items_ = configMap;
}

void QbusConfigLoader::setGlobalConfig(const std::string &name, const std::string &value)
{
	this->set_global_config_items_[name] = value;
}

void QbusConfigLoader::setTopicConfig(std::map<std::string, std::string> &configMap)
{
	this->set_topic_config_items_ = configMap;

}

void QbusConfigLoader::setTopicConfig(const std::string &name, const std::string &value)
{
	this->set_topic_config_items_[name] = value;
}

void QbusConfigLoader::setSdkConfig(std::map<std::string, std::string> &configMap)
{
	this->set_sdk_configs_ = configMap;

}

void QbusConfigLoader::setSdkConfig(const std::string &name, const std::string &value)
{
	this->set_sdk_configs_[name] = value;
}

//void QbusConfigLoader::LoadConfig(const std::string& path) {
//    std::ifstream stream(path.c_str());
//    if (!stream) {
//       QBUS_WARNING(__FUNCTION__ << " | Can't open config file : " << path
//                   << " | use default config"); 
//        return ;
//    }
//    stream.close();
//
// //   pt::ini_parser::read_ini(path, root_tree_);
// //   boost::optional<pt::ptree&> set_global_config_items = root_tree_.get_child_optional(GLOBAL_CONFIG);
// //   if (set_global_config_items) {
//	//set_global_config_items_ = *set_global_config_items;
// //   }
// //   boost::optional<pt::ptree&> set_topic_config_items = root_tree_.get_child_optional(TOPIC_CONFIG);
// //   if (set_topic_config_items) {
//	//set_topic_config_items_ = *set_topic_config_items;
// //   }
// //   boost::optional<pt::ptree&> set_sdk_configs = root_tree_.get_child_optional(SDK_CONFIG);
// //   if (set_sdk_configs) {
//	//set_sdk_configs_ = *set_sdk_configs;
// //   }
//}

bool QbusConfigLoader::LoadRdkafkaConfig(rd_kafka_conf_s* rd_kafka_conf,
            rd_kafka_topic_conf_s* rd_kafka_topic_conf) const{
   	
#ifdef SIGIO
	char tmp[16] = { 0 };
    snprintf(tmp, sizeof(tmp), "%i", SIGIO);
    QbusHelper::SetRdKafkaConfig(rd_kafka_conf,
                RD_KAFKA_CONFIG_INTERNAL_TERMINATION_SIGNAL,
                tmp);
#endif
    std::string enable_rdkafka_log = GetSdkConfig(RD_KAFKA_SDK_CONFIG_ENABLE_RD_KAFKA_LOG,
                RD_KAFKA_SDK_CONFIG_ENABLE_RD_KAFKA_LOG_DEFAULT);
	if(0== enable_rdkafka_log.compare(RD_KAFKA_SDK_CONFIG_ENABLE_RD_KAFKA_LOG_DEFAULT)){
    //if (0 == strncasecmp(enable_rdkafka_log.c_str(),
    //                RD_KAFKA_SDK_CONFIG_ENABLE_RD_KAFKA_LOG_DEFAULT,
                    //enable_rdkafka_log.length())) {
        rd_kafka_conf_set_log_cb(rd_kafka_conf, NULL);
    } else {
        rd_kafka_conf_set_log_cb(rd_kafka_conf, &QbusHelper::RdKafkaLogger);
    }

	
    for (auto i = set_global_config_items_.begin(), e = set_global_config_items_.end();
        i != e; ++i) {
		if (!QbusHelper::SetRdKafkaConfig(rd_kafka_conf,
			i->first.c_str(),
			i->second.c_str()))
		{
			return false;
		}
    }

    for (auto i = set_topic_config_items_.begin(), e = set_topic_config_items_.end();
        i != e; ++i) {
		if (!QbusHelper::SetRdKafkaTopicConfig(rd_kafka_topic_conf,
			i->first.c_str(),
			i->second.c_str())
			)
		{
			return false;
		}
    }
	return true;
}

std::string QbusConfigLoader::GetSdkConfig(const std::string& config_name, const std::string& default_value) const {
    //std::string value(default_value);
	auto iter = set_sdk_configs_.find(config_name);
	if(iter != set_sdk_configs_.end()){
		return (*iter).second;
	}
    //boost::optional<std::string> found = set_sdk_configs_.get_optional<std::string>(
    //    pt::ptree::path_type(config_name, INI_CONFIG_KEY_VALUE_SPLIT));
    //if (boost::none != found) {
    //    value = *found;
    //}

    return default_value;
}

bool QbusConfigLoader::IsSetConfig(const std::string& config_name, bool is_topic_config) const {

    //QBUS_DEBUG(__FUNCTION__ << " | Check topic config | key: " << config_name);

	return is_topic_config ?
		set_topic_config_items_.find(config_name) != set_topic_config_items_.end() :
		set_global_config_items_.find(config_name) != set_global_config_items_.end();
        //set_topic_config_items_.get_child_optional(pt::ptree::path_type(config_name, INI_CONFIG_KEY_VALUE_SPLIT)) != boost::none:
        //set_global_config_items_.get_child_optional(pt::ptree::path_type(config_name, INI_CONFIG_KEY_VALUE_SPLIT)) != boost::none;

}

}
