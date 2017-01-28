/*************************************************
Copyright
File name: MyServer.h
Author: �ų���
Version: 
Date: 2012/11/22
Description: 
������CIOCPServer�����࣬ʵ�������¼�����ʱ�Ĵ�����
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
	// �¼�֪ͨ����
	// ������һ���µ�����
	virtual void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	// һ�����ӹر�
	virtual void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	// ��һ�������Ϸ����˴���
	virtual void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer, int nError);
	// һ�������ϵĶ��������
	virtual void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	// һ�������ϵ�д�������
	virtual void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	//����������
	virtual bool SplitPacket(CIOCPContext *pContext, CIOCPBuffer *pBuffer);

	virtual void OnPacketError();
};
