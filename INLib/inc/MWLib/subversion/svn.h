//////////////////////////////////////////////////////////////////////
//  created:    2012/02/08
//  filename:   MWLib/subversion/svn.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __MWLIB_SUBVERSION_SVN_H__
#define __MWLIB_SUBVERSION_SVN_H__

#include <BCLib/utility/string.h>
#include <BCLib/utility/baseDec.h>

namespace BCLib
{
    namespace Utility
    {
        class CStringW;
        class CStringA;
        class CXmlFile;
        class CBinFile;
    }//Utility
}//BCLib

namespace TDLib
{
    namespace Subversion
    {
        class CSvn;
    }//Subversion
}//TDLib

namespace MWLib
{
    namespace Subversion
    {
        class CSvn
        {
#define MAX_SVN_ARG 20
        public:
            enum ESVN_FLUSH_TYPE
            {
                ESVN_FLUSH_TYPE_BASE,
                ESVN_FLUSH_TYPE_HEAD,
                ESVN_FLUSH_TYPE_ROOT_URL,
                ESVN_FLUSH_TYPE_CURRENT_URL,
                ESVN_FLUSH_TYPE_USER,
                ESVN_FLUSH_TYPE_LOG
            };

            class CSvnLogPathInfo
            {
            public:
                CSvnLogPathInfo()
                {
                }
                ~CSvnLogPathInfo()
                {
                }

                std::string kind() const
                {
                    return m_kind;
                }
                void kind(const std::string& val)
                {
                    m_kind = val;
                }
                std::string action() const
                {
                    return m_action;
                }
                void action(const std::string& val)
                {
                    m_action = val;
                }
                std::string path() const
                {
                    return m_path;
                }
                void path(const std::string& val)
                {
                    m_path = val;
                }

            private:
                std::string m_kind;
                std::string m_action;
                std::string m_path;
            };

            class CSvnLogInfo
            {
            public:
                CSvnLogInfo()
                {
                }
                ~CSvnLogInfo()
                {
                    int count = getPathInfoCount();
                    for (int i = 0; i < count; ++i)
                    {
                        CSvnLogPathInfo* path = m_paths[i];
                        if (path != NULL)
                        {
                            delete path;
                            path = NULL;
                        }
                    }
                }

                std::string user() const
                {
                    return m_user;
                }
                void user(const std::string& val)
                {
                    m_user = val;
                }

                std::string datetime() const
                {
                    return m_datetime;
                }
                void datetime(const std::string& val)
                {
                    m_datetime = val;
                }

                std::string log() const
                {
                    return m_log;
                }
                void log(const std::string& val)
                {
                    m_log = val;
                }

                std::string revision() const
                {
                    return m_revision;
                }
                void revision(const std::string& val)
                {
                    m_revision = val;
                }

                void addPathInfo(CSvnLogPathInfo*& val)
                {
                    m_paths.push_back(val);
                }

                int getPathInfoCount()
                {
                    return m_paths.size();
                }

                CSvnLogPathInfo* getPathInfo(int index)
                {
                    if (index >= 0 && index < getPathInfoCount())
                    {
                        return m_paths[index];
                    }
                    return NULL;
                }

            private:
                std::string m_user;
                std::string m_datetime;
                std::string m_log;
                std::string m_revision;
                std::vector<CSvnLogPathInfo*> m_paths;
            };

            class CSvnExternalInfo
            {
            public:
                CSvnExternalInfo()
                    :m_revision("HEAD")
                {
                }
                ~CSvnExternalInfo()
                {
                }

                std::string name() const
                {
                    return m_name;
                }
                void name(std::string val)
                {
                    m_name = val;
                }

                std::string revision() const
                {
                    return m_revision;
                }
                void revision(const std::string& val)
                {
                    m_revision = val;
                }

                std::string url() const
                {
                    return m_url;
                }
                void url(std::string val)
                {
                    m_url = val;
                }

            private:
                std::string m_name;
                std::string m_revision;
                std::string m_url;
            };

        public:
            CSvn();
            virtual ~CSvn();

            std::string flush(ESVN_FLUSH_TYPE flushType, bool force);

            std::string cleanup();
            std::string revert();
            std::string checkout(const std::string& repositoryPath, const std::string& reposityUrl, const std::string& revision, bool setRepository);

            std::string commit(const std::string& message);

            std::string update(const std::string& revision);

            int getLog(const std::string& revision, const std::string& arg, CSvnLogInfo**& svnLogInfos);
            void releaseLog(CSvnLogInfo**& svnLogInfos, int count);

            int getExternal(const std::string& repositoryPath, CSvnExternalInfo**& svnExternalInfos);
            void releaseExternal(CSvnExternalInfo**& svnExternalInfos, int count);

            std::string reposityPath() const
            {
                return m_reposityPath;
            }
            void reposityPath(const std::string& val)
            {
                m_reposityPath = val;
            }

        private:
            std::string _exeSvnCommand(int argcount, int& result, bool output = false);

            void _parseSvnLogConsoleString(std::string& revision);

            void _parseSvnInfoConsoleString(std::string& url, std::string& revision, std::string& user);
        private:

            bool m_supportSVN;
            std::string m_reposityPath;

            std::string m_baseRevision;
            std::string m_headRevision;
            std::string m_commitUser;
            std::string m_commitLog;
            std::string m_reposityUrl;
            std::string m_currentUrl;
            BCLib::Utility::CXmlFile* m_xmlFile;
            BCLib::Utility::CBinFile* m_binFile;
            TDLib::Subversion::CSvn* m_svn;
            //int m_oldStdout;
            FILE* m_stdout;
            const char* m_argv[MAX_SVN_ARG];
            char* m_buffcache;
            unsigned int m_buffcacheLength;
        };
    }//Subversion
}//MWLib

#endif//__MWLIB_SUBVERSION_SVN_H__
