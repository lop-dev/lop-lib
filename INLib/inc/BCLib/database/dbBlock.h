//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/dbBlock.h
//  author:     League of Perfect
/// @brief	
/// 
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_DBBLOACK_H__
#define __BCLIB_DATABASE_DBBLOACK_H__

#include <BCLib/utility/stream.h>
#include <BCLib/database/baseDef.h>

namespace BCLib
{
namespace Database
{
typedef BCLib::uint16 DBVersion;
const DBVersion INVALID_DB_VERSION = 0;

//////////////////////////////////////////////////////////////////////
struct BCLIB_DATABASE_API SDBBlockBase
{
public:
    SDBBlockBase(BCLib::uint16 version);
    virtual ~SDBBlockBase();

    BCLib::uint16 getVersion() const { return m_version; }

    bool serializeTo(BCLib::Utility::CStream& stream) const;
    bool serializeFrom(BCLib::Utility::CStream& stream);

protected:
    virtual bool _serializeTo(BCLib::Utility::CStream& stream) const = 0;
    virtual bool _serializeFrom(BCLib::Utility::CStream& stream) = 0;

private:
    BCLib::uint16 m_version;
};

//////////////////////////////////////////////////////////////////////
struct BCLIB_DATABASE_API SDBBlock
{
public:
    SDBBlock(BCLib::uint16 version);
    virtual ~SDBBlock();

    BCLib::uint16 getVersion() const { return m_version; }

    bool serializeTo(BCLib::Utility::CStream& stream) const;
    bool serializeFrom(BCLib::Utility::CStream& stream);

	virtual bool transform(const BCLib::Database::SDBBlock* dbBlock) = 0;
    virtual void reset() = 0;

protected:
    virtual bool _serializeTo(BCLib::Utility::CStream& stream) const = 0;
    virtual bool _serializeFrom(BCLib::Utility::CStream& stream) = 0;

private:
    BCLib::uint16 m_version;
};

class BCLIB_DATABASE_API CDBBlockMgr
{
public:
    CDBBlockMgr();
    ~CDBBlockMgr();

    template<class DBBLOCK>
    bool registerDBBlock()
    {
        SDBBlock* block = new DBBLOCK();
        if(NULL == block)
        {
            return false;
        }
        if(_findDBBlock(block->getVersion()))
        {
            delete block;
            return false;
        }
        if(!m_DBBlockMapOfVersion.insert(std::make_pair(block->getVersion(), block)).second)
        {
            delete block;
            return false;
        }
        return true;
    }

    enum ESErr { ESE_OK=0, ESE_EMPTY_STREAM, ESE_DATA_ERROR, ESE_UNKNOWN_VERSION, ESE_SERIALIZE_FAIL, ESE_BLOCK_ISNULL, ESE_TRANSFORM_ERROR };
    ESErr serializeFrom(BCLib::Utility::CStream& stream, BCLib::Database::SDBBlock*& dbBlock);
    ESErr serializeFromAndTransform(BCLib::Utility::CStream& stream, BCLib::Database::SDBBlock* dbBlock);

private:
    SDBBlock* _findDBBlock(BCLib::uint16 version) const;

private:
    typedef std::map<BCLib::uint16, SDBBlock*> SDBBlockMapOfVersion;
    SDBBlockMapOfVersion m_DBBlockMapOfVersion;
};

//////////////////////////////////////////////////////////////////////
struct SDBBlockSample1 : public SDBBlock
{
public:
    BCLib::int32 m_i32;
    std::vector<BCLib::int32> m_intList;

public:
    SDBBlockSample1();
    virtual ~SDBBlockSample1();

    virtual bool transform(const BCLib::Database::SDBBlock* dbBlock);
    virtual void reset();

protected:
    virtual bool _serializeTo(BCLib::Utility::CStream& stream) const;
    virtual bool _serializeFrom(BCLib::Utility::CStream& stream);
};

struct SDBBlockSample2 : public SDBBlock
{
public:
    float m_float;
    std::vector<float> m_floatList;

public:
    SDBBlockSample2();
    virtual ~SDBBlockSample2();

    virtual bool transform(const BCLib::Database::SDBBlock* dbBlock);
    virtual void reset();

protected:
    virtual bool _serializeTo(BCLib::Utility::CStream& stream) const;
    virtual bool _serializeFrom(BCLib::Utility::CStream& stream);
};

struct SDBBlockSample3 : public SDBBlock
{
public:
    double m_double;
    std::vector<double> m_doubleList;

public:
    SDBBlockSample3();
    virtual ~SDBBlockSample3();

    virtual bool transform(const BCLib::Database::SDBBlock* dbBlock);
    virtual void reset();

protected:
    virtual bool _serializeTo(BCLib::Utility::CStream& stream) const;
    virtual bool _serializeFrom(BCLib::Utility::CStream& stream);
};

class CDBBlockMgrSample : public CDBBlockMgr
{
public:
    CDBBlockMgrSample();

    void test();
};
}//Database
}//BCLib

#endif//__BCLIB_DATABASE_DBBLOACK_H__
