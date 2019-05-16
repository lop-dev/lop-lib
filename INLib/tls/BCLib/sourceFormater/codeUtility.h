//////////////////////////////////////////////////////////////////////
//  created:    2012/08/27
//  filename:   CodeChecker\codeUtility.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __CODE_UTILITY_H__
#define __CODE_UTILITY_H__

#include <string>
#include <vector>
#include <map>

typedef std::vector<std::string> strList;

typedef std::map<std::string, strList > cfgMap;
bool readCfg(const std::string& fullFileName, cfgMap& map);

bool readFile(std::string& file, const std::string& fullFileName);
bool writeFile(const std::string& file, const std::string& fullFileName);

void pumpComments(std::string& file, const std::string& replaceStr, strList& comments);
void putBackComments(std::string& file, const std::string& replaceStr, strList& comments);

bool getLine(std::string& file, std::string& line);
bool getEndLine(std::string& file, std::string& line);
bool getWord(std::string& line, std::string& word);
void cutAllLineEndBlanks(std::string& file);
void cutFileEndBlankLines(std::string& file);
void cutLineHeadEndBlanks(std::string& line);
void cutLineEndBlanks(std::string& line);
bool isBlanks(const std::string& line);
int judgeLineType(const std::string& line);//! 0:²»ÊÇ×¢ÊÍ,1://×¢ÊÍ,2:/*×¢ÊÍ,3:¿ÕÐÐ
bool cutMultiLineComment(std::string& file, std::string& comment);

void spliteDir(const std::string& line, strList& dirs);
void combineDir(std::string& line, const strList& dirs);


#endif//__CODE_UTILITY_H__
