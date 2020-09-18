//////////////////////////////////////////////////////////////////////
//  created:    2015/07/16
//  filename:   BCLib/utility/io/csvExcel.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SHLIB_UTILITY_CSVEXCEL_H__
#define __SHLIB_UTILITY_CSVEXCEL_H__

#include <BCLib/utility/io/excelCell.h>

namespace BCLib
{
namespace Utility
{
class CCsvExcelBook;

class BCLIB_UTILITY_API CCsvExcel : public BCLib::Utility::CNoncopyable
{
public:
	CCsvExcel();
	~CCsvExcel();

	bool loadFile(const BCLib::CString& fileName);
	bool loadString(const BCLib::CString& csvString);
	void clear();

	CExcelTable* getTable(const std::string& tableName) const;
	CExcelTable* getTable(int tableNum) const;

	const BCLib::CString& getTableName() const;

private:
	bool _loadFromFile();
	bool _loadFromString(const BCLib::CString& csvString);

private:
	BCLib::CString m_fileName;
	CCsvExcelBook* m_excelBook;
};
}//Utility
}//BCLib
#endif//__SHLIB_UTILITY_CSVEXCEL_H__
