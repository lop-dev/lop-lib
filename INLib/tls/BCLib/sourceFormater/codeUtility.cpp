//////////////////////////////////////////////////////////////////////
//  created:    2012/08/27
//  filename:   CodeChecker\codeUtility.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include "codeUtility.h"


bool readCfg(const std::string& fullFileName, cfgMap& map)
{
    map.clear();
    std::string file;
    if(!readFile(file, fullFileName))
    {
        return false;
    }

    strList comments;
    pumpComments(file, "", comments);

    std::string line;
    std::string tmp;
    std::string key;
    std::string val;
    int npos = 0;
    cfgMap::iterator itr = map.end();
    while(getLine(file, line))
    {
        npos = line.find('=');
        if(npos == -1)
        {
            continue;
        }

        tmp = line.substr(0, npos);
        if(!getWord(tmp, key))
        {
            continue;
        }

        tmp = line.substr(npos + 1, line.length() - npos - 1);
        if(!getWord(tmp, val))
        {
            continue;
        }

        itr = map.find(key);
        if(itr == map.end())
        {
            cfgMap::_Pairib ret = map.insert(std::make_pair(key, strList()));
            if(ret.second)
            {
                ret.first->second.push_back(val);
            }
        }
        else
        {
            itr->second.push_back(val);
        }
    }
    return true;
}

bool readFile(std::string& file, const std::string& fullFileName)
{
    file.clear();
    FILE* pFile = NULL;
    int fileSize = 0;
    char* pBuf = NULL;

    pFile = fopen(fullFileName.c_str(), "rb");
    if(!pFile)
    {
        return false;
    }

    fseek(pFile, 0, SEEK_END);
    fileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
    pBuf = new char[fileSize + 1];
    fread(pBuf, fileSize, 1, pFile);
    fclose(pFile);
    pBuf[fileSize] = 0;
    file = pBuf;
    return true;
}

bool writeFile(const std::string& file, const std::string& fullFileName)
{
    FILE* pFile = NULL;

    pFile = fopen(fullFileName.c_str(), "wb");
    if(!pFile)
    {
        return false;
    }

    fwrite(file.c_str(), file.length(), 1, pFile);
    fclose(pFile);
    return true;
}

void pumpComments(std::string& file, const std::string& replaceStr, strList& comments)
{
    comments.clear();
    std::string comment;
    int npos = 0, npos1 = 0, npos2 = 0;

    for(; ;)
    {
        npos1 = file.find("//");
        npos2 = file.find("/*");
        if(npos1 == -1 && npos2 == -1)
        {
            break;
        }

        bool bLineComment = false;
        if(npos1 != -1)
        {
            if(npos2 != -1)
            {
                if(npos1 < npos2)
                {
                    //   //
                    bLineComment = true;
                }
                else
                {
                    //   /*
                    bLineComment = false;
                }
            }
            else
            {
                //   //
                bLineComment = true;
            }
        }
        else
        {
            //   /*
            bLineComment = false;
        }

        if(bLineComment)
        {
            npos = file.find("\r\n", npos1);
            if(npos != -1)
            {
                comment = file.substr(npos1, npos - npos1);
                comments.push_back(comment);
                file = file.substr(0, npos1) + replaceStr + file.substr(npos, file.length() - npos);
            }
            else
            {
                comment = file.substr(npos1, file.length() - npos1);
                comments.push_back(comment);
                file = file.substr(0, npos1) + replaceStr;
            }
        }
        else
        {
            npos = file.find("*/", npos2 + 2);
            if(npos != -1)
            {
                comment = file.substr(npos2, npos + 2 - npos2);
                comments.push_back(comment);
                file = file.substr(0, npos2) + replaceStr + file.substr(npos + 2, file.length() - npos - 2);
            }
            else
            {
                //! 代码有错误
                break;
            }
        }
    }
}

void putBackComments(std::string& file, const std::string& replaceStr, strList& comments)
{
    int npos = 0;
    int commentIdx = 0;
    int replaceLen = replaceStr.length();

    for(; ;)
    {
        npos = file.find(replaceStr);
        if(npos == -1)
        {
            break;
        }

        if(commentIdx >= (int)comments.size())
        {
            break;
        }

        file = file.substr(0, npos) + comments[commentIdx] + file.substr(npos + replaceLen, file.length() - npos - replaceLen);
        ++commentIdx;
    }
}

bool getLine(std::string& file, std::string& line)
{
    line.clear();
    if(file.empty())
    {
        return false;
    }

    int npos = file.find("\r\n");
    if(npos != -1)
    {
        line = file.substr(0, npos + 2);
        file.erase(0, npos + 2);
    }
    else
    {
        line = file;
        file.clear();
    }
    return true;
}

bool getEndLine(std::string& file, std::string& line)
{
    line.clear();
    if(file.empty())
    {
        return false;
    }

    int npos = file.rfind("\r\n");
    if(npos != -1)
    {
        if(npos + 2 < (int)file.length())
        {
            line = file.substr(npos + 2, file.length() - (npos + 2));
            file.erase(npos + 2, file.length() - (npos + 2));
        }
        else
        {
            npos = file.rfind("\r\n", npos - 1);
            if(npos != -1)
            {
                line = file.substr(npos + 2, file.length() - (npos + 2));
                file.erase(npos + 2, file.length() - (npos + 2));
            }
            else
            {
                line = file;
                file.clear();
            }
        }
    }
    else
    {
        line = file;
        file.clear();
    }
    return true;
}

bool getWord(std::string& line, std::string& word)
{
    word.clear();
    if(line.empty())
    {
        return false;
    }

    int npos = 0, npos1 = 0;
    for(; npos < (int)line.length(); ++npos)
    {
        if(line[npos] != ' ' && line[npos] != '\t' && line[npos] != '\r' && line[npos] != '\n')
        {
            break;
        }
    }

    if(npos >= (int)line.length())
    {
        return false;
    }

    npos1 = npos + 1;
    for(; npos1 < (int)line.length(); ++npos1)
    {
        //if(line[npos1] == ' ' || line[npos1] == '\t' || line[npos1] == '\r' || line[npos1] == '\n')
        //{
        //    break;
        //}

		// 为了支持“League of Perfect”，去掉了空格的检查
		if(line[npos1] == '\t' || line[npos1] == '\r' || line[npos1] == '\n')
		{
			break;
		}
    }

    word = line.substr(npos, npos1 - npos);
    line.erase(0, npos1);
    return true;
}

void cutAllLineEndBlanks(std::string& file)
{
    std::string outFile;
    std::string line;
    while(getLine(file, line))
    {
        cutLineEndBlanks(line);
        outFile = outFile + line;
    }
    file = outFile;
}

void cutFileEndBlankLines(std::string& file)
{
    std::string line;
    while(getEndLine(file, line))
    {
        int ret = judgeLineType(line);
        if(ret != 3)
        {
            file = file + line;
            break;
        }
    }
}

void cutLineHeadEndBlanks(std::string& line)
{
    if(line.empty())
    {
        return;
    }

    int npos = 0;
    for(; npos < (int)line.length(); ++npos)
    {
        if(line[npos] != ' ' && line[npos] != '\t')
        {
            break;
        }
    }

    line.erase(0, npos);

    if(line.empty())
    {
        return;
    }

    bool bHasReturn = false;
    if(line.length() > 1 && line.substr(line.length() - 2, 2) == "\r\n")
    {
        bHasReturn = true;

        if(line.length() == 2)
        {
            return;
        }

        line.erase(line.length() - 2, 2);
    }

    npos = line.length() - 1;
    for(; npos >= 0; --npos)
    {
        if(line[npos] != ' ' && line[npos] != '\t')
        {
            break;
        }
    }

    ++npos;
    line.erase(npos, line.length() - npos);

    if(bHasReturn)
    {
        line = line + "\r\n";
    }
}

void cutLineEndBlanks(std::string& line)
{
    if(line.empty())
    {
        return;
    }

    int npos = 0;
    bool bHasReturn = false;
    if(line.length() > 1 && line.substr(line.length() - 2, 2) == "\r\n")
    {
        bHasReturn = true;

        if(line.length() == 2)
        {
            return;
        }

        line.erase(line.length() - 2, 2);
    }

    npos = line.length() - 1;
    for(; npos >= 0; --npos)
    {
        if(line[npos] != ' ' && line[npos] != '\t')
        {
            break;
        }
    }

    ++npos;
    line.erase(npos, line.length() - npos);

    if(bHasReturn)
    {
        line = line + "\r\n";
    }
}

bool isBlanks(const std::string& line)
{
    if(line.empty())
    {
        return true;
    }

    int npos = 0;
    for(; npos < (int)line.length(); ++npos)
    {
        if(line[npos] != ' ' && line[npos] != '\t')
        {
            return false;
        }
    }
    return true;
}

//! 0:不是注释,1://注释,2:/*注释,3:空行
int judgeLineType(const std::string& line)
{
    int npos = 0;
    for(; npos < (int)line.length(); ++npos)
    {
        if(line[npos] != ' ' && line[npos] != '\t')
        {
            break;
        }
    }

    if(npos >= (int)line.length())
    {
        return 3;
    }

    std::string str = line.substr(npos, line.length() - npos);
    if(str.length() < 2)
    {
        return 0;
    }

    str.erase(2, str.length() - 2);
    if(str == "//")
    {
        return 1;
    }
    else if(str == "/*")
    {
        return 2;
    }
    else if(str == "\r\n")
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

bool cutMultiLineComment(std::string& file, std::string& comment)
{
    comment.clear();
    int npos = file.find("*/");
    if(npos == -1)
    {
        return false;
    }

    comment = file.substr(0, npos + 2);
    file.erase(0, npos + 2);
    return true;
}

void spliteDir(const std::string& line, strList& dirs)
{
    dirs.clear();
    if(line.empty())
    {
        return;
    }

    std::string lineCpy = line;

    int npos = 0, npos1 = 0;
    for(; npos < (int)lineCpy.length(); ++npos)
    {
        if(lineCpy[npos] == '\\')
        {
            lineCpy[npos] = '/';
        }
    }

    npos = 0;
    npos1 = lineCpy.find('/', npos);
    while(npos1 != -1)
    {
        if(npos < npos1)
        {
            dirs.push_back(lineCpy.substr(npos, npos1 - npos));
        }

        npos = npos1 + 1;
        npos1 = lineCpy.find('/', npos);
    }

    if(npos < (int)lineCpy.length())
    {
        dirs.push_back(lineCpy.substr(npos, lineCpy.length() - npos));
    }
}

void combineDir(std::string& line, const strList& dirs)
{
    line.clear();

    for(int i = 0; i < (int)dirs.size(); ++i)
    {
        if(i == 0)
        {
            line = dirs[i];
        }
        else
        {
            line = line + '/' + dirs[i];
        }
    }
}
