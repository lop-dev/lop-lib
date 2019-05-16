//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/dbParamDef.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_DBPARAMDEF_H__
#define __BCLIB_DATABASE_DBPARAMDEF_H__

#include <BCLib/database/baseDef.h>

namespace BCLib
{
namespace Database
{
typedef enum
{
    DB_PARAM_TYPE_NULL,
    DB_PARAM_TYPE_BOOLEAN,
    DB_PARAM_TYPE_INT8,
    DB_PARAM_TYPE_UINT8,
    DB_PARAM_TYPE_INT16,
    DB_PARAM_TYPE_UINT16,
    DB_PARAM_TYPE_INT32,
    DB_PARAM_TYPE_UINT32,
    DB_PARAM_TYPE_INT64,
    DB_PARAM_TYPE_UINT64,
    DB_PARAM_TYPE_FLOAT,
    DB_PARAM_TYPE_DOUBLE,
    DB_PARAM_TYPE_STRING,
    DB_PARAM_TYPE_BLOB,
} ENUM_DB_PARAM_TYPE;

typedef struct _DB_PARAM_TYPE
{
    ENUM_DB_PARAM_TYPE type;
    union
    {
        bool bValue;
        int8 i8Value;
        uint8 u8Value;
        int16 i16Value;
        uint16 u16Value;
        int32 i32Value;
        uint32 u32Value;
        int64 i64Value;
        uint64 u64Value;
        float fValue;
        double dValue;
        char* strValue;
        struct
        {
            void* pBlobData;
            int32 nBlobLen;
        };
    } value;

    _DB_PARAM_TYPE()
    :type(DB_PARAM_TYPE_NULL)
    {
        memset(&value, 0, sizeof(value));
    }
}DB_PARAM_TYPE, * DB_PARAM_TYPE_P;

typedef enum
{
    DB_PROCEDURE_PARAM_DIR_NULL,
    DB_PROCEDURE_PARAM_DIR_INPUT,
    DB_PROCEDURE_PARAM_DIR_OUTPUT,
    DB_PROCEDURE_PARAM_DIR_BOTH,
} ENUM_DB_PROCEDURE_PARAM_DIR;

typedef struct _DB_PROCEDURE_PARAM_TYPE
{
    ENUM_DB_PARAM_TYPE type;
    ENUM_DB_PROCEDURE_PARAM_DIR direction;
    void* databuf;
    uint32 datalen;

    _DB_PROCEDURE_PARAM_TYPE()
    :type(DB_PARAM_TYPE_NULL)
    ,direction(DB_PROCEDURE_PARAM_DIR_NULL)
    ,databuf(NULL)
    ,datalen(0)
    {
    }

    void value(bool& bValue, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_BOOLEAN;
        direction = dir;
        databuf = &bValue;
        datalen = sizeof(bool);
    }

    void value(int8& i8Value, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_INT8;
        direction = dir;
        databuf = &i8Value;
        datalen = sizeof(int8);
    }

    void value(uint8& u8Value, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_UINT8;
        direction = dir;
        databuf = &u8Value;
        datalen = sizeof(uint8);
    }

    void value(int16& i16Value, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_INT16;
        direction = dir;
        databuf = &i16Value;
        datalen = sizeof(int16);
    }

    void value(uint16& u16Value, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_UINT16;
        direction = dir;
        databuf = &u16Value;
        datalen = sizeof(uint16);
    }

    void value(int32& i32Value, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_INT32;
        direction = dir;
        databuf = &i32Value;
        datalen = sizeof(int32);
    }

    void value(uint32& u32Value, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_UINT32;
        direction = dir;
        databuf = &u32Value;
        datalen = sizeof(uint32);
    }

    void value(int64& i64Value, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_INT64;
        direction = dir;
        databuf = &i64Value;
        datalen = sizeof(int64);
    }

    void value(uint64& u64Value, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_UINT64;
        direction = dir;
        databuf = &u64Value;
        datalen = sizeof(uint64);
    }

    void value(float& fValue, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_FLOAT;
        direction = dir;
        databuf = &fValue;
        datalen = sizeof(float);
    }

    void value(double& dValue, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_DOUBLE;
        direction = dir;
        databuf = &dValue;
        datalen = sizeof(double);
    }

    void value(const char* pszValue)
    {
        type = DB_PARAM_TYPE_STRING;
        direction = DB_PROCEDURE_PARAM_DIR_INPUT;
        databuf = (char*)pszValue;
        datalen = (int32)strlen(pszValue);
    }

    void value(void* pBlob, int32 i32Len, ENUM_DB_PROCEDURE_PARAM_DIR dir = DB_PROCEDURE_PARAM_DIR_INPUT)
    {
        type = DB_PARAM_TYPE_BLOB;
        direction = dir;
        databuf = pBlob;
        datalen = i32Len;
    }

    bool getBool() const
    {
        return *((bool*)databuf);
    }

    void setBool(bool value)
    {
        if(datalen >= sizeof(value))
        {
            *((bool*)databuf) = value;
        }
    }

    int8 getInt8() const
    {
        return *((int8 *)databuf);
    }

    void setInt8(int8 value)
    {
        if(datalen >= sizeof(value))
        {
            *((int8 *)databuf) = value;
        }
    }

    uint8 getUint8() const
    {
        return *((uint8 *)databuf);
    }

    void setUint8(uint8 value)
    {
        if(datalen >= sizeof(value))
        {
            *((uint8 *)databuf) = value;
        }
    }

    int16 getInt16() const
    {
        return *((int16 *)databuf);
    }

    void setInt16(int16 value)
    {
        if(datalen >= sizeof(value))
        {
            *((int16 *)databuf) = value;
        }
    }

    uint16 getUint16() const
    {
        return *((uint16 *)databuf);
    }

    void setUint16(uint16 value)
    {
        if(datalen >= sizeof(value))
        {
            *((uint16 *)databuf) = value;
        }
    }

    int32 getInt32() const
    {
        return *((int32 *)databuf);
    }

    void setInt32(int32 value)
    {
        if(datalen >= sizeof(value))
        {
            *((int32 *)databuf) = value;
        }
    }

    uint32 getUint32() const
    {
        return *((uint32 *)databuf);
    }

    void setUint32(uint32 value)
    {
        if(datalen >= sizeof(value))
        {
            *((uint32 *)databuf) = value;
        }
    }

    int64 getInt64() const
    {
        return *((int64 *)databuf);
    }

    void setInt64(int64 value)
    {
        if(datalen >= sizeof(value))
        {
            *((int64 *)databuf) = value;
        }
    }

    uint64 getUint64() const
    {
        return *((uint64 *)databuf);
    }

    void setUint64(uint64 value)
    {
        if(datalen >= sizeof(value))
        {
            *((uint64 *)databuf) = value;
        }
    }

    float getFloat() const
    {
        return *((float*)databuf);
    }

    void setFloat(float value)
    {
        if(datalen >= sizeof(value))
        {
            *((float*)databuf) = value;
        }
    }

    double getDouble() const
    {
        return *((double*)databuf);
    }

    void setDouble(double value)
    {
        if(datalen >= sizeof(value))
        {
            *((double*)databuf) = value;
        }
    }

    const char* getString() const
    {
        return (const char*)databuf;
    }

    void setString(const char* value)
    {
        uint32 len = (uint32)strlen(value);

        if(datalen < len)
        {
            len = datalen;
        }

        if(len > 0)
        {
            --len;
            memcpy(databuf, value, len);
            *((char*)databuf + len) = 0;
        }
    }

    void* getBlob()
    {
        return databuf;
    }

    void setBlob(void* value, int32 len)
    {
        if(datalen > (uint32)len)
        {
            datalen = len;
        }
        memcpy(databuf, value, datalen);
    }
}DB_PROCEDURE_PARAM_TYPE, * DB_PROCEDURE_PARAM_TYPE_P;
}//Database
}//BCLib

#endif//__BCLIB_DATABASE_DBPARAMDEF_H__
