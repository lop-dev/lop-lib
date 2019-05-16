//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/p2p/msgSample/message.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_SAMPLE_P2P_MSGSAMPLE_MESSAGE_H__
#define __BCLIB_SAMPLE_P2P_MSGSAMPLE_MESSAGE_H__

#include <BCLib/framework/message.h>

#define GAME_NAME_MAX 32
#define GAME_PSWD_MAX 32
#define GAME_TEXT_MAX 256

const BCLib::uint16 ETYPE_P2P_SAMPLE = 1;

enum EMsgID
{
    EMID_PC2PS_REQ_LOGIN = 1,
    EMID_PC2PS_NTF_SAY,
    EMID_PC2PC_NTF_SAY,
    EMID_PC2PC_NTF_SAY_BK,
};

#ifdef WIN32
#pragma pack (push, 1)
#else
#pragma pack (1)
#endif

// µÇÂ¼p2sp·þÎñÆ÷
struct SMsgPc2PsReqLogin : public BCLib::Framework::SMessage
{
public:
    char m_name[GAME_NAME_MAX + 1];
    char m_pswd[GAME_PSWD_MAX + 1];

    void setName(const char* name)
    {
        memcpy(m_name, name, GAME_NAME_MAX);
    }

    void setPassword(const char* password)
    {
        memcpy(m_pswd, password, GAME_PSWD_MAX);
    }

    SMsgPc2PsReqLogin()
    :BCLib::Framework::SMessage(ETYPE_P2P_SAMPLE, EMID_PC2PS_REQ_LOGIN)
    {
        memset(m_name, 0, sizeof(m_name));
        memset(m_pswd, 0, sizeof(m_pswd));
    }
};

struct SMsgPc2PsNtfSay : public BCLib::Framework::SMessage
{
public:
    char m_text[GAME_TEXT_MAX + 1];

    SMsgPc2PsNtfSay()
    :BCLib::Framework::SMessage(ETYPE_P2P_SAMPLE, EMID_PC2PS_NTF_SAY)
    {
        memset(m_text, 0, sizeof(m_text));
    }
    void setText(const std::string& text)
    {
        strncpy(m_text, text.c_str(), GAME_TEXT_MAX);
    }
};

struct SMsgPc2PcNtfSay : public BCLib::Framework::SMessage
{
public:
    char m_text[GAME_TEXT_MAX + 1];
    BCLib::int64 m_time;

    SMsgPc2PcNtfSay()
    :BCLib::Framework::SMessage(ETYPE_P2P_SAMPLE, EMID_PC2PC_NTF_SAY)
    {
        memset(m_text, 0, sizeof(m_text));
    }
    void setText(const std::string& text)
    {
        strncpy(m_text, text.c_str(), GAME_TEXT_MAX);
    }
};

struct SMsgPc2PcNtfSayBk : public BCLib::Framework::SMessage
{
public:
    BCLib::int64 m_time;

    SMsgPc2PcNtfSayBk()
    :BCLib::Framework::SMessage(ETYPE_P2P_SAMPLE, EMID_PC2PC_NTF_SAY_BK)
    {
    }
};

#ifdef WIN32
#pragma pack (pop)
#else
#pragma pack ()
#endif

#endif//__BCLIB_SAMPLE_P2P_MSGSAMPLE_MESSAGE_H__
