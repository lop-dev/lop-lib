//////////////////////////////////////////////////////////////////////
//  created:    2012/08/14
//  filename:   CodeChecker\codeChecker.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include "codeChecker.h"
#include "codeUtility.h"
#include <io.h>
#include <algorithm>


CCodeChecker::CCodeChecker()
:m_year(0)
,m_month(0)
,m_day(0)
,m_dateMode(0)
{
    ;
}

CCodeChecker::~CCodeChecker()
{
    ;
}

bool CCodeChecker::initCfg(const std::string& cfgName)
{
    //! 屏蔽的目录/文件转小写
    //! .h/.cpp后缀转小写,加.

    cfgMap map;
    cfgMap::iterator itr = map.end();
    if(!readCfg(cfgName, map))
    {
        printf("ERRO: Config file[%s] open failed\n", cfgName.c_str());
        return false;
    }

    itr = map.find("CheckDir");
    if(itr != map.end())
    {
        m_checkDirs = itr->second;
    }
    else
    {
        printf("ERRO: Can not find key-val[CheckDir] in config file[%s]\n", cfgName.c_str());
        return false;
    }

    itr = map.find("ExcludeDir");
    if(itr != map.end())
    {
        m_excludeDirs = itr->second;

        for(int i = 0; i < (int)m_excludeDirs.size(); ++i)
        {
            std::transform(m_excludeDirs[i].begin(), m_excludeDirs[i].end(), m_excludeDirs[i].begin(), tolower);
        }
    }

    itr = map.find("ExcludeFile");
    if(itr != map.end())
    {
        m_excludeFiles = itr->second;

        for(int i = 0; i < (int)m_excludeFiles.size(); ++i)
        {
            std::transform(m_excludeFiles[i].begin(), m_excludeFiles[i].end(), m_excludeFiles[i].begin(), tolower);
        }
    }

    itr = map.find("HeadFileExt");
    if(itr != map.end())
    {
        m_headFileExts = itr->second;

        for(int i = 0; i < (int)m_headFileExts.size(); ++i)
        {
            std::transform(m_headFileExts[i].begin(), m_headFileExts[i].end(), m_headFileExts[i].begin(), tolower);
            m_headFileExts[i] = "." + m_headFileExts[i];
        }
    }
    else
    {
        printf("ERRO: Can not find key-val[HeadFileExt] in config file[%s]\n", cfgName.c_str());
        return false;
    }

    itr = map.find("SourceFileExt");
    if(itr != map.end())
    {
        m_cppFileExts = itr->second;

        for(int i = 0; i < (int)m_cppFileExts.size(); ++i)
        {
            std::transform(m_cppFileExts[i].begin(), m_cppFileExts[i].end(), m_cppFileExts[i].begin(), tolower);
            m_cppFileExts[i] = "." + m_cppFileExts[i];
        }
    }
    else
    {
        printf("ERRO: Can not find key-val[SourceFileExt] in config file[%s]\n", cfgName.c_str());
        return false;
    }

    itr = map.find("ValidAuthor");
    if(itr != map.end())
    {
        m_validAuthorVec = itr->second;
    }

    itr = map.find("DefaultAuthor");
    if(itr != map.end())
    {
        m_defaultAuthor = itr->second[0];
    }
    else
    {
        printf("ERRO: Can not find key-val[DefaultAuthor] in config file[%s]\n", cfgName.c_str());
        return false;
    }

    itr = map.find("FileDate");
    if(itr != map.end())
    {
        sscanf(itr->second[0].c_str(), "%d/%d/%d", &m_year, &m_month, &m_day);
    }
    else
    {
        printf("ERRO: Can not find key-val[FileDate] in config file[%s]\n", cfgName.c_str());
        return false;
    }

    itr = map.find("FileDateMode");
    if(itr != map.end())
    {
        if(itr->second[0] == "1")
        {
            m_dateMode = 1;
        }
        else
        {
            m_dateMode = 0;
        }
    }
    else
    {
        printf("ERRO: Can not find key-val[FileDateMode] in config file[%s]\n", cfgName.c_str());
        return false;
    }

    itr = map.find("IfndefMacroPrefix");
    if(itr != map.end())
    {
        m_prefix = itr->second[0];
        std::transform(m_prefix.begin(), m_prefix.end(), m_prefix.begin(), toupper);
    }
    else
    {
        printf("ERRO: Can not find key-val[IfndefMacroPrefix] in config file[%s]\n", cfgName.c_str());
        return false;
    }

    itr = map.find("InlibName");
    if(itr != map.end())
    {
        m_inLibNames = itr->second;
        m_lowerInLibNames = m_inLibNames;
        for(int i = 0; i < (int)m_lowerInLibNames.size(); ++i)
        {
            std::transform(m_lowerInLibNames[i].begin(), m_lowerInLibNames[i].end(), m_lowerInLibNames[i].begin(), tolower);
        }
    }

    itr = map.find("ShieldMacro");
    if(itr != map.end())
    {
        m_shieldMacros = itr->second;
    }

    itr = map.find("LeftAlignWord");
    if(itr != map.end())
    {
        m_leftAlignWords = itr->second;
    }

    itr = map.find("LegalNameSpace");
    if(itr != map.end())
    {
        m_legalNameSpaces = itr->second;
    }

    itr = map.find("IndentWord");
    if(itr != map.end())
    {
        int indent = 0;
        int npos = 0;
        strList::iterator itrB = itr->second.begin();
        strList::iterator itrE = itr->second.end();
        for(; itrB != itrE; ++itrB)
        {
            npos = itrB->find('+');
            if(npos == -1 || npos == 0)
            {
                printf("ERRO: Parse [%s] error in key-val[IndentWord] in config file[%s]\n", itrB->c_str(), cfgName.c_str());
                continue;
            }

            indent = atoi(itrB->substr(npos + 1, itrB->length() - npos - 1).c_str());
            if(indent < 0)
            {
                printf("ERRO: Parse [%s] error in key-val[IndentWord] in config file[%s]\n", itrB->c_str(), cfgName.c_str());
                continue;
            }

            m_wordIndentMap.insert(std::make_pair(itrB->substr(0, npos), indent));
        }
    }

    return true;
}

//! 0 : 检查代码, 1 : 屏蔽宏,为SourceStyler预处理, 2 : 解除宏屏蔽,为SourceStyler后处理
bool CCodeChecker::check(int checkMode)
{
    if(checkMode < 0 || 2 < checkMode)
    {
        return false;
    }

    m_filesInProj.clear();
    m_filesInInclude.clear();
    m_fileIncludeFilesMap.clear();
    m_filesInDir.clear();
    m_filesAndDirsInDir.clear();
    m_lowerFilesAndDirsInDir.clear();
    m_lowerFilesAndParentDirInDir.clear();

    if(checkMode == 0)
    {
        for(int i = 0; i < (int)m_checkDirs.size(); ++i)
        {
            _checkOneDir(m_checkDirs[i], 4);//! 目录/文件扫描
        }
    }

    for(int i = 0; i < (int)m_checkDirs.size(); ++i)
    {
        _checkOneDir(m_checkDirs[i], checkMode);
    }

    if(checkMode == 0)
    {
        for(int i = 0; i < (int)m_checkDirs.size(); ++i)
        {
            _checkOneDir(m_checkDirs[i], 3);//! proj扫描
        }

        _scanUnUsedFiles();
    }
    return true;
}

bool CCodeChecker::_checkOneDir(const std::string& dir, int checkMode)
{
    std::string fullDir;
    strList dirs;
    spliteDir(dir, dirs);

    if(dirs.empty() || dirs.back() == "." || dirs.back() == "..")
    {
        printf("ERRO: CheckDir[%s] is invalid\n", dirs.back().c_str());
        return false;
    }

    combineDir(fullDir, dirs);

    int rootLength = (int)fullDir.rfind('/');
    if(rootLength != -1)
    {
        ++rootLength;
    }
    else
    {
        rootLength = 0;
    }

    fullDir += '/';

    _traverseDir(fullDir, rootLength, checkMode);
    return true;
}

void CCodeChecker::_traverseDir(const std::string& dir, int rootLength, int checkMode)
{
    _finddata_t fileInfo;
    std::string strFind = dir + "*.*";
    long handle = _findfirst(strFind.c_str(), &fileInfo);
    if(handle == -1)
    {
        return;
    }

    do
    {
        std::string fileName = fileInfo.name;

        if(fileInfo.attrib & _A_SUBDIR)
        {
            if(fileName != "." && fileName != "..")
            {
                if(_isExcludeDir(fileName))
                {
                    continue;
                }

                if(checkMode == 4)//! 目录/文件扫描
                {
                    std::string lowerDir = fileName;
                    std::transform(lowerDir.begin(), lowerDir.end(), lowerDir.begin(), tolower);
                    m_filesAndDirsInDir.push_back(fileName);
                    m_lowerFilesAndDirsInDir.push_back(lowerDir);
                }

                std::string subDir = dir + fileName + "/";
                _traverseDir(subDir, rootLength, checkMode);
            }
        }
        else
        {
            if(_isExcludeFile(fileName))
            {
                continue;
            }

            if(checkMode == 0)//! 检查
            {
                if(_isHeadFile(fileName))
                {
                    _checkHeadFile(dir, fileName, rootLength);
                }
                else if(_isCppFile(fileName))
                {
                    _checkCppFile(dir, fileName, rootLength);
                }
            }
            else if(checkMode == 1)//! 预处理
            {
                if(_isHeadFile(fileName) || _isCppFile(fileName))
                {
                    _preSourceStyler(dir, fileName);
                }
            }
            else if(checkMode == 2)//! 后处理
            {
                if(_isHeadFile(fileName) || _isCppFile(fileName))
                {
                    _postSourceStyler(dir, fileName);
                }
            }
            else if(checkMode == 3)//! proj扫描
            {
                if(_isProjFile(fileName))
                {
                    _parseProjFile(dir, fileName);
                }
            }
            else if(checkMode == 4)//! 目录/文件扫描
            {
                if(_isHeadFile(fileName) || _isCppFile(fileName))
                {
                    std::string lowerFileName = fileName;
                    std::transform(lowerFileName.begin(), lowerFileName.end(), lowerFileName.begin(), tolower);
                    m_filesAndDirsInDir.push_back(fileName);
                    m_lowerFilesAndDirsInDir.push_back(lowerFileName);
                    m_filesInDir.push_back(std::make_pair(dir, fileName));
                    strList dirs;
                    spliteDir(dir, dirs);
                    if(!dirs.empty())
                    {
                        m_lowerFilesAndParentDirInDir.push_back(std::make_pair(dirs.back(), lowerFileName));
                    }
                }
            }
        }
    }
    while(_findnext(handle, &fileInfo) == 0);

    _findclose(handle);
}

bool CCodeChecker::_preSourceStyler(const std::string& dir, const std::string& fileName)
{
    std::string file;

    if(!readFile(file, dir + fileName))
    {
        return false;
    }

    _shieldMacros(file);

    if(!writeFile(file, dir + fileName))
    {
        return false;
    }

    return true;
}

bool CCodeChecker::_postSourceStyler(const std::string& dir, const std::string& fileName)
{
    std::string file;

    if(!readFile(file, dir + fileName))
    {
        return false;
    }

    _unShieldMacros(file);

    std::string replaceStr("$$$$$$$$");
    strList comments;
    pumpComments(file, replaceStr, comments);

    _procLeftAlignWords(file);
    _procWordIndents(file);

    putBackComments(file, replaceStr, comments);

    if(!writeFile(file, dir + fileName))
    {
        return false;
    }

    return true;
}

bool CCodeChecker::_parseProjFile(const std::string& dir, const std::string& fileName)
{
    std::string file;
    strList dirs;

    if(!readFile(file, dir + fileName))
    {
        return false;
    }

    int npos = 0, npos1 = 0, npos2 = 0;
    npos = file.find("<Files>");
    if(npos == -1)
    {
        return false;
    }

    npos1 = file.find("</Files>", npos);
    if(npos1 == -1)
    {
        return false;
    }

    file = file.substr(npos, npos1 - npos);

    npos = 0;
    for(; ;)
    {
        npos = file.find("RelativePath=", npos);
        if(npos == -1)
        {
            break;
        }

        npos += 13;

        npos1 = file.find("\"", npos);
        if(npos1 == -1)
        {
            continue;
        }

        ++npos1;

        npos2 = file.find("\"", npos1);
        if(npos2 == -1)
        {
            continue;
        }

        spliteDir(file.substr(npos1, npos2 - npos1), dirs);
        if(!dirs.empty())
        {
            m_filesInProj.push_back(dirs.back());
        }
    }

    return true;
}

bool CCodeChecker::_checkHeadFile(const std::string& dir, const std::string& fileName, int rootLength)
{
    std::string file;
    std::string comment;
    int year = 0, month = 0, day = 0;
    std::string author;
    strList briefs;

    if(!readFile(file, dir + fileName))
    {
        return false;
    }
    _cutRepeatSemicolon(file);
    cutAllLineEndBlanks(file);
    _cutHeadCommentLines(file, comment);
    cutFileEndBlankLines(file);
    if(!_cutIfDefGuard(file))
    {
        printf("WARN: Can't find #ifndef/#define/#endif guard in file[%s%s]\n", dir.c_str(), fileName.c_str());
    }
    _checkInclude(file, fileName);
    _checkNameSpace(file, dir, fileName);
    if(!_parseComment(comment, year, month, day, author, briefs))
    {
        printf("WARN: Can't find Author/Time/Briefs in file[%s%s]\n", dir.c_str(), fileName.c_str());
    }
    _addIfDefGuard(file, dir, fileName, rootLength);
    _addComment(file, dir, fileName, rootLength, year, month, day, author, briefs);
    if(!writeFile(file, dir + fileName))
    {
        return false;
    }

    return true;
}

bool CCodeChecker::_checkCppFile(const std::string& dir, const std::string& fileName, int rootLength)
{
    std::string file;
    std::string comment;
    int year = 0, month = 0, day = 0;
    std::string author;
    strList briefs;

    if(!readFile(file, dir + fileName))
    {
        return false;
    }
    _cutRepeatSemicolon(file);
    cutAllLineEndBlanks(file);
    _cutHeadCommentLines(file, comment);
    _checkInclude(file, fileName);
    _checkNameSpace(file, dir, fileName);
    if(!_parseComment(comment, year, month, day, author, briefs))
    {
        printf("WARN: Can't find Author/Time/Briefs in file[%s%s]\n", dir.c_str(), fileName.c_str());
    }
    _addComment(file, dir, fileName, rootLength, year, month, day, author, briefs);
    if(!writeFile(file, dir + fileName))
    {
        return false;
    }

    return true;
}

void CCodeChecker::_scanUnUsedFiles()
{
    bool bErase = false;
    bool bUnUse = false;
    strList filesInInclude;
    filesInDir::iterator itrB = m_filesInDir.begin();
    for(; itrB != m_filesInDir.end();)
    {
        bErase = false;
        bUnUse = false;

        if(_isHeadFile(itrB->second))
        {
            if(_isFileInProj(itrB->second))
            {
                bErase = true;
                filesInInclude = _findIncludeFiles(itrB->second);
                m_filesInInclude.insert(m_filesInInclude.end(), filesInInclude.begin(), filesInInclude.end());
            }
        }
        else if(_isCppFile(itrB->second))
        {
            if(_isFileInProj(itrB->second))
            {
                bErase = true;
                filesInInclude = _findIncludeFiles(itrB->second);
                m_filesInInclude.insert(m_filesInInclude.end(), filesInInclude.begin(), filesInInclude.end());
            }
            else
            {
                bErase = true;
                bUnUse = true;
            }
        }

        if(bUnUse)
        {
            printf("WARN: Find unused file[%s%s]\n", itrB->first.c_str(), itrB->second.c_str());
        }

        if(bErase)
        {
            itrB = m_filesInDir.erase(itrB);
        }
        else
        {
            ++itrB;
        }
    }

    bool bStop = false;
    while(!bStop)
    {
        bErase = false;
        itrB = m_filesInDir.begin();
        for(; itrB != m_filesInDir.end(); ++itrB)
        {
            if(_isHeadFile(itrB->second) && _isFileInInclude(itrB->second))
            {
                bErase = true;
                filesInInclude = _findIncludeFiles(itrB->second);
                m_filesInInclude.insert(m_filesInInclude.end(), filesInInclude.begin(), filesInInclude.end());
                break;
            }
        }

        if(bErase)
        {
            m_filesInDir.erase(itrB);
        }
        else
        {
            bStop = true;
            itrB = m_filesInDir.begin();
            for(; itrB != m_filesInDir.end(); ++itrB)
            {
                printf("WARN: Find unused file[%s%s]\n", itrB->first.c_str(), itrB->second.c_str());
            }
        }
    }
}

void CCodeChecker::_cutRepeatSemicolon(std::string& file)
{
    std::string outFile;
    std::string line;
    while(getLine(file, line))
    {
        _cutRepeatSemicolonInLine(line);
        outFile = outFile + line;
    }
    file = outFile;
}

void CCodeChecker::_cutRepeatSemicolonInLine(std::string& line)
{
    int npos = 0, npos1 = 0, npos2 = 0, npos3 = 0, npos4 = 0;
    for(;;)
    {
        npos = line.find(';', npos);
        if(npos == -1)
        {
            break;
        }

        npos1 = line.find(';', npos + 1);
        if(npos1 == -1)
        {
            break;
        }

        if(!isBlanks(line.substr(npos + 1, npos1 - npos - 1)))
        {
            npos = npos1;
            continue;
        }

        npos2 = line.rfind('(', npos);
        if(npos2 != -1)
        {
            npos3 = line.find(')', npos1);
            if(npos3 != -1)
            {
                npos4 = line.find(')', npos);
                if(npos4 != -1 && npos4 == npos3)
                {
                    npos4 = line.rfind('(', npos1);
                    if(npos4 != -1 && npos4 == npos2)
                    {
                        npos4 = line.rfind("for", npos2);
                        if(npos4 != -1 && isBlanks(line.substr(npos4 + 3, npos2 - npos4 - 3)))
                        {
                            npos = npos1;
                            continue;
                        }
                    }
                }
            }
        }

        line.erase(npos1, 1);
    }
}

void CCodeChecker::_cutHeadCommentLines(std::string& file, std::string& comment)
{
    comment.clear();
    bool bFindBrief = false;

    std::string line;
    while(getLine(file, line))
    {
        int ret = judgeLineType(line);
        switch(ret)
        {
        case 0:
            //! 不是注释
            {
                file = line + file;
            }
            break;
        case 1:
            //! //注释
            {
                bool bRet = false;

                if(!bFindBrief)
                {
                    if(line.find("@brief") != -1)
                    {
                        bFindBrief = true;
                    }
                }
                else
                {
                    if(line.find("///////////////////////////////////////////////") != -1)
                    {
                        bRet = true;
                    }
                }

                comment = comment + line;

                if(bRet)
                {
                    ret = 0;
                }
            }
            break;
        case 2:
            //! /*注释
            {
                file = line + file;
                if(!cutMultiLineComment(file, line))
                {
                    ret = 0;//! /* */不匹配,代码有问题
                }
                comment = comment + line;
            }
            break;
        case 3:
            //! 空行
            {
                comment = comment + line;
            }
            break;
        }

        if(ret == 0)
        {
            break;
        }
    }
}

bool CCodeChecker::_cutIfDefGuard(std::string& file)
{
    std::string line;
    std::string line1;
    std::string line2;
    if(!getLine(file, line))
    {
        return false;
    }

    int npos = 0;
    npos = line.find("#pragma once");
    if(npos != -1)
    {
        if(!getLine(file, line))
        {
            return false;
        }
    }

    npos = line.find("#ifndef");
    if(npos == -1)
    {
        file = line + file;
        return false;
    }

    if(!getLine(file, line1))
    {
        file = line + file;
        return false;
    }

    npos = line1.find("#define");
    if(npos == -1)
    {
        file = line + line1 + file;
        return false;
    }

    if(!getEndLine(file, line2))
    {
        file = line + line1 + file;
        return false;
    }

    npos = line2.find("#endif");
    if(npos == -1)
    {
        file = line + line1 + file + line2;
        return false;
    }

    return true;
}

void CCodeChecker::_checkInclude(std::string& file, const std::string& fileName)
{
    strList includeFiles;

    std::string outFile;
    std::string line;
    while(getLine(file, line))
    {
        _checkIncludeInLine(line, includeFiles);
        outFile = outFile + line;
    }
    file = outFile;

    m_fileIncludeFilesMap.insert(std::make_pair(fileName, includeFiles));
}

void CCodeChecker::_checkIncludeInLine(std::string& line, strList& includeFiles)
{
    if(line.empty())
    {
        return;
    }

    int npos = 0, npos1 = 0, npos2 = 0;
    npos = line.find("include");
    if(npos == -1)
    {
        return;
    }

    npos1 = npos;

    npos = line.rfind('#', npos);
    if(npos == -1)
    {
        return;
    }

    if(!isBlanks(line.substr(npos + 1, npos1 - npos - 1)))
    {
        return;
    }

    npos = npos1 + 7;
    npos1 = line.find('<', npos);
    npos2 = line.find('\"', npos);

    if(npos1 == -1 && npos2 == -1)
    {
        return;
    }

    char cl = 0, cr = 0;
    if(npos1 != -1 && npos2 != -1)
    {
        if(npos1 < npos2)
        {
            npos = npos1;
            cl = '<';
            cr = '>';
        }
        else
        {
            npos = npos2;
            cl = cr = '\"';
        }
    }
    else if(npos1 != -1)
    {
        npos = npos1;
        cl = '<';
        cr = '>';
    }
    else
    {
        npos = npos2;
        cl = cr = '\"';
    }

    npos1 = npos;

    npos2 = line.find(cr, npos1 + 1);
    if(npos2 == -1)
    {
        return;
    }

    std::string line1;
    std::string line2;
    std::string line3;
    line1 = line.substr(0, npos1);
    line2 = line.substr(npos1 + 1, npos2 - npos1 - 1);
    line3 = line.substr(npos2 + 1, line.length() - npos2 - 1);

    strList dirs;
    spliteDir(line2, dirs);

    strList lowerDirs = dirs;
    strList::iterator itr = lowerDirs.begin();
    for(; itr != lowerDirs.end(); ++itr)
    {
        std::transform(itr->begin(), itr->end(), itr->begin(), tolower);
    }

    bool bFindInLib = false;
    for(int i = 0; i < (int)lowerDirs.size(); ++i)
    {
        for(int j = 0; j < (int)m_lowerInLibNames.size(); ++j)
        {
            if(lowerDirs[i] == m_lowerInLibNames[j])
            {
                bFindInLib = true;
                dirs[i] = m_inLibNames[j];
                break;
            }
        }
    }

    if(bFindInLib)
    {
        cl = '<';
        cr = '>';
    }
    else
    {
        if(lowerDirs.size() > 1)
        {
            bFindInLib = true;

            for(int i = 0; i < (int)m_lowerFilesAndParentDirInDir.size(); ++i)
            {
                if(lowerDirs[lowerDirs.size()-2] == m_lowerFilesAndParentDirInDir[i].first &&
                    lowerDirs.back() == m_lowerFilesAndParentDirInDir[i].second)
                {
                    bFindInLib = false;
                    break;
                }
            }

            if(!bFindInLib)
            {
                cl = '\"';
                cr = '\"';
            }
        }

        for(int i = 0; i < (int)lowerDirs.size(); ++i)
        {
            for(int j = 0; j < (int)m_lowerFilesAndDirsInDir.size(); ++j)
            {
                if(lowerDirs[i] == m_lowerFilesAndDirsInDir[j])
                {
                    dirs[i] = m_filesAndDirsInDir[j];
                    break;
                }
            }
        }
    }

    combineDir(line2, dirs);

    line = line1 + cl + line2 + cr + line3;

    if(!dirs.empty())
    {
        includeFiles.push_back(dirs.back());
    }
}

void CCodeChecker::_checkNameSpace(std::string& file, const std::string& dir, const std::string& fileName)
{
    std::string replaceStr("$$$$$$$$");
    std::string nameSpaceMask("@@@@@@");

    strList comments;
    pumpComments(file, replaceStr, comments);

    int npos = 0, npos1 = 0, npos2 = 0;
    npos = file.find("namespace", npos);
    while(npos != -1)
    {
        if((npos == 0 || (file[npos - 1] == ' ' || file[npos - 1] == '\t' || file[npos - 1] == '\r' || file[npos - 1] == '\n'))
        && (npos < (int)file.length() - 1 && (file[npos + 9] == ' ' || file[npos + 9] == '\t' || file[npos + 9] == '\r' || file[npos + 9] == '\n')))
        {
            npos1 = file.rfind("using", npos);
            if(npos1 == -1 || !isBlanks(file.substr(npos1 + 5, npos - npos1 - 5)))
            {
                _checkOneNameSpace(file, npos + 9, nameSpaceMask, dir, fileName);
            }
        }

        npos = file.find("namespace", npos + 9);
    }

    putBackComments(file, replaceStr, comments);

    char c = 0;
    npos = 0;
    npos = file.find(nameSpaceMask, npos);
    while(npos != -1)
    {
        file.erase(npos, nameSpaceMask.length());

        //-----------------------------------------------------------------------------------------
        for(npos1 = npos; npos1 < (int)file.length(); ++npos1)
        {
            c = file[npos1];
            if(c == '/')
            {
                if(npos1 + 1 < (int)file.length())
                {
                    c = file[npos1 + 1];
                    if(c == '/')
                    {
                        npos1 = file.find("\r\n", npos1);
                        if(npos1 == -1)
                        {
                            npos1 = file.length();
                        }
                        break;
                    }
                    else if(c == '*')
                    {
                        npos2 = file.find("*/", npos1 + 2);
                        if(npos2 == -1)
                        {
                            break;
                        }
                        else
                        {
                            npos1 = npos2 + 2 - 1;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            else if(c != ' ' && c != '\t' && c != ';')
            {
                break;
            }
        }

        file.erase(npos, npos1 - npos);
        if(npos < (int)file.length())
        {
            c = file[npos];
            if(c != '\r' && c != '\n')
            {
                file.insert(npos, "\r\n");
            }
        }
        //-----------------------------------------------------------------------------------------

        npos = file.find(nameSpaceMask, npos);
    }
}

void CCodeChecker::_checkOneNameSpace(std::string& file, int pos, const std::string& mask, const std::string& dir, const std::string& fileName)
{
    int npos = 0;
    npos = file.find('{', pos);
    if(npos == -1)
    {
        return;
    }

    std::string tmp;
    std::string spaceName;
    tmp = file.substr(pos, npos - pos);
    getWord(tmp, spaceName);
    if(!spaceName.empty() && !_isLegalNameSpace(spaceName))
    {
        printf("WARN: Illegal namespace[%s] in file[%s%s]\n", spaceName.c_str(), dir.c_str(), fileName.c_str());
    }
    spaceName = "//" + spaceName + mask;

    ++npos;
    int leftBraceCnt = 1;

    for(; npos < (int)file.length(); ++npos)
    {
        if(file[npos] == '{')
        {
            ++leftBraceCnt;
        }
        else if(file[npos] == '}')
        {
            --leftBraceCnt;

            if(leftBraceCnt == 0)
            {
                ++npos;
                file.insert(npos, spaceName);
                break;
            }
        }
    }
}

void CCodeChecker::_shieldMacros(std::string& file)
{
    int npos = 0, npos1 = 0;
    std::string macro;
    std::string line;

    for(int i = 0; i < (int)m_shieldMacros.size(); ++i)
    {
        macro = m_shieldMacros[i];
        npos = 0;

        for(; ;)
        {
            npos = file.find(macro, npos);
            if(npos == -1)
            {
                break;
            }

            npos1 = file.rfind("\r\n", npos);
            if(npos1 == -1)
            {
                line = file.substr(0, npos);
            }
            else
            {
                line = file.substr(npos1 + 2, npos - npos1 - 2);
            }

            npos1 = line.rfind("define", line.length() - 1);
            if(npos1 != -1)
            {
                line.erase(0, npos1 + 6);
                if(!line.empty() && isBlanks(line))
                {
                    ++npos;
                    continue;
                }
            }

            file.insert(npos, "//");
            npos += 2;
            npos += macro.length();
        }
    }
}

void CCodeChecker::_unShieldMacros(std::string& file)
{
    int npos = 0;
    std::string macro;

    for(int i = 0; i < (int)m_shieldMacros.size(); ++i)
    {
        macro = "//" + m_shieldMacros[i];
        npos = 0;

        for(; ;)
        {
            npos = file.find(macro, npos);
            if(npos == -1)
            {
                break;
            }

            file.erase(npos, 2);
            npos += macro.length() - 2;
        }
    }
}

void CCodeChecker::_procLeftAlignWords(std::string& file)
{
    int npos = 0, npos1 = 0;
    std::string word;

    for(int i = 0; i < (int)m_leftAlignWords.size(); ++i)
    {
        word = m_leftAlignWords[i];
        npos = 0;

        for(; ;)
        {
            npos = file.find(word, npos);
            if(npos == -1)
            {
                break;
            }

            npos1 = file.rfind("\r\n", npos);
            if(npos1 == -1)
            {
                npos1 = 0;
            }
            else
            {
                npos1 += 2;
            }

            if(isBlanks(file.substr(npos1, npos - npos1)))
            {
                file.erase(npos1, npos - npos1);
                npos = npos1;
            }

            ++npos;
        }
    }
}

void CCodeChecker::_procWordIndents(std::string& file)
{
    std::string word;
    int indent = 0;
    strList excludeWords;
    wordIndentMap::iterator itrB;
    wordIndentMap::iterator itrE = m_wordIndentMap.end();
    wordIndentMap::iterator itrB1;
    wordIndentMap::iterator itrE1 = m_wordIndentMap.end();

    for(itrB = m_wordIndentMap.begin(); itrB != itrE; ++itrB)
    {
        word = itrB->first;
        indent = itrB->second;
        excludeWords.clear();

        for(itrB1 = m_wordIndentMap.begin(); itrB1 != itrE1; ++itrB1)
        {
            if(word != itrB1->first)
            {
                if(itrB1->first.find(word) == 0)
                {
                    excludeWords.push_back(itrB1->first);
                }
            }
        }

        _procOneWordIndent(file, word, indent, excludeWords);
    }
}

void CCodeChecker::_procOneWordIndent(std::string& file, const std::string& word, int indent, const strList& excludeWords)
{
    if(indent <= 0)
    {
        return;
    }

    int npos = 0, npos1 = 0;
    std::string blanks;
    bool bContinue = false;

    for(int i = 0; i < indent; ++i)
    {
        blanks = blanks + ' ';
    }

    for(; ;)
    {
        npos = file.find(word, npos);
        if(npos == -1)
        {
            break;
        }

        bContinue = false;
        for(int i = 0; i < (int)excludeWords.size(); ++i)
        {
            npos1 = file.find(excludeWords[i], npos);
            if(npos == npos1)
            {
                bContinue = true;
                break;
            }
        }

        if(bContinue)
        {
            ++npos;
            continue;
        }

        npos1 = file.rfind("\r\n", npos);
        if(npos1 == -1)
        {
            npos1 = 0;
        }
        else
        {
            npos1 += 2;
        }

        if(isBlanks(file.substr(npos1, npos - npos1)))
        {
            file.insert(npos, blanks);
            npos += indent + 1;
        }
        else
        {
            ++npos;
        }
    }
}

bool CCodeChecker::_parseComment(const std::string& comment, int& year, int& month, int& day, std::string& author, strList& briefs)
{
    bool bFindTime = false;
    bool bFindAuthor = false;
    bool bFindBrief = false;

    year = month = day = 0;
    author.clear();
    briefs.clear();

    std::string commentCpy = comment;
    std::string line;
    std::string word;
    int npos = 0;
    while(getLine(commentCpy, line))
    {
        npos = line.find("created:");
        if(npos != -1)
        {
            bFindTime = true;
            line.erase(0, npos + 8);
            getWord(line, word);
            sscanf(word.c_str(), "%d/%d/%d", &year, &month, &day);
            continue;
        }

        npos = line.find("author:");
        if(npos != -1)
        {
            bFindAuthor = true;
            line.erase(0, npos + 7);
            getWord(line, word);
            author = word;
            continue;
        }

        npos = line.find("@brief");
        if(npos != -1)
        {
            bFindBrief = true;
            line.erase(0, npos);
            do
            {
                if(getWord(line, word))
                {
                    cutLineHeadEndBlanks(line);
                    if(line.length() > 1 && line.substr(line.length() - 2, 2) == "\r\n")
                    {
                        line.erase(line.length() - 2, 2);
                    }
                    if(!line.empty())
                    {
                        briefs.push_back(line);
                    }
                }
            }
            while(getLine(commentCpy, line));
            break;
        }
    }

    if(!bFindTime || !bFindAuthor || !bFindBrief)
    {
        return false;
    }
    return true;
}

void CCodeChecker::_addIfDefGuard(std::string& file, const std::string& dir, const std::string& fileName, int rootLength)
{
    char buf[2048] =
    {
        0
    };
    std::string macroName = dir + fileName;
    int npos = dir.find('/', rootLength);
    if(npos != -1)
    {
        macroName = dir.substr(npos + 1, dir.length() - npos - 1) + fileName;
    }

    for(npos = 0; npos < (int)macroName.length(); ++npos)
    {
        if(macroName[npos] == '/' || macroName[npos] == '.')
        {
            macroName[npos] = '_';
        }
    }

    std::transform(macroName.begin(), macroName.end(), macroName.begin(), toupper);

    sprintf(buf, "#ifndef __%s_%s__\r\n#define __%s_%s__\r\n", m_prefix.c_str(), macroName.c_str(), m_prefix.c_str(), macroName.c_str());

    file = buf + file;

    if(file.substr(file.length() - 2, 2) != "\r\n")
    {
        file = file + "\r\n";
    }

    sprintf(buf, "#endif//__%s_%s__\r\n", m_prefix.c_str(), macroName.c_str());

    file = file + buf;
}

void CCodeChecker::_addComment(std::string& file,
                               const std::string& dir,
                               const std::string& fileName,
                               int rootLength,
                               int year,
                               int month,
                               int day,
                               std::string& author,
                               strList& briefs)
{
    char buf[1024] =
    {
        0
    };
    int npos = 0;
    std::string fullName;

    sprintf(buf, "//////////////////////////////////////////////////////////////////////\r\n");
    file = buf + file;
    sprintf(buf, "///\r\n");
    file = buf + file;

    //------------------------------------------------------------------------------------------------------------------//

    if(briefs.empty())
    {
        sprintf(buf, "/// @brief\r\n");
        file = buf + file;
    }
    else
    {
        int idx = briefs.size() - 1;
        for(; idx >= 0; --idx)
        {
            if(idx == 0)
            {
                sprintf(buf, "/// @brief      %s\r\n", briefs[idx].c_str());
            }
            else
            {
                sprintf(buf, "///             %s\r\n", briefs[idx].c_str());
            }
            file = buf + file;
        }
    }

    //------------------------------------------------------------------------------------------------------------------//

    if(_isValidAuthor(author))
    {
        sprintf(buf, "//  author:     %s\r\n", author.c_str());
    }
    else
    {
        sprintf(buf, "//  author:     %s\r\n", m_defaultAuthor.c_str());
    }
    file = buf + file;

    //------------------------------------------------------------------------------------------------------------------//

    fullName = dir + fileName;
    fullName.erase(0, rootLength);
    for(; npos < (int)fullName.length(); ++npos)
    {
        if(fullName[npos] == '\\')
        {
            fullName[npos] = '/';
        }
    }
    sprintf(buf, "//  filename:   %s\r\n", fullName.c_str());
    file = buf + file;

    //------------------------------------------------------------------------------------------------------------------//

    if(m_dateMode == 0)//! 全部替换
    {
        year = m_year;
        month = m_month;
        day = m_day;
    }
    else//! 比较大小,小的替换
    {
        if(year < m_year)
        {
            year = m_year;
            month = m_month;
            day = m_day;
        }
        else if(year == m_year)
        {
            if(month < m_month)
            {
                month = m_month;
                day = m_day;
            }
            else if(month == m_month)
            {
                if(day < m_day)
                {
                    day = m_day;
                }
            }
        }
    }
    sprintf(buf, "//  created:    %d/%02d/%02d\r\n", year, month, day);
    file = buf + file;

    //------------------------------------------------------------------------------------------------------------------//

    sprintf(buf, "//////////////////////////////////////////////////////////////////////\r\n");
    file = buf + file;
}

bool CCodeChecker::_isExcludeDir(const std::string& dir)
{
    std::string lowerDir = dir;
    std::transform(lowerDir.begin(), lowerDir.end(), lowerDir.begin(), tolower);

    strList::iterator itr = m_excludeDirs.begin();
    for(; itr != m_excludeDirs.end(); ++itr)
    {
        if(lowerDir == *itr)
        {
            return true;
        }
    }
    return false;
}

bool CCodeChecker::_isExcludeFile(const std::string& fileName)
{
    std::string lowerFileName = fileName;
    std::transform(lowerFileName.begin(), lowerFileName.end(), lowerFileName.begin(), tolower);

    strList::iterator itr = m_excludeFiles.begin();
    for(; itr != m_excludeFiles.end(); ++itr)
    {
        if(lowerFileName == *itr)
        {
            return true;
        }
    }
    return false;
}

bool CCodeChecker::_isHeadFile(const std::string& fileName)
{
    std::string lowFileName = fileName;
    std::transform(lowFileName.begin(), lowFileName.end(), lowFileName.begin(), tolower);
    int nameLen = lowFileName.length();
    int extLen = 0;

    strList::iterator itr = m_headFileExts.begin();
    for(; itr != m_headFileExts.end(); ++itr)
    {
        extLen = (*itr).length();
        if(nameLen > extLen && lowFileName.substr(nameLen - extLen, extLen) == *itr)
        {
            return true;
        }
    }
    return false;
}

bool CCodeChecker::_isCppFile(const std::string& fileName)
{
    std::string lowFileName = fileName;
    std::transform(lowFileName.begin(), lowFileName.end(), lowFileName.begin(), tolower);
    int nameLen = lowFileName.length();
    int extLen = 0;

    strList::iterator itr = m_cppFileExts.begin();
    for(; itr != m_cppFileExts.end(); ++itr)
    {
        extLen = (*itr).length();
        if(nameLen > extLen && lowFileName.substr(nameLen - extLen, extLen) == *itr)
        {
            return true;
        }
    }
    return false;
}

bool CCodeChecker::_isProjFile(const std::string& fileName)
{
    std::string lowFileName = fileName;
    std::transform(lowFileName.begin(), lowFileName.end(), lowFileName.begin(), tolower);
    int nameLen = lowFileName.length();
    std::string ext(".vcproj");
    int extLen = ext.length();

    if(nameLen > extLen && lowFileName.substr(nameLen - extLen, extLen) == ext)
    {
        return true;
    }
    return false;
}

bool CCodeChecker::_isValidAuthor(const std::string& author)
{
    strList::iterator itr = m_validAuthorVec.begin();
    for(; itr != m_validAuthorVec.end(); ++itr)
    {
        if(author == *itr)
        {
            return true;
        }
    }
    return false;
}

bool CCodeChecker::_isLegalNameSpace(const std::string& nameSpace)
{
    strList::iterator itr = m_legalNameSpaces.begin();
    for(; itr != m_legalNameSpaces.end(); ++itr)
    {
        if(nameSpace == *itr)
        {
            return true;
        }
    }
    return false;
}

bool CCodeChecker::_isFileInProj(const std::string& fileName)
{
    strList::iterator itr = m_filesInProj.begin();
    for(; itr != m_filesInProj.end(); ++itr)
    {
        if(fileName == *itr)
        {
            return true;
        }
    }
    return false;
}

bool CCodeChecker::_isFileInInclude(const std::string& fileName)
{
    strList::iterator itr = m_filesInInclude.begin();
    for(; itr != m_filesInInclude.end(); ++itr)
    {
        if(fileName == *itr)
        {
            return true;
        }
    }
    return false;
}

strList CCodeChecker::_findIncludeFiles(const std::string& fileName)
{
    fileIncludeFilesMap::iterator itr = m_fileIncludeFilesMap.find(fileName);
    if(itr == m_fileIncludeFilesMap.end())
    {
        return strList();
    }
    else
    {
        return itr->second;
    }
}
