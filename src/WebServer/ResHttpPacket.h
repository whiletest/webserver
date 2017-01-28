/*************************************************
Copyright
File name: ResHttpPacket.h
Author: �ų���
Version: 
Date: 2012/11/22
Description: 
���յ�һ���ͻ��������(REQ_PACKET)����������һ����Ӧ��(RES_PACKET)
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
	Description: ���յ�һ���ͻ��������(REQ_PACKET)����������һ����Ӧ��(RES_PACKET)
	Input: const REQ_PACKET* �ͻ���������������
	Output: 
	Return: RES_PACKET �������Ӧ�������Ӧ��
	*************************************************/
	RES_PACKET ResponsePacket(const REQ_PACKET *pReqPacket);
protected:
	/*************************************************
	Function: CheckURL
	Description: ���URL·���Ƿ�Ϸ�(�ڸ�Ŀ¼��)
	Input: const char *pURL �ͻ��������ĵ���·��
	Output: 
	Return: ·���Ϸ�����true,���򷵻�false
	*************************************************/
	bool CheckURL(const char *pURL); 
	/*************************************************
	Function: GetDocument
	Description: ��ȡ�ͻ���������ĵ�
	Input: cpReqPacket �ͻ��������İ�
	Output: RES_PACKET *pResPacket ��װ�õ���Ӧ�ͻ����������Ӧ��
	Return:
	*************************************************/
	void GetDocument(const REQ_PACKET *pReqPacket, RES_PACKET *pResPacket);
	/*************************************************
	Function: GetAbsolutePath
	Description: ��URL·��װ��Ϊ�ڸ�Ŀ¼�µľ���·��
	Input: const char *pURL �ͻ��������ĵ���·��
	Output: 
		char *pAbsolutePath ת����ľ���·��,Ϊ0�Ǻ�������·������pLen
		UINT *pLen ����·������
	Return:
	*************************************************/
	void GetAbsolutePath(char *pAbsolutePath, UINT *pLen, const char *pURL);
	/*************************************************
	Function: GetSuffixName
	Description: ��ȡ�����ĵ��ĺ�׺��
	Input: const char *pURL �ͻ��������ĵ���·��
	Output: char *pSuffixName ��׺�����ַ�����'\0'����
	Return:
	*************************************************/
	void GetSuffixName(const char *pURL, char *pSuffixName);
	/*************************************************
	Function: Responde404
	Description: �����ĵ�ʧ�ܵĴ�����
	Input: 
	Output: RES_PACKET *packet ��Ӧ��
	Return:
	*************************************************/
	virtual void Responde404(const REQ_PACKET *pReqPacket, RES_PACKET *packet) = 0;
	/*************************************************
	Function: Responde202
	Description: �����ĵ��ɹ��Ĵ�����
	Input: const char *pURL �����ĵ���·��
	Output: RES_PACKET *packet ��Ӧ��
	Return:
	*************************************************/
	virtual void Responde202(const REQ_PACKET *pReqPacket, RES_PACKET *packet) = 0;
	/*************************************************
	Function: RespondeText
	Description: ��Ӧ�ض�����Ϣ���ͻ���
	Input:  pText �ض��ı���Ϣ
	Output: packet ��Ӧ��
	Return:
	*************************************************/
	virtual void RespondeText(const REQ_PACKET *pReqPacket, const char *pText, RES_PACKET *packet);
};