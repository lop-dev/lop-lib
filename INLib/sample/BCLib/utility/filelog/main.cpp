//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/filelog/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <BCLib/utility/logFile.h>
#include <BCLib/utility/thread/thread.h>
#include <BCLib/utility/baseDec.h>
#include <BCLib/utility/string.h>
#include <signal.h>

enum ELogModule
{
    ELOGMODULE_TEST_ONE = BCLib::ELOGMODULE_EXTEND,
    ELOGMODULE_TEST_TWO = ELOGMODULE_TEST_ONE + 10,
};

void testTracer()
{
    BCLIB_LOG_DEBUG(ELOGMODULE_TEST_ONE, "1.1 ≤‚ ‘");
    BCLIB_LOG_DEBUG(ELOGMODULE_TEST_TWO, "2.1 ≤‚ ‘");

    BCLIB_LOG_TRACE(ELOGMODULE_TEST_ONE, "1.2 ≤‚ ‘");
    BCLIB_LOG_TRACE(ELOGMODULE_TEST_TWO, "2.2 ≤‚ ‘");

    BCLIB_LOG_WARNING(ELOGMODULE_TEST_ONE, "1.3 ≤‚ ‘");
    BCLIB_LOG_WARNING(ELOGMODULE_TEST_TWO, "2.3 ≤‚ ‘");

    BCLIB_LOG_INFOR(ELOGMODULE_TEST_ONE, "1.4 ≤‚ ‘");
    BCLIB_LOG_INFOR(ELOGMODULE_TEST_TWO, "2.4 ≤‚ ‘");

    BCLIB_LOG_ERROR(ELOGMODULE_TEST_ONE, "1.5 ≤‚ ‘");
    BCLIB_LOG_ERROR(ELOGMODULE_TEST_TWO, "2.5 ≤‚ ‘");
}

void int_proc(int sig)
{
    printf("int_proc\n");
}

void term_proc(int sig)
{
    printf("term_proc\n");
}

void abrt_proc(int sig)
{
    printf("abrt_proc\n");
}

int main(int argc, char* argv[])
{
    //     signal(SIGINT, int_proc);
    //     signal(SIGFPE, term_proc);
    //     signal(SIGSEGV, term_proc);
    //     signal(SIGTERM, term_proc);
    //     signal(SIGBREAK, term_proc);
    //     signal(SIGABRT, abrt_proc);

    //BCLib::Utility::CLogFile::singleton().setOutFile("./test.log", 100);
    //BCLib::Utility::CLogFile::singleton().setOutFile("./test.log", BCLib::Utility::CFileTracerLog4cxx::EFLT_SECOND);
    BCLib::Utility::CLogFile::singleton().setOutFile("./test.log");
    BCLib::Utility::CLogFile::singleton().setModuleON(BCLib::ELOGMODULE_ALL);
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::ELOGMODULE_ALL, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole();


    BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "≤‚ ‘»’÷æø™ º........");

    while(true)
    {
        testTracer();

        BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "πÿ±’ 1 ≤‚ ‘........");
        BCLIB_LOG_SET_MODULE_OFF(ELOGMODULE_TEST_ONE);

        testTracer();

        BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "πÿ±’ 2 ≤‚ ‘........");
        BCLIB_LOG_SET_MODULE_OFF(ELOGMODULE_TEST_TWO);

        testTracer();

        BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "ø™∆Ù 1 ≤‚ ‘ º∂± infor ........");
        BCLIB_LOG_SET_MODULE_ON(ELOGMODULE_TEST_ONE);
        BCLIB_LOG_SET_MODULE_LEVEL(ELOGMODULE_TEST_ONE, BCLib::Utility::CLog::ELOGLEVEL_INFOR);

        testTracer();

        BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "ø™∆Ù 2 ≤‚ ‘ º∂± warn ........");
        BCLIB_LOG_SET_MODULE_ON(ELOGMODULE_TEST_TWO);
        BCLIB_LOG_SET_MODULE_LEVEL(ELOGMODULE_TEST_TWO, BCLib::Utility::CLog::ELOGLEVEL_WARNING);

        testTracer();
    }
}