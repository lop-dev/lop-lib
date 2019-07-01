//////////////////////////////////////////////////////////////////////
//  created:    2019/04/09
//  filename:   MWLib/httpServer/httpServer.cpp
//  author:     League of Perfect (xums)
/// @brief      会话和服务器
///
//////////////////////////////////////////////////////////////////////

#include <mongoose.h>
#include <MWLib/httpServer/httpServer.h>
#include <BCLib/utility/logFile.h>

namespace MWLib
{
    namespace HttpServer
    {
        CHttpServer::CHttpServer()
            :m_currentSid(0)
            ,m_mgMgr(NULL)
        {
        }

        bool CHttpServer::init(const std::string &address)
        {
            m_mgMgr = (struct mg_mgr*)malloc(sizeof(struct mg_mgr));
            mg_mgr_init(m_mgMgr, this);

            auto nc = mg_bind(m_mgMgr, address.c_str(), mgEventHandler);

            if (!nc)
            {
                BCLIB_LOG_ERROR(BCLib::ELOGMODULE_DEFAULT, "mg_bind_opt error.");
                return false;
            }

            mg_set_protocol_http_websocket(nc);
            return true;
        }

        void CHttpServer::close()
        {
            if (!m_mgMgr)
            {
                return;
            }
            mg_mgr_free(m_mgMgr);
            free(m_mgMgr);
            m_mgMgr = NULL;
            for (auto it = m_sessions.begin(); it != m_sessions.end(); it++)
            {
                delete it->second;
            }
            m_sessions.clear();
        }

        void CHttpServer::update(int timeout_ms)
        {
            if (m_mgMgr)
            {
                mg_mgr_poll(m_mgMgr, timeout_ms);
            }

        }

        void CHttpServer::addHandler(const std::string& uri, HttpHandleFunc handler)
        {
            m_handlers[uri] = handler;
        }

        void CHttpServer::removeHandler(const std::string& uri)
        {
            m_handlers.erase(uri);
        }

        void CHttpServer::reply(BCLib::uint32 sid, std::string& data)
        {
            reply(sid, data.c_str());
        }

        void CHttpServer::reply(BCLib::uint32 sid, const char* data)
        {
            CHttpSession* s = findSession(sid);
            if (s != NULL)
            {
                s->reply(data);
            }
            else
            {
                BCLIB_LOG_WARNING(BCLib::ELOGMODULE_DEFAULT, "not find session, %u", sid);
            }
        }

        void CHttpServer::replyStatus(BCLib::uint32 sid, int code)
        {
            CHttpSession* s = findSession(sid);
            if (s != NULL)
            {
                s->replyStatus(code);
            }
            else
            {
                BCLIB_LOG_WARNING(BCLib::ELOGMODULE_DEFAULT, "not find session, %u", sid);
            }
        }

        void CHttpServer::replyNone(BCLib::uint32 sid)
        {
            replyStatus(sid, 200);
        }

        void CHttpServer::onClose(struct mg_connection* nc)
        {
            CHttpSession* s = (CHttpSession*)nc->user_data;
            if (s != NULL)
            {
                s->onClose();
                s->setConnection(NULL);
                m_sessions.erase(s->getSid());
                delete s;
            }
            nc->user_data = NULL;
        }

        void CHttpServer::onRequest(struct mg_connection* nc, struct http_message* hm)
        {
            std::string uri(hm->uri.p, hm->uri.len);
            auto it = m_handlers.find(uri);
            if (it == m_handlers.end())
            {
                BCLIB_LOG_WARNING(BCLib::ELOGMODULE_DEFAULT, "unhandle request, uri=%s", uri.c_str());
                mg_printf(nc, "%s", "HTTP/1.1 404 Not Found\r\n"
                    "Content-Length: 0\r\n\r\n");
                return;
            }
            BCLib::uint32 sid = m_currentSid++;
            while (m_sessions.find(sid) != m_sessions.end())
            {
                sid = m_currentSid++;
            }
            CHttpSession* s = newSession(sid);
            s->setConnection(nc);
            s->setBodyParam(hm->body.p, hm->body.len);
            s->setQueryParam(hm->query_string.p, hm->query_string.len);
            m_sessions[sid] = s;

            nc->user_data = (void*)s;
            BCLIB_LOG_DEBUG(BCLib::ELOGMODULE_DEFAULT, "connection[%u] request[%s] coming", sid, uri.c_str());
            if (beforeHandler(s))
            {
                auto handler = it->second;
                handler(s);
                afterHandler(s);
            }

        }

        CHttpSession* CHttpServer::findSession(BCLib::uint32 sid)
        {
            auto it = m_sessions.find(sid);
            return it != m_sessions.end() ? it->second : NULL;
        }

        CHttpSession* CHttpServer::newSession(BCLib::uint32 sid)
        {
            return new CHttpSession(this, sid);
        }

        void CHttpServer::destroySession(CHttpSession* s)
        {
            BCLIB_SAFE_DELETE(s);
        }

        bool CHttpServer::beforeHandler(CHttpSession* s)
        {
            return true;
        }

        void CHttpServer::afterHandler(CHttpSession* s)
        {
        }

        void CHttpServer::mgEventHandler(struct mg_connection *nc, int ev, void *ev_data)
        {
            CHttpServer* server = (CHttpServer*)nc->mgr->user_data;
            switch (ev)
            {
            case MG_EV_HTTP_REQUEST:
                server->onRequest(nc, (struct http_message*)ev_data);
                break;
            case MG_EV_CLOSE:
                server->onClose(nc);
                break;
            default:
                break;
            }
        }

        CHttpSession::CHttpSession(CHttpServer* server, BCLib::uint32 sid)
            :m_server(server)
            , m_sid(sid)
            , m_conn(NULL)
        {
        }

        CHttpSession::~CHttpSession()
        {

        }

        bool CHttpSession::getHttpVar(const std::string& name, std::string* value)
        {
            if (getQueryVar(name, value))
            {
                return true;
            }
            return getBodyVar(name, value);
        }

        bool CHttpSession::getQueryVar(const std::string& name, std::string* value)
        {
            mg_str queryParam = mg_mk_str(m_queryParam.c_str());
            char dst[128];
            int rt = mg_get_http_var(&queryParam, name.c_str(), dst, sizeof(dst));
            if (rt > 0)
            {
                value->assign(dst);
                return true;
            }
            else if (rt == -3)
            {
                char *buf = new char[MG_MAX_HTTP_REQUEST_SIZE];
                rt = mg_get_http_var(&queryParam, name.c_str(), buf, MG_MAX_HTTP_REQUEST_SIZE);
                if (rt > 0)
                {
                    value->assign(buf);
                }
                delete[] buf;
                if (rt > 0)
                {
                    return true;
                }
            }
            return false;
        }

        bool CHttpSession::getBodyVar(const std::string& name, std::string* value)
        {
            mg_str queryBody = mg_mk_str(m_bodyParam.c_str());
            char dst[128];
            int rt = mg_get_http_var(&queryBody, name.c_str(), dst, sizeof(dst));
            if (rt > 0)
            {
                value->assign(dst);
                return true;
            }
            else if (rt == -3)
            {
                char *buf = new char[MG_MAX_HTTP_REQUEST_SIZE];
                rt = mg_get_http_var(&queryBody, name.c_str(), buf, MG_MAX_HTTP_REQUEST_SIZE);
                if (rt > 0)
                {
                    value->assign(buf);
                }
                delete[] buf;
                if (rt > 0)
                {
                    return true;
                }
            }
            return false;
        }

        void CHttpSession::setQueryParam(const char* val, size_t len)
        {
            m_queryParam.assign(val, len);
        }

        void CHttpSession::setBodyParam(const char* val, size_t len)
        {
            m_bodyParam.assign(val, len);
        }

        void CHttpSession::reply(const std::string& s)
        {
            return reply(s.c_str());
        }

        void CHttpSession::reply(const char* s)
        {
            if (!m_conn)
            {
                BCLIB_LOG_WARNING(BCLib::ELOGMODULE_DEFAULT, "conn is closed.");
                return;
            }
            mg_printf(m_conn, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
            mg_printf_http_chunk(m_conn, "%s", s);
            mg_send_http_chunk(m_conn, "", 0); /* Send empty chunk, the end of response */
            m_conn->flags |= MG_F_SEND_AND_CLOSE;
        }

        void CHttpSession::replyStatus(int code)
        {
            if (!m_conn)
            {
                BCLIB_LOG_WARNING(BCLib::ELOGMODULE_DEFAULT, "conn is closed.");
                return;
            }
            mg_http_send_error(m_conn, code, NULL);
        }

        void CHttpSession::replyNone()
        {
            return replyStatus(200);
        }

        void CHttpSession::setConnection(struct mg_connection* con)
        {
            m_conn = con;
        }
    }//HttpServer
}//MWLib
