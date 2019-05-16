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


namespace BCLib
{
namespace Utility
{
class CExcelBook;

//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CXmlExcelCell
{
public:
    CXmlExcelCell()
    :m_isValid(false)
    {
    }
    virtual ~CXmlExcelCell()
    {
    }

    bool getString(std::string& value) const;
    bool getBoolean(bool& value) const;
    bool getDouble(double& value) const;
    bool getFloat(float& value) const;
    bool getInt32(int32& value) const;
    bool getUint32(uint32& value) const;
    bool getInt64(int64& value) const;
    bool getUint64(uint64& value) const;

    const std::string& getString() const;
    bool getBoolean() const;
    double getDouble() const;
    float getFloat() const;
    int32 getInt32() const;
    uint32 getUint32() const;
    int64 getInt64() const;
    uint64 getUint64() const;

    bool setString(const std::string& value);
    bool setBoolean(bool& value);
    bool setDouble(double& value);
    bool setFloat(float& value);
    bool setInt32(int32& value);
    bool setUint32(uint32& value);
    bool setInt64(int64& value);
    bool setUint64(uint64& value);

    CXmlExcelCell& operator =(const std::string& value);
    CXmlExcelCell& operator =(bool& value);
    CXmlExcelCell& operator =(double& value);
    CXmlExcelCell& operator =(float& value);
    CXmlExcelCell& operator =(int32& value);
    CXmlExcelCell& operator =(uint32& value);
    CXmlExcelCell& operator =(int64& value);
    CXmlExcelCell& operator =(uint64& value);

    template<class VALUE_TYPE>
    bool getValue(VALUE_TYPE& value) const;

    template<class VALUE_TYPE>
    bool setValue(VALUE_TYPE& value);

    template<class VALUE_TYPE>
    bool operator()(VALUE_TYPE& value) const
    {
        return getValue(value);
    }

protected:
    std::string m_value;
    bool m_isValid;
};

template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(std::string& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(bool& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(double& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(float& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(int8& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(uint8& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(int16& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(uint16& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(int32& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(uint32& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(int64& value) const;
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::getValue(uint64& value) const;

template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(std::string& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(bool& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(double& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(float& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(int8& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(uint8& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(int16& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(uint16& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(int32& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(uint32& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(int64& value);
template<>
BCLIB_UTILITY_API bool CXmlExcelCell::setValue(uint64& value);

typedef CXmlExcelCell CExcelTableCell;
typedef CTableRow<void, CExcelTableCell> CExcelTableRow;
typedef CTable<void, CExcelTableCell, CExcelTableRow> CExcelTable;

//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CXmlExcel : public BCLib::Utility::CNoncopyable
{
public:
    /*    struct SLoadTableArea
        {
            std::string tableName;
            int m_beginRow;
            int m_beginColumn;
            int m_endRow;
            int m_endColumn;
        };
        struct SLoadFileConfig
        {
            std::vector<SLoadTableArea> m_loadTableArea;
        };
    */
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
    CExcelBook* m_excelBook;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_IO_XMLEXCEL_H__
