//////////////////////////////////////////////////////////////////////////
//    created:    2011/11/01
//    filename:   WMLib\subversion\svn.h
//    author:     League of Perfect
///   @brief 
///
//////////////////////////////////////////////////////////////////////////
#ifndef __CLLIB_SUBVERSION_SVN_H__
#define __CLLIB_SUBVERSION_SVN_H__
#include <CLLib/Utility/baseDef.h>

namespace MWLib
{
namespace Subversion
{
    class CSvn;
    class CSvn::CSvnLogPathInfo;
    class CSvn::CSvnLogInfo;
}
}

namespace CLLib
{
namespace Subversion
{

public ref class CSvn
{
public:
    enum class ESVN_FLUSH_TYPE
    {
        ESVN_FLUSH_TYPE_BASE,
        ESVN_FLUSH_TYPE_HEAD,
        ESVN_FLUSH_TYPE_ROOT_URL,
        ESVN_FLUSH_TYPE_CURRENT_URL,
        ESVN_FLUSH_TYPE_USER,
        ESVN_FLUSH_TYPE_LOG
    };

    ref class CSvnLogPathInfo
    {
    public:
        CSvnLogPathInfo():m_svnLogPathInfo(NULL){}
        ~CSvnLogPathInfo()
        {
            CLLIB_SAFE_DELETE(m_svnLogPathInfo)
        }

        System::String^ Kind();
        System::String^ Action();
        System::String^ Path();
        void SetSvnLogPathInfo(MWLib::Subversion::CSvn::CSvnLogPathInfo* val) { m_svnLogPathInfo = val; }

    private:
        MWLib::Subversion::CSvn::CSvnLogPathInfo* m_svnLogPathInfo;

    };

    ref class CSvnLogInfo
    {
    public:
        CSvnLogInfo():m_svnLogInfo(NULL){}
        ~CSvnLogInfo()
        {
            CLLIB_SAFE_DELETE(m_svnLogInfo)
        }

        System::String^ User();
        System::String^ Datetime();
        System::String^ Log();
        System::String^ Revision();
        int GetPathInfoCount();
        CLLib::Subversion::CSvn::CSvnLogPathInfo^ GetPathInfo(int index);

        void SetSvnLogInfo(MWLib::Subversion::CSvn::CSvnLogInfo* val);

    private:
        MWLib::Subversion::CSvn::CSvnLogInfo* m_svnLogInfo;        
    };

    ref class CSvnExternalInfo
    {
    public:
        CSvnExternalInfo():m_svnExternalInfo(NULL){}
        ~CSvnExternalInfo()
        {
            CLLIB_SAFE_DELETE(m_svnExternalInfo)
        }

        System::String^ Name();
        System::String^ Revision();
        System::String^ Url();

        void SetSvnExternalInfo(MWLib::Subversion::CSvn::CSvnExternalInfo* val);

    private:
        MWLib::Subversion::CSvn::CSvnExternalInfo* m_svnExternalInfo;        
    };

public:
    CSvn();
    virtual ~CSvn();

    System::String^ Flush(ESVN_FLUSH_TYPE flushType,bool force);
    System::String^ Cleanup();
    System::String^ Revert();
    System::String^ Checkout(System::String^ repositoryPath,System::String^ reposityUrl,System::String^ revision, bool setRepository);
    System::String^ Commit(System::String^ message);
    System::String^ Update(System::String^ revision);

    int GetLog(System::String^ revision,System::String^ arg,array<CSvnLogInfo^>^% svnLogInfos);
    void ReleaseLog(array<CSvnLogInfo^>^% svnLogInfos,int count);

    int GetExternals( System::String^ repositoryPath,array<CSvnExternalInfo^>^% svnExternalInfos );
    void ReleaseExternal(array<CSvnExternalInfo^>^% svnExternalInfos,int count);

    System::String^ ReposityPath();
    void ReposityPath(System::String^ val);

    static CSvn^ Instance = gcnew CSvn();

private:
    MWLib::Subversion::CSvn* m_svn;    
};

}//Utility

}//CLLib

#endif//__CLLIB_SUBVERSION_SVN_H__