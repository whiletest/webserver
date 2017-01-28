/*************************************************
Copyright
File name: ParseHttpPacket.h
Author: 张朝旭
Version: 
Date: 2012/11/22
Description: 
使用单例模式。基于http协议，进行http协议包的解析，封装。
History: 
1. 
Date:
Author:
Modification:
*************************************************/
#pragma once

#include "packet.h"

class CParseHttpPacket
{
private:
	CParseHttpPacket(void);
public:
	~CParseHttpPacket(void);
	/*************************************************
	Function: GetInstance
	Description: 创建全局的并且唯一的实例化对象指针，并且返回它
	Input: 
	Output: 
	Return: 本类型的实例化对象的指针
	*************************************************/
	static CParseHttpPacket* GetInstance()
	{
		if (!m_pThis)
			m_pThis = new CParseHttpPacket;
		return m_pThis;
	}
	/*************************************************
	Function: ParsePacket
	Description: 接收一个http协议的缓冲区包，对其进行解析，返回一个客户端请求包
	Input: 
		buff http协议的缓冲区
		nLen 缓冲区大小
	Output: 
	Return: 客户端请求包
	*************************************************/
	REQ_PACKET ParsePacket(char *buff, int nLen);
	/*************************************************
	Function: PackagePacket
	Description: 接收一个服务端响应请求的响应包，将其封装成满足http协议格式的缓冲区
	Input: 
		packet 服务器响应请求的响应包
	Output: 
		buff 封装后的满足http协议格式的缓冲区。buff为0时，函数返回pLenBytes
		pLenBytes 缓冲区的大小
	Return: 
	*************************************************/
	void PackagePacket(char *buff, UINT *pLenBytes, const RES_PACKET *packet);
private:
	/*************************************************
	Function: CheckMethod
	Description: 将http协议缓冲区中字符形式的方法名转换为UCHAR表示的类型
	Input: pMethod 缓冲区中字符形式的方法名
	Output: 
	Return: UCHAR表示的方法名类型
	*************************************************/
	UCHAR CheckMethod(const char *pMethod);
	/*************************************************
	Function: CheckVerson
	Description: 将http协议缓冲区中字符形式的版本名转换为UCHAR表示的类型
	Input: pVerson 缓冲区中字符形式的版本名
	Output: 
	Return: UCHAR表示的类型的版名名
	*************************************************/
	UCHAR CheckVerson(const char *pVerson);
	/*************************************************
	Function: TranslateURL
	Description: 将URL路径转换为电脑文件目录格式的路径，例:/index.html->\\index.html
	Input: pURL URL路径
	Output: packet 电脑文件目录格式的路径,并且如果访问动态文档将解析出其参数
	Return: 
	*************************************************/
	void TranslateURL(REQ_PACKET *packet, const char *pURL);
private:
	static CParseHttpPacket *m_pThis;
	static char m_ppMethod[METHOD_TOTAL][METHOD_SIZE];
	static char m_ppVerson[VERSON_TOTAL][VERSON_SIZE];
public:
	static char m_ppStatusCode[STATUSCODE_TOTAL][STATUSCODE_LEN];
	static char m_ppScDesc[STATUSCODE_TOTAL][SC_DESC_LEN];
};

#define PARSEHANDLE CParseHttpPacket::GetInstance()