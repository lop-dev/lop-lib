//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/dataReader.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_DATAREADER_H__
#define __BCLIB_DATABASE_DATAREADER_H__

#include <BCLib/database/baseDef.h>
#include <BCLib/utility/pointer.h>
#include <string>

namespace BCLib
{
namespace Database
{
class BCLIB_DATABASE_API CDataReader
{
public:
    CDataReader();
    virtual ~CDataReader();

    virtual int32 getRecordCount() const = 0;
    virtual bool next() = 0;

    virtual bool getString(uint32 columnIndex, std::string& strValue)  const = 0;
    virtual bool getString(const char* columnLabel, std::string& strValue) const = 0;

    virtual bool getBoolean(uint32 columnIndex, bool& bValue) const = 0;
    virtual bool getBoolean(const char* columnLabel, bool& bValue) const = 0;

    virtual bool getDouble(uint32 columnIndex, double& dValue) const = 0;
    virtual bool getDouble(const char* columnLabel, double& dValue) const = 0;

    virtual bool getInt32(uint32 columnIndex, int32& iValue) const = 0;
    virtual bool getInt32(const char* columnLabel, int32& iValue) const = 0;

    virtual bool getUint32(uint32 columnIndex, uint32& dwValue) const = 0;
    virtual bool getUint32(const char* columnLabel, uint32& dwValue) const = 0;

    virtual bool getInt64(uint32 columnIndex, int64& qValue) const = 0;
    virtual bool getInt64(const char* columnLabel, int64& qValue) const = 0;

    virtual bool getUint64(uint32 columnIndex, uint64& qwValue) const = 0;
    virtual bool getUint64(const char* columnLabel, uint64& qwValue) const = 0;

    virtual bool getBlob(uint32 columnIndex, void* pBuffer, int32& nBytes) = 0;
    virtual bool getBlob(const char* columnLabel, void* pBuffer, int32& nBytes) = 0;

    virtual void release() = 0;
};

}//Database
}//BCLib

#endif//__BCLIB_DATABASE_DATAREADER_H__
