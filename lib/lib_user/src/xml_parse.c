#include <sys/file.h>
#include <unistd.h>

#include "log_printf.h"
#include "xml_parse.h"

/**
 * @brief 打开一个xml文件
 * @param xmlFile xml文件结构体
 * @return 0:成功 -1:失败
 */
int openXmlFile(XML_FILE_T *xmlFile)
{
	xmlFile->doc = xmlParseFile(xmlFile->file);
	if (xmlFile->doc == NULL)
	{
		logPrintf("xml file open failed!");
		return -1;
	}
	
	return 0;
}

/**
 * @brief 创建一个xml文件
 * @param xmlFile xml文件结构体
 * @return 0:成功 -1:失败
 */
int createXmlFile(XML_FILE_T *xmlFile, const char *rootName)
{
	xmlNodePtr rootNode = NULL;

	/* 创建xml文档 */
	xmlFile->doc = xmlNewDoc(BAD_CAST "1.0");
	if (xmlFile->doc == NULL)
	{
		logPrintf("create xml failed!");
		return -1;
	}

	/* 创建根节点 */
	rootNode = xmlNewNode(NULL, BAD_CAST rootName);
	if (rootNode == NULL)
	{
		logPrintf("create root node failed!");
		return -1;
	}
	
	/* 将根节点添加到文档中 */
	xmlDocSetRootElement(xmlFile->doc, rootNode);

	/* 将文档保存到文件中 */
	xmlSaveFormatFileEnc(xmlFile->file, xmlFile->doc, "UTF-8", 1);

	return 0;
}

/**
 * @brief 根据xpath查找节点集合
 * @param xmlFile xml文件结构体
 * @param xpath 路径  
 * @return 查找到的节点集合
 */
xmlNodeSetPtr xmlFindNodeAsXpath(XML_FILE_T *xmlFile, xmlChar *xPath)
{
	xmlXPathContextPtr context = NULL;
	xmlXPathObjectPtr result = NULL;

	context = xmlXPathNewContext(xmlFile->doc);
	if (context == NULL)
	{
		logPrintf("context is NULL!");
		return NULL;
	}
	
	result = xmlXPathEvalExpression(xPath, context);
	xmlXPathFreeContext(context);
	if (result == NULL)
	{
		logPrintf("result is NULL!");
		return NULL;
	}

	if (xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
		logPrintf("nodeset is empty!");
		return NULL;
	}
	
	return result->nodesetval;
}

/**
 * @brief 创建节点，并添加到父节点
 * @param parentNode 父节点
 * @param nodeName 新节点名称  
 * @return 0:成功 -1:失败
 */
int createNewNodeToParent(XML_FILE_T *xmlFile, const char *xPath, const char *nodeName)
{
	int i;
	xmlNodePtr newNode = NULL;
	xmlNodePtr parentNode = NULL;
	xmlNodeSetPtr nodeSet = NULL;

	/* 创建新节点 */
	newNode = xmlNewNode(NULL, BAD_CAST nodeName);
	if (newNode == NULL)
	{
		logPrintf("create %s node failed!", nodeName);
		return -1;
	}

	/* 根据xpath获取节点集合 */
	nodeSet = xmlFindNodeAsXpath(xmlFile, BAD_CAST xPath);
	if (nodeSet == NULL)
	{
		return -1;
	}

	/* 将新节点添加到所有节点集合下 */
	for (i = 0; i < nodeSet->nodeNr; i++)
	{
		parentNode = nodeSet->nodeTab[i];
		xmlAddChild(parentNode, newNode);
	}
	
	xmlSaveFormatFileEnc(xmlFile->file, xmlFile->doc, "UTF-8", 1);
	
	return 0;
}

/**
 * @brief 给节点添加属性，如果已有属性，进行修改
 * @param xmlFile 文件名
 * @return 0:成功 -1:失败
 */
int addXmlNodeProp(XML_FILE_T *xmlFile, const char *xPath, XML_NODE_PROP_T *prop, int propCnt)
{
	int i, j;
	xmlNodeSetPtr nodeSet = NULL;

	/* 根据xpath获取节点集合 */
	nodeSet = xmlFindNodeAsXpath(xmlFile, BAD_CAST xPath);
	if (nodeSet == NULL)
	{
		return -1;
	}

	/* 将新节点添加到所有节点集合下 */
	for (i = 0; i < nodeSet->nodeNr; i++)
	{
		for (j = 0; j < propCnt; j++)
		{
			xmlSetProp(nodeSet->nodeTab[i], BAD_CAST (prop + j)->name, BAD_CAST (prop + j)->value);
		}
	}

	xmlSaveFormatFileEnc(xmlFile->file, xmlFile->doc, "UTF-8", 1);
	return 0;
}