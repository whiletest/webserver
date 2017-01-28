/*************************************************
Copyright
File name: maindlg.h
Author: �ų���
Version: 
Date: 2012/11/26
Description: 
	�Խ�����й���
History: 
1. 
Date:
Author:
Modification:
*************************************************/
#pragma once

class CIOCPServer;
class CConfig;

#define HTTP_PORT 80

class CMainDlg
{
#define TIMEBUFF_LEN 30
public:
	CMainDlg(HWND hParentWnd = 0);
	~CMainDlg(void);
	/*************************************************
	Function: DoModal
	Description: ��ʾģ̬�Ի���
	Input: 
	Output: 
	Return: 
	*************************************************/
	int DoModal();
	/*************************************************
	Function: AddItemToList
	Description: ���һ��б�
	Input:
		Ip �ͻ���IP��ַ
		pURL �����ĵ�·��
		pStatus ����״̬��
		nBytesTrans �����ֽ���
	Output: 
	Return: 
	*************************************************/
	void AddItemToList(TCHAR *Ip, TCHAR *pURL, TCHAR *pStatus, UINT nBytesTrans);
	/*************************************************
	Function: AddConnectNum
	Description: ����һ�����ʼ���
	Input: 
	Output: 
	Return: 
	*************************************************/
	void AddConnectNum() {m_cConnNum++;ShowConnectNum();}
	/*************************************************
	Function: WriteLog
	Description: д��־
	Input: 
		Ip �ͻ���IP��ַ
		pURL �����ĵ�·��
		pStatus ����״̬��
		nBytesTrans �����ֽ���
	Output: 
	Return: 
	*************************************************/
	void WriteLog(char *Ip, char *pURL, char *pStatus, UINT nBytesTrans);
public:
	CConfig *m_pCfg;
private:
	HWND m_hwnd;
	HWND m_hParentWnd;
	int m_WndWidth;
	int m_WndHeight;
	UINT m_cConnNum;
	bool m_bSwitch;
	bool m_bConfig;
	RECT m_rcConfigOn;
	RECT m_rcConfigOff;
	CIOCPServer *m_pServ;
	/*************************************************
	Function: OnInitialDlg
	Description: ��Ӧ�Ի����ʼ����Ϣ
	Input: hwnd �Ի��򴰿ھ��
	Output: 
	Return: 
	*************************************************/
	void OnInitialDlg(HWND hwnd);
	/*************************************************
	Function: OnClickSwitch
	Description: ��Ӧ����������ذ�ť��Ϣ
	Input: 
	Output: 
	Return: 
	*************************************************/
	void OnClickSwitch();
	/*************************************************
	Function: OnClickConfig
	Description: ������ð�ť��Ϣ
	Input: 
	Output: 
	Return: 
	*************************************************/
	void OnClickConfig();
	/*************************************************
	Function: OnClickScanPath
	Description: ��Ӧ��������ť��Ϣ
	Input: 
	Output: 
	Return: 
	*************************************************/
	void OnClickScanPath();
	/*************************************************
	Function: WriteConfig
	Description: ����������д�������ļ�
	Input: 
	Output: 
	Return: 
	*************************************************/
	void OpenConfigPanel(bool bOpen = true);
	/*************************************************
	Function: CheckRecordSetting
	Description: �����¼��������
	Input: 
		bOpen ���������Ϊtrue,�ر�Ϊfalse
	Output: 
	Return: 
	*************************************************/
	void CheckRecordSetting(bool bOpen);
	/*************************************************
	Function: CheckShowSetting
	Description: ������ʾ��������
	Input: 
		bOpen ���������Ϊtrue,�ر�Ϊfalse
	Output: 
	Return: 
	*************************************************/
	void CheckShowSetting(bool bOpen);
	/*************************************************
	Function: CheckScriptSetting
	Description: ����ű���������
	Input: 
		bOpen ���������Ϊtrue,�ر�Ϊfalse
	Output: 
	Return: 
	*************************************************/
	void CheckScriptSetting(bool bOpen);
	/*************************************************
	Function: CheckRootPathSetting
	Description: �����Ŀ¼��������
	Input: 
		bOpen ���������Ϊtrue,�ر�Ϊfalse
	Output: 
	Return: 
	*************************************************/
	void CheckRootPathSetting(bool bOpen);
	/*************************************************
	Function: InitilReportList
	Description: ��ʼ���б��
	Input: 
	Output: 
	Return: 
	*************************************************/
	void InitilReportList();
	/*************************************************
	Function: ShowConnectNum
	Description: ��ʾ��ʷ������
	Input: 
	Output: 
	Return: 
	*************************************************/
	void ShowConnectNum();
	/*************************************************
	Function: DlgProc
	Description: �Ի�����Ӧ���̺���
	Input: 
	Output: 
	Return: 
	*************************************************/
	static INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
};
