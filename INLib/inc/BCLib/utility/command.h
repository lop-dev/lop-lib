//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/command.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_COMMAND_H__
#define __BCLIB_UTILITY_COMMAND_H__

#include <BCLib/utility/hashMap.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CProperties
{
public:
    CProperties();
    virtual ~CProperties();

    void setValueString(const std::string& key, const std::string& value);
    const std::string& getValueString(const std::string& key);
    bool getValueString(const std::string& key, std::string& value);

    uint32 getValueInt32(const std::string& key);
    bool getValueInt32(const std::string& key, uint32& value);

    void clear();
    const std::string& operator[](const std::string& key);

protected:
private:
    CHashMap<std::string, std::string> m_properties;
};

class BCLIB_UTILITY_API CCommand
{
public:
    CCommand();
    virtual ~CCommand();

    int parseCommand(const std::string& cmdLine, void* tempPara);
    bool getCommand(const std::string& cmdLine, std::string& command);
    bool getCommand(const std::string& cmdLine, std::string& command, CProperties& properties);

private:
    int _split(const std::string& cmdLine, std::vector<std::string>& subCmdList, char separator);
    virtual int _execCmd(const std::string& command, CProperties& properties, void* tempPara) = 0;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_COMMAND_H__
