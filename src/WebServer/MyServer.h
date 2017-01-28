/*************************************************
Copyright
File name: MyServer.h
Author: 张朝旭
Version: 
Date: 2012/11/22
Description: 
派生于CIOCPServer抽象类，实现网络事件发生时的处理函数
History: 
1. 
Date:
Author:
Modification:
*************************************************/
#pragma once
#include "iocp.h"

class CMyServer :
	public CIOCPServer
{
public:
	CMyServer(void);
	virtual ~CMyServer(void);
protected:
	// 事件通知函数
	// 建立了一个新的连接
	virtual void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	// 一个连接关闭
	virtual void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	// 在一个连接上发生了错误
	virtual void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer, int nError);
	// 一个连接上的读操作完成
	virtual void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	// 一个连接上的写操作完成
	virtual void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	//拆包处理过程
	virtual bool SplitPacket(CIOCPContext *pContext, CIOCPBuffer *pBuffer);

	virtual void OnPacketError();
};
