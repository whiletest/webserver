/*************************************************
Copyright
File name: maindlg.h
Author: 张朝旭
Version: 
Date: 2012/11/26
Description: 
	对界面进行管理。
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
	Description: 显示模态对话框
	Input: 
	Output: 
	Return: 
	*************************************************/
	int DoModal();
	/*************************************************
	Function: AddItemToList
	Description: 添加一项到列表
	Input:
		Ip 客户度IP地址
		pURL 访问文档路径
		pStatus 返回状态码
		nBytesTrans 传输字节数
	Output: 
	Return: 
	*************************************************/
	void AddItemToList(TCHAR *Ip, TCHAR *pURL, TCHAR *pStatus, UINT nBytesTrans);
	/*************************************************
	Function: AddConnectNum
	Description: 增加一条访问记数
	Input: 
	Output: 
	Return: 
	*************************************************/
	void AddConnectNum() {m_cConnNum++;ShowConnectNum();}
	/*************************************************
	Function: WriteLog
	Description: 写日志
	Input: 
		Ip 客户度IP地址
		pURL 访问文档路径
		pStatus 返回状态码
		nBytesTrans 传输字节数
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
	Description: 响应对话框初始化消息
	Input: hwnd 对话框窗口句柄
	Output: 
	Return: 
	*************************************************/
	void OnInitialDlg(HWND hwnd);
	/*************************************************
	Function: OnClickSwitch
	Description: 响应点击监听开关按钮消息
	Input: 
	Output: 
	Return: 
	*************************************************/
	void OnClickSwitch();
	/*************************************************
	Function: OnClickConfig
	Description: 点击配置按钮消息
	Input: 
	Output: 
	Return: 
	*************************************************/
	void OnClickConfig();
	/*************************************************
	Function: OnClickScanPath
	Description: 响应点击浏览按钮消息
	Input: 
	Output: 
	Return: 
	*************************************************/
	void OnClickScanPath();
	/*************************************************
	Function: WriteConfig
	Description: 将配置数据写入配置文件
	Input: 
	Output: 
	Return: 
	*************************************************/
	void OpenConfigPanel(bool bOpen = true);
	/*************************************************
	Function: CheckRecordSetting
	Description: 管理记录配置数据
	Input: 
		bOpen 打开配置面板为true,关闭为false
	Output: 
	Return: 
	*************************************************/
	void CheckRecordSetting(bool bOpen);
	/*************************************************
	Function: CheckShowSetting
	Description: 管理显示配置数据
	Input: 
		bOpen 打开配置面板为true,关闭为false
	Output: 
	Return: 
	*************************************************/
	void CheckShowSetting(bool bOpen);
	/*************************************************
	Function: CheckScriptSetting
	Description: 管理脚本配置数据
	Input: 
		bOpen 打开配置面板为true,关闭为false
	Output: 
	Return: 
	*************************************************/
	void CheckScriptSetting(bool bOpen);
	/*************************************************
	Function: CheckRootPathSetting
	Description: 管理根目录配置数据
	Input: 
		bOpen 打开配置面板为true,关闭为false
	Output: 
	Return: 
	*************************************************/
	void CheckRootPathSetting(bool bOpen);
	/*************************************************
	Function: InitilReportList
	Description: 初始化列表框
	Input: 
	Output: 
	Return: 
	*************************************************/
	void InitilReportList();
	/*************************************************
	Function: ShowConnectNum
	Description: 显示历史连接数
	Input: 
	Output: 
	Return: 
	*************************************************/
	void ShowConnectNum();
	/*************************************************
	Function: DlgProc
	Description: 对话框响应过程函数
	Input: 
	Output: 
	Return: 
	*************************************************/
	static INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
};
