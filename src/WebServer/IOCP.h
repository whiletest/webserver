/*************************************************
Copyright
File name: IOCP.h
Author: �ų���
Version: 
Date: 2012/11/22
Description: 
����ɶ˿�ģ�ͣ��ܹ��첽�ķ������ˡ������࣬���������¼�����ʱ����������д���
History: 
1. 
Date:
Author:
Modification:
*************************************************/
#pragma once

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <Mswsock.h>
#define USE_DATA_LONGTH 8012
#define NET_DATA_LONGTH 4096
#define BUFFER_SIZE 1024*10     // I/O����Ļ�������С //1024*4	
#define MAX_THREAD	2			// I/O�����̵߳�����


// ����per-I/O���ݡ������������׽����ϴ���I/O�����ı�Ҫ��Ϣ
struct CIOCPBuffer
{
	WSAOVERLAPPED ol;

	SOCKET sClient;			// AcceptEx���յĿͻ����׽���

	char *buff;				// I/O����ʹ�õĻ�����
	int nLen;				// buff��������ʹ�õģ���С

	ULONG nSequenceNumber;	// ��I/O�����к�

	int nOperation;			// ��������
#define OP_ACCEPT	1
#define OP_WRITE	2
#define OP_READ		3
	CIOCPBuffer *pNext;
};

// ����per-Handle���ݡ���������һ���׽��ֵ���Ϣ
struct CIOCPContext
{
	SOCKET s;						// �׽��־��

	SOCKADDR_IN addrLocal;			// ���ӵı��ص�ַ
	SOCKADDR_IN addrRemote;			// ���ӵ�Զ�̵�ַ

	BOOL bClosing;					// �׽����Ƿ�ر�

	int nOutstandingRecv;			// ���׽������׳����ص�����������
	int nOutstandingSend;


	ULONG nReadSequence;			// ���Ÿ����յ���һ�����к�
	ULONG nCurrentReadSequence;		// ��ǰҪ�������к�
	CIOCPBuffer *pOutOfOrderReads;	// ��¼û�а�˳����ɵĶ�I/O
	BYTE* lpBufBegin; //ƴ��������ͷָ��
    BYTE* lpBufEnd; //ƴ��������βָ��
    BYTE arrayDataBuf[USE_DATA_LONGTH]; //ƴ��������
	CRITICAL_SECTION Lock;			// ��������ṹ
	CIOCPContext *pNext;
};


class CIOCPServer   // �����߳�
{
public:
	CIOCPServer();
	virtual ~CIOCPServer();
	/*************************************************
	Function: Start
	Description: ���������������������������߳�
	Input: 
	Output: 
	Return: �����ɹ�����TRUE,����FALSE
	*************************************************/
	BOOL Start(int nPort = 12984, int nMaxConnections = 2000, 
			int nMaxFreeBuffers = 200, int nMaxFreeContexts = 100, int nInitialReads = 4);
	// ֹͣ����
	void Shutdown();

	// �ر�һ�����Ӻ͹ر���������
	void CloseAConnection(CIOCPContext *pContext);
	void CloseAllConnections();	

	// ȡ�õ�ǰ����������
	ULONG GetCurrentConnection() { return m_nCurrentConnection; }

	// ��ָ���ͻ������ı�
	BOOL SendText(CIOCPContext *pContext, char *pszText, int nLen); 
protected:

	// ������ͷŻ���������
	CIOCPBuffer *AllocateBuffer(int nLen);
	void ReleaseBuffer(CIOCPBuffer *pBuffer);

	// ������ͷ��׽���������
	CIOCPContext *AllocateContext(SOCKET s);
	void ReleaseContext(CIOCPContext *pContext);

	// �ͷſ��л����������б�Ϳ��������Ķ����б�
	void FreeBuffers();
	void FreeContexts();

	// �������б������һ������
	BOOL AddAConnection(CIOCPContext *pContext);

	// ������Ƴ�δ���Ľ�������
	BOOL InsertPendingAccept(CIOCPBuffer *pBuffer);
	BOOL RemovePendingAccept(CIOCPBuffer *pBuffer);

	// ȡ����һ��Ҫ��ȡ��
	CIOCPBuffer *GetNextReadBuffer(CIOCPContext *pContext, CIOCPBuffer *pBuffer);


	// Ͷ�ݽ���I/O������I/O������I/O
	BOOL PostAccept(CIOCPBuffer *pBuffer);
	BOOL PostSend(CIOCPContext *pContext, CIOCPBuffer *pBuffer);
	BOOL PostRecv(CIOCPContext *pContext, CIOCPBuffer *pBuffer);

	void HandleIO(DWORD dwKey, CIOCPBuffer *pBuffer, DWORD dwTrans, int nError);


		// �¼�֪ͨ����
	// ������һ���µ�����
	virtual void OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer) = 0;
	// һ�����ӹر�
	virtual void OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer) = 0;
	// ��һ�������Ϸ����˴���
	virtual void OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer, int nError) = 0;
	// һ�������ϵĶ��������
	virtual void OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)=0;
	// һ�������ϵ�д�������
	virtual void OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)=0;
	//����������
	virtual bool SplitPacket(CIOCPContext *pContext, CIOCPBuffer *pBuffer)=0;

	virtual void OnPacketError()=0;

protected:

	// ��¼���нṹ��Ϣ
	CIOCPBuffer *m_pFreeBufferList;
	CIOCPContext *m_pFreeContextList;
	int m_nFreeBufferCount;
	int m_nFreeContextCount;	
	CRITICAL_SECTION m_FreeBufferListLock;
	CRITICAL_SECTION m_FreeContextListLock;

	// ��¼�׳���Accept����
	CIOCPBuffer *m_pPendingAccepts;   // �׳������б�
	long m_nPendingAcceptCount;
	CRITICAL_SECTION m_PendingAcceptsLock;

	// ��¼�����б�
	CIOCPContext *m_pConnectionList;
	int m_nCurrentConnection;
	CRITICAL_SECTION m_ConnectionListLock;

	// ����Ͷ��Accept����
	HANDLE m_hAcceptEvent;
	HANDLE m_hRepostEvent;
	LONG m_nRepostCount;

	int m_nPort;				// �����������Ķ˿�

	int m_nInitialAccepts;
	int m_nInitialReads;
	int m_nMaxAccepts;
	int m_nMaxSends;
	int m_nMaxFreeBuffers;
	int m_nMaxFreeContexts;
	int m_nMaxConnections;

	HANDLE m_hListenThread;			// �����߳�
	HANDLE m_hCompletion;			// ��ɶ˿ھ��
	SOCKET m_sListen;				// �����׽��־��
	LPFN_ACCEPTEX m_lpfnAcceptEx;	// AcceptEx������ַ
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockaddrs; // GetAcceptExSockaddrs������ַ

	BOOL m_bShutDown;		// ����֪ͨ�����߳��˳�
	BOOL m_bServerStarted;	// ��¼�����Ƿ�����


private:	// �̺߳���
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);
};