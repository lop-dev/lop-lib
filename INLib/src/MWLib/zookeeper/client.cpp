//////////////////////////////////////////////////////////////////////
//  created:    2019/04/09
//  filename:   MWLib/zookeeper/client.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
/** use the lib zookeeper as static lib */
#ifndef USE_STATIC_LIB
#define USE_STATIC_LIB
#endif

#include <zookeeper.h>
#include <MWLib/zookeeper/client.h>
#include <BCLib/utility/logFile.h>

#define DEFAULT_CONNECT_TIMEOUT 30000
#define NODE_VALUE_BYTE_SIZE 1024*1024

namespace MWLib
{
    namespace Zookeeper
    {
        static void initWatcher(zhandle_t* zh, int type, int state, const char* path, void* context)
        {
            int eventType = EEVENT_TYPE_UNKNOWN;

            if (type == ZOO_SESSION_EVENT)
            {
                if (state == ZOO_EXPIRED_SESSION_STATE)
                {
                    eventType = EEVENT_TYPE_WATCH_SESSION_EXPIRED;
                }
                else if (state == ZOO_CONNECTING_STATE)
                {
                    eventType = EEVENT_TYPE_WATCH_SESSION_CONNECTING;
                }
                else if (state == ZOO_CONNECTED_STATE)
                {
                    eventType = EEVENT_TYPE_WATCH_SESSION_CONNECTED;
                }
            }
            else if (type == ZOO_CHANGED_EVENT)
            {
                eventType = EEVENT_TYPE_WATCH_NODE_CHANGED;
            }
            else if (type == ZOO_DELETED_EVENT)
            {
                eventType = EEVENT_TYPE_WATCH_NODE_DELETED;
            }
            else if (type == ZOO_CHILD_EVENT)
            {
                eventType = EEVENT_TYPE_WATCH_NODE_CHILD;
            }
            else if (type == ZOO_CREATED_EVENT)
            {
                eventType = EEVENT_TYPE_WATCH_NODE_CREATE;
            }

            Event* ev = new Event();
            ev->path = path;
            ev->type = eventType;

            BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "watch event, type=%d, state=%d, path=%s", eventType, state, ev->path.c_str());
            CClient* client = (CClient*)context;
            client->queueEvent(ev);
        }

        static void createCompletion(int rc, const char* value, const void* data)
        {
            Context* ctx = (Context*)data;
            Event* ev = new Event();
            ev->type = EEVENT_TYPE_ASYNC_CREATE;
            ev->path = ctx->path;
            ev->rc = rc;
            if (rc == ZOK)
            {
                ev->value = value;
            }
            CClient* client = ctx->client;
            delete ctx;

            client->queueEvent(ev);
        }

        static void setCompletion(int rc, const struct Stat* stat, const void* data)
        {
            Context* ctx = (Context*)data;
            Event* ev = new Event();
            ev->type = EEVENT_TYPE_ASYNC_SET;
            ev->path = ctx->path;
            ev->rc = rc;

            CClient* client = ctx->client;
            delete ctx;

            client->queueEvent(ev);
        }

        static void deleteCompletion(int rc, const void* data)
        {
            Context* ctx = (Context*)data;
            Event* ev = new Event();
            ev->type = EEVENT_TYPE_ASYNC_DELETE;
            ev->path = ctx->path;
            ev->rc = rc;

            CClient* client = ctx->client;
            delete ctx;

            client->queueEvent(ev);
        }

        static void getCompletion(int rc, const char* value, int value_len, const struct Stat* stat, const void* data)
        {
            Context* ctx = (Context*)data;
            Event* ev = new Event();
            ev->type = EEVENT_TYPE_ASYNC_GET;
            ev->path = ctx->path;
            ev->rc = rc;
            ev->value = std::string(value, value_len);
            //ar->value
            CClient* client = ctx->client;
            delete ctx;

            client->queueEvent(ev);
        }

        static void getChildrenCompletion(int rc, const struct String_vector* strings, const void* data)
        {

            Context* ctx = (Context*)data;
            Event* ev = new Event();
            ev->type = EEVENT_TYPE_ASYNC_GET_CHILDREN;
            ev->path = ctx->path;
            ev->rc = rc;
            for (int i = 0; i < strings->count; ++i) {
                ev->values.push_back(strings->data[i]);
            }
            CClient* client = ctx->client;
            delete ctx;

            client->queueEvent(ev);

        }

        static void existCompletion(int rc, const struct Stat* stat, const void* data)
        {

            Context* ctx = (Context*)data;
            Event* ar = new Event();
            ar->type = EEVENT_TYPE_ASYNC_EXIST;
            ar->path = ctx->path;
            ar->rc = rc;

            CClient* client = ctx->client;
            delete ctx;

            client->queueEvent(ar);
        }

        CClient::CClient()
            :m_handle(NULL)
            , m_isConnected(false)
            , m_logFile(NULL)
        {
        }

        CClient::~CClient()
        {
            close();
        }

        void CClient::update()
        {
            while (true)
            {
                Event *ev = dequeueEvent();
                if (ev == NULL)
                {
                    break;
                }
                processEvent(ev);
                delete ev;
            }
        }

        bool CClient::connect(const std::string& host, const std::string& logFile, int timeout)
        {
            close();
            m_host = host;

            if (!initLog(logFile))
            {
                BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "init log fail.");
                return false;
            }

            if (timeout == 0)
            {
                timeout = DEFAULT_CONNECT_TIMEOUT;
            }
            m_handle = ((void*)zookeeper_init(host.c_str(), initWatcher, timeout, NULL, this, 0));
            if (!m_handle)
            {
                BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "zookeeper_init fail.");
                return false;
            }
            return true;
        }

        void CClient::close()
        {
            if (m_handle)
            {
                zookeeper_close((zhandle_t*)m_handle);
                m_handle = NULL;
            }
            if (m_logFile) {
                fclose(m_logFile);
                m_logFile = NULL;
            }

            freeEvents();
        }

        bool CClient::asyncCreate(const std::string &path, const std::string& value, ENodeType nodeType)
        {
            Context* ctx = new Context();
            ctx->client = this;
            ctx->path = path;

            int flags = 0;
            switch (nodeType)
            {
            case ENODE_TYPE_PERSIST:
                break;
            case ENODE_TYPE_EPHEMERAL:
                flags |= ZOO_EPHEMERAL;
                break;
            case ENODE_TYPE_PERSIST_SEQ:
                flags |= ZOO_SEQUENCE;
                break;
            case ENODE_TYPE_EPHEMERAL_SEQ:
                flags |= ZOO_EPHEMERAL;
                flags |= ZOO_SEQUENCE;
                break;
            default:
                break;
            }

            int rc = zoo_acreate(((zhandle_t*)m_handle), path.c_str(), value.c_str(), value.size(), &ZOO_OPEN_ACL_UNSAFE, flags, createCompletion, ctx);
            if (rc != ZOK)
            {
                delete ctx;
            }
            return rc == ZOK;
        }

        bool CClient::syncCreate(const std::string &path, const std::string& value, ENodeType nodeType, std::string* result)
        {
            char *buf = new char[NODE_VALUE_BYTE_SIZE];
            memset(buf, 0, NODE_VALUE_BYTE_SIZE);
            int buffLen = NODE_VALUE_BYTE_SIZE;

            int flags = 0;
            switch (nodeType)
            {
            case ENODE_TYPE_PERSIST:
                break;
            case ENODE_TYPE_EPHEMERAL:
                flags |= ZOO_EPHEMERAL;
                break;
            case ENODE_TYPE_PERSIST_SEQ:
                flags |= ZOO_SEQUENCE;
                break;
            case ENODE_TYPE_EPHEMERAL_SEQ:
                flags |= ZOO_EPHEMERAL;
                flags |= ZOO_SEQUENCE;
                break;
            default:
                break;
            }

            int rc = zoo_create((zhandle_t*)m_handle, path.c_str(), value.c_str(), value.size(), &ZOO_OPEN_ACL_UNSAFE, flags, buf, buffLen);
            if (rc == ZOK)
            {
                result->assign(buf);
                //*result = buff;
            }
            delete[]buf;
            return rc == ZOK;

        }

        bool CClient::asyncSet(const std::string& path, const std::string& value)
        {
            Context* ctx = new Context();
            ctx->client = this;
            ctx->path = path;


            int rc = zoo_aset((zhandle_t*)m_handle, path.c_str(), value.c_str(), value.size(), -1, setCompletion, ctx);
            if (rc != ZOK)
            {
                delete ctx;
            }
            return rc == ZOK;
        }

        bool CClient::syncSet(const std::string& path, const std::string& value)
        {
            int rc = zoo_set((zhandle_t*)m_handle, path.c_str(), value.c_str(), value.length(), -1);
            return rc == ZOK;
        }

        bool CClient::asyncDelete(const std::string& path)
        {
            Context* ctx = new Context();
            ctx->client = this;
            ctx->path = path;

            int rc = zoo_adelete((zhandle_t*)m_handle, path.c_str(), -1, deleteCompletion, ctx);
            if (rc != ZOK)
            {
                delete ctx;
            }
            return rc == ZOK;
        }

        bool CClient::syncDelete(const std::string& path)
        {
            int rc = zoo_delete((zhandle_t*)m_handle, path.c_str(), -1);
            return rc == ZOK;
        }

        bool CClient::asyncExist(const std::string& path, bool watch)
        {
            Context* ctx = new Context();
            ctx->client = this;
            ctx->path = path;

            int rc = zoo_aexists((zhandle_t*)m_handle, path.c_str(), (int)watch, existCompletion, ctx);
            if (rc != ZOK)
            {
                delete ctx;
            }
            return rc == ZOK;
        }

        bool CClient::syncExist(const std::string& path, bool watch)
        {
            int rc = zoo_exists((zhandle_t*)m_handle, path.c_str(), (int)watch, NULL);
            return rc == ZOK;
        }

        bool CClient::asyncGet(const std::string& path, bool watch)
        {
            Context* ctx = new Context();
            ctx->client = this;
            ctx->path = path;

            int rc = zoo_aget((zhandle_t*)m_handle, path.c_str(), (int)watch, getCompletion, ctx);
            if (rc != ZOK)
            {
                delete ctx;
            }
            return rc == ZOK;
        }

        bool CClient::syncGet(const std::string& path, bool watch, std::string* result)
        {
            char *buf = new char[NODE_VALUE_BYTE_SIZE];
            memset(buf, 0, NODE_VALUE_BYTE_SIZE);
            int len = NODE_VALUE_BYTE_SIZE;
            int rc = zoo_get((zhandle_t*)m_handle, path.c_str(), (int)watch, buf, &len, NULL);
            if (rc == ZOK)
            {
                result->assign(buf);
            }
            delete[]buf;
            return rc == ZOK;
        }

        bool CClient::asyncGetChildren(const std::string& path, bool watch)
        {
            Context* ctx = new Context();
            ctx->client = this;
            ctx->path = path;

            int rc = zoo_aget_children((zhandle_t*)m_handle, path.c_str(), (int)watch, getChildrenCompletion, ctx);
            if (rc != ZOK)
            {
                delete ctx;
            }
            return rc == ZOK;
        }

        bool CClient::syncGetChildren(const std::string& path, bool watch, std::vector<std::string> *result)
        {
            struct String_vector stringVec;
            int rc = zoo_get_children((zhandle_t*)m_handle, path.c_str(), (int)watch, &stringVec);
            if (rc == ZOK)
            {
                for (int i = 0; i < stringVec.count; ++i)
                {
                    result->push_back(stringVec.data[i]);
                }
            }
            return rc == ZOK;
        }

        void CClient::queueEvent(Event* result)
        {
            m_eventsMtx.lock();
            m_events.push_back(result);
            m_eventsMtx.unlock();
        }

        Event* CClient::dequeueEvent()
        {
            Event* rt = NULL;
            m_eventsMtx.lock();
            if (!m_events.empty())
            {
                rt = m_events.front();
                m_events.pop_front();
            }
            m_eventsMtx.unlock();
            return rt;
        }

        bool CClient::initLog(const std::string& logFile)
        {
            auto logLevel = BCLIB_LOG_GET_MODULE_LEVEL(BCLib::ELOGMODULE_DEFAULT);
            if (logLevel == BCLib::Utility::CLog::ELOGLEVEL_DEBUG)
            {
                zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
            }
            else if (logLevel == BCLib::Utility::CLog::ELOGLEVEL_TRACE)
            {
                zoo_set_debug_level(ZOO_LOG_LEVEL_INFO);
            }
            else if (logLevel == BCLib::Utility::CLog::ELOGLEVEL_WARNING)
            {
                zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
            }
            else if (logLevel == BCLib::Utility::CLog::ELOGLEVEL_ERROR)
            {
                zoo_set_debug_level(ZOO_LOG_LEVEL_ERROR);
            }

            if (logFile.empty())
            {
                BCLIB_LOG_WARNING(BCLib::ELOGMODULE_DEFAULT, "log file is empty, use default.");
            }
            else
            {
                m_logFile = fopen(logFile.c_str(), "w+");
                if (!m_logFile)
                {
                    BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "fopen file fail, file name=%s", logFile.c_str());
                    return false;
                }
                zoo_set_log_stream(m_logFile);
            }
            return true;
        }

        void CClient::processEvent(Event* ev)
        {
            BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "process event type;%d, rc:%d, path=%s", ev->type, ev->rc, ev->path.c_str());
            switch (ev->type)
            {
            case EEVENT_TYPE_ASYNC_CREATE:
                onAsyncCreate(ev->path, ev->rc == ZOK);
                break;
            case EEVENT_TYPE_ASYNC_SET:
                onAsyncSet(ev->path, ev->rc == ZOK);
                break;
            case EEVENT_TYPE_ASYNC_DELETE:
                onAsyncDelete(ev->path, ev->rc == ZOK);
                break;
            case EEVENT_TYPE_ASYNC_EXIST:
                onAsyncExist(ev->path, ev->rc == ZOK);
                break;
            case EEVENT_TYPE_ASYNC_GET:
                onAsyncGet(ev->path, ev->rc == ZOK, ev->value);
                break;
            case EEVENT_TYPE_ASYNC_GET_CHILDREN:
                onAsyncGetChildren(ev->path, ev->rc == ZOK, ev->values);
                break;
            case EEVENT_TYPE_WATCH_SESSION_EXPIRED:
                m_isConnected = false;
                onWatchSessionExpired();
                break;
            case EEVENT_TYPE_WATCH_SESSION_CONNECTING:
                onWatchSessionConnecting();
                break;
            case EEVENT_TYPE_WATCH_SESSION_CONNECTED:
                m_isConnected = true;
                onWatchSessionConnected();
                break;
            case EEVENT_TYPE_WATCH_NODE_CHANGED:
                onWatchNodeChange(ev->path);
                break;
            case EEVENT_TYPE_WATCH_NODE_DELETED:
                onWatchNodeDelete(ev->path);
                break;
            case EEVENT_TYPE_WATCH_NODE_CHILD:
                onWatchNodeChild(ev->path);
                break;
            case EEVENT_TYPE_WATCH_NODE_CREATE:
                onWatchNodeCreate(ev->path);
                break;
            default:
                BCLIB_LOG_WARNING(BCLib::ELOGMODULE_DEFAULT, "invliad event type;%d, rc:%d, path=%s", ev->type, ev->rc, ev->path.c_str());
                break;
            }
        }

        void CClient::freeEvents()
        {
            int size = 0;
            m_eventsMtx.lock();
            size = (int)m_events.size();
            for (auto it = m_events.begin(); it != m_events.end(); it++)
            {
                delete *it;
            }
            m_events.clear();
            m_eventsMtx.unlock();
            BCLIB_LOG_INFOR(BCLib::ELOGMODULE_DEFAULT, "free not process events, size:%d", size);
        }

    }//Zookeeper
}//MWLib
