
#include <stdlib.h>
#include <signal.h>


#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "qbus_producer.h"
//---------------------------------------
bool kStop = false;
const char* kConfigPath = "";

void Stop(int) {
    kStop = true;
}

int main(int argc, const char* argv[]) {
    const char* topic_name = "";

    if (argc > 5) {
        topic_name = argv[1];
        std::cout << " Topic: " << topic_name << std::endl;
    } else {
        std::cout << "qbus_producer_example topic key[\"\" or ] send_count config_full_path cluster_list" << std::endl;
        return 0;
    }

    signal(SIGINT, Stop);
    //signal(SIGALRM,Stop);
    int64_t loop = atoll(argv[3]);
    std::string key = argv[2];

	auto start = std::chrono::system_clock::now();
    //struct timeval now_time; 
    //gettimeofday(&now_time, NULL); 
    //int64_t start_time_ms = ((long)now_time.tv_sec)*1000+(long)now_time.tv_usec/1000;
	qbus::QbusConfigLoader configLoader;
	std::map<std::string, std::string> mapGlobalConfig;
	configLoader.setGlobalConfig(mapGlobalConfig);

	std::map<std::string, std::string> mapTopicConfig;
	configLoader.setTopicConfig(mapTopicConfig);
	std::map<std::string, std::string> mapSdkConfig;
	mapSdkConfig[RD_KAFKA_SDK_CONFIG_TOPIC_NAME] = std::string(topic_name);
	configLoader.setSdkConfig(mapSdkConfig);
	//configLoader.setGlobalConfig()

    qbus::QbusProducer qbus_producer;
    if (!qbus_producer.init(configLoader)) {
        std::cout << "Failed to init" << std::endl;
        return 0;
    }

    std::cout << "Producer init is ok!" << std::endl;

    std::string msg("aaaaaaaaaa");
    while (!kStop) {
        if (kStop) {
            break;
        }

        if (-1 == loop) {
            while (!kStop) {
                if (!qbus_producer.produce(msg.c_str(), msg.length(), key)) {
                    std::cout << "Failed to produce" << std::endl;
                    //Retry to produce
                }
				std::this_thread::sleep_for(std::chrono::microseconds(5));
                //usleep(5000);
            }
        } else {
            for (int i = 0; i < loop; ++i) {
                if (kStop) {
                    break;
                }
                if (!qbus_producer.produce(msg.c_str(), msg.length(), key)) {
                    std::cout << "Failed to produce" << std::endl;
                    //Retry to produce
                }
            }
            break;
        }
    }


    qbus_producer.uninit();
	auto end = std::chrono::system_clock::now();
	
    //gettimeofday(&now_time, NULL); 
    //int64_t end_time_ms = ((long)now_time.tv_sec)*1000+(long)now_time.tv_usec/1000;

    std::cout << "banche mark: " << (end-start).count() << std::endl;

    return 0;
}
