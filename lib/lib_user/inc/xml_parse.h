#ifndef _XML_PARSE_H_
#define _XML_PARSE_H_

#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xpath.h>

typedef struct
{
	const char *file;
	xmlDocPtr doc;
} XML_FILE_T;


typedef struct
{
	const char *name;
	const char *value;
} XML_NODE_PROP_T;

extern int openXmlFile(XML_FILE_T *xmlFile);
extern int createXmlFile(XML_FILE_T *xmlFile, const char *rootName);
extern xmlNodeSetPtr xmlFindNodeAsXpath(XML_FILE_T *xmlFile, xmlChar *xPath);
extern int createNewNodeToParent(XML_FILE_T *xmlFile, const char *xPath, const char *nodeName);
extern int addXmlNodeProp(XML_FILE_T *xmlFile, const char *xPath, XML_NODE_PROP_T *prop, int propCnt);

#endif