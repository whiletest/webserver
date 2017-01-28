/*************************************************
Copyright
File name: config.h
Author: �ų���
Version: 
Date: 2012/11/26
Description: 
	�������ļ�config.txt���й���
History: 
1. 
Date:
Author:
Modification:
*************************************************/
#pragma once

enum
{
	RECORD_NONE = 0x00,   //��ѡ����
	RECORD_ONLIST = 0x01, //��ʾ���б�
	RECORD_LOG = 0x02,    //��¼��־
	RECORD_CONNECTNUM = 0x04 //��ʾ��ʷ������
};

enum
{
	LIST_NONE = 0x00, //��ѡ����
	LIST_URL = 0x01, //��ʾURL
	LIST_STATUS = 0x02, //��ʾ״̬��
	LIST_REMOTEIP = 0x04, //��ʾ�ͻ���ip
	LIST_BYTES = 0x08, //��ʾ�����ֽ���
	LIST_DATE = 0x10 //��ʾ����ʱ��
};

enum
{
	SCRIPT_NONE = 0x00, //��ѡ����
	SCRIPT_SIMPLEC = 0x01, //����simple C�ű�
	SCRIPT_STATICHTML = 0x02 //���þ�̬html�ĵ�
};

#define RESET_WHOLE(x)    ((x) &= 0x00)
#define RESET_BIT(x,y)    ((x) &= ~(y))
#define SET_BIT(x,y)      ((x) |= (y))
#define ISSET_BIT(x,y)    ((x) & (y))

struct CONFIG
{
	UINT record;               //��¼����
	UINT list;                 //�б���ʾ����
	UINT script;               //֧�ֽű�����
	char szRootPath[MAX_PATH]; //��Ŀ¼����
};

class CConfig
{
//�����ļ�·��
#define CONFIG_FILE "config.txt"
public:
	CConfig(void);
	~CConfig(void);
	/*************************************************
	Function: WriteConfig
	Description: ����������д�������ļ�
	Input: 
	Output: 
	Return: 
	*************************************************/
	void WriteConfig();
	/*************************************************
	Function: ReadConfig
	Description: ���������ݴ������ļ��ж���
	Input: 
	Output: 
	Return: ��ȡ�ɹ�����true,���򷵻�false
	*************************************************/
	bool ReadConfig(); 
public:
	CONFIG m_cfg; //��������
};
