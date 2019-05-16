#ifndef QBUS_PRODUCER_H_
#define QBUS_PRODUCER_H_

#include <string>
#include "qbus_logger.h"
//#include "qbus_config.h"
//-----------------------------------------------------
namespace qbus {

class QbusProducerImp;
class QbusConfigLoader;

typedef void(*ProduceCallback)(const char* msg, const size_t len, const char* key, const size_t key_len);

class QbusProducer {
  public:
    QbusProducer();
    ~QbusProducer();

  public:
    bool init(const QbusConfigLoader& config_loader_);
	void setProduceCallback(ProduceCallback cb);
	
    void uninit();
    bool produce(const char* data, size_t data_len, const std::string& key);

	//void set
  private:

    QbusProducerImp* qbus_producer_imp_;
};
} //namespace qbus
#endif //#define QBUS_PRODUCER_H_

