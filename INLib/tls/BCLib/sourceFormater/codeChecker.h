//////////////////////////////////////////////////////////////////////
//  created:    2012/08/14
//  filename:   CodeChecker\codeChecker.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __CODE_CHECKER_H__
#define __CODE_CHECKER_H__

#include <string>
#include <vector>
#include <map>
#include "codeUtility.h"

/**
    1.遍历指定目录的所有子目录,*.h/*.cpp文件
    2.*.h文件处理
        读入整个文件
        按行解析
        去除所有行末的空白字符
        取出文件头部的所有注释行
        解析文件头部非注释第一行,如果是#pragma once,去除
        解析文件头部非注释第一行,如果是#ifndef,去除对应的#define/#endif
        检查#include
        检查namespace{}
        解析取出的文件头部注释行,识别"时间/作者/注释"
        添加新的#ifndef/#define/#endif
        添加新文件头注释
        保存文件

    3.*.cpp文件处理
        读入整个文件
        按行解析
        去除所有行末的空白字符
        取出文件头部的所有注释行
        检查#include
        检查namespace{}
        解析取出的文件头部注释行,识别"时间/作者/注释"
        添加新文件头注释
        保存文件

**/

class CCodeChecker
{
public:
    CCodeChecker();
    ~CCodeChecker();

    bool initCfg(const std::string& cfgName);
    bool check(int checkMode);//! 0 : 检查代码, 1 : 屏蔽宏,为SourceStyler预处理, 2 : 解除宏屏蔽,为SourceStyler后处理

protected:
    bool _checkOneDir(const std::string& dir, int checkMode);
    void _traverseDir(const std::string& dir, int rootLength, int checkMode);
    bool _preSourceStyler(const std::string& dir, const std::string& fileName);
    bool _postSourceStyler(const std::string& dir, const std::string& fileName);
    bool _parseProjFile(const std::string& dir, const std::string& fileName);
    bool _checkHeadFile(const std::string& dir, const std::string& fileName, int rootLength);
    bool _checkCppFile(const std::string& dir, const std::string& fileName, int rootLength);
    void _scanUnUsedFiles();
    void _cutRepeatSemicolon(std::string& file);
    void _cutRepeatSemicolonInLine(std::string& line);
    void _cutHeadCommentLines(std::string& file, std::string& comment);
    bool _cutIfDefGuard(std::string& file);
    void _checkInclude(std::string& file, const std::string& fileName);
    void _checkIncludeInLine(std::string& line, strList& includeFiles);
    void _checkNameSpace(std::string& file, const std::string& dir, const std::string& fileName);
    void _checkOneNameSpace(std::string& file, int pos, const std::string& mask, const std::string& dir, const std::string& fileName);
    void _shieldMacros(std::string& file);
    void _unShieldMacros(std::string& file);
    void _procLeftAlignWords(std::string& file);
    void _procWordIndents(std::string& file);
    void _procOneWordIndent(std::string& file, const std::string& word, int indent, const strList& excludeWords);
    bool _parseComment(const std::string& comment, int& year, int& month, int& day, std::string& author, strList& briefs);
    void _addIfDefGuard(std::string& file, const std::string& dir, const std::string& fileName, int rootLength);
    void _addComment(std::string& file, const std::string& dir, const std::string& fileName, int rootLength, int year, int month, int day, std::string& author, strList& briefs);
    bool _isExcludeDir(const std::string& dir);
    bool _isExcludeFile(const std::string& fileName);
    bool _isHeadFile(const std::string& fileName);
    bool _isCppFile(const std::string& fileName);
    bool _isProjFile(const std::string& fileName);
    bool _isValidAuthor(const std::string& author);
    bool _isLegalNameSpace(const std::string& nameSpace);
    bool _isFileInProj(const std::string& fileName);
    bool _isFileInInclude(const std::string& fileName);
    strList _findIncludeFiles(const std::string& fileName);

protected:
    strList m_checkDirs;
    strList m_excludeDirs;      //! 屏蔽的目录
    strList m_excludeFiles;     //! 屏蔽的文件
    strList m_headFileExts;
    strList m_cppFileExts;
    strList m_validAuthorVec;
    std::string m_defaultAuthor;
    int m_year;
    int m_month;
    int m_day;
    int m_dateMode;             //! mode:0全部替换,1小的替换,大的保留
    std::string m_prefix;
    strList m_inLibNames;
    strList m_lowerInLibNames;
    strList m_shieldMacros;
    strList m_leftAlignWords;   //! 需要左对齐的单词
    strList m_legalNameSpaces;
    typedef std::map<std::string, int> wordIndentMap;
    wordIndentMap m_wordIndentMap;
    strList m_filesInProj;
    strList m_filesInInclude;
    typedef std::map<std::string, strList> fileIncludeFilesMap;
    fileIncludeFilesMap m_fileIncludeFilesMap;
    typedef std::vector<std::pair<std::string, std::string>> filesInDir;
    filesInDir m_filesInDir;
    strList m_filesAndDirsInDir;
    strList m_lowerFilesAndDirsInDir;
    filesInDir m_lowerFilesAndParentDirInDir;
};


#endif//__CODE_CHECKER_H__