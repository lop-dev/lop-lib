//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/logFile/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <BCLib/utility/logFile.h>
#include <BCLib/utility/thread/thread.h>

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

int main(int argc, char* argv[])
{
    BCLib::Utility::CLogFile::singleton().setOutFile("./test.log");
    BCLib::Utility::CLogFile::singleton().setModuleON(BCLib::ELOGMODULE_ALL);
    BCLib::Utility::CLogFile::singleton().setModuleLevel(BCLib::ELOGMODULE_ALL, BCLib::Utility::CLog::ELOGLEVEL_DEBUG);
    BCLib::Utility::CLogFile::singleton().setOutConsole();


    BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "≤‚ ‘»’÷æø™ º........");

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