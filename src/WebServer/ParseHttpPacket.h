/*************************************************
Copyright
File name: ParseHttpPacket.h
Author: �ų���
Version: 
Date: 2012/11/22
Description: 
ʹ�õ���ģʽ������httpЭ�飬����httpЭ����Ľ�������װ��
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
	Description: ����ȫ�ֵĲ���Ψһ��ʵ��������ָ�룬���ҷ�����
	Input: 
	Output: 
	Return: �����͵�ʵ���������ָ��
	*************************************************/
	static CParseHttpPacket* GetInstance()
	{
		if (!m_pThis)
			m_pThis = new CParseHttpPacket;
		return m_pThis;
	}
	/*************************************************
	Function: ParsePacket
	Description: ����һ��httpЭ��Ļ���������������н���������һ���ͻ��������
	Input: 
		buff httpЭ��Ļ�����
		nLen ��������С
	Output: 
	Return: �ͻ��������
	*************************************************/
	REQ_PACKET ParsePacket(char *buff, int nLen);
	/*************************************************
	Function: PackagePacket
	Description: ����һ���������Ӧ�������Ӧ���������װ������httpЭ���ʽ�Ļ�����
	Input: 
		packet ��������Ӧ�������Ӧ��
	Output: 
		buff ��װ�������httpЭ���ʽ�Ļ�������buffΪ0ʱ����������pLenBytes
		pLenBytes �������Ĵ�С
	Return: 
	*************************************************/
	void PackagePacket(char *buff, UINT *pLenBytes, const RES_PACKET *packet);
private:
	/*************************************************
	Function: CheckMethod
	Description: ��httpЭ�黺�������ַ���ʽ�ķ�����ת��ΪUCHAR��ʾ������
	Input: pMethod ���������ַ���ʽ�ķ�����
	Output: 
	Return: UCHAR��ʾ�ķ���������
	*************************************************/
	UCHAR CheckMethod(const char *pMethod);
	/*************************************************
	Function: CheckVerson
	Description: ��httpЭ�黺�������ַ���ʽ�İ汾��ת��ΪUCHAR��ʾ������
	Input: pVerson ���������ַ���ʽ�İ汾��
	Output: 
	Return: UCHAR��ʾ�����͵İ�����
	*************************************************/
	UCHAR CheckVerson(const char *pVerson);
	/*************************************************
	Function: TranslateURL
	Description: ��URL·��ת��Ϊ�����ļ�Ŀ¼��ʽ��·������:/index.html->\\index.html
	Input: pURL URL·��
	Output: packet �����ļ�Ŀ¼��ʽ��·��,����������ʶ�̬�ĵ��������������
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