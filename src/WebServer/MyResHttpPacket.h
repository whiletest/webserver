/*************************************************
Copyright
File name: ResHttpPacket.h
Author: 张朝旭
Version: 
Date: 2012/12/02
Description: 
派生类，实现具体的返回静态文档，动态文档响应包函数
History: 
1. 
Date:
Author:
Modification:
*************************************************/
#pragma once
#include "reshttppacket.h"

class CMyResHttpPacket :
	public CResHttpPacket
{
	enum
	{
		FILE_TYPE_HTML,
		FILE_TYPE_SIMPLE_C //simple c
	};
public:
	CMyResHttpPacket(void);
	virtual ~CMyResHttpPacket(void);
protected:
	void ResponseHtml(const char *pFileName, UCHAR verson, RES_PACKET *packet);
	void ResponseSimpleC(const char *pFileName, const vector<string> &vecParm, RES_PACKET *packet);
//////////////////////////////////////////////////////////////////////////
	virtual void Responde404(const REQ_PACKET *pReqPacket, RES_PACKET *packet);
	virtual void Responde202(const REQ_PACKET *pReqPacket, RES_PACKET *packet);
};
