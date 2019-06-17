// ------------------------------------------------------------------------------
//  <autogenerated>
//      This code was generated by a tool.
//      Changes to this file may cause incorrect behavior and will be lost if 
//      the code is regenerated.
//  </autogenerated>
// ------------------------------------------------------------------------------

#ifndef PROTOEXT_CProjectInfo__INCLUDED
#define PROTOEXT_CProjectInfo__INCLUDED

#include <BCLib/utility/singleton.h>
#include <BCLib/utility/hashMap.h>
#include "projectInfo.pb.h"

namespace PTExt
{

class CProjectInfo
{
public:
    CProjectInfo();
    CProjectInfo(const PTBuf::CProjectInfo& ptBuf);
    virtual ~CProjectInfo();

    PTBuf::CProjectInfo& getBuf();
    void dumpInfo();

    static bool isValidity(PTBuf::CProjectInfo& ptBuf);
    static bool makeValidity(PTBuf::CProjectInfo& ptBuf);
    static bool canUpdate(PTBuf::CProjectInfo& oldBuf, PTBuf::CProjectInfo& newBuf);

private:
    PTBuf::CProjectInfo m_ptBuf;
};

// Use EntityID Key
class CProjectInfoOwner
{
    BCLIB_SINGLETON_DECLARE(CProjectInfoOwner);

public:
    typedef BCLib::Utility::CHashMap<BCLib::uint64, PTExt::CProjectInfo> THashMap;

protected:
    CProjectInfoOwner();
    virtual ~CProjectInfoOwner();

public:
    bool addObject(BCLib::uint64 key, const PTBuf::CProjectInfo& ptBuf);
    bool addObject(BCLib::uint64 key, const PTExt::CProjectInfo& ptBuf);
    PTExt::CProjectInfo* getObject(BCLib::uint64 entityID);
    bool delObject(BCLib::uint64 entityID);

    THashMap& getMap(){ return m_hashMap; }
    void dumpInfo(BCLib::uint64 entityID);

private:
    THashMap m_hashMap;
};

// Use Oneself Key
class CProjectInfoList
{
public:
    CProjectInfoList();
    CProjectInfoList(const PTBuf::CProjectInfoList& ptList);
    virtual ~CProjectInfoList();

    PTBuf::CProjectInfoList& getList();
    void dumpInfo();

private:
    PTBuf::CProjectInfoList m_List;
};

// Only define CProjectInfoList singleton
class CProjectInfoListCtrl : public CProjectInfoList
{
    BCLIB_SINGLETON_DECLARE(CProjectInfoListCtrl);

protected:
    CProjectInfoListCtrl();
    virtual ~CProjectInfoListCtrl();
};

// Use EntityID Key
class CProjectInfoListOwner
{
    BCLIB_SINGLETON_DECLARE(CProjectInfoListOwner);

public:
    typedef BCLib::Utility::CHashMap<BCLib::uint64, PTExt::CProjectInfoList> THashMap;

protected:
    CProjectInfoListOwner();
    virtual ~CProjectInfoListOwner();

public:
    bool addObject(BCLib::uint64 entityID, const PTBuf::CProjectInfoList& ptList);
    bool addObject(BCLib::uint64 entityID, const PTExt::CProjectInfoList& ptList);
    PTExt::CProjectInfoList* getObject(BCLib::uint64 entityID);
    bool delObject(BCLib::uint64 entityID);

    THashMap& getMap(){ return m_hashMap; }
    void dumpInfo(BCLib::uint64 entityID);

private:
    THashMap m_hashMap;
};

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_SENTENCE_INSERT_CProjectInfo(_PTBUF_)    \
    sqlSentence.str("");    \
    sqlSentence << "INSERT INTO " << strTBName.c_str() << " ";    \
    sqlSentence << "( ";    \
    sqlSentence << "ProjectName, ";    \
    sqlSentence << "AccountID, ";    \
    sqlSentence << "ProjectType, ";    \
    sqlSentence << "ProjectState, ";    \
    sqlSentence << "Members ";    \
    sqlSentence << ") ";    \
    sqlSentence << "VALUES ";    \
    sqlSentence << "( ";    \
    sqlSentence << "'" << _PTBUF_.projectname() << "', ";    \
    sqlSentence << "'" << _PTBUF_.accountid() << "', ";    \
    sqlSentence << "'" << _PTBUF_.projecttype() << "', ";    \
    sqlSentence << "'" << _PTBUF_.projectstate() << "', ";    \
    sqlSentence << "'" << _PTBUF_.members() << "' ";    \
    sqlSentence << ");";

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_SENTENCE_SELECT_CProjectInfo(_PTBUF_)    \
    sqlSentence.str("");    \
    sqlSentence << "SELECT * FROM " << strTBName.c_str() << " ";    \
    sqlSentence << "WHERE ProjectName = '" << _PTBUF_.projectname() << "'; ";

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_SENTENCE_UPDATE_CProjectInfo(_PTBUF_)    \
    sqlSentence.str("");    \
    sqlSentence << "UPDATE " << strTBName.c_str() << " SET ";    \
    sqlSentence << "AccountID = '" << _PTBUF_.accountid() << "', ";    \
    sqlSentence << "ProjectType = '" << _PTBUF_.projecttype() << "', ";    \
    sqlSentence << "ProjectState = '" << _PTBUF_.projectstate() << "', ";    \
    sqlSentence << "Members = '" << _PTBUF_.members() << "' ";    \
    sqlSentence << "WHERE ProjectName = '" << _PTBUF_.projectname() << "'; ";

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_SENTENCE_DELETE_CProjectInfo(_PTBUF_)    \
    sqlSentence.str("");    \
    sqlSentence << "DELETE FROM " << strTBName.c_str() << " ";    \
    sqlSentence << "WHERE ProjectName = '" << _PTBUF_.projectname() << "'; ";

#define CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ExecuteReader_CProjectInfo(_PTBUF_)    \
    CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadUint64(_PTBUF_, projectid);    \
    CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadString(_PTBUF_, projectname);    \
    CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadUint64(_PTBUF_, accountid);    \
    CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadUint32(_PTBUF_, projecttype);    \
    CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadUint32(_PTBUF_, projectstate);    \
    CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadString(_PTBUF_, members);    \
    CDATABASETASK_SUBCLASS_DEFINE_REPLY_SQL_ReadString(_PTBUF_, modifytime);

} // namespace PTExt

#endif // PROTOEXT_CProjectInfo__INCLUDED