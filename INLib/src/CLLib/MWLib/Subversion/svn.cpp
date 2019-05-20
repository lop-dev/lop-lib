//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   WMLib\subversion\svn.cpp
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#include <MWLib/subversion/svn.h>
#include <CLLib/MWLib/Subversion/svn.h>
#include <CLLib/Utility/convert.h>
#include <CLLib/Utility/encodingHelper.h>

namespace CLLib
{
    namespace MWLib_Subversion
    {
        CSvn::CSvn() : m_svn(NULL)
        {
            m_svn = new MWLib::Subversion::CSvn();
        }

        CSvn::~CSvn()
        {
            if (m_svn != NULL)
            {
                CLLIB_SAFE_DELETE(m_svn);
            }
        }

        System::String^ CSvn::Flush(ESVN_FLUSH_TYPE flushType, bool force)
        {
            if (m_svn != NULL)
            {
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svn->flush((MWLib::Subversion::CSvn::ESVN_FLUSH_TYPE)flushType, force));
            }
            return "";
        }

        System::String^ CSvn::Cleanup()
        {
            if (m_svn != NULL)
            {
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svn->cleanup());
            }
            return "";
        }

        System::String^ CSvn::Revert()
        {
            if (m_svn != NULL)
            {
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svn->revert());
            }
            return "";
        }

        System::String^ CSvn::Checkout(System::String^ repositoryPath, System::String^ reposityUrl, System::String^ revision, bool setRepository)
        {
            if (m_svn != NULL)
            {
                std::string nativerepositoryPath = CLLib::Utility::CConvert::DotNetStringToSTLString(repositoryPath);
                std::string nativereposityUrl = CLLib::Utility::CConvert::DotNetStringToSTLString(reposityUrl);
                std::string nativerevision = CLLib::Utility::CConvert::DotNetStringToSTLString(revision);
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svn->checkout(nativerepositoryPath, nativereposityUrl, nativerevision, setRepository));
            }
            return "";
        }

        System::String^ CSvn::Commit(System::String^ message)
        {
            if (m_svn != NULL)
            {
                std::string nativemessage = CLLib::Utility::CConvert::DotNetStringToSTLString(message);
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svn->commit(nativemessage));
            }
            return "";
        }

        System::String^ CSvn::Update(System::String^ revision)
        {
            if (m_svn != NULL)
            {
                std::string nativerevision = CLLib::Utility::CConvert::DotNetStringToSTLString(revision);
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svn->update(nativerevision));
            }
            return "";
        }

        int CSvn::GetLog(System::String^ revision, System::String^ arg, array<CSvnLogInfo^>^% svnLogInfos)
        {
            if (m_svn != NULL)
            {
                std::string nativerevision = CLLib::Utility::CConvert::DotNetStringToSTLString(revision);
                std::string nativearg = CLLib::Utility::CConvert::DotNetStringToSTLString(arg);
                MWLib::Subversion::CSvn::CSvnLogInfo** nativeSvnLogInfos = NULL;
                int logCount = m_svn->getLog(nativerevision, nativearg, nativeSvnLogInfos);
                if (nativeSvnLogInfos != NULL&&logCount > 0)
                {
                    svnLogInfos = gcnew array<CSvnLogInfo^>(logCount);
                    for (int i = 0; i < logCount; ++i)
                    {
                        MWLib::Subversion::CSvn::CSvnLogInfo* nativeSvnLogInfo = nativeSvnLogInfos[i];
                        CSvnLogInfo^ svnLogInfo = gcnew CSvnLogInfo();
                        svnLogInfo->SetSvnLogInfo(nativeSvnLogInfo);
                        svnLogInfos[i] = svnLogInfo;
                    }
                    CLLIB_SAFE_DELETE_ARRAY(nativeSvnLogInfos);
                }
                return logCount;
            }
            return 0;
        }

        void CSvn::ReleaseLog(array<CSvnLogInfo^>^% svnLogInfos, int count)
        {
            if (svnLogInfos != nullptr&&count > 0)
            {
                for (int i = 0; i < count; ++i)
                {
                    CSvnLogInfo^ svnLogInfo = svnLogInfos[i];
                    delete svnLogInfo;
                }
                svnLogInfos = nullptr;
            }
        }

        int CSvn::GetExternals(System::String^ repositoryPath, array<CSvnExternalInfo^>^% svnExternalInfos)
        {
            if (m_svn != NULL)
            {
                std::string nativereRepositoryPath = CLLib::Utility::CConvert::DotNetStringToSTLString(repositoryPath);
                MWLib::Subversion::CSvn::CSvnExternalInfo** nativeSvnExternalInfos = NULL;
                int externalCount = m_svn->getExternal(nativereRepositoryPath, nativeSvnExternalInfos);
                if (nativeSvnExternalInfos != NULL&&externalCount > 0)
                {
                    svnExternalInfos = gcnew array<CSvnExternalInfo^>(externalCount);
                    for (int i = 0; i < externalCount; ++i)
                    {
                        MWLib::Subversion::CSvn::CSvnExternalInfo* nativeSvnExternalInfo = nativeSvnExternalInfos[i];
                        CSvnExternalInfo^ svnExternalInfo = gcnew CSvnExternalInfo();
                        svnExternalInfo->SetSvnExternalInfo(nativeSvnExternalInfo);
                        svnExternalInfos[i] = svnExternalInfo;
                    }
                    CLLIB_SAFE_DELETE_ARRAY(nativeSvnExternalInfos);
                }
                return externalCount;
            }
            return 0;
        }

        void CSvn::ReleaseExternal(array<CSvnExternalInfo^>^% svnExternalInfos, int count)
        {
            if (svnExternalInfos != nullptr&&count > 0)
            {
                for (int i = 0; i < count; ++i)
                {
                    CSvnExternalInfo^ svnExternalInfo = svnExternalInfos[i];
                    delete svnExternalInfo;
                }
                svnExternalInfos = nullptr;
            }
        }
        System::String^ CSvn::ReposityPath()
        {
            if (m_svn != NULL)
            {
                return CLLib::Utility::CConvert::STLStringToDotNetString(m_svn->reposityPath());
            }
            return "";
        }

        void CSvn::ReposityPath(System::String^ val)
        {
            if (m_svn != NULL)
            {
                std::string nativeval = CLLib::Utility::CConvert::DotNetStringToSTLString(val);
                m_svn->reposityPath(nativeval);
            }
        }

        System::String^ CSvn::CSvnLogPathInfo::Kind()
        {
            if (m_svnLogPathInfo != NULL)
            {
                return CLLib::Utility::CConvert::STLStringToDotNetString(m_svnLogPathInfo->kind());
            }
            return "";
        }

        System::String^ CSvn::CSvnLogPathInfo::Action()
        {
            if (m_svnLogPathInfo != NULL)
            {
                return CLLib::Utility::CConvert::STLStringToDotNetString(m_svnLogPathInfo->action());
            }
            return "";
        }

        System::String^ CSvn::CSvnLogPathInfo::Path()
        {
            if (m_svnLogPathInfo != NULL)
            {
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svnLogPathInfo->path());
            }
            return "";
        }

        System::String^ CSvn::CSvnLogInfo::User()
        {
            if (m_svnLogInfo != NULL)
            {
                return CLLib::Utility::CConvert::STLStringToDotNetString(m_svnLogInfo->user());
            }
            return "";
        }

        System::String^ CSvn::CSvnLogInfo::Datetime()
        {
            if (m_svnLogInfo != NULL)
            {
                return CLLib::Utility::CConvert::STLStringToDotNetString(m_svnLogInfo->datetime());
            }
            return "";
        }

        System::String^ CSvn::CSvnLogInfo::Log()
        {
            if (m_svnLogInfo != NULL)
            {
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svnLogInfo->log());
            }
            return "";
        }

        System::String^ CSvn::CSvnLogInfo::Revision()
        {
            if (m_svnLogInfo != NULL)
            {
                return CLLib::Utility::CConvert::STLStringToDotNetString(m_svnLogInfo->revision());
            }
            return "";
        }

        int CSvn::CSvnLogInfo::GetPathInfoCount()
        {
            if (m_svnLogInfo != NULL)
            {
                return m_svnLogInfo->getPathInfoCount();
            }
            return 0;
        }

        CLLib::MWLib_Subversion::CSvn::CSvnLogPathInfo^ CSvn::CSvnLogInfo::GetPathInfo(int index)
        {
            CLLib::MWLib_Subversion::CSvn::CSvnLogPathInfo^ svnLogPathInfo = nullptr;
            if (m_svnLogInfo != NULL)
            {
                MWLib::Subversion::CSvn::CSvnLogPathInfo* nativeSvnLogPathInfo = m_svnLogInfo->getPathInfo(index);
                svnLogPathInfo = gcnew CLLib::MWLib_Subversion::CSvn::CSvnLogPathInfo();
                svnLogPathInfo->SetSvnLogPathInfo(nativeSvnLogPathInfo);
            }
            return svnLogPathInfo;
        }

        void CSvn::CSvnLogInfo::SetSvnLogInfo(MWLib::Subversion::CSvn::CSvnLogInfo* val)
        {
            m_svnLogInfo = val;
        }

        System::String^ CSvn::CSvnExternalInfo::Name()
        {
            if (m_svnExternalInfo != NULL)
            {
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svnExternalInfo->name());
            }
            return "";
        }

        System::String^ CSvn::CSvnExternalInfo::Revision()
        {
            if (m_svnExternalInfo != NULL)
            {
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svnExternalInfo->revision());
            }
            return "";
        }

        System::String^ CSvn::CSvnExternalInfo::Url()
        {
            if (m_svnExternalInfo != NULL)
            {
                return CLLib::Utility::CEncodingHelper::singleton()->ConvertUTF8String(m_svnExternalInfo->url());
            }
            return "";
        }

        void CSvn::CSvnExternalInfo::SetSvnExternalInfo(MWLib::Subversion::CSvn::CSvnExternalInfo* val)
        {
            m_svnExternalInfo = val;
        }
    }
}