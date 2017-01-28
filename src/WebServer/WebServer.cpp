// WebServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WebServer.h"
#include "MainDlg.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE g_hInst;
CMainDlg g_MainDlg;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	g_hInst = hInstance;

	
	g_MainDlg.DoModal();

 	return 0;
}

