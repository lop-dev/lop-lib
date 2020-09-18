//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/xmlExcel.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_XMLEXCEL_H__
#define __BCLIB_UTILITY_IO_XMLEXCEL_H__

#include <string>
#include <BCLib/utility/baseDec.h>
#include <BCLib/utility/table.h>
#include <BCLib/utility/io/excelCell.h>

namespace BCLib
{
namespace Utility
{
class CXmlExcelBook;

class BCLIB_UTILITY_API CXmlExcel : public BCLib::Utility::CNoncopyable
{
public:
    CXmlExcel();
    ~CXmlExcel();

    bool createFile(const std::string& tableName, int columnMax);
    bool createFile(const std::string& fileName, const std::string& tableName, int columnMax);

    bool loadFile(const std::string& fileName);
    bool loadString(const std::string& xmlString);
    bool save();
    bool save(const std::string& fileName);
    void clear();

    bool appendTable(const std::string& tableName, int columnMax);
    int tableCount();

    CExcelTable* getTable(const std::string& tableName) const;
    CExcelTable* getTable(int tableNum) const;

    const std::string& getTableName(int tableNum) const;

    CExcelTable& operator[](const std::string& tableName) const
    {
        return *getTable(tableName);
    }

    CExcelTable& operator[](int tableNum) const
    {
        return *getTable(tableNum);
    }

private:
    bool _loadFromFile();
    bool _saveToFile();
    bool _loadFromString(const std::string& xmlString);

private:
    std::string m_fileName;
    CXmlExcelBook* m_excelBook;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_IO_XMLEXCEL_H__
