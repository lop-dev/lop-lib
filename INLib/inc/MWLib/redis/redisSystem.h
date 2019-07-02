//////////////////////////////////////////////////////////////////////
//  created:    2014/07/18
//  filename:   MWLib/redis/redisSystem.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_REDIS_REDISSYSTEM_H__
#define __MWLIB_REDIS_REDISSYSTEM_H__

#include <google/protobuf/message_lite.h>
#include <SFLib/commonDefine/logFile.h>
#include <BCLib/utility/singleton.h>
#include <MWLib/redis/redlock.h>
#include <unordered_map>
#include <hiredis.h>

#define MWLIB_REDIS_DEFAULT_UNIQUEID 1000000

#define MWLIB_REDIS_LOCK(key, uniqueid, subkey)    \
    CRedLock* pRedLock = MWLib::Redis::CRedisSystem::singleton().getRedLock();    \
    CLock __my_lock_;    \
    if (pRedLock != NULL)    \
    {    \
        while (1){    \
            char key1[1024] = { 0 };    \
            std::string fild = BCLib::Utility::CConvert::toStringA(uniqueid);    \
            char subkey1[512] = { 0 };    \
            snprintf(subkey1, 512, "%s:lock", subkey);    \
            char key2[512] = { 0 };    \
            snprintf(key2, 512, "%s:[%s]:", key, fild.c_str());    \
            snprintf(key1, 1024, "%s%s",key2, subkey1);    \
            bool _lock_flag_ = pRedLock->Lock(key1, 200, __my_lock_);    \
            if(_lock_flag_)    \
            {

#define MWLIB_REDIS_UNLOCK    \
                pRedLock->Unlock(__my_lock_);    \
                break;    \
            }    \
            else    \
            {    \
                BCLib::Utility::CThread::msleep(rand() % 3);    \
            }    \
        }\
    }

#define MWLIB_REDIS_RETURN    \
    if (pRedLock!=NULL)    \
    {    \
        pRedLock->Unlock(__my_lock_);    \
    }    \
    return;

namespace MWLib
{
    namespace Redis
    {
        enum EREDIS_CONTEXT_TYPE
        {
            E_REDIS_SERVERTYPE_BEGIN = 0,
            E_REDIS_SERVERTYPE_LOGIC,
            E_REDIS_SERVERTYPE_CACHE,
            E_REDIS_SERVERTYPE_FRIEND,
            E_REDIS_SERVERTYPE_END,

        };

        enum EREDIS_ACCESS_RIGHT_TYPE
        {
            E_REDIS_READ_AND_WRITE = 0,
            E_REDIS_READ_ONLY,
        };

        typedef struct Redis_Node
        {
            redisContext *m_redisContext;

            std::string m_host;
            int m_port;
            std::string m_passwd;
            Redis_Node()
            {
                m_redisContext = NULL;
                m_host = "";
                m_port = 0;
                m_passwd = "";

            }
            ~Redis_Node()
            {
                if (m_redisContext != NULL)
                {
                    redisFree(m_redisContext);
                    m_redisContext = NULL;
                }
            }
        }REDIS_NODE;

        class CRedisSystem
        {
            BCLIB_SINGLETON_DECLARE(CRedisSystem);

        protected:
            CRedisSystem();
            virtual ~CRedisSystem();

        public:
            /**
            * 功能:以二进制的形式保存ptbuf
            * @param key 键值即对应hash表 名这里使用服务系统名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param strPTbufName 要保存的ptbuf名
            * @param pPtbuf 要保存的ptbuf指针
            * @return true Or false
            */
            bool savePTBuf(std::string &key, BCLib::uint64 uniqueid, std::string & strPTbufName, const ::google::protobuf::MessageLite *pPtbuf, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:以二进制的形式保存ptbuf
            * @param key 键值即对应hash表 名这里使用服务系统名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param strPTbufName 要保存的ptbuf名
            * @param pPtbuf 接收容器的ptbuf指针
            * @param ptCacheSize 接收容器大小，默认为 (1024*16) redis 接收容器大小
            * @return 返回从redis读取的二进制数据长度，不会超过(1024*16)
            */
            BCLib::uint32 loadPTBuf(std::string &key, BCLib::uint64 uniqueid, std::string & strPTbufName, ::google::protobuf::MessageLite *pPtbuf, BCLib::uint32 ptCacheSize = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

        public:
            void setInfo(std::string& host, int port, std::string& passwd, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            bool init(EREDIS_ACCESS_RIGHT_TYPE type = E_REDIS_READ_AND_WRITE);

            bool connect(std::string host, int port, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            bool connectWithTimeout(std::string host, int port, const struct timeval tv, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            void  disconnect(EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            void  disconnectAll();
            bool  exec(const char *cmd, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            bool setCommandTimeout(const struct timeval tv, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            bool checkStatus(EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            bool clear(EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //String
            bool setString(const char *key, const char *value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            bool setString(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            std::string getString(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            std::string getString(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //Uint64
            bool setUint64(const char *key, BCLib::uint64 value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            bool setUint64(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::uint64 value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            BCLib::uint64 getUint64(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            BCLib::uint64 getUint64(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //Bin
            bool setBin(const char *key, const char *value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            bool setBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            BCLib::uint32 getBin(const char *key, char *value, BCLib::uint32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            BCLib::uint32 getBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, char *value, BCLib::uint32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //删除指定key和其下所有值
            bool delKey(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            bool delKey(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //设置消亡时间 单位s -1 代表永久
            bool expireSecond(const char *key, BCLib::int32 second, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            bool expireSecond(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 second, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //移除消亡时间
            bool persist(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            bool persist(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /*	KEYS命令，返回所有相匹配的key，O(N)效率较低，尽量别用
            KEYS * 匹配数据库中所有 key 。
            KEYS h?llo 匹配 hello ， hallo 和 hxllo 等。
            KEYS h*llo 匹配 hllo 和 heeeeello 等。
            KEYS h[ae]llo 匹配 hello 和 hallo ，但不匹配 hillo 。
            特殊符号用 \ 隔开
            * @param key 需要匹配的key的字符串
            * @param keys 返回的所有相匹配的key
            */
            bool keys(const char *key, std::vector<std::string> &values, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /*迭代遍历 key
            * @param matchKey * 匹配数据库中所有 key 。
                            KEYS h?llo 匹配 hello ， hallo 和 hxllo 等。
                            KEYS h*llo 匹配 hllo 和 heeeeello 等。
                            KEYS h[ae]llo 匹配 hello 和 hallo ，但不匹配 hillo 。
                            特殊符号用 \ 隔开
            * @param values 返回的keys
            * @param start 遍历起始游标
            * @param count 遍历返回最大数量
            * @param type 所使用的redis服务器
            * @return 返回本次遍历结束时的游标，为0表示已全部遍历
            */
            BCLib::uint64 scan(const char *matchKey, std::vector<std::string> &values, BCLib::uint64 start = 0, BCLib::uint64 count = 50, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /*
            *map 封装↓↓↓↓↓
            */

            /**
             * 功能:将特定field - value(域-值)对设置到哈希表key中
             * @param key 键值即对应hash表名
             * @param field 哈希表key中的域
             * @param value 对应value
             */
            bool hsetString(const char*key, const char*field, const char*value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:将特定field - value(域-值)对设置到哈希表key中
            * @param key 键值即对应hash表名
            * @param field 哈希表key中的域
            * @param value 对应value
            */
            bool hsetString(const char*key, const char*field, const std::string &value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:将特定field - value(域-值)对设置到哈希表key中
            * @param key 键值即对应hash表名
            * @param field 哈希表key中的域
            * @param value 对应二进制流指针value
            * @param len 二进制流长度
            */
            bool hsetBin(const char *key, const char *field, const char *value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:将特定field - value(域-值)对设置到哈希表key中
            * @param key 键值即对应hash表名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @param field 哈希表key中的域 字符串类型
            * @param value 对应value 字符串地址
            */
            bool hsetString(const char*key, BCLib::uint64 uniqueid, const char*subkey, const char*field, const char*value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:将特定field - value(域-值)对设置到哈希表key中
            * @param key 键值即对应hash表名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @param field 哈希表key中的域 字符串类型
            * @param value 对应value 字符串地址
            */
            bool hsetString(const char*key, BCLib::uint64 uniqueid, const char*subkey, const char*field, const std::string &value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:将特定field - value(域-值)对设置到哈希表key中
            * @param key 键值即对应hash表名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @param field 哈希表key中的域 字符串类型
            * @param value 对应value 二进制流指针
            * @param len value二进制流长度
            */
            bool hsetBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *field, const char *value, BCLib::uint32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
             * 功能:获取哈希表key特定field值
             * @param key 键值即对应hash表名
             * @param field 哈希表key中的域
             * @return  对应域value
             */
            std::string hgetString(const char*key, const char*field, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:将特定field - value(域-值)对设置到哈希表key中
            * @param field 哈希表key中的域 字符串类型
            * @param value 二进制流的接收buffer指针
            * @param len 接收buffer的长度
            * @return 读出的二进制流长度
            */
            BCLib::uint32 hgetBin(const char *key, const char *field, char *value, BCLib::uint32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:将特定field - value(域-值)对设置到哈希表key中
            * @param key 键值即对应hash表名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @param field 哈希表key中的域 字符串类型
            * @return  对应域value
            */
            std::string hgetString(const char*key, BCLib::uint64 uniqueid, const char*subkey, const char*field, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:将特定field - value(域-值)对设置到哈希表key中
            * @param key 键值即对应hash表名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @param field 哈希表key中的域 字符串类型
            * @param len 接收buffer的长度
            * @return 读出的二进制流长度
            */
            BCLib::uint32 hgetBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *field, char *value, BCLib::uint32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
             * 功能:获取哈希表key特定所有值
             * @param key 键值即对应hash表名
             * @param field_values 获取到的hash表field - value(域-值)
             */
            void hgetall(const char *key, std::map<std::string, std::string>& field_values, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:获取哈希表key特定所有值
            * @param key 键值即对应hash表名
            * @return const std::map<std::string, std::pair<char*, BCLib::uint32>*>* 的map<键值，<二进制流，流长度>>指针
            */
            const std::map<std::string, std::pair<char*, BCLib::uint32>*>* hgetallBin(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:获取哈希表key特定所有值
            * @param key 键值即对应hash表名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @param field_values 获取到的hash表field - value(域-值)
            */
            void hgetall(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::map<std::string, std::string>& field_values, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:获取哈希表key特定所有值
            * @param key 键值即对应hash表名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @return const std::map<std::string, std::pair<char*, BCLib::uint32>*>* 的map<键值，<二进制流，流长度>>指针
            */
            const std::map<std::string, std::pair<char*, BCLib::uint32>*>* hgetallBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
             * 功能:同时将多个field - value(域-值)对设置到哈希表key中
             * @param key 键值即对应hash表名
             * @param fields_value 多个field value(域 值)
             * e.g.: hmset key_hash name "李三" age 18 birthday "20010101"
             */
            void hmset(const char *key, EREDIS_CONTEXT_TYPE type, std::string format, ...);

            /**
            * 功能:同时将多个field - value(域-值)对设置到哈希表key中
            * @param key 键值即对应hash表名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @param fields_value 多个field value(域 值)
            * e.g.: hmset key_hash name "李三" age 18 birthday "20010101"
            */
            void hmset(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type, std::string format, ...);

            //void hmsetBin(const char *key, const char * filed, const char * value, BCLib::uint32 len);
            //void hmsetBin(const char*key, BCLib::uint64 uniqueid, const char*subkey, const char *filed, const char *value, BCLib::uint32 len);

            /**
            * 功能:删除哈希表key特定field-value键值对
            * @param key 键值即对应hash表名
            * @param field 哈希表key中的域
            * @return  true or false
            */
            bool herase(const char *key, const char *field, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能:删除哈希表key特定field-value键值对
            * @param key 键值即对应hash表名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @param field 哈希表key中的域
            * @return  true or false
            */
            bool herase(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *field, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除一个成员或多个成员
            * @param key 集合名
            * @param matchKey 匹配字符串，匹配规则同scan
            * @param field_values 返回key-value map
            * @param start 遍历开始游标
            * @param count 遍历个数，最大返回成员数
            * @param type 所使用的redis服务器
            * @return 返回遍历结束时游标
            */
            BCLib::uint64 hscan(const char *key, const char *matchKey, std::map<std::string, std::string>& field_values, BCLib::uint64 start = 0, BCLib::uint64 count = 50, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除一个成员或多个成员
            * @param key 集合名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @param matchKey 匹配字符串，匹配规则同scan
            * @param field_values 返回key-value map
            * @param start 遍历开始游标
            * @param count 遍历个数，最大返回成员数
            * @param type 所使用的redis服务器
            * @return 返回遍历结束时游标
            */
            BCLib::uint64 hscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, std::map<std::string, std::string>& field_values, BCLib::uint64 start = 0, BCLib::uint64 count = 50, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除一个成员或多个成员
            * @param key 集合名
            * @param matchKey 匹配字符串，匹配规则同scan
            * @param end 返回本次遍历的结束时的游标 0表示完全遍历
            * @param start 遍历开始游标
            * @param count 遍历个数，最大返回成员数
            * @param type 所使用的redis服务器
            * @return 返回key-value map
            */
            const std::map<std::string, std::pair<char*, BCLib::uint32>*>* hscan(const char *key, const char *matchKey, BCLib::uint64 &end, BCLib::uint64 start = 0, BCLib::uint64 count = 50, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除一个成员或多个成员
            * @param key 集合名
            * @param uniqueid 实体ID 没有实体ID的可以考虑统一使用1000000来代替
            * @param subkey 二级表名
            * @param matchKey 匹配字符串，匹配规则同scan
            * @param end 返回本次遍历的结束时的游标 0表示完全遍历
            * @param start 遍历开始游标
            * @param count 遍历个数，最大返回成员数
            * @param type 所使用的redis服务器
            * @return 返回key-value map
            */
            const std::map<std::string, std::pair<char*, BCLib::uint32>*>* hscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, BCLib::uint64 &end, BCLib::uint64 start = 0, BCLib::uint64 count = 50, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /*
            *list 封装↓↓↓↓↓
            */

            /**
            * 功能：将指定值value添加到list头部
            * @param key 列表名
            * @param value 添加到列表中的value值string 类型
            */
            bool lpushString(const char *key, const char *value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：将指定值value添加到list头部
            * @param key 列表名
            * @param value 添加到列表中的value值二进制流指针
            * @param len 二进制流长度
            */
            bool lpushBin(const char *key, const char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：将指定值value添加到list头部
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param value 添加到列表中的value值string 类型
            */
            bool lpushString(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：将指定值value添加到list头部
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param value 添加到列表中的value值二进制流指针
            * @param len 二进制流长度
            */
            bool lpushBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取列表内头部元素值
            * @param key 列表名
            * @return 返回读取到的数据value，string类型
            */
            std::string lreadString(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取列表内头部元素值
            * @param key 列表名
            * @param value 读取到的数据value，接收二进制buffer指针
            * @param len 接收buffer大小
            * @return 读取的二进制数据流大小
            */
            BCLib::uint32 lreadBin(const char *key, char *value, const BCLib::int32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取列表内头部元素值
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey
            * @param value 读取到的数据value，字符串类型
            */
            std::string lreadString(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取列表内头部元素值
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey
            * @param value 二进制数据流的接收buffer
            * @param len 二进制数据流的接收buffer长度
            * @return 读取的返回二进制数据流长度
            */
            BCLib::uint32 lreadBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, char *value, const BCLib::int32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：将指定值value添加到list尾部
            * @param key 列表名
            * @param value 添加到列表中的value值字符串类型
            */
            bool rpushString(const char *key, const char *value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：将指定值value添加到list尾部
            * @param key 列表名
            * @param value 添加到列表中的value二进制数据流
            * @param len 二进制数据流长度
            */
            bool rpushBin(const char *key, const char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：将指定值value添加到list尾部
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey
            * @param value 添加到列表中的value值字符串类型
            */
            bool rpushString(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：将指定值value添加到list尾部
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey
            * @param value 添加到列表中的value二进制数据流
            * @param len 二进制数据流长度
            */
            bool rpushBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *value, const BCLib::int32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取列尾部的value值
            * @param key 列表名
            * @return 读取到的数据value，string类型
            */
            std::string rreadString(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取列尾部的value值
            * @param key 列表名
            * @param value 二进制数据接收buffer指针
            * @param len 接收buffer 长度
            */
            BCLib::uint32 rreadBin(const char *key, char *value, const BCLib::int32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取列尾部的value值
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey
            * @return 读取到的数据value，string类型
            */
            std::string rreadString(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取列尾部的value值
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey
            * @param value 二进制流接收buffer指针
            * @param len 二进制流接收buffer的长度
            * @return 返回读取的二进制流长度
            */
            BCLib::uint32 rreadBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, char *value, const BCLib::int32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：获取列表中元素个数
            * @param key 列表名
            * @return 返回列表元素个数 失败返回 -1
            */
            BCLib::int32 llen(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：获取列表中元素个数
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey
            * @return 返回列表元素个数 失败返回 -1
            */
            BCLib::int32 llen(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除列表中第一个元素并返回
            * @param key 列表名
            * @param value 返回元素
            * @return 成功或失败
            */
            std::string lpopString(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取列尾部的value值
            * @param key 列表名
            * @param value 二进制流接收buffer指针
            * @param len 二进制流接收buffer的长度
            * @return 返回pop出来的二进制流长度
            */
            BCLib::uint32  lpopBin(const char *key, char*value, const BCLib::int32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除列表中第一个元素并返回
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param value 返回元素
            * @return 成功或失败
            */
            std::string lpopString(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取列尾部的value值
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey
            * @param value 二进制流接收buffer指针
            * @param len 二进制流接收buffer的长度
            * @return 返回pop出来的二进制流长度
            */
            BCLib::uint32 lpopBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, char*value, const BCLib::int32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除列表中最后一个元素并返回
            * @param key 列表名
            * @param value 返回元素
            * @return 成功或失败
            */
            std::string rpopString(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除列表中最后一个元素并返回
            * @param key 列表名
            * @param value 二进制流接收buffer指针
            * @param len 二进制流接收buffer的长度
            * @return 成功或失败
            */
            BCLib::uint32 rpopBin(const char *key, char*value, const BCLib::int32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除列表中第一个元素并返回
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param value 返回元素
            * @return 成功或失败
            */
            std::string rpopString(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除列表中第一个元素并返回
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param value 二进制流接收buffer指针
            * @param len 二进制流接收buffer的长度
            * @return 成功或失败
            */
            BCLib::uint32 rpopBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, char*value, const BCLib::int32 len = REDIS_READER_MAX_BUF, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：设置列表中index位置上的元素值
            * @param key 列表名
            * @param index 元素索引 也可以使用负数下标，以 -1 表示列表的最后一个元素， -2 表示列表的倒数第二个元素，以此类推。index越界返回错误
            * @param value 返回元素
            * @return 成功或失败
            */
            bool lsetString(const char *key, BCLib::int32 index, const char *value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：设置列表中index位置上的元素值
            * @param key 列表名
            * @param index 元素索引 也可以使用负数下标，以 -1 表示列表的最后一个元素， -2 表示列表的倒数第二个元素，以此类推。index越界返回错误
            * @param value 设置的二进制流指针
            * @param len 二进制流大小
            * @return 成功或失败
            */
            bool lsetBin(const char *key, BCLib::int32 index, char*value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：设置列表中index位置上的元素值
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param index 元素索引 也可以使用负数下标，以 -1 表示列表的最后一个元素， -2 表示列表的倒数第二个元素，以此类推。index越界返回错误
            * @param value 返回元素
            * @return 成功或失败
            */
            bool lsetString(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 index, const char *value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：设置列表中index位置上的元素值
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param index 元素索引 也可以使用负数下标，以 -1 表示列表的最后一个元素， -2 表示列表的倒数第二个元素，以此类推。index越界返回错误
            * @param value 设置的二进制流指针
            * @param len 二进制流大小
            * @return 成功或失败
            */
            bool lsetBin(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 index, char*value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：将src列表的最后一个元素移除，并添加到tar列表的头部
            * @param keyTar 目标列表名
            * @param keySrc 源列表名
            * @return 成功或失败
            */
            bool rpoplpush(const char *keyTar, const char *keySrc, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：将src列表的最后一个元素移除，并添加到tar列表的头部
            * @param keyTar 目标列表名
            * @param uniqueidTar key表名下的 列表编号
            * @param subkeyTar 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param keySrc 源列表名
            * @param uniqueidSrc key表名下的 列表编号
            * @param subkeySrc 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @return 成功或失败
            */
            bool rpoplpush(const char *keyTar, BCLib::uint64 uniqueidTar, const char *subkeyTar, const char *keySrc, BCLib::uint64 uniqueidSrc, const char *subkeySrc, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取redis内指定list全部值
            * @param key 列表名
            * @param mylist 返回列表容器
            * @return 成功或失败
            */
            bool lgetAllElement(const char *key, std::list<std::string>& mylist, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取redis内指定list全部值
            * @param key 列表名
            * @return const std::list<std::pair<char*, BCLib::uint32>*> 指针，std::pair<char*, BCLib::uint32> 为<二进制流，长度>
            * 注意：这个指针获取后要立即将数据拷贝出去，在下一次请求是数据就会被清空重写
            */
            const std::list<std::pair<char*, BCLib::uint32>*> * lgetAllElement(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);


            /**
            * 功能：读取redis内指定list全部值
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param mylist 返回列表容器
            * @return 成功或失败
            */
            bool lgetAllElement(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::list<std::string>& mylist, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：读取redis内指定list全部值
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @return const std::list<std::pair<char*, BCLib::uint32>*> 指针，std::pair<char*, BCLib::uint32> 为<二进制流，长度>
            * 注意：这个指针获取后要立即将数据拷贝出去，在下一次请求是数据就会被清空重写
            */
            const std::list<std::pair<char*, BCLib::uint32>*> *  lgetAllElement(const char *key, BCLib::uint64 uniqueid, const char*subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除列表中与value值相等的元素
            * @param key 列表名
            * @param count 移除数量，count>0表示从表头向表尾搜索，count < 0表示表尾向表头搜索，count=0 表示全部移除
            * @param value 字符串
            * @return const std::list<std::pair<char*, BCLib::uint32>*> 指针，std::pair<char*, BCLib::uint32> 为<二进制流，长度>
            * 注意：这个指针获取后要立即将数据拷贝出去，在下一次请求是数据就会被清空重写
            */
            bool lrem(const char *key, BCLib::int32 count, char*value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除列表中与value值相等的元素
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param count 移除数量，count>0表示从表头向表尾搜索，count < 0表示表尾向表头搜索，count=0 表示全部移除
            * @param value 字符串
            * @return const std::list<std::pair<char*, BCLib::uint32>*> 指针，std::pair<char*, BCLib::uint32> 为<二进制流，长度>
            * 注意：这个指针获取后要立即将数据拷贝出去，在下一次请求是数据就会被清空重写
            */
            bool lrem(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 count, char *value, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除列表中与value值相等的元素
            * @param key 列表名
            * @param count 移除数量，count>0表示从表头向表尾搜索，count < 0表示表尾向表头搜索，count=0 表示全部移除
            * @param value 二进制数据指针
            * @param len 二进制数据长度
            * @return const std::list<std::pair<char*, BCLib::uint32>*> 指针，std::pair<char*, BCLib::uint32> 为<二进制流，长度>
            * 注意：这个指针获取后要立即将数据拷贝出去，在下一次请求是数据就会被清空重写
            */
            bool lrem(const char *key, BCLib::int32 count, char *value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除列表中与value值相等的元素
            * @param key 列表名
            * @param uniqueid key表名下的 列表编号
            * @param subkey 列编号下的二级表名 与key uniqueid 共同组成唯一表名 key:[uniqueid]:subkey  多维结构
            * @param count 移除数量，count>0表示从表头向表尾搜索，count < 0表示表尾向表头搜索，count=0 表示全部移除
            * @param value 二进制数据指针
            * @param len 二进制数据长度
            * @return const std::list<std::pair<char*, BCLib::uint32>*> 指针，std::pair<char*, BCLib::uint32> 为<二进制流，长度>
            * 注意：这个指针获取后要立即将数据拷贝出去，在下一次请求是数据就会被清空重写
            */
            bool lrem(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 count, char *value, BCLib::int32 len, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //删除整个列表 使用 delKey 接口

            /*
            *set 无序集合封装↓↓↓↓↓
            */

            /**
            * 功能：添加一个或多个成员到无序集合
            * @param key 集合名
            * @param format 添加的成员们 format 多个成员 使用空格隔开 如 "member1 member2 member3"
            * @return 成功或失败
            */
            bool sadd(const char *key, EREDIS_CONTEXT_TYPE type, std::string members, ...);

            /**
            *功能：添加一个或多个成员到无序集合
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param format 添加的成员们 format 多个成员 使用空格隔开 如 "format format format"
            * @return 成功或失败
            */
            bool sadd(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE, std::string members, ...);

            /**
            * 功能：获取集合内成员数量
            * @param key 集合名
            * @return 成员数量，如果该集合不存在返回0
            */
            BCLib::uint32 scard(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            *功能：获取集合内成员数量
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @return 成员数量，如果该集合不存在返回0
            */
            BCLib::uint32 scard(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：判断member是否为该集合的成员
            * @param key 集合名
            * @param member为检测成员
            * @return true or false
            */
            bool sismember(const char *key, const char *member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            *功能：判断member是否为该集合的成员
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param member为检测成员
            * @return true or false
            */
            bool sismember(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：获取set所有成员
            * @param key 集合名
            * @return true or false
            */
            bool sgetAllMembers(const char *key, std::set<std::string> &mySet, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：获取set所有成员
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @return true or false
            */
            bool sgetAllMembers(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::set<std::string> &mySet, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除并返回一个随机成员
            * @param key 集合名
            * @return true or false
            */
            bool spop(const char *key, std::string &member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除并返回一个随机成员
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @return true or false
            */
            bool spop(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::string &member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除一个成员或多个成员
            * @param key 集合名
            * @param members 多个成员用 空格分割
            * @return true or false
            */
            bool sremove(const char *key, const std::string members, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除一个成员或多个成员
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param members 多个成员用 空格分割
            * @return true or false
            */
            bool sremove(const char *key, BCLib::uint64 uniqueid, const char *subkey, const std::string members, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：将指定成员 member 元素从 source 集合移动到 destination 集合
            * @param disKey 目的集合名
            * @param srcKey 源集合名
            * @param member 移动的成员
            * @return 如果成员元素被成功移除，返回true 如果成员元素不是src集合的成员或集合不存在，并且没有任何操作对 destination 集合执行，那么返回 false
            */
            bool smove(const char *disKey, const char *srcKey, const std::string member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除一个成员或多个成员
            * @param disKey 集合名
            * @param disUniqueid key集合下的子集和编号
            * @param disSubkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param srcKey 集合名
            * @param srcUniqueid key集合下的子集和编号
            * @param srcSubkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param member 移动的成员
            * @return 如果成员元素被成功移除，返回true 如果成员元素不是src集合的成员或集合不存在，并且没有任何操作对 destination 集合执行，那么返回 false
            */
            bool smove(const char *disKey, BCLib::uint64 disUniqueid, const char *disSubkey, const char *srcKey, BCLib::uint64 srcUniqueid, const char *srcSubkey, const std::string member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：获取给定所有给定集合的交集,不存在的集合 key 被视为空集
            * @param keys 集合名 多个成员 使用空格隔开 如 "key1 key2 key3" / "key:[uniqueid]:subkey1 key:[uniqueid]:subkey2 key:[uniqueid]:subkey3"  多维结构
            * @return 成功或失败
            */
            bool sinter(std::set<std::string> &set, EREDIS_CONTEXT_TYPE type, std::string keys, ...);

            //同上  by bhu
            bool sinter(std::set<std::string> &set, const std::vector<std::string> &keys, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：模糊匹配
            * @param key 集合名
            * @param matchKey 匹配字符串，匹配规则同scan
            * @param mySet 返回集合
            * @param start 遍历开始游标
            * @param count 遍历个数，最大返回成员数
            * @param type 所使用的redis服务器
            * @return 返回遍历结束时游标
            */
            BCLib::uint64 sscan(const char *key, const char *matchKey, std::set<std::string> &mySet, BCLib::uint64 start = 0, BCLib::uint64 count = 50, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：模糊匹配
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param matchKey 匹配字符串，匹配规则同scan
            * @param mySet 返回集合
            * @param start 遍历开始游标
            * @param count 遍历个数，最大返回成员数
            * @param type 所使用的redis服务器
            * @return 返回遍历结束时游标
            */
            BCLib::uint64 sscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, std::set<std::string> &mySet, BCLib::uint64 start = 0, BCLib::uint64 count = 50, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /*
            *zset 有序集合封装↓↓↓↓↓
            */

            /**
            * 功能：添加成员到有序集合
            * @param key 集合名
            * @param scores 一个或多个集合成员对应的分值
            * @param members 一个或多个集合成员
            * @param precision 小数点位数
            * @return true or false
            */
            bool zadd(const char *key, const std::vector<double> &scores, const std::vector<std::string> &members, BCLib::uint32 precision = 6, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：添加成员到有序集合
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param scores 一个或多个集合成员对应的分值
            * @param members 一个或多个集合成员
            * @param precision 小数点位数
            * @return true or false
            */
            bool zadd(const char *key, BCLib::uint64 uniqueid, const char *subkey, const std::vector<double> &scores, const std::vector<std::string> &members, BCLib::uint32 precision = 6, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //返回结合内元素个数

            /**
            * 功能：返回结合内元素个数
            * @param key 集合名
            * @return 集合内成员数量 集合不存在返回0
            */
            BCLib::uint32  zcard(const char *key, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：返回集合内元素个数
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @return 集合内成员数量 集合不存在返回0
            */
            BCLib::uint32  zcard(const char *key, BCLib::uint64 uniqueid, const char *subkey, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //返回集合内指定member的索引/排名 score 低->高 索引/排名从0开始
            /**
            * 功能：返回集合内指定成员的排名 score 低->高 排名从0开始
            * @param key 集合名
            * @param member 查询排名的指定成员
            * @return 返回集合内指定成员的排名,不存在返回 -1
            */
            BCLib::int32 zrank(const char *key, const char *member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：返回结合内指定成员的排名 score 低->高 排名从0开始
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param member 查询排名的指定成员
            * @return 返回集合内指定成员的排名,不存在返回 -1
            */
            BCLib::int32 zrank(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //返回集合中指定成员的排名 score 高->低 排名从0开始

            /**
            * 功能：返回集合内指定成员的排名 score 高->低 排名从0开始
            * @param key 集合名
            * @param member 查询排名的指定成员
            * @return 返回集合内指定成员的排名,不存在返回 -1
            */
            BCLib::int32 zrevRank(const char *key, const char *member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：返回结合内指定成员的排名 score 高->低 排名从0开始
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param member 查询排名的指定成员
            * @return 返回集合内指定成员的排名,不存在返回 -1
            */
            BCLib::int32 zrevRank(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除集合内一个成员或多个成员
            * @param key 集合名
            * @param members 移除一个或多个成员的vector
            * @return 移除成功或失败
            */
            bool zrem(const char *key, std::vector<std::string> &members, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：移除集合内一个成员或多个成员
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param members 移除一个或多个成员的vector
            * @return 移除成功或失败
            */
            bool zrem(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::vector<std::string> &members, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //返回集合内指定索引范围内的所有成员 score 低->高 0 ~-1 全部取出
            /**
            * 功能：取出集合内指定索引范围内的所有成员(按分数由低->高) 默认全部取出
            * @param key 集合名
            * @param members 读取到的成员-分数的vector
            * @param startIndex 指定的开始读取索引
            * @param stopIndex 指定的结束读取索引 -1表示倒数第一个，-2表示倒数第二个,...
            * @return 成功或失败
            */
            bool zrange(const char *key, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex = 0, BCLib::int32 stopIndex = -1, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：取出集合内指定索引范围内的所有成员(按分数由低->高) 默认全部取出
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param members 读取到的成员-分数的vector
            * @param startIndex 指定的开始读取索引
            * @param stopIndex 指定的结束读取索引 -1表示倒数第一个，-2表示倒数第二个,...
            * @return 成功或失败
            */
            bool zrange(const char *key, const BCLib::uint64 uniqueid, const char *subkey, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex = 0, BCLib::int32 stopIndex = -1, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //返回集合内指定索引范围内的所有成员 score 高->低 0 ~-1 全部取出
            /**
            * 功能：取出集合内指定索引范围内的所有成员(按分数由高->低) 默认全部取出
            * @param key 集合名
            * @param members 读取到的<成员-分数>的vector
            * @param startIndex 指定的开始读取索引
            * @param stopIndex 指定的结束读取索引 -1表示倒数第一个，-2表示倒数第二个,...
            * @return 成功或失败
            */
            bool zrevRange(const char *key, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex = 0, BCLib::int32 stopIndex = -1, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);// ZREVRANGE salary 0 - 1 WITHSCORES

            /**
            * 功能：取出集合内指定索引范围内的所有成员(按分数由高->低) 默认全部取出
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param members 读取到的<成员-分数>的vector
            * @param startIndex 指定的开始读取索引
            * @param stopIndex 指定的结束读取索引 -1表示倒数第一个，-2表示倒数第二个,...
            * @return 成功或失败
            */
            bool zrevRange(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::vector<std::pair<std::string, double>> &members, BCLib::int32 startIndex = 0, BCLib::int32 stopIndex = -1, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            //返回指定成员的分数值

            /**
            * 功能：取出集合内指定成员的分值
            * @param key 集合名
            * @param members 查找指定成员
            * @return 返回指定成员的分值 没有该成员返回0
            */
            double zscore(const char *key, const char *member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：取出集合内指定成员的分值
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param members 查找指定成员
            * @return 返回指定成员的分值 没有该成员返回0
            */
            double zscore(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *member, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);
            //ZSCORE key member

            //删除集合内指定排名区间的所有成员 0 ~-1 删除所有

            /**
            * 功能：删除集合内指定排名区间的所有成员 默认删除集合内所有成员
            * @param key 集合名
            * @param startIndex 排名区间开始索引
            * @param stopIndex 排名区间结束索引 -1表示倒数第一个，-2表示倒数第二个,...
            * @return 成功或失败
            */
            bool zremRangeByRank(const char *key, BCLib::int32 startIndex = 0, BCLib::int32 stopIndex = -1, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：删除集合内指定排名区间的所有成员 默认删除集合内所有成员
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param startIndex 排名区间开始索引
            * @param stopIndex 排名区间结束索引 -1表示倒数第一个，-2表示倒数第二个,...
            * @return 成功或失败
            */
            bool zremRangeByRank(const char *key, BCLib::uint64 uniqueid, const char *subkey, BCLib::int32 startIndex = 0, BCLib::int32 stopIndex = -1, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：按score自增increment
            * @param key 集合名
            * @param member 进行自增的成员
            * @param increment 自增的增量
            * @return 成功或失败 member 不在集合内返回失败
            */
            bool zincrby(const char *key, const char *member, double increment, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：按score自增increment
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param member 进行自增的成员
            * @param increment 自增的增量
            * @return 成功或失败 member 不在集合内返回失败
            */
            bool zincrby(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *member, double increment, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：有序集合中指定分数区间的成员数量[min,max]
            * @param key 集合名
            * @param min 分数区间下限
            * @param max 分数区间上限
            * @return 返回成员数量 key不存在返回-1
            */
            BCLib::int32 zcount(const char *key, double min, double max, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：有序集合中指定分数区间的成员数量[min,max]
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param min 分数区间下限
            * @param max 分数区间上限
            * @return 返回成员数量 key不存在返回-1
            */
            BCLib::int32 zcount(const char *key, BCLib::uint64 uniqueid, const char *subkey, double min, double max, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：有序集合中指定分数区间的成员列表 有序集成员按分数值递增(从小到大)次序排列
            * @param key 集合名
            * @param minStr 分数区间下限 开区间:"5.0"，闭区间:"(5.0" ， 负无穷:"-inf"
            * @param maxStr 分数区间上限 开区间:"9.0"，闭区间:"(9.0" ， 正无穷:"+inf"
            * @return 成功或失败 member 不在集合内返回失败
            */
            bool zrangeByScore(const char *key, std::vector<std::pair<std::string, double>> &members, const char *minStr, const char *maxStr, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：有序集合中指定分数区间的成员列表 有序集成员按分数值递增(从小到大)次序排列
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param minStr 分数区间下限 开区间:"5.0"，闭区间:"(5.0" ， 负无穷:"-inf"
            * @param maxStr 分数区间上限 开区间:"9.0"，闭区间:"(9.0" ， 正无穷:"+inf"
            * @return 成功或失败 member 不在集合内返回失败
            */
            bool zrangeByScore(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::vector<std::pair<std::string, double>> &members, const char *minStr, const char *maxStr, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：有序集中指定分数区间内的所有的成员 有序集成员按分数值递减(从大到小)的次序排列
            * @param key 集合名
            * @param maxStr 分数区间上限 开区间:"9.0"，闭区间:"(9.0" ， 正无穷:"+inf"
            * @param minStr 分数区间下限 开区间:"5.0"，闭区间:"(5.0" ， 负无穷:"-inf"
            * @return 成功或失败 member 不在集合内返回失败
            */
            bool zrevRangeByScore(const char *key, std::vector<std::pair<std::string, double>> &members, const char *maxStr, const char *minStr, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：有序集中指定分数区间内的所有的成员 有序集成员按分数值递减(从大到小)的次序排列
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param maxStr 分数区间上限 开区间:"9.0"，闭区间:"(9.0" ， 正无穷:"+inf"
            * @param minStr 分数区间下限 开区间:"5.0"，闭区间:"(5.0" ， 负无穷:"-inf"
            * @return 成功或失败 member 不在集合内返回失败
            */
            bool zrevRangeByScore(const char *key, BCLib::uint64 uniqueid, const char *subkey, std::vector<std::pair<std::string, double>> &members, const char *maxStr, const char *minStr, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：模糊匹配
            * @param key 集合名
            * @param matchKey 匹配字符串，匹配规则同scan
            * @param members 返回集合
            * @param start 遍历开始游标
            * @param count 遍历个数，最大返回成员数
            * @param type 所使用的redis服务器
            * @return 返回遍历结束时游标
            */
            BCLib::uint64 zscan(const char *key, const char *matchKey, std::vector<std::pair<std::string, double>> &members, BCLib::uint64 start = 0, BCLib::uint64 count = 50, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            /**
            * 功能：模糊匹配
            * @param key 集合名
            * @param uniqueid key集合下的子集和编号
            * @param subkey 子集和编号下的二级子集和名 与key uniqueid 共同组成唯一集合名 key:[uniqueid]:subkey  多维结构
            * @param matchKey 匹配字符串，匹配规则同scan
            * @param members 返回集合
            * @param start 遍历开始游标
            * @param count 遍历个数，最大返回成员数
            * @param type 所使用的redis服务器
            * @return 返回遍历结束时游标
            */
            BCLib::uint64 zscan(const char *key, BCLib::uint64 uniqueid, const char *subkey, const char *matchKey, std::vector<std::pair<std::string, double>> &members, BCLib::uint64 start = 0, BCLib::uint64 count = 50, EREDIS_CONTEXT_TYPE type = E_REDIS_SERVERTYPE_LOGIC);

            CRedLock *getRedLock() { return m_pRedLock; }

        protected:
            void destoryTempList()
            {
                for (std::list<std::pair<char*, BCLib::uint32>*>::iterator iter = m_list.begin(); iter != m_list.end(); iter++)
                {
                    if ((*iter) == NULL)
                    {
                        continue;
                    }
                    delete ((*iter)->first);
                    (*iter)->first = NULL;
                    (*iter)->second = 0;

                    delete (*iter);
                    (*iter) = NULL;
                }
                m_list.clear();
            }
            void destoryTempMap()
            {
                for (std::map<std::string, std::pair<char*, BCLib::uint32>*>::iterator iter = m_map.begin(); iter != m_map.end(); iter++)
                {
                    std::pair<char*, BCLib::uint32>*p = iter->second;
                    if (p == NULL)
                    {
                        continue;
                    }
                    //char *pChar = p->first;		
                    delete (p->first);
                    //pChar = NULL;
                    p->first = NULL;
                    p->second = 0;

                    delete p;
                    p = NULL;
                    iter->second = NULL;

                }
                m_map.clear();
            }
            void destoryRedLock()
            {
                if (m_pRedLock != NULL)
                {
                    delete m_pRedLock;
                }
                return;
            }

        private:
            std::string DoubleToString(const double value, BCLib::uint32 precisionAfterPoint = 6);

        private:
            redisContext *m_redisContext;
            redisReply *m_redisReply;
            CRedLock  *m_pRedLock;
            EREDIS_ACCESS_RIGHT_TYPE m_eAccessRight;

            std::unordered_map<BCLib::uint16, REDIS_NODE> m_redisContextMap;
            std::list<std::pair<char *, BCLib::uint32>*> m_list;
            std::map<std::string, std::pair<char *, BCLib::uint32>*> m_map;
        };
    }//Redis
}//MWLib

#endif//__MWLIB_REDIS_REDISSYSTEM_H__
