/*************************************************
Copyright
File name: packet.h
Author: �ų���
Version: 
Date: 2012/11/22
Description: 
ʵ�ֿͻ��˷�����httpЭ������������REQ_PACKET�Ķ���
ʵ�ַ������Կͻ������������Ӧ����Ӧ��RES_PACKET�Ķ���
History: 
1. 
Date:
Author:
Modification:
*************************************************/
#pragma once

#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

#define METHOD_SIZE 10 //���������ַ�����
enum
{
	METHOD_GET,
	METHOD_POST,
	METHOD_TOTAL
};

#define VERSON_SIZE 10
enum
{
	VERSON_1_1,
	VERSON_1_0,
	VERSON_TOTAL
};

typedef struct _REQ_PACKET
{
	UCHAR method; //����
	char szURL[MAX_PATH]; //�ļ�·��
	vector<string> vecParm; //ִ�нű��Ǵ��ݵĲ���(��̬htmlû��)
	UCHAR verson; //Э��汾
	map<string, string> HeadLine; //�ײ���
}REQ_PACKET, *PREQ_PACKET; //request packet


#define STATUSCODE_LEN 4
#define SC_DESC_LEN    20
enum //status code
{
	STATUSCODE_404,
	STATUSCODE_202,
	STATUSCODE_TOTAL
};

typedef struct _RES_PACKET
{
	UCHAR verson; //Э��汾
	UCHAR StatusCode; //״̬��
	map<string, string> HeadLine; //�ײ���
	char *pEntityBody; //ʵ������
	UINT nBodyBytes; //EntityBody�ֽ���
	//////////////////////////////////////////////////////////////////////////
	_RES_PACKET():pEntityBody(0),nBodyBytes(0){}
	//�������캯������ֹǳ����
	_RES_PACKET(const _RES_PACKET &packet)
	{
		verson = packet.verson;
		StatusCode = packet.StatusCode;
		HeadLine.insert(packet.HeadLine.begin(), packet.HeadLine.end());
		nBodyBytes = packet.nBodyBytes;
		if (packet.pEntityBody)
		{
			pEntityBody = new char[nBodyBytes];
			memcpy(pEntityBody, packet.pEntityBody, nBodyBytes);
		}
	}
	//���ظ�ֵ����������ֹǳ����
	_RES_PACKET& operator = (const _RES_PACKET &packet)
	{
		if (this != &packet)
		{
			verson = packet.verson;
			StatusCode = packet.StatusCode;
			HeadLine.clear();
			HeadLine.insert(packet.HeadLine.begin(), packet.HeadLine.end());
			nBodyBytes = packet.nBodyBytes;
			if (pEntityBody)
				delete []pEntityBody;
			pEntityBody = new char[nBodyBytes];
			memcpy(pEntityBody, packet.pEntityBody, nBodyBytes);
		}
		return *this;
	}
	//����
	~_RES_PACKET()
	{
		if (pEntityBody)
		{
			delete []pEntityBody;
			pEntityBody = 0;
		}
	}
	//����ײ����ֽ�
	UINT GetHeadLineBytes()const
	{
		UINT HeadLineBytes = 0;
		for (map<string,string>::const_iterator iter = HeadLine.begin();
			iter != HeadLine.end();iter++)
		{
			HeadLineBytes += iter->first.size() + 2; //:' '
			HeadLineBytes += iter->second.size() + 2; //\r\n
		}
		return HeadLineBytes;
	}
}RES_PACKET, *PRES_PACKET; //response packet


#define CR '\r' //�س�
#define LF '\n' //����