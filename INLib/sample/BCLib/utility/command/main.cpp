//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/command/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <BCLib/utility/cmdExecMgr.h>

int testQuit(BCLib::Utility::CProperties& properties, void* tempPara)
{
    printf("√¸¡Ó[%s]\n", properties.getValueString("0").c_str());
    return -1;
}

class CTestCmd : public BCLib::Utility::CCmdExecMgr
{
public:
    CTestCmd()
    {
    }
    virtual ~CTestCmd()
    {
    }

    int login(BCLib::Utility::CProperties& properties, void* tempPara)
    {
        std::string name = properties.getValueString("name");
        std::string password = properties.getValueString("password");
        if(name.empty() || name == "")
        {
            name = properties.getValueString("1");
            if(name.empty() || name == "")
            {
                printf("login ±ÿ–Î”–name ≤Œ ˝\n");
                return 0;
            }
            if(password.empty() || password == "")
            {
                password = properties.getValueString("2");
            }
        }
        else
        {
            if(password.empty() || password == "")
            {
                password = properties.getValueString("1");
            }
        }
        printf("login √¸¡Ó≤Œ ˝name[%s] password[%s]\n", name.c_str(), password.c_str());
        return 1;
    }

protected:
    virtual bool _createCmdExecPtr(const std::string& name, TCmdExecFun*& cmdExecPtr)
    {
        BCLIB_CMDEXEC_DEFINE_BEGIN(name, cmdExecPtr);
            BCLIB_CMDEXEC_GLOBAL_FUN("quit", testQuit);
            BCLIB_CMDEXEC_OBJECT_FUN("login", CTestCmd, &CTestCmd::login, this);
        BCLIB_CMDEXEC_DEFINE_END();
        return BCLib::Utility::CCmdExecMgr::_createCmdExecPtr(name, cmdExecPtr);
    }
};

int main(int argc, char* argv[])
{
    CTestCmd testCmd;
    char buff[2048];
    printf("Ω¯»Î√¸¡Ó≤‚ ‘>>>>>>>>>\n");
    while(true)
    {
        printf("«Î ‰»Î√¸¡Ó:\n");
        gets_s(buff);
        int res = testCmd.executeCommand(buff, NULL);
        if(res < 0)
        {
            break;
        }
    }
    printf("ÕÀ≥ˆ√¸¡Ó≤‚ ‘<<<<<<<<<\n");
}