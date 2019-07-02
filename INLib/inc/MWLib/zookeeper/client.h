//////////////////////////////////////////////////////////////////////
//  created:    2019/04/09
//  filename:   MWLib/zookeeper/client.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_ZOOKEEPER_CLIENT_H__
#define __MWLIB_ZOOKEEPER_CLIENT_H__

#include <mutex>
#include <BCLib/utility/noncopyable.h>

namespace MWLib
{
    namespace Zookeeper
    {
        enum ENodeType
        {
            ENODE_TYPE_PERSIST,
            ENODE_TYPE_EPHEMERAL,
            ENODE_TYPE_PERSIST_SEQ,
            ENODE_TYPE_EPHEMERAL_SEQ,
        };

        enum EEventType
        {
            EEVENT_TYPE_UNKNOWN,
            EEVENT_TYPE_ASYNC_CREATE,
            EEVENT_TYPE_ASYNC_SET,
            EEVENT_TYPE_ASYNC_DELETE,
            EEVENT_TYPE_ASYNC_EXIST,
            EEVENT_TYPE_ASYNC_GET,
            EEVENT_TYPE_ASYNC_GET_CHILDREN,
            EEVENT_TYPE_WATCH_SESSION_EXPIRED,
            EEVENT_TYPE_WATCH_SESSION_CONNECTING,
            EEVENT_TYPE_WATCH_SESSION_CONNECTED,
            EEVENT_TYPE_WATCH_NODE_CHANGED,
            EEVENT_TYPE_WATCH_NODE_DELETED,
            EEVENT_TYPE_WATCH_NODE_CHILD,
            EEVENT_TYPE_WATCH_NODE_CREATE,
        };


        struct Event
        {
            int type;
            int rc;
            std::string path;
            std::string value;
            std::vector<std::string> values;
        };
        class CClient;
        struct Context
        {
            CClient *client;
            std::string path;
        };

        class CClient : public BCLib::Utility::CNoncopyable
        {
        public:
            CClient();
            virtual ~CClient();

            void update();

            bool connect(const std::string& host, const std::string& logFile, int timeout = 0);
            void close();

            bool asyncCreate(const std::string &path, const std::string& value, ENodeType nodeType);
            bool syncCreate(const std::string &path, const std::string& value, ENodeType nodeType, std::string* result);

            bool asyncSet(const std::string& path, const std::string& value);
            bool syncSet(const std::string& path, const std::string& value);

            bool asyncDelete(const std::string& path);
            bool syncDelete(const std::string& path);

            bool asyncExist(const std::string& path, bool watch);
            bool syncExist(const std::string& path, bool watch);

            bool asyncGet(const std::string& path, bool watch);
            bool syncGet(const std::string& path, bool watch, std::string* result);

            bool asyncGetChildren(const std::string& path, bool watch);
            bool syncGetChildren(const std::string& path, bool watch, std::vector<std::string> *result);

            //异步请求回调, 由update 触发
            virtual void onAsyncCreate(const std::string& path, bool rc) {}
            virtual void onAsyncSet(const std::string& path, bool rc) {}
            virtual void onAsyncDelete(const std::string& path, bool rc) {}
            virtual void onAsyncExist(const std::string& path, bool rc) {}
            virtual void onAsyncGet(const std::string& path, bool rc, const std::string& result) {}
            virtual void onAsyncGetChildren(const std::string& path, bool rc, const std::vector<std::string>& result) {}
            //watch 订阅回调, 由update 触发
            virtual void onWatchSessionConnected() {}
            virtual void onWatchSessionExpired() {}
            virtual void onWatchSessionConnecting() {}
            virtual void onWatchNodeChange(const std::string& path) {}
            virtual void onWatchNodeDelete(const std::string& path) {}
            virtual void onWatchNodeChild(const std::string& path) {}
            virtual void onWatchNodeCreate(const std::string& path) {}

            //异步回调回来的事件 
            void queueEvent(Event* result);
            Event* dequeueEvent();
            bool isConnected() const { return m_isConnected; }
        private:
            bool initLog(const std::string& logFile);
            void processEvent(Event* ev);
            void freeEvents();

        private:
            std::string m_host;

            void *m_handle;
            bool m_isConnected;
            FILE *m_logFile;

            std::list<Event*> m_events;
            std::mutex m_eventsMtx;
        };
    }
}//MWLib

#endif
