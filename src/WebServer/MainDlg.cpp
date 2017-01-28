/*************************************************
Copyright
File name: MainDlg.cpp
Author: 张朝旭
Version: 
Date: 2012/11/26
*************************************************/
#include "StdAfx.h"
#include "Resource.h"
#include "MainDlg.h"
#include "MyServer.h"
#include "Config.h"
#include "packet.h"
#include <CommCtrl.h>
#include <commdlg.h>
#include <ctime>
#include <fstream>
using std::ofstream;

extern HINSTANCE g_hInst;

CMainDlg::CMainDlg(HWND hParentWnd)
:m_hParentWnd(hParentWnd),
m_WndHeight(0),m_WndWidth(0),
m_cConnNum(0),
m_bSwitch(false),
m_bConfig(false),
m_pServ(new CMyServer),
m_pCfg(new CConfig)
{
}

CMainDlg::~CMainDlg(void)
{
	if (m_pServ)
		delete m_pServ;
	if (m_pCfg)
	{
		m_pCfg->WriteConfig();
		delete m_pCfg;
	}
}

int CMainDlg::DoModal()
{
	return DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_MAINDLG),
		m_hParentWnd, DlgProc, (LPARAM)this);
}

void CMainDlg::OnInitialDlg(HWND hwnd)
{
	m_hwnd = hwnd;

	GetWindowRect(m_hwnd, &m_rcConfigOn);
	HWND hDvideWnd = GetDlgItem(m_hwnd, IDC_DIVIDE);
	RECT rcTem;
	GetWindowRect(hDvideWnd, &rcTem);
	SetRect(&m_rcConfigOff, 0, 0, m_rcConfigOn.right, rcTem.top);
	
	if (!m_pCfg->ReadConfig())
		MessageBox(m_hwnd, TEXT("读取配置文件失败"), 0, 0);

	HWND hListWnd = GetDlgItem(m_hwnd, IDC_LIST_REPORT);
	ListView_SetExtendedListViewStyleEx(hListWnd, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	ShowConnectNum();
	OpenConfigPanel(false);

	int cxScreen = GetSystemMetrics(SM_CXFULLSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYFULLSCREEN);
	MoveWindow(hwnd, cxScreen/2-m_WndWidth/2, cyScreen/2-m_WndHeight/2,
		m_WndWidth, m_WndHeight, FALSE);
}

void CMainDlg::OnClickSwitch()
{
	m_bSwitch = !m_bSwitch;
	if (m_bSwitch)
	{
		m_pServ->Start(HTTP_PORT);
		SetDlgItemText(m_hwnd, ID_SWITCH, TEXT("关闭监听"));
		EnableWindow(GetDlgItem(m_hwnd, ID_CONFIG), FALSE);
	}
	else
	{
		m_pServ->Shutdown();
		SetDlgItemText(m_hwnd, ID_SWITCH, TEXT("启动监听"));
		EnableWindow(GetDlgItem(m_hwnd, ID_CONFIG), TRUE);
	}
}

void CMainDlg::OnClickConfig()
{
	m_bConfig = !m_bConfig;
	if (m_bConfig)
	{
		SetDlgItemText(m_hwnd, ID_CONFIG, TEXT("应用"));
		EnableWindow(GetDlgItem(m_hwnd, ID_SWITCH), FALSE);
	}
	else 
	{
		SetDlgItemText(m_hwnd, ID_CONFIG, TEXT("设置"));
		EnableWindow(GetDlgItem(m_hwnd, ID_SWITCH), TRUE);
	}

	CheckRecordSetting(m_bConfig);
	CheckShowSetting(m_bConfig);
	CheckScriptSetting(m_bConfig);
	CheckRootPathSetting(m_bConfig);
	ShowConnectNum();
	OpenConfigPanel(m_bConfig);
}

void CMainDlg::OnClickScanPath()
{
	OPENFILENAMEA ofn;
	ofn.lStructSize = sizeof (OPENFILENAME) ;
	ofn.hwndOwner = m_hwnd ;
	ofn.hInstance = NULL ;
	ofn.lpstrFilter = 0 ;
	ofn.lpstrCustomFilter = NULL ;
	ofn.nMaxCustFilter = 0 ;
	ofn.nFilterIndex = 0 ;
	ofn.lpstrFile = m_pCfg->m_cfg.szRootPath;
	strcpy_s(ofn.lpstrFile, MAX_PATH, "nouse.txt");
	ofn.nMaxFile = MAX_PATH ;
	ofn.lpstrFileTitle = NULL ;         
	ofn.nMaxFileTitle = MAX_PATH ;
	ofn.lpstrInitialDir = NULL ;
	ofn.lpstrTitle = NULL ;
	ofn.Flags = OFN_HIDEREADONLY;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = 0;
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL ;
	ofn.lpTemplateName = NULL ;

	char szCurDir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, szCurDir);
	GetOpenFileNameA(&ofn);
	SetCurrentDirectoryA(szCurDir);

	ofn.lpstrFile[strlen(ofn.lpstrFile)-strlen("nouse.txt")-1] = '\0';
	SetDlgItemTextA(m_hwnd, IDC_EDIT_ROOTPATH, ofn.lpstrFile);
}

void CMainDlg::OpenConfigPanel(bool bOpen)
{
	RECT rcDlg;
	GetWindowRect(m_hwnd, &rcDlg);
	if (bOpen)
	{
		m_WndWidth = m_rcConfigOn.right;
		m_WndHeight = m_rcConfigOn.bottom;
	}
	else
	{
		m_WndWidth = m_rcConfigOff.right;
		m_WndHeight = m_rcConfigOff.bottom;

		InitilReportList();
	}

	MoveWindow(m_hwnd, rcDlg.left, rcDlg.top, m_WndWidth, m_WndHeight, TRUE);
}

void CMainDlg::AddItemToList(TCHAR *Ip, TCHAR *pURL, TCHAR *pStatus, UINT nBytesTrans)
{
	if (!ISSET_BIT(m_pCfg->m_cfg.record, RECORD_ONLIST))
		return;

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = MAX_PATH;
	lvi.pszText = 0;
	HWND hListWnd = GetDlgItem(m_hwnd, IDC_LIST_REPORT);
	lvi.iItem = ListView_GetItemCount(hListWnd);
	lvi.iSubItem = 0;
	ListView_InsertItem(hListWnd, &lvi);

	TCHAR szText[5][MAX_PATH];
	_tcscpy_s(szText[0], _tcslen(Ip)+1, Ip);
	_tcscpy_s(szText[1], _tcslen(pURL)+1, pURL);
	_tcscpy_s(szText[2], STATUSCODE_LEN, pStatus);
	_stprintf_s(szText[3], 10, TEXT("%d"), nBytesTrans);
	time_t tm;
	tm = time(&tm);
	char szTime[TIMEBUFF_LEN];
	ctime_s(szTime, TIMEBUFF_LEN, &tm);
	int nLen = strlen(szTime);
	MultiByteToWideChar(CP_ACP, 0, szTime, nLen+1, szText[4], nLen+1);

	int cSubItem = 0;
	const int nCols = 5;
	UINT bits[nCols] = {LIST_REMOTEIP, LIST_URL, 
		LIST_STATUS,LIST_BYTES, LIST_DATE};
	for (int i = 0;i != nCols;i++)
	{
		if (ISSET_BIT(m_pCfg->m_cfg.list, bits[i]))
		{
			ListView_SetItemText(hListWnd, lvi.iItem, cSubItem, szText[i]);
			cSubItem++;
		}
	}
}

void CMainDlg::InitilReportList()
{
	HWND hListWnd = GetDlgItem(m_hwnd, IDC_LIST_REPORT);
	ListView_DeleteAllItems(hListWnd);
	while (ListView_DeleteColumn(hListWnd, 0));

	const int nCol = 5;
	const int nMaxLen = 20;
	TCHAR szColumn[nCol][nMaxLen] = {TEXT("客户端IP地址"),TEXT("URL路径"),
		TEXT("状态码"),TEXT("字节数"),TEXT("日期时间")};
	int cColWidth[nCol] = {100, 200, 50, 50, 150};
	UINT bits[nCol] = {LIST_REMOTEIP, LIST_URL, 
		LIST_STATUS,LIST_BYTES, LIST_DATE};
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	lvc.cchTextMax = nMaxLen;
	for (int i = 0;i != nCol;i++)
	{
		if (ISSET_BIT(m_pCfg->m_cfg.list, bits[i]))
		{
			lvc.cx = cColWidth[i];
			lvc.pszText = szColumn[i];
			ListView_InsertColumn(hListWnd, i, &lvc);
		}
	}
}

void CMainDlg::CheckRecordSetting(bool bOpen)
{
	if (bOpen)
	{
		if (ISSET_BIT(m_pCfg->m_cfg.record, RECORD_ONLIST))
			SendDlgItemMessage(m_hwnd, IDC_CHECK_ONLIST, BM_SETCHECK, 1, 0);

		if (ISSET_BIT(m_pCfg->m_cfg.record, RECORD_LOG))
			SendDlgItemMessage(m_hwnd, IDC_CHECK_LOG, BM_SETCHECK, 1, 0);

		if (ISSET_BIT(m_pCfg->m_cfg.record, RECORD_CONNECTNUM))
			SendDlgItemMessage(m_hwnd, IDC_CHECK_RECORDCONN, BM_SETCHECK, 1, 0);
	}
	else
	{
		LRESULT bSelect;
		bSelect = SendDlgItemMessage(m_hwnd, IDC_CHECK_ONLIST, BM_GETCHECK, 0, 0);
		if (bSelect)
			SET_BIT(m_pCfg->m_cfg.record, RECORD_ONLIST);
		else 
			RESET_BIT(m_pCfg->m_cfg.record, RECORD_ONLIST);

		bSelect = SendDlgItemMessage(m_hwnd, IDC_CHECK_LOG, BM_GETCHECK, 0, 0);
		if (bSelect)
			SET_BIT(m_pCfg->m_cfg.record, RECORD_LOG);
		else 
			RESET_BIT(m_pCfg->m_cfg.record, RECORD_LOG);

		bSelect = SendDlgItemMessage(m_hwnd, IDC_CHECK_RECORDCONN, BM_GETCHECK, 0, 0);
		if (bSelect)
			SET_BIT(m_pCfg->m_cfg.record, RECORD_CONNECTNUM);
		else 
			RESET_BIT(m_pCfg->m_cfg.record, RECORD_CONNECTNUM);
	}
}

void CMainDlg::CheckShowSetting(bool bOpen)
{
	if (bOpen)
	{
		if (ISSET_BIT(m_pCfg->m_cfg.list, LIST_URL))
			SendDlgItemMessage(m_hwnd, IDC_CHECK_URL, BM_SETCHECK, 1, 0);

		if (ISSET_BIT(m_pCfg->m_cfg.list, LIST_STATUS))
			SendDlgItemMessage(m_hwnd, IDC_CHECK_STATUS, BM_SETCHECK, 1, 0);

		if (ISSET_BIT(m_pCfg->m_cfg.list, LIST_REMOTEIP))
			SendDlgItemMessage(m_hwnd, IDC_CHECK_IP, BM_SETCHECK, 1, 0);

		if (ISSET_BIT(m_pCfg->m_cfg.list, LIST_BYTES))
			SendDlgItemMessage(m_hwnd, IDC_CHECK_BYTES, BM_SETCHECK, 1, 0);

		if (ISSET_BIT(m_pCfg->m_cfg.list, LIST_DATE))
			SendDlgItemMessage(m_hwnd, IDC_CHECK_DATE, BM_SETCHECK, 1, 0);
	}
	else
	{
		LRESULT bSelect;
		bSelect = SendDlgItemMessage(m_hwnd, IDC_CHECK_URL, BM_GETCHECK, 0, 0);
		if (bSelect)
			SET_BIT(m_pCfg->m_cfg.list, LIST_URL);
		else 
			RESET_BIT(m_pCfg->m_cfg.list, LIST_URL);

		bSelect = SendDlgItemMessage(m_hwnd, IDC_CHECK_STATUS, BM_GETCHECK, 0, 0);
		if (bSelect)
			SET_BIT(m_pCfg->m_cfg.list, LIST_STATUS);
		else 
			RESET_BIT(m_pCfg->m_cfg.list, LIST_STATUS);

		bSelect = SendDlgItemMessage(m_hwnd, IDC_CHECK_IP, BM_GETCHECK, 0, 0);
		if (bSelect)
			SET_BIT(m_pCfg->m_cfg.list, LIST_REMOTEIP);
		else 
			RESET_BIT(m_pCfg->m_cfg.list, LIST_REMOTEIP);

		bSelect = SendDlgItemMessage(m_hwnd, IDC_CHECK_BYTES, BM_GETCHECK, 0, 0);
		if (bSelect)
			SET_BIT(m_pCfg->m_cfg.list, LIST_BYTES);
		else 
			RESET_BIT(m_pCfg->m_cfg.list, LIST_BYTES);

		bSelect = SendDlgItemMessage(m_hwnd, IDC_CHECK_DATE, BM_GETCHECK, 0, 0);
		if (bSelect)
			SET_BIT(m_pCfg->m_cfg.list, LIST_DATE);
		else 
			RESET_BIT(m_pCfg->m_cfg.list, LIST_DATE);
	}
}

void CMainDlg::CheckScriptSetting(bool bOpen)
{
	if (bOpen)
	{
		if (ISSET_BIT(m_pCfg->m_cfg.script, SCRIPT_SIMPLEC))
			SendDlgItemMessage(m_hwnd, IDC_CHECK_SIMPLEC, BM_SETCHECK, 1, 0);

		if (ISSET_BIT(m_pCfg->m_cfg.script, SCRIPT_STATICHTML))
			SendDlgItemMessage(m_hwnd, IDC_CHECK_STATICHTML, BM_SETCHECK, 1, 0);
	}
	else
	{
		LRESULT bSelect;
		bSelect = SendDlgItemMessage(m_hwnd, IDC_CHECK_SIMPLEC, BM_GETCHECK, 0, 0);
		if (bSelect)
			SET_BIT(m_pCfg->m_cfg.script, SCRIPT_SIMPLEC);
		else 
			RESET_BIT(m_pCfg->m_cfg.script, SCRIPT_SIMPLEC);

		bSelect = SendDlgItemMessage(m_hwnd, IDC_CHECK_STATICHTML, BM_GETCHECK, 0, 0);
		if (bSelect)
			SET_BIT(m_pCfg->m_cfg.script, SCRIPT_STATICHTML);
		else 
			RESET_BIT(m_pCfg->m_cfg.script, SCRIPT_STATICHTML);
	}
}

void CMainDlg::CheckRootPathSetting(bool bOpen)
{
	if (bOpen)
	{
		SetDlgItemTextA(m_hwnd, IDC_EDIT_ROOTPATH, m_pCfg->m_cfg.szRootPath);
	}
	else
	{
		GetDlgItemTextA(m_hwnd, IDC_EDIT_ROOTPATH, m_pCfg->m_cfg.szRootPath, MAX_PATH);
	}
}

void CMainDlg::ShowConnectNum()
{
	if (ISSET_BIT(m_pCfg->m_cfg.record, RECORD_CONNECTNUM))
	{
		ShowWindow(GetDlgItem(m_hwnd, IDC_CONNECTNUM), SW_SHOW);
		TCHAR szConn[20];
		_stprintf_s(szConn, 20, TEXT("连接数:%d"), m_cConnNum);
		SetDlgItemText(m_hwnd, IDC_CONNECTNUM, szConn);
	}
	else
	{
		ShowWindow(GetDlgItem(m_hwnd, IDC_CONNECTNUM), SW_HIDE);
	}
}

void CMainDlg::WriteLog(char *Ip, char *pURL, char *pStatus, UINT nBytesTrans)
{
	if (!ISSET_BIT(m_pCfg->m_cfg.record, RECORD_LOG))
		return;

	time_t tm;
	tm = time(&tm);
	char szTime[TIMEBUFF_LEN];
	ctime_s(szTime, TIMEBUFF_LEN, &tm);
	const int nExtra = 6+5;
	char szFileName[TIMEBUFF_LEN+nExtra];
	strcpy_s(szFileName, 7, ".\\log\\");
	strcat_s(szFileName, szTime);
	int nLen = strlen(szTime);
	szFileName[11+6] = szTime[nLen-5];
	szFileName[12+6] = szTime[nLen-4];
	szFileName[13+6] = szTime[nLen-3];
	szFileName[14+6] = szTime[nLen-2];
	szFileName[15+6] = '\0';
	strcat_s(szFileName, ".txt");
	char szBytes[10];
	sprintf_s(szBytes, 10, "%d", nBytesTrans);

	ofstream out(szFileName, std::ios::out | std::ios::app);
	if (out)
	{
		out << szTime;
		out << Ip << '\t' << pURL << '\t'\
			<< pStatus << '\t' << szBytes << "\n\n";
		out.close();
	}
	else
	{
		MessageBox(m_hwnd, TEXT("请在执行文件目录下建立log文件夹"), 0, 0);
	}
}

INT_PTR CALLBACK CMainDlg::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CMainDlg *pThis = 0;
	switch (message)
	{
	case WM_INITDIALOG:
		pThis = (CMainDlg*)lParam;
		pThis->OnInitialDlg(hDlg);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == ID_SWITCH)
		{
			pThis->OnClickSwitch();
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == ID_CONFIG)
		{
			pThis->OnClickConfig();
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == ID_SCANPATH)
		{
			pThis->OnClickScanPath();
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}