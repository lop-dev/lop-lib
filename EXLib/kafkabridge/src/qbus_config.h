#ifndef QBUS_CONFIG_H
#define QBUS_CONFIG_H

#include <string>
#include <map>

#include "qbus_rdkafka_def.h"
//#include <boost/property_tree/ptree.hpp>

namespace qbus {
class QbusConfigLoader {
  public:
    enum ConfigType {
      CT_CONSUMER = 0,
      CT_PRODUCER,
    };

    QbusConfigLoader() {
    }

	void setGlobalConfig(const std::string &name, const std::string &value);
	void setSdkConfig(const std::string &name, const std::string &value);
	void setTopicConfig(const std::string &name, const std::string &value);

	void setGlobalConfig(std::map<std::string, std::string> &configMap);
	void setTopicConfig(std::map<std::string, std::string> &configMap);
	void setSdkConfig(std::map<std::string, std::string> &configMap);
    //void LoadConfig(const std::string& path);
    bool LoadRdkafkaConfig(rd_kafka_conf_s* rd_kafka_conf,  rd_kafka_topic_conf_s* rd_kafka_topic_conf) const;
    bool IsSetConfig(const std::string& config_name, bool is_topic_config) const;
    std::string GetSdkConfig(const std::string& config_name, const std::string& default_value) const;

  private:
	  std::map<std::string, std::string> set_global_config_items_;
	  std::map<std::string, std::string> set_topic_config_items_;
	  std::map<std::string, std::string> set_sdk_configs_;


    //pt::ptree root_tree_;
    //pt::ptree set_global_config_items_;
    //pt::ptree set_topic_config_items_;
    //pt::ptree set_sdk_configs_;

  private:
    QbusConfigLoader(const QbusConfigLoader&);
    QbusConfigLoader& operator=(const QbusConfigLoader&);
};//QbusConfigLoader

}//namespace qbus

#endif//#define QBUS_CONFIG_H
