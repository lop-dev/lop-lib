//////////////////////////////////////////////////////////////////////
//  created:    2019/4/9
//  filename:   httpServer.h
//  author:     xums
//  brief Http 会话和服务器
//
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_HTTPSERVER_HTTPSERVER_H__
#define __MWLIB_HTTPSERVER_HTTPSERVER_H__


#include <string>
#include <map>


#include <BCLib/utility/noncopyable.h>

namespace MWLib
{
	namespace Http
	{
		class CHttpServer;

		/**
		 **http 会话类 
		 **非线程安全，请保持所有调用在同一线程内
		 **禁止用户另外保存 CHttpSession 指针， 链接断开后有可能会话会被底层释放
		 **如果有需要， 请保存会话id 通过CHttpServer 查找会话
		*/
		class CHttpSession
		{
			friend class CHttpServer;
		public:
			CHttpSession(CHttpServer* server, BCLib::uint32 sid);

			virtual ~CHttpSession();

			BCLib::uint32 getSid() const { return m_sid; }
				//both getVar from query string and body(Get & Post)
			bool getHttpVar(const std::string& name, std::string* value);

			//Get var
			bool getQueryVar(const std::string& name, std::string* value);
			//Post var
			bool getBodyVar(const std::string& name, std::string* value);

			const std::string& getAllQueryVar() const { return m_queryParam; }
			const std::string& getAllBodyVar() const { return m_bodyParam; }
			std::string& getAllQueryVar() { return m_queryParam; }
			std::string& getAllBodyVar() { return m_bodyParam;  }

			void setQueryParam(const char* val, size_t len);
			void setBodyParam(const char* val, size_t len);

			//!! After reply, this session will close .
			//If nothing repay, call replyNone();
			void reply(const std::string& s);
			void reply(const char* s);
			//http head code , but not include 302 (redirect)
			void replyStatus(int code);

			void replyNone();

			virtual void onClose() {}
		private:
			void setConnection(struct mg_connection* con);
		private:
			CHttpServer* m_server;
			BCLib::uint32 m_sid;
			struct mg_connection* m_conn;
			std::string m_queryParam;
			std::string m_bodyParam;
		};

		typedef void(*HttpHandleFunc)(CHttpSession* session);

		class CHttpServer : public BCLib::Utility::CNoncopyable
		{
		public:	
			bool init(const std::string &address);
			void close();
			//update trigger handler function
			void update(int timeout_ms = 10);

			//s.registHandler("/api/some", callback);
			void addHandler(const std::string& uri, HttpHandleFunc handler);
			void removeHandler(const std::string& uri);

			//!! After reply, this session will close .
			//If nothing repay, call replyNone();
			void reply(BCLib::uint32 sid, std::string& reply);
			void reply(BCLib::uint32 sid, const char* reply);
			void replyNone(BCLib::uint32 sid);
			//http head code , but not include 302 (redirect)
			void replyStatus(BCLib::uint32 sid, int code);

			CHttpSession* findSession(BCLib::uint32 sid);
			virtual CHttpSession* newSession(BCLib::uint32 sid);
			virtual void destroySession(CHttpSession* s);
			//before call user handler,
			//if return false, It's will not call handler and afterHandler
			virtual bool beforeHandler(CHttpSession* s);
			//after call user handler
			virtual void afterHandler(CHttpSession* s);
		protected:
			CHttpServer();
			static void mgEventHandler(struct mg_connection *nc, int ev, void *ev_data);
			
			void onClose(struct mg_connection* nc);
			void onRequest(struct mg_connection* nc, struct http_message* hm);
			

		private:
			//
			BCLib::uint32 m_currentSid;
			std::map<BCLib::uint32, CHttpSession*> m_sessions;
			std::map<std::string, HttpHandleFunc> m_handlers;
			struct mg_mgr* m_mgMgr;
		};
	}
}//MWLib

#endif
