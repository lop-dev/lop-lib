#ifndef QBUS_RECORD_MSG_H_
#define QBUS_RECORD_MSG_H_

#include <string>


//--------------------------------------------------------------------
namespace qbus {

class QbusRecordMsg {
  public:
    static void recordMsg(const std::string& topic, const std::string& msg);

  private:
    static void init();
  private:
    //static log4cplus::Logger sLogger;
    static bool sInit;

  private:
    QbusRecordMsg();
};
}
#endif// #ifndef QBUS_RECORD_MSG_H_
