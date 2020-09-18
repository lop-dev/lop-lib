//////////////////////////////////////////////////////////////////////
//  created:    2015/07/16
//  filename:   BCLib/utility/io/csvReader.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __SHLIB_UTILITY_CSVREADER_H__
#define __SHLIB_UTILITY_CSVREADER_H__

#include <BCLib/utility/baseDec.h>
#include <BCLib/utility/string.h>

namespace BCLib
{
namespace Utility
{
typedef std::vector<BCLib::CString> ColList;
class CColList
{
public:
	int Size()
	{
		return m_colList.size();
	}

	BCLib::CString& operator[](int i)
	{
		return m_colList[i];
	}

	void Add(const BCLib::CString& data)
	{
		m_colList.push_back(data);
	}
private:
	ColList m_colList;
};

typedef std::vector<CColList> CRowList;
class CCsvReader
{
public:
	CCsvReader(void);

	~CCsvReader(void);

	CCsvReader(BCLib::CString fileName);

	const BCLib::CString& GetFileName();

	void SetFilename(const BCLib::CString& value);

	/// @brief 获取行数
	int GetRowCount();

	/// @brief 获取列数
	int GetColCount();

	/// @brief 获取某行某列的数据
	/// @param row 行数, =1 第一行
	/// @param col 列数, =1 第一列
	const BCLib::CString& GetCellData(int row, int col);

	/// @brief 获取某行的数据
	/// @return row 行数, = 1 第一行
	const CColList& GetRowData(int row);

	/// @brief 读取文件
	virtual bool Load(const BCLib::CString& fileName);

	/// @brief 读取Csv字符串
	virtual bool LoadCsv(const BCLib::CString& csv);

private:
	/// @brief 检查行数是否有效sss
	void _checkRowValid(int row);
	/// @brief 检查列数是否有效
	void _checkColValid(int col);
	/// @brief 检查最大行数是否有效
	void _checkMaxColValid(int maxCol);
	/// @brief 加载CSV文件
	bool _loadCsvFile(std::basic_istream<char>& stream);

	/// @brief 判断字符串是否包含奇数个引号
	/// @param dataLine 数据行
	/// @return 为奇数时，返回true，否则返回false
	bool _ifOddQuota(const BCLib::CString& dataLine);
	/// @brief 判断是否以奇数个引号开始
	/// @param dataCell 数据格
	/// @return 为奇数时，返回true，否则返回false
	bool _ifOddStartQuota(const BCLib::CString& dataCell);
	/// @brief 判断是否以奇数个引号结尾
	/// @param dataCell 数据格
	/// @return 为奇数时，返回true，否则返回false
	bool _ifOddEndQuota(const BCLib::CString& dataCell);
	/// @brief 加入新的数据行
	/// @param newDataLine 新数据行
	void _addNewDataLine(const BCLib::CString& newDataLine);
	/// @brief 去掉格子的首尾引号，把双引号变成单引号
	/// @param fileCellData 数据格
	/// @return 变成单引号的数据
	BCLib::CString _getHandleData(const BCLib::CString& fileCellData);

private:
	CRowList m_rowList;		// 行链表，CSV文件的每一行
	BCLib::CString m_fileName;

	static BCLib::CString ms_seperator;
	static BCLib::CString ms_quota;
	static BCLib::CString ms_doubleQuota;
};
}//Utility
}//BCLib

#endif//__SHLIB_UTILITY_CSVREADER_H__
