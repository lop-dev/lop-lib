//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/table.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_TABLE_H__
#define __BCLIB_UTILITY_TABLE_H__

#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/noncopyable.h>

namespace BCLib
{
namespace Utility
{
template<class TYPE>
class CTableCell : public BCLib::Utility::CNoncopyable
{
public:
    CTableCell()
    {
    }
    virtual ~CTableCell()
    {
    }

    const TYPE& getValue() const
    {
        return m_value;
    }

    TYPE& operator =(TYPE& value)
    {
        m_value = value;
        return m_value;
    }

    TYPE& operator ()() const
    {
        return m_value;
    }

    template<class VALUE_TYPE>
    bool getValue(VALUE_TYPE& value) const
    {
        value = m_value;
        return true;
    }

    template<class VALUE_TYPE>
    bool setValue(VALUE_TYPE& value)
    {
        value = (TYPE)m_value;
        return true;
    }

    template<class VALUE_TYPE>
    bool operator()(VALUE_TYPE& value) const
    {
        value = (VALUE_TYPE)m_value;
        return true;
    }

protected:
    TYPE m_value;
};

template<class TYPE, class CELLTYPE, class ROW_TYPE>
class CTable;

template<class TYPE, class CELL_TYPE = CTableCell<TYPE> >
class CTableRow : public BCLib::Utility::CNoncopyable
{
public:
    CTableRow(CTable<TYPE, CELL_TYPE, CTableRow>* table, int cellMax)
    :m_table(table)
    {
        m_cells.resize(cellMax, NULL);
        for(uint32 i = 0; i < m_cells.size(); ++i)
        {
            m_cells[i] = _createCell();
        }
    }
    virtual ~CTableRow()
    {
        _clear();
    }

    CELL_TYPE* getColumn(uint32 columnNum) const
    {
        if(columnNum >= m_cells.size())
        {
            return NULL;
        }
        return m_cells[columnNum];
    }

    CELL_TYPE* getColumn(const std::string& columnName) const
    {
        int columnNum = m_table->getColumnNum(columnName);
        if(columnNum < 0)
        {
            return NULL;
        }
        return m_cells[columnNum];
    }

    CELL_TYPE& operator[](int columnNum) const
    {
        return *m_cells[columnNum];
    }

    CELL_TYPE& operator[](const std::string& columnName) const
    {
        return *m_cells[m_table->getColumnNum(columnName)];
    }

    int countCell() const
    {
        return (int)m_cells.size();
    }

protected:
    void _clear()
    {
        for(uint32 i = 0; i < m_cells.size(); ++i)
        {
            CELL_TYPE* cell = m_cells[i];
            if(cell != NULL)
            {
                BCLIB_SAFE_DELETE(cell);
                m_cells[i] = NULL;
            }
        }
        m_cells.clear();
    }

    CELL_TYPE* _createCell()
    {
        return m_table->_createCell();
    }

protected:
    CTable<TYPE, CELL_TYPE, CTableRow>* m_table;
    std::vector<CELL_TYPE*> m_cells;
};

template<class TYPE, class CELL_TYPE = CTableCell<TYPE>, class ROW_TYPE = CTableRow<TYPE, CELL_TYPE> >
class CTable : public BCLib::Utility::CNoncopyable
{
public:
    CTable(int columnMax)
    :m_columnMax(columnMax)
    {
        m_excelColumnNameList.resize(m_columnMax);
    }

    virtual ~CTable()
    {
        clear();
    }

    /// @brief 返回第I行指针
    /// @return ROW_TYPE* 行指针
    /// @param rowNum 索引
    ROW_TYPE* getRow(uint32 rowNum) const
    {
        if(rowNum >= m_excelRows.size())
        {
            return NULL;
        }
        return m_excelRows[rowNum];
    }

    int countRows() const
    {
        return (int)m_excelRows.size();
    }

    int countColumns() const
    {
        return m_columnMax;
    }

    virtual void setColumnName(int columnNum, const std::string& columnName)
    {
        m_excelColumnNameList[columnNum] = columnName;
        m_excelColumnNameHashMap.setValue(columnName, columnNum);
    }

    int getColumnNum(const std::string& name) const
    {
        int columnNum = -1;
        if(!m_excelColumnNameHashMap.getValue(name, columnNum))
        {
            return -1;
        }
        return columnNum;
    }

    const std::string& getColumnName(int columnNum) const
    {
        return m_excelColumnNameList[columnNum];
    }

    /// @brief 添加行
    /// @return bool
    /// @param num 待添加行数
    bool appendRow(int num = 1)
    {
        for(int i = 0; i < num; ++i)
        {
            ROW_TYPE* row = _createRow();
            if(row == NULL)
            {
                return false;
            }
            m_excelRows.push_back(row);
        }
        return true;
    }

    /// @brief 移除[whereRowNum, whereRowNum + num]之间的行数
    /// @return bool
    /// @param whereRowNum
    /// @param num
    bool removeRow(int whereRowNum, int num = 1)
    {
        int excelRowSize = (int)m_excelRows.size();
        if(whereRowNum < 0 || whereRowNum >= excelRowSize || whereRowNum + num > excelRowSize)
        {
            return false;
        }
        for(int i = 0; i < num; ++i)
        {
            ROW_TYPE* row = m_excelRows[whereRowNum + i];
            if(row != NULL)
            {
                BCLIB_SAFE_DELETE(row);
                m_excelRows[whereRowNum + i] = NULL;
            }
        }
        for(int i = whereRowNum; i < excelRowSize - num; ++i)
        {
            m_excelRows[i] = m_excelRows[i + num];
        }
        for(int i = 0; i < num; ++ i)
        {
            typename std::vector<ROW_TYPE*>::iterator it = m_excelRows.end();
            --it;
            m_excelRows.erase(it);
        }
        return true;
    }

    /// @brief 插入[whereRowNum, whereRowNum + num]之间的行数
    /// @return bool
    /// @param whereRowNum
    /// @param num
    bool insertRow(int whereRowNum, int num = 1)
    {
        int excelRowSize = (int)m_excelRows.size();
        if(whereRowNum < 0 || whereRowNum >= excelRowSize)
        {
            return false;
        }
        for(int i = 0; i < num; ++i)
        {
            m_excelRows.push_back(NULL);
        }
        for(int i = excelRowSize - 1; i >= whereRowNum; --i)
        {
            m_excelRows[i + num] = m_excelRows[i];
        }
        bool res = true;
        for(int i = 0; i < num; ++i)
        {
            ROW_TYPE* row = _createRow();
            m_excelRows[whereRowNum + i] = row;
            if(row == NULL)
            {
                res = false;
            }
        }
        return res;
    }

    void clear()
    {
        for(uint32 i = 0; i < m_excelRows.size(); ++ i)
        {
            ROW_TYPE* row = m_excelRows[i];
            if(row != NULL)
            {
                BCLIB_SAFE_DELETE(row);
                m_excelRows[i] = NULL;
            }
        }
        m_excelRows.clear();
    }

    ROW_TYPE& operator[](uint32 rowNum) const
    {
        return *(m_excelRows[rowNum]);
    }

    /// @brief 得到rowNum行columnNum列值，保存到value
    /// @return bool
    /// @param rowNum
    /// @param columnNum
    /// @param value
    template<class VALUE_TYPE>
    bool getColumn(uint32 rowNum, int columnNum, VALUE_TYPE& value) const
    {
        if(rowNum >= m_excelRows.size())
        {
            return false;
        }
        ROW_TYPE* row = m_excelRows[rowNum];
        if(row == NULL)
        {
            return false;
        }
        CELL_TYPE* cell = row->getColumn(columnNum);
        if(cell == NULL)
        {
            return false;
        }
        return cell->getValue(value);
    }

    template<class VALUE_TYPE>
    bool getColumn(uint32 rowNum, const std::string& columnName, VALUE_TYPE& value) const
    {
        if(rowNum >= m_excelRows.size())
        {
            return false;
        }
        ROW_TYPE* row = m_excelRows[rowNum];
        if(row == NULL)
        {
            return false;
        }
        CELL_TYPE* cell = row->getColumn(columnName);
        if(cell == NULL)
        {
            return false;
        }
        return cell->getValue(value);
    }

    /// @brief 设置第rowNum行columnNum列为value
    /// @return bool
    /// @param rowNum
    /// @param columnNum
    /// @param value
    template<class VALUE_TYPE>
    bool setColumn(uint32 rowNum, int columnNum, VALUE_TYPE& value)
    {
        if(rowNum >= m_excelRows.size())
        {
            return false;
        }
        ROW_TYPE* row = m_excelRows[rowNum];
        if(row == NULL)
        {
            return false;
        }
        CELL_TYPE* cell = row->getColumn(columnNum);
        if(cell == NULL)
        {
            return false;
        }
        return cell->setValue(value);
    }

    template<class VALUE_TYPE>
    bool setColumn(uint32 rowNum, const std::string& columnName, VALUE_TYPE& value)
    {
        if(rowNum >= m_excelRows.size())
        {
            return false;
        }
        ROW_TYPE* row = m_excelRows[rowNum];
        if(row == NULL)
        {
            return false;
        }
        CELL_TYPE* cell = row->getColumn(columnName);
        if(cell == NULL)
        {
            return false;
        }
        return cell->setValue(value);
    }

    template<class VALUE_TYPE>
    bool operator()(uint32 rowNum, int columnNum, VALUE_TYPE& value) const
    {
        return getColumn(rowNum, columnNum, value);
    }

    template<class VALUE_TYPE>
    bool operator()(uint32 rowNum, const std::string& columnName, VALUE_TYPE& value)const
    {
        return getColumn(rowNum, columnName, value);
    }

protected:
    virtual CELL_TYPE* _createCell()
    {
        return new CELL_TYPE();
    }

    virtual ROW_TYPE* _createRow()
    {
        return new ROW_TYPE(this, m_columnMax);
    }

protected:
    int m_columnMax;
    std::vector<ROW_TYPE*> m_excelRows;
    CHashMap<std::string, int> m_excelColumnNameHashMap;
    std::vector<std::string> m_excelColumnNameList;

    friend class CTableRow<TYPE, CELL_TYPE>;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_TABLE_H__
