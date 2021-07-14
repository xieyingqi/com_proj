#include "includes.h"

EP_LISTEN_T gMainLoop;
SOCKET_IPC_T gIpc;
TIMER_STRU_T gTimer1, gTimer2;
CMD_LIST_T cmdList;

CMD_PARA_T cmdParaList[] = 
{
	{"id",        TYPE_TEXT, cmdList.id},
	{"log_level", TYPE_TEXT, cmdList.log_level},
	{"type",      TYPE_INT,  &cmdList.type},
};

XML_FILE_T xmlFile = 
{
	.file = "test.xml",
	.doc = NULL,
};

XML_NODE_PROP_T prop[] = 
{
	{"id", "1"},
	{"num", "158"},
};

void cbTimer1(sigval_t i)
{
	// IPC_MSG_T send_msg;
	
	// strcpy(send_msg.buf, "hello");

	// if(strcmp(cmdList.id, "recv") != 0)
	// {
	// 	sendIpc(&gIpc, "recv", send_msg);
	// }
}

void cbSock(IPC_MSG_T *msg)
{
	logPrintf("rcv %d data from:%s msg:%s", msg->cnt, msg->id, msg->buf);
}

int main(int argc, char *argv[])
{
	/* 解析命令行参数 */
	parseCmdPara(argc, argv, cmdParaList, sizeof(cmdParaList) / sizeof(CMD_PARA_T));
	/* 设置打印等级 */
	setLogLevel(cmdList.log_level);
	
	/* 创建监听主循环 */
	epollCreate(&gMainLoop);

	/* 创建ipc，设置回调函数 */
	creatIpc(&gMainLoop, &gIpc, cmdList.id);
	setIpcCallBack(&gIpc, cbSock);
	
	/* 创建或解析xml文件 */
	if (openXmlFile(&xmlFile) < 0)
	{
		createXmlFile(&xmlFile, "root");
		createNewNodeToParent(&xmlFile, "/root", "tel");
		addXmlNodeProp(&xmlFile, "/root/tel", prop, sizeof(prop) / sizeof(XML_NODE_PROP_T));
	}

	/* 设定定时器 */
	addTimer(&gTimer1, 1, cbTimer1);
	startTimer(&gTimer1);
	
	/* 启动epoll监听 */
	epollListenLoop(&gMainLoop);

	return 0;
}