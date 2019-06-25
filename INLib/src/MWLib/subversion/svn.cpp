//////////////////////////////////////////////////////////////////////
//  created:    2012/02/08
//  filename:   MWLib/subversion/svn.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <BCLib/utility/string.h>
#include <BCLib/utility/io/binFile.h>
#include <BCLib/utility/io/fileinfo.h>
#include <BCLib/utility/io/xmlFile.h>
#include <MWLib/subversion/svn.h>
#include <ext/subversion/subversion/svn/svn.h>


namespace MWLib
{
    namespace Subversion
    {
        typedef std::string svnstring;
        typedef std::stringbuf svnstringbuf;

        CSvn::CSvn()
            :m_supportSVN(true)
            , m_reposityPath("")
            , m_baseRevision("0")
            , m_headRevision("0")
            , m_commitUser("")
            , m_commitLog("")
            , m_reposityUrl("")
            , m_currentUrl("")
            , m_stdout(NULL)
            , m_buffcache(NULL)
        {
            m_xmlFile = new BCLib::Utility::CXmlFile();
            m_binFile = new BCLib::Utility::CBinFile();
            m_svn = new TDLib::Subversion::CSvn();
            m_argv[0] = "svn";
            m_buffcache = new char[4096];
            m_buffcacheLength = 4096;
        }

        CSvn::~CSvn()
        {
            if (m_xmlFile != NULL)
            {
                BCLIB_SAFE_DELETE(m_xmlFile);
            }
            if (m_stdout != NULL)
            {
                fclose(m_stdout);
                m_stdout = NULL;
            }
            if (m_buffcache != NULL)
            {
                BCLIB_SAFE_DELETE(m_buffcache);
            }
        }

        std::string CSvn::flush(ESVN_FLUSH_TYPE flushType, bool force)
        {
            std::string returnValue = "";
            BCLib::CString args = "";
            int result = 0;
            switch (flushType)
            {
            case ESVN_FLUSH_TYPE_BASE:
            {
                if (force || m_baseRevision == "0")
                {
                    m_argv[1] = "info";
                    m_argv[2] = m_reposityPath.c_str();
                    m_argv[3] = "-r";
                    m_argv[4] = "base";
                    m_argv[5] = "--xml";
                    _exeSvnCommand(6, result);
                    _parseSvnInfoConsoleString(m_currentUrl, m_baseRevision, m_commitUser);
                }
                returnValue = m_baseRevision;
            }
            break;
            case  ESVN_FLUSH_TYPE_HEAD:
            {
                if (force || m_headRevision == "0")
                {
                    m_argv[1] = "info";
                    m_argv[2] = m_reposityPath.c_str();
                    m_argv[3] = "-r";
                    m_argv[4] = "head";
                    m_argv[5] = "--xml";
                    _exeSvnCommand(6, result);
                    _parseSvnInfoConsoleString(m_currentUrl, m_headRevision, m_commitUser);
                }
                returnValue = m_headRevision;
            }
            break;
            case  ESVN_FLUSH_TYPE_ROOT_URL:
            {
                if (force || m_reposityUrl == "")
                {
                    m_argv[1] = "info";
                    m_argv[2] = m_reposityPath.c_str();
                    m_argv[3] = "-r";
                    m_argv[4] = "head";
                    m_argv[5] = "--xml";
                    _exeSvnCommand(6, result);
                    _parseSvnInfoConsoleString(m_currentUrl, m_headRevision, m_commitUser);
                }
                returnValue = m_reposityUrl;
            }
            break;
            case  ESVN_FLUSH_TYPE_CURRENT_URL:
            {
                if (force || m_currentUrl == "")
                {
                    m_argv[1] = "info";
                    m_argv[2] = m_reposityPath.c_str();
                    m_argv[3] = "-r";
                    m_argv[4] = "head";
                    m_argv[5] = "--xml";
                    _exeSvnCommand(6, result);
                    _parseSvnInfoConsoleString(m_currentUrl, m_headRevision, m_commitUser);
                }
                returnValue = m_currentUrl;
            }
            break;
            case  ESVN_FLUSH_TYPE_USER:
            {
                if (force || m_commitUser == "")
                {
                    m_argv[1] = "log";
                    m_argv[2] = m_reposityPath.c_str();
                    m_argv[3] = "-r";
                    m_argv[4] = "base";
                    m_argv[5] = "--xml";
                    _exeSvnCommand(6, result);
                    _parseSvnLogConsoleString(m_baseRevision);
                }
                returnValue = m_commitUser;
            }
            break;
            case  ESVN_FLUSH_TYPE_LOG:
            {
                if (force || m_commitLog == "")
                {
                    m_argv[1] = "log";
                    m_argv[2] = m_reposityPath.c_str();
                    m_argv[3] = "-r";
                    m_argv[4] = "base";
                    m_argv[5] = "--xml";
                    _exeSvnCommand(6, result);
                    _parseSvnLogConsoleString(m_baseRevision);
                }
                returnValue = m_commitLog;
            }
            break;
            }
            return returnValue;
        }

        std::string CSvn::cleanup()
        {
            int result = 0;
            m_argv[1] = "cleanup";
            m_argv[2] = m_reposityPath.c_str();
            return _exeSvnCommand(3, result, true);
        }

        std::string CSvn::revert()
        {
            int result = 0;
            m_argv[1] = "revert";
            m_argv[2] = m_reposityPath.c_str();
            m_argv[3] = "-R";
            return _exeSvnCommand(4, result, true);
        }

        std::string CSvn::checkout(const std::string& repositoryPath, const std::string& reposityUrl, const std::string& revision, bool setRepository)
        {
            if (setRepository)
            {
                m_reposityUrl = reposityUrl;
                m_reposityPath = repositoryPath;
            }
            int result = 0;
            m_argv[1] = "checkout";
            m_argv[2] = reposityUrl.c_str();
            m_argv[3] = "-r";
            m_argv[4] = revision.c_str();
            m_argv[5] = repositoryPath.c_str();
            return _exeSvnCommand(6, result, true);
        }

        std::string CSvn::commit(const std::string& message)
        {
            int result = 0;
            m_argv[1] = "commit";
            m_argv[2] = m_reposityPath.c_str();
            m_argv[3] = "-m";
            m_argv[4] = message.c_str();
            return _exeSvnCommand(5, result, true);
        }

        std::string CSvn::update(const std::string& revision)
        {
            int result = 0;
            m_argv[1] = "update";
            m_argv[2] = m_reposityPath.c_str();
            m_argv[3] = "-r";
            m_argv[4] = revision.c_str();
            m_argv[5] = "--force";
            std::string output = _exeSvnCommand(6, result, true);
            while (result != 0)
            {
                cleanup();
                m_argv[1] = "update";
                m_argv[2] = m_reposityPath.c_str();
                m_argv[3] = "-r";
                m_argv[4] = revision.c_str();
                m_argv[5] = "--force";
                output = _exeSvnCommand(6, result, true);
            }
            flush(ESVN_FLUSH_TYPE_BASE, true);
            return output;
        }

        int CSvn::getLog(const std::string& revision, const std::string& arg, CSvnLogInfo**& svnLogInfos)
        {
            int result = 0;
            int logentryCount = 0;
            m_argv[1] = "log";
            m_argv[2] = m_reposityPath.c_str();
            m_argv[3] = "-r";
            m_argv[4] = revision.c_str();
            m_argv[5] = arg.c_str();
            m_argv[6] = "--xml";
            _exeSvnCommand(7, result);
            BCLib::Utility::HXmlNode rootNode = m_xmlFile->getRootNode();
            if (rootNode != NULL)
            {
                BCLib::Utility::HXmlNode logentryNode = BCLib::Utility::CXmlFile::getChildNode(rootNode);
                while (logentryNode != NULL)
                {
                    logentryCount++;
                    logentryNode = BCLib::Utility::CXmlFile::getNextNode(logentryNode);
                }
                if (logentryCount > 0)
                {
                    svnLogInfos = new CSvnLogInfo *[logentryCount];
                    logentryNode = BCLib::Utility::CXmlFile::getChildNode(rootNode);
                    int index = 0;
                    while (logentryNode != NULL)
                    {
                        CSvnLogInfo* svnLogInfo = new CSvnLogInfo();
                        svnLogInfos[index] = svnLogInfo;
                        svnstring value;
                        if (BCLib::Utility::CXmlFile::getNodeAttrValue(logentryNode, "revision", value))
                        {
                            svnLogInfo->revision(value);
                        }
                        BCLib::Utility::HXmlNode authorNode = BCLib::Utility::CXmlFile::getChildNode(logentryNode);
                        if (BCLib::Utility::CXmlFile::getNodeValue(authorNode, value))
                        {
                            svnLogInfo->user(value);
                        }
                        BCLib::Utility::HXmlNode dateNode = BCLib::Utility::CXmlFile::getNextNode(authorNode);
                        if (BCLib::Utility::CXmlFile::getNodeValue(dateNode, value))
                        {
                            svnLogInfo->datetime(value);
                        }
                        BCLib::Utility::HXmlNode pathsNode = BCLib::Utility::CXmlFile::getNextNode(dateNode);
                        if (BCLib::Utility::CXmlFile::getNodeValue(pathsNode, value))
                        {
                            BCLib::Utility::HXmlNode pathNode = BCLib::Utility::CXmlFile::getChildNode(pathsNode);
                            while (pathNode != NULL)
                            {
                                CSvnLogPathInfo* svnLogPathInfo = new CSvnLogPathInfo();
                                if (BCLib::Utility::CXmlFile::getNodeAttrValue(pathNode, "kind", value))
                                {
                                    svnLogPathInfo->kind(value);
                                }

                                if (BCLib::Utility::CXmlFile::getNodeAttrValue(pathNode, "action", value))
                                {
                                    svnLogPathInfo->action(value);
                                }
                                if (BCLib::Utility::CXmlFile::getNodeValue(pathNode, value))
                                {
                                    svnLogPathInfo->path(value);
                                }
                                svnLogInfo->addPathInfo(svnLogPathInfo);
                                pathNode = BCLib::Utility::CXmlFile::getNextNode(pathNode);
                            }
                        }
                        BCLib::Utility::HXmlNode msgNode = BCLib::Utility::CXmlFile::getNextNode(pathsNode);
                        if (BCLib::Utility::CXmlFile::getNodeValue(msgNode, value))
                        {
                            svnLogInfo->log(value);
                        }
                        logentryNode = BCLib::Utility::CXmlFile::getNextNode(logentryNode);
                        index++;
                    }
                }
            }
            return logentryCount;
        }
        void CSvn::releaseLog(CSvnLogInfo**& svnLogInfos, int count)
        {
            if (svnLogInfos != NULL && count > 0)
            {
                for (int i = 0; i < count; ++i)
                {
                    CSvnLogInfo* svnLogInfo = svnLogInfos[i];
                    if (svnLogInfo != NULL)
                    {
                        BCLIB_SAFE_DELETE(svnLogInfo);
                    }
                }
                BCLIB_SAFE_DELETE(svnLogInfos);
            }
        }

        int CSvn::getExternal(const std::string& repositoryPath, CSvnExternalInfo**& svnExternalInfos)
        {
            int result = 0;
            int externalCount = 0;
            m_argv[1] = "propget";
            m_argv[2] = "svn:externals";
            m_argv[3] = repositoryPath.c_str();
            m_argv[4] = "--xml";
            _exeSvnCommand(5, result);
            BCLib::Utility::HXmlNode propertiesNode = m_xmlFile->getRootNode();
            if (propertiesNode != NULL)
            {
                BCLib::Utility::HXmlNode targetNode = BCLib::Utility::CXmlFile::getChildNode(propertiesNode);
                if (targetNode != NULL)
                {
                    BCLib::Utility::HXmlNode propertyNode = BCLib::Utility::CXmlFile::getChildNode(targetNode);
                    if (propertyNode != NULL)
                    {
                        BCLib::CString externalstring;
                        BCLib::Utility::CXmlFile::getNodeValue(propertyNode, externalstring);
                        std::list<BCLib::CString> externals;
                        externalstring.split(externals, '\n');
                        externalCount = (int)externals.size();
                        if (externalCount > 0)
                        {
                            svnExternalInfos = new CSvnExternalInfo *[externalCount];
                            int index = 0;
                            std::list<BCLib::CString>::iterator it;
                            for (it = externals.begin(); it != externals.end(); ++it)
                            {
                                BCLib::CString external = *it;
                                std::list<BCLib::CString> args;
                                external.split(args, ' ');
                                if (args.size() > 0)
                                {
                                    CSvnExternalInfo* externalInfo = new CSvnExternalInfo();
                                    svnExternalInfos[index++] = externalInfo;
                                    std::list<BCLib::CString>::iterator argit;
                                    argit = args.begin();
                                    externalInfo->name(*argit);
                                    argit++;
                                    if (*argit == "-r")
                                    {
                                        argit++;
                                        externalInfo->revision(*argit);
                                    }
                                    else
                                    {
                                        externalInfo->url(*argit);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return externalCount;
        }

        void CSvn::releaseExternal(CSvnExternalInfo**& svnExternalInfos, int count)
        {
            if (svnExternalInfos != NULL && count > 0)
            {
                for (int i = 0; i < count; ++i)
                {
                    CSvnExternalInfo* externalInfo = svnExternalInfos[i];
                    if (externalInfo != NULL)
                    {
                        BCLIB_SAFE_DELETE(externalInfo);
                    }
                }
                BCLIB_SAFE_DELETE(svnExternalInfos);
            }
        }

        std::string CSvn::_exeSvnCommand(int argcount, int& result, bool output)
        {
            svnstring buffer = "";
            svnstring svntmpfile = "svndumy.fil";
            m_stdout = freopen(svntmpfile.c_str(), "wb+", stdout);

            result = m_svn->exe(argcount, m_argv);
            fflush(stdout);
            fclose(stdout);

            unsigned int length = BCLib::Utility::CFileInfo::getLength(svntmpfile);
            if (length > m_buffcacheLength)
            {
                m_buffcacheLength = length * 2;
                BCLIB_SAFE_DELETE(m_buffcache);
                m_buffcache = new char[m_buffcacheLength];
                m_buffcache[length] = 0;
            }
            else if (length >= 0 && length < m_buffcacheLength)
            {
                m_buffcache[length] = 0;
            }
            if (length != 0)
            {
                m_binFile->open(svntmpfile);
                m_binFile->read(m_buffcache, length);
                m_binFile->close();
                buffer = m_buffcache;
            }

            if (m_argv[argcount - 1] == "--xml" && buffer != "")
            {
                m_xmlFile->clear();
                m_xmlFile->loadString(buffer);
            }

            if (output)
            {
                return buffer;
            }
            return "";
        }

        void CSvn::_parseSvnLogConsoleString(std::string& revision)
        {
            BCLib::Utility::HXmlNode rootNode = m_xmlFile->getRootNode();
            if (rootNode != NULL)
            {
                BCLib::Utility::HXmlNode logentryNode = BCLib::Utility::CXmlFile::getChildNode(rootNode);
                if (logentryNode != NULL)
                {
                    BCLib::Utility::CXmlFile::getNodeAttrValue(logentryNode, "revision", revision);
                    BCLib::Utility::HXmlNode authorNode = BCLib::Utility::CXmlFile::getChildNode(logentryNode);
                    if (authorNode != NULL)
                    {
                        BCLib::Utility::CXmlFile::getNodeValue(authorNode, m_commitUser);
                        BCLib::Utility::HXmlNode dateNode = BCLib::Utility::CXmlFile::getNextNode(authorNode);
                        if (dateNode != NULL)
                        {
                            BCLib::Utility::HXmlNode msgNode = BCLib::Utility::CXmlFile::getNextNode(dateNode);
                            BCLib::Utility::CXmlFile::getNodeValue(msgNode, m_commitLog);
                        }
                    }
                }
            }
        }

        void CSvn::_parseSvnInfoConsoleString(std::string& url, std::string& revision, std::string& user)
        {
            BCLib::Utility::HXmlNode infoNode = m_xmlFile->getRootNode();
            if (infoNode != NULL)
            {
                BCLib::Utility::HXmlNode entryNode = BCLib::Utility::CXmlFile::getChildNode(infoNode);
                if (entryNode != NULL)
                {
                    BCLib::Utility::HXmlNode urlNode = BCLib::Utility::CXmlFile::getChildNode(entryNode);
                    if (urlNode != NULL)
                    {
                        BCLib::Utility::CXmlFile::getNodeValue(urlNode, url);
                        BCLib::Utility::HXmlNode repositoryNode = BCLib::Utility::CXmlFile::getNextNode(urlNode);

                        if (repositoryNode != NULL)
                        {
                            BCLib::Utility::HXmlNode rootNode = BCLib::Utility::CXmlFile::getChildNode(repositoryNode);
                            if (rootNode != NULL)
                            {
                                BCLib::Utility::CXmlFile::getNodeValue(urlNode, m_reposityUrl);
                            }
                            BCLib::Utility::HXmlNode commitNode = BCLib::Utility::CXmlFile::getNextNode(repositoryNode);
                            if (commitNode != NULL)
                            {
                                BCLib::Utility::CXmlFile::getNodeAttrValue(commitNode, "revision", revision);
                            }
                            BCLib::Utility::HXmlNode authorNode = BCLib::Utility::CXmlFile::getChildNode(commitNode);
                            if (authorNode != NULL)
                            {
                                BCLib::Utility::CXmlFile::getNodeValue(authorNode, user);
                            }
                        }
                    }
                }
            }
        }
    }//Subversion
}//MWLib
