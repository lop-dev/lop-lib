#include "qbus_logger.h"

#include <iostream>



namespace qbus {
	
	bool Logger::sInit = false;
	int Logger::slevel = 0;
	LogCallback Logger::sLogCallback = NULL;
	std::mutex Logger::sInitLogMutex;


	bool Logger::enableLevel(int level)
	{
		return level >= slevel;
	}

	void Logger::init(int level, LogCallback cb)
	{
		sInitLogMutex.lock();
		if(!sInit)
		{
			sLogCallback = cb;
			sInit = true;
		}
		sInitLogMutex.unlock();
	}

	void Logger::writeLog(LOG_LEVEL level, const std::string& str)
	{
		sLogCallback(int(level), str.c_str());
	}


	const char* Logger::descLevel(LOG_LEVEL level)
	{
		static const char *types[] = {
			 "debug",
			 "trace",
			 "warning",
			 "error",
			 "info",
			 "unknown"
		};

		return types[level];

	}

}
