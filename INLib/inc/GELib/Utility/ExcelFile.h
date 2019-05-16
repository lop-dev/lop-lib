//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/ExcelFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_EXCELFILE_H__
#define __GELIB_UTILITY_EXCELFILE_H__

#include <vector>
#include <string>
#include <GELib/GeConfig.h>

namespace pugi
{
	class xml_document;
}//pugi

namespace GELib
{


class CExcelTable
{
public:
	CExcelTable();
	virtual ~CExcelTable();

	void Clear();

	void SetName(const char *name) { m_Name = name; }
	const std::string& GetName() { return m_Name; }

	int GetNumRows();
	int GetNumColumns();
	int GetRowIndex(int column, const char *text);			// return -1 if not found
	int GetColumnIndex(int row, const char *text);		// return -1 if not found

	void SetCell(int row, int column, const char *txt);

	const char * GetCell(int row, int column);			// return null string if no data
	int GetCellInt(int row, int column);
	float GetCellFloat(int row, int column);

	bool GetCell(int row, int column, std::string &value);	// return false if no data, value unchanged
	bool GetCell(int row, int column, int &value);
	bool GetCell(int row, int column, short &value);
	bool GetCell(int row, int column, float &value);
	bool GetCell(int row, int column, double &value);
	bool GetCell(int row, int column, bool &value);

private:

	int m_iNumColumns;
	std::string m_Name;
	typedef std::vector<const char *> Row;
	std::vector<Row> m_Rows;
};

class CExcelFile
{
public:

    CExcelFile();
    virtual ~CExcelFile();

    bool Load(const char *fileName);	// only support xml excel file
    bool Load(const void *fileInMemory, int fileSize);
	bool Save(const char *fileName);
    void Unload();

	CExcelTable* CreateTable();

	int GetNumTables();
	CExcelTable* GetTable(int index);
	CExcelTable* GetTableByName(const char *name);

private:

	bool Parse(pugi::xml_document &doc);

	std::vector<CExcelTable*> m_Tables;
};


}//GELib

#endif//__GELIB_UTILITY_EXCELFILE_H__
