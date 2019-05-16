#ifndef QBUS_LOGGER_H_
#define QBUS_LOGGER_H_



#include <string>
#include <sstream>
#include <mutex>

typedef void(*LogCallback)(int level, const char*msg);


#define QBUS_TRACE(msg)												   \
do																	   \
{																	   \
	if (!qbus::Logger::enableLevel(qbus::Logger::LL_TRACE))			   \
	{																   \
		break;														   \
	}																   \
	std::ostringstream ss;											   \
	ss << msg;														   \
	qbus::Logger::writeLog(qbus::Logger::LL_TRACE, ss.str());		   \
} while (0);														  
#define  QBUS_INFO(msg) 											   \
do																	   \
{																	   \
	if (!qbus::Logger::enableLevel(qbus::Logger::LL_INFO))			   \
	{																   \
		break;														   \
	}																   \
	std::ostringstream ss;											   \
	ss << msg;														   \
	qbus::Logger::writeLog(qbus::Logger::LL_INFO, ss.str());		   \
} while (0);														   
#define  QBUS_DEBUG(msg) 											   \
do																	   \
{																	   \
	if (!qbus::Logger::enableLevel(qbus::Logger::LL_DEBUG))			   \
	{																   \
		break;														   \
	}																   \
	std::ostringstream ss;											   \
	ss << msg;														   \
	qbus::Logger::writeLog(qbus::Logger::LL_DEBUG, ss.str());		   \
} while (0);														 
#define  QBUS_WARNING(msg)											   \
do																	   \
{																	   \
	if (!qbus::Logger::enableLevel(qbus::Logger::LL_WARNING))		   \
	{																   \
		break;														   \
	}																   \
	std::ostringstream ss;											   \
	ss << msg;														   \
	qbus::Logger::writeLog(qbus::Logger::LL_WARNING, ss.str());		   \
} while (0);														 
#define  QBUS_ERROR(msg)											   \
do																	   \
{																	   \
	if (!qbus::Logger::enableLevel(qbus::Logger::LL_ERROR))			   \
	{																   \
		break;														   \
	}																   \
	std::ostringstream ss;											   \
	ss << msg;														   \
	qbus::Logger::writeLog(qbus::Logger::LL_ERROR, ss.str());		   \
} while (0);
//---------------------------------------------
namespace qbus
{


	class Logger
	{
	public:
		enum LOG_LEVEL
		{
			LL_DEBUG = 0,
			LL_TRACE,
			LL_WARNING,
			LL_ERROR,
			LL_INFO,
			LL_END,
		};


#define LOG_EMERG   0
#define LOG_ALERT   1
#define LOG_CRIT    2
#define LOG_ERR     3
#define LOG_WARNING 4
#define LOG_NOTICE  5
#define LOG_INFO    6
#define LOG_DEBUG   7
	public:

		static bool enableLevel(int level);
		static void init(int level, LogCallback cb);
		static void writeLog(LOG_LEVEL level, const std::string& str);
		static const char* descLevel(LOG_LEVEL level);

		static void uninit() {		}


	private:
		static std::mutex sInitLogMutex;
		static bool sInit;
		static int slevel;
		static LogCallback sLogCallback;

	private:
		Logger(const Logger&);
		Logger& operator=(const Logger&);
	};
}//namespace qbus 
#endif //#ifndef QBUS_LOGGER_IMP_H_
