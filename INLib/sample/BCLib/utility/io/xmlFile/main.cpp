//////////////////////////////////////////////////////////////////////
//  created:    2011/11/01
//  filename:   BCLib/utility/io/xmlFile/main.cpp
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
//#include <string>

#include <tchar.h>
#include <BCLib/utility/io/fileInfo.h>
#include <BCLib/utility/io/xmlFile.h>
#include <BCLib/utility/baseDec.h>
#include <BCLib/utility/string.h>

#define SETTTING_NUM 10
void readXmlFile(BCLib::Utility::CXmlFile& xmlFile)
{
    //
    //BCLib::CString
    BCLib::Utility::HXmlNode rootNode = xmlFile.getRootNode();
    BCLib::Utility::HXmlNode childNode = NULL;
    BCLib::CString settingNodeName(_T("setting"));
    childNode = xmlFile.getChildNode(rootNode, settingNodeName);

    BCLib::Utility::HXmlNode grandChildNode = NULL;
    BCLib::CString grandNodeName;
    int grandNodeValue = 0;
    BCLib::Utility::HXmlAttr grandNodeAttr = NULL;
    BCLib::CString grandNodeAttrName;
    int grandNodeAttrValue = 0;

    grandChildNode = xmlFile.getChildNode(childNode);
    while((grandChildNode) != NULL)
    {
        xmlFile.getNodeName(grandChildNode, grandNodeName);
        xmlFile.getNodeValue(grandChildNode, grandNodeValue);
        printf("name = %s, value = %d\n", grandNodeName.c_str(), grandNodeValue);
        grandNodeAttr = xmlFile.getFirstAttr(grandChildNode);

        while(grandNodeAttr)
        {
            xmlFile.getAttrName(grandNodeAttr, grandNodeAttrName);
            xmlFile.getAttrValue(grandNodeAttr, grandNodeAttrValue);
            printf("      attrname = %s, value = %d\n", grandNodeAttrName.c_str(), grandNodeAttrValue);
            grandNodeAttr = xmlFile.getNextAttr(grandNodeAttr);
        }
        grandChildNode = xmlFile.getNextNode(grandChildNode);
    }
}

void writeXmlFile(BCLib::Utility::CXmlFile& xmlFile)
{
    //
    BCLib::Utility::HXmlNode rootNode = xmlFile.getRootNode();
    BCLib::Utility::HXmlNode childNode = NULL;
    BCLib::CString settingNodeName(_T("setting"));
    childNode = xmlFile.appendNode(rootNode, settingNodeName);

    BCLib::Utility::HXmlNode grandChildNode = NULL;
    BCLib::CString grandNodeName(_T("subsetting可以"));
    for(int i = 0; i < SETTTING_NUM; i++)
    {
        grandChildNode = xmlFile.appendNode(childNode, grandNodeName, i);
        xmlFile.appendAttr(grandChildNode, BCLib::CString(_T("attr")), i);
    }
}

int main(int argc, char* argv[])
{
    BCLib::CString/*std::wstring*/ fileName = _T("test.xml");

    BCLib::Utility::CXmlFile xmlFile;
    xmlFile.loadString(BCLib::CString(_T("<?xml version='1.0'  encoding='utf-8'?><subsetting可以 attr=\"0\" attr2=\"13\">0</subsetting可以>")));
    if(!xmlFile.loadFile(fileName))
    {
        printf("读取文件[%s]失败\n", fileName.c_str());
        BCLib::CString rootNodeName = _T("RootNode");
        if(!xmlFile.createXml(rootNodeName))
        {
            printf("创建根节点[%s]失败\n", rootNodeName.c_str());
            return 0;
        }
        printf("创建根节点[%s]成功\n", rootNodeName.c_str());

        writeXmlFile(xmlFile);
    }

    readXmlFile(xmlFile);
    if(!xmlFile.save(fileName))
    {
        printf("保存文件[%s]失败\n", fileName.c_str());
    }
    else
    {
        printf("保存文件[%s]成功\n", fileName.c_str());
    }
    system("pause");
    return 0;
}