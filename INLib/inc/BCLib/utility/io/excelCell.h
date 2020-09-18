//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/excelCell.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_EXCELCELL_H__
#define __BCLIB_UTILITY_IO_EXCELCELL_H__

#include <string>
#include <BCLib/utility/baseDec.h>
#include <BCLib/utility/table.h>

namespace BCLib
{
namespace Utility
{
class BCLIB_UTILITY_API CExcelCell
{
public:
    CExcelCell()
    :m_isValid(false)
    {
    }
    virtual ~CExcelCell()
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

    CExcelCell& operator =(const std::string& value);
    CExcelCell& operator =(bool& value);
    CExcelCell& operator =(double& value);
    CExcelCell& operator =(float& value);
    CExcelCell& operator =(int32& value);
    CExcelCell& operator =(uint32& value);
    CExcelCell& operator =(int64& value);
    CExcelCell& operator =(uint64& value);

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
BCLIB_UTILITY_API bool CExcelCell::getValue(std::string& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(bool& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(double& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(float& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(int8& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(uint8& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(int16& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(uint16& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(int32& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(uint32& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(int64& value) const;
template<>
BCLIB_UTILITY_API bool CExcelCell::getValue(uint64& value) const;

template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(std::string& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(bool& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(double& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(float& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(int8& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(uint8& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(int16& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(uint16& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(int32& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(uint32& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(int64& value);
template<>
BCLIB_UTILITY_API bool CExcelCell::setValue(uint64& value);

//////////////////////////////////////////////////////////////////////////
typedef CExcelCell CExcelTableCell;
typedef CTableRow<void, CExcelTableCell> CExcelTableRow;
typedef CTable<void, CExcelTableCell, CExcelTableRow> CExcelTable;
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_IO_EXCELCELL_H__
