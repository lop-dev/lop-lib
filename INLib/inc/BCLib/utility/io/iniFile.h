//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/iniFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_INIFILE_H__
#define __BCLIB_UTILITY_IO_INIFILE_H__

#include <BCLib/utility/hashMap.h>
#include <BCLib/utility/baseDec.h>

namespace BCLib
{
namespace Utility
{
	
typedef std::vector<BCLib::CString> CStringVec;

class BCLIB_UTILITY_API CIniItem
{
public:
	CIniItem(const BCLib::CString& name, const BCLib::CString& value)
		: m_name (name)
		, m_value(value)
	{}
	bool writeToFile(FILE* fp);

	BCLib::CString getName() const { return m_name; }
	void setName(const BCLib::CString& val) { m_name = val; }
	BCLib::CString getValue() const { return m_value; }
	BCLib::CString& getValue() { return m_value;  }
	void setValue(const BCLib::CString& val) { m_value = val; }

	void setComments(const CStringVec& val) { m_comments = val; }
	CStringVec& getComments() { return m_comments;  }
private:
	BCLib::CString m_name;
	BCLib::CString m_value;
	CStringVec m_comments;
};

class BCLIB_UTILITY_API CIniSection
{
public:
	CIniSection(const BCLib::CString& name)
		: m_name(name)
	{}
	CIniItem* getItem(const BCLib::CString& name);
	CIniItem* setItem(const BCLib::CString& name, const BCLib::CString& value);
	void removeItem(const BCLib::CString& name);
	void clear();
	BCLib::uint32 size() const;
	void showAllData();

	
	bool writeToFile(FILE* fp);
	BCLib::CString getName() const { return m_name; }
	void setName(const BCLib::CString& val) { m_name = val; }

	void setComments(const CStringVec& val) { m_comments = val; }
	CStringVec& getComments() { return m_comments; }

	typedef std::map< std::string, CIniItem*> ItemMap;
	typedef ItemMap::iterator ItemIter;
	typedef ItemMap::const_iterator ItemConstIter;

	ItemIter itemBegin() { return m_items.begin();  }
	ItemIter itemEnd() { return m_items.end();  }
	ItemConstIter itemBegin() const { return m_items.begin();  }
	ItemConstIter itemEnd() const { return m_items.end(); }
private:
	BCLib::CString m_name;

	ItemMap m_items;
	CStringVec m_comments;
};

//////////////////////////////////////////////////////////////////////////
/// @brief 对ini文件操作
//////////////////////////////////////////////////////////////////////////
class BCLIB_UTILITY_API CIniFile
{
public:
	~CIniFile();
	bool open(const std::string& strFile);
	bool save();
	bool saveAs(const std::string& strFile);

	std::string& getValue(const std::string& key);
	void setValue(const std::string& key, const std::string& value);
	void remove(const std::string& key);
	bool find(const std::string& key);
	void clear();
	unsigned int size() const;

	//支持 section/item 格式
	int getValueInt(const std::string& key);
	int getValueInt(const std::string& appName, const std::string& paramName);
	std::string getValueStr(const std::string& key);
	std::string getValueStr(const std::string& appName, const std::string& paramName);

	void showAllData();
	std::string& operator[](const std::string& key);

	CIniSection* getSection(const BCLib::CString& name);
	CIniSection* setSection(const BCLib::CString& name);
	CIniItem* getItem(const BCLib::CString& secionName, const BCLib::CString& itemName);
	CIniItem* setItem(const BCLib::CString& section, const BCLib::CString& itemName, const BCLib::CString& value);

private:
	bool isSection(const BCLib::CString& line);
	bool isItem(const BCLib::CString& line);
	bool isComment(const BCLib::CString& line);

private:
	std::string m_fileName;
	//因为保存文件需要有序性， 所以改用了std::map
	std::map<std::string, CIniSection*> m_paraHashMap;
	//BCLib::Utility::CHashMap< std::string, CIniSection*> m_paraHashMap;
};

}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_IO_INIFILE_H__