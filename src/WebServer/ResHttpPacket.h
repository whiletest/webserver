/*************************************************
Copyright
File name: ResHttpPacket.h
Author: 张朝旭
Version: 
Date: 2012/11/22
Description: 
接收到一个客户的请求包(REQ_PACKET)经过处理返回一个响应包(RES_PACKET)
History: 
1. 
Date:2012.12.2
Author:zcx
Modification:
	GetDocument(const char *pURL, RES_PACKET *pResPacket)
		->GetDocument(const REQ_PACKET *pReqPacket, RES_PACKET *pResPacket)
	Responde404(RES_PACKET *packet)
		->Responde404(const REQ_PACKET *pReqPacket, RES_PACKET *packet)
	Responde202(const char *pURL, RES_PACKET *packet)
		->Responde202(const REQ_PACKET *pReqPacket, RES_PACKET *packet)
	RespondeText(const char *pText, RES_PACKET *packet)
		->RespondeText(const REQ_PACKET *pReqPacket, const char *pText, RES_PACKET *packet)
*************************************************/
#pragma once
#include "packet.h"


class CResHttpPacket
{
public:
	CResHttpPacket(void);
	virtual ~CResHttpPacket(void);
	/*************************************************
	Function: ResponsePacket
	Description: 接收到一个客户的请求包(REQ_PACKET)经过处理返回一个响应包(RES_PACKET)
	Input: const REQ_PACKET* 客户端浏览器的请求包
	Output: 
	Return: RES_PACKET 服务端响应请求的响应包
	*************************************************/
	RES_PACKET ResponsePacket(const REQ_PACKET *pReqPacket);
protected:
	/*************************************************
	Function: CheckURL
	Description: 检查URL路径是否合法(在根目录下)
	Input: const char *pURL 客户端请求文档的路径
	Output: 
	Return: 路径合法返回true,否则返回false
	*************************************************/
	bool CheckURL(const char *pURL); 
	/*************************************************
	Function: GetDocument
	Description: 获取客户端请求的文档
	Input: cpReqPacket 客户端请求文包
	Output: RES_PACKET *pResPacket 封装好的响应客户端请求的响应包
	Return:
	*************************************************/
	void GetDocument(const REQ_PACKET *pReqPacket, RES_PACKET *pResPacket);
	/*************************************************
	Function: GetAbsolutePath
	Description: 将URL路径装换为在根目录下的绝对路径
	Input: const char *pURL 客户端请求文档的路径
	Output: 
		char *pAbsolutePath 转换后的绝对路径,为0是函数返回路径长度pLen
		UINT *pLen 绝对路径长度
	Return:
	*************************************************/
	void GetAbsolutePath(char *pAbsolutePath, UINT *pLen, const char *pURL);
	/*************************************************
	Function: GetSuffixName
	Description: 获取请求文档的后缀名
	Input: const char *pURL 客户端请求文档的路径
	Output: char *pSuffixName 后缀名，字符串以'\0'结束
	Return:
	*************************************************/
	void GetSuffixName(const char *pURL, char *pSuffixName);
	/*************************************************
	Function: Responde404
	Description: 请求文档失败的处理函数
	Input: 
	Output: RES_PACKET *packet 响应包
	Return:
	*************************************************/
	virtual void Responde404(const REQ_PACKET *pReqPacket, RES_PACKET *packet) = 0;
	/*************************************************
	Function: Responde202
	Description: 请求文档成功的处理函数
	Input: const char *pURL 请求文档的路径
	Output: RES_PACKET *packet 响应包
	Return:
	*************************************************/
	virtual void Responde202(const REQ_PACKET *pReqPacket, RES_PACKET *packet) = 0;
	/*************************************************
	Function: RespondeText
	Description: 响应特定的消息到客户端
	Input:  pText 特定文本消息
	Output: packet 响应包
	Return:
	*************************************************/
	virtual void RespondeText(const REQ_PACKET *pReqPacket, const char *pText, RES_PACKET *packet);
};