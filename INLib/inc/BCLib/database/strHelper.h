//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/database/strHelper.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_DATABASE_STRHELPER_H__
#define __BCLIB_DATABASE_STRHELPER_H__

#include <BCLib/database/baseDef.h>

namespace BCLib
{
namespace Database
{

class BCLIB_DATABASE_API CStrHelper
{
public:
	/// @brief 处理字符串中数据库无法处理的特殊字符
	/// @return std::string
	/// @param strValue
    static std::string escapeString(const std::string& strValue);
};

}//Database
}//BCLib

#endif//__BCLIB_DATABASE_STRHELPER_H__
