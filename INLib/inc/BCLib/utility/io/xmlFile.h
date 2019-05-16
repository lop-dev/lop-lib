//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/xmlFile.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __BCLIB_UTILITY_IO_XMLFILE_H__
#define __BCLIB_UTILITY_IO_XMLFILE_H__

#include <BCLib/utility/baseDef.h>

namespace BCLib
{
namespace Utility
{
typedef void* HXmlNode;
typedef void* HXmlAttr;

class BCLIB_UTILITY_API CXmlFile
{
public:
    CXmlFile();
    virtual ~CXmlFile();

    /// @brief 加载文件
    /// @return bool
    /// @param fileName
    bool loadFile(const std::string& fileName);

    /// @brief 加载字符串
    /// @return bool
    /// @param xml
    bool loadString(const std::string& xml);

    /// @brief 关闭
    /// @return void
    void clear();

    /// @brief 获取xml根节点
    /// @return BCLib::Utility::HXmlNode
    /// @param rootName
    HXmlNode getRootNode(const std::string& rootName = "");

    /// @brief 创建Xml
    /// @return bool
    bool createXml();
    HXmlNode createXml(const std::string& rootName, const std::string& rootValue = "", const std::string& nodePi = "xml version='1.0' encoding='utf-8'");
    HXmlNode createRootNode(const std::string& rootName, const std::string& rootValue = "");

    /// @brief 保存
    /// @return bool
    /// @param fileName
    /// @param replace
    bool save(const std::string& fileName = "", bool replace = true);

    /// @brief  提取xml
    /// @return bool
    /// @param xml
    bool getXml(std::string& xml);

    /// @brief 添加子节点
    /// @return bool
    /// @param nodeName
    bool appendPiNode(const std::string& nodeName);
    HXmlNode appendNode(HXmlNode node, const std::string& nodeName);
    HXmlNode appendNode(HXmlNode node, const std::string& nodeName, const std::string& nodeValue);
    HXmlNode appendNode(HXmlNode node, const std::string& nodeName, double nodeValue);
    HXmlNode appendNode(HXmlNode node, const std::string& nodeName, float nodeValue);
    HXmlNode appendNode(HXmlNode node, const std::string& nodeName, int8 nodeValue);
    HXmlNode appendNode(HXmlNode node, const std::string& nodeName, int16 nodeValue);
    HXmlNode appendNode(HXmlNode node, const std::string& nodeName, int32 nodeValue);
    HXmlNode appendNode(HXmlNode node, const std::string& nodeName, int64 nodeValue);

    /// @brief 添加属性
    /// @return BCLib::Utility::HXmlAttr
    /// @param node
    /// @param attrName
    /// @param attrValue
    HXmlAttr appendAttr(HXmlNode node, const std::string& attrName, const std::string& attrValue);
    HXmlAttr appendAttr(HXmlNode node, const std::string& attrName, double attrValue);
    HXmlAttr appendAttr(HXmlNode node, const std::string& attrName, float attrValue);
    HXmlAttr appendAttr(HXmlNode node, const std::string& attrName, int8 attrValue);
    HXmlAttr appendAttr(HXmlNode node, const std::string& attrName, int16 attrValue);
    HXmlAttr appendAttr(HXmlNode node, const std::string& attrName, int32 attrValue);
    HXmlAttr appendAttr(HXmlNode node, const std::string& attrName, int64 attrValue);

    /// @brief 删除node所有子节点
    /// @return bool
    /// @param node
    bool removeNode(HXmlNode node);

    /// @brief 删除node指定子节点childNode
    /// @return bool
    /// @param node
    /// @param childNode
    bool removeNode(HXmlNode node, HXmlNode childNode);

    /// @brief 删除node所有属性
    /// @return bool
    /// @param node
    bool removeAttr(HXmlNode node);

    /// @brief 删除node指定属性attr
    /// @return bool
    /// @param node
    /// @param attr
    bool removeAttr(HXmlNode node, HXmlAttr attr);

public:

    /// @brief 获取node的首子节点
    /// @return BCLib::Utility::HXmlNode
    /// @param node
    static HXmlNode getChildNode(HXmlNode node);

    /// @brief 获取node的首个childName子节点
    /// @return BCLib::Utility::HXmlNode
    /// @param node
    /// @param childName
    static HXmlNode getChildNode(HXmlNode node, const std::string& childName);

    /// @brief 获取node的下个节点
    /// @return BCLib::Utility::HXmlNode
    /// @param node
    static HXmlNode getNextNode(HXmlNode node);

    /// @brief 获取node的下个nextNodeName节点
    /// @return BCLib::Utility::HXmlNode
    /// @param node
    /// @param nextNodeName
    static HXmlNode getNextNode(HXmlNode node, const std::string& nextNodeName);

    /// @brief 获取node的首属性
    /// @return BCLib::Utility::HXmlAttr
    /// @param node
    static HXmlAttr getFirstAttr(HXmlNode node);

    /// @brief 获取attr的下个属性
    /// @return BCLib::Utility::HXmlAttr
    /// @param attr
    static HXmlAttr getNextAttr(HXmlAttr attr);

    /// @brief 获取node的attrName属性
    /// @return BCLib::Utility::HXmlAttr
    /// @param node
    /// @param attrName
    static HXmlAttr getNodeAttr(HXmlNode node, const std::string& attrName);

    /// @brief 获取节点名称
    /// @return bool
    /// @param node
    /// @param valueBuff
    /// @param buffSize
    static bool getNodeName(HXmlNode node, char* valueBuff, int buffSize);
    static bool getNodeName(HXmlNode node, std::string& value);

    /// @brief 获取节点内容
    /// @return bool
    /// @param node
    /// @param valueBuff
    /// @param buffSize
    static bool getNodeValue(HXmlNode node, char* valueBuff, int buffSize);
    static bool getNodeValue(HXmlNode node, std::string& value);
    static bool getNodeValue(HXmlNode node, double& nodeValue);
    static bool getNodeValue(HXmlNode node, float& nodeValue);
    static bool getNodeValue(HXmlNode node, int8& nodeValue);
    static bool getNodeValue(HXmlNode node, int16& nodeValue);
    static bool getNodeValue(HXmlNode node, int32& nodeValue);
    static bool getNodeValue(HXmlNode node, int64& nodeValue);

    /// @brief 获取属性名称
    /// @return bool
    /// @param attr
    /// @param valueBuff
    /// @param buffSize
    static bool getAttrName(HXmlAttr attr, char* valueBuff, int buffSize);
    static bool getAttrName(HXmlAttr attr, std::string& value);

    /// @brief 获取属性值
    /// @return bool
    /// @param attr
    /// @param valueBuff
    /// @param buffSize
    static bool getAttrValue(HXmlAttr attr, char* valueBuff, int buffSize);
    static bool getAttrValue(HXmlAttr attr, std::string& value);
    static bool getAttrValue(HXmlAttr attr, double& nodeValue);
    static bool getAttrValue(HXmlAttr attr, float& nodeValue);
    static bool getAttrValue(HXmlAttr attr, int8& nodeValue);
    static bool getAttrValue(HXmlAttr attr, int16& nodeValue);
    static bool getAttrValue(HXmlAttr attr, int32& nodeValue);
    static bool getAttrValue(HXmlAttr attr, int64& nodeValue);

    /// @brief 获取node的attrName属性值
    /// @return bool
    /// @param node
    /// @param attrName
    /// @param valueBuff
    /// @param buffSize
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, char* valueBuff, int buffSize);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, std::string& attrValue);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, double& nodeValue);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, float& nodeValue);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, int8& nodeValue);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, int16& nodeValue);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, int32& nodeValue);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, int64& nodeValue);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, uint8& nodeValue);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, uint16& nodeValue);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, uint32& nodeValue);
    static bool getNodeAttrValue(HXmlNode node, const std::string& attrName, uint64& nodeValue);

private:
    std::string m_fileName;
    void* m_xmlDoucument;
    std::vector<char> m_xmlBuff;
};
}//Utility
}//BCLib

#endif//__BCLIB_UTILITY_IO_XMLFILE_H__
