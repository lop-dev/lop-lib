//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   MWLib/crushDump/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <time.h>
#include <MWLib/crashDump/crashDump.h>

int SPrintf(char *buffer, size_t sizeOfBuffer, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int n = vsprintf_s(buffer, sizeOfBuffer, format, args);
    va_end(args);
    return n;
}

static std::string _GetDumpFilename(const std::string& moduleName)
{
    char dumpFileName[_MAX_PATH];
    time_t ltime;
    time(&ltime);

    struct tm localTime;
    if (localtime_s(&localTime, &ltime) == 0)
    {
        SPrintf(dumpFileName, sizeof(dumpFileName), "%s_%s_%02d%02d_%02d%02d.dmp", moduleName.c_str(),"revision", localTime.tm_mon+1, localTime.tm_mday, localTime.tm_hour, localTime.tm_min);
    }
    else
    {
        SPrintf(dumpFileName, sizeof(dumpFileName), "%s_%s.dmp", moduleName.c_str(),"revision");
    }

    return dumpFileName;

}

int main(int argc, char *argv[])
{
    MWLib::CrashDump::SetupCrashDump(_GetDumpFilename, NULL);
    RaiseException(1, 0, 0, 0);
    return 1;
}