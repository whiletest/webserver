/*************************************************
Copyright
File name: MyServer.cpp
Author: ÕÅ³¯Ðñ
Version: 
Date: 2012/11/22
*************************************************/
#include "StdAfx.h"
#include "MyServer.h"
#include "ParseHttpPacket.h"
#include "MyResHttpPacket.h"
#include "MainDlg.h"

extern CMainDlg g_MainDlg;

CMyServer::CMyServer(void)
{
}

CMyServer::~CMyServer(void)
{
}

void CMyServer::OnConnectionEstablished(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
	OnReadCompleted(pContext, pBuffer);
}

void CMyServer::OnConnectionClosing(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}

void CMyServer::OnConnectionError(CIOCPContext *pContext, CIOCPBuffer *pBuffer, int nError)
{

}

void CMyServer::OnReadCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
	REQ_PACKET ReqPacket = PARSEHANDLE->ParsePacket(pBuffer->buff, pBuffer->nLen);

	CMyResHttpPacket response;
	RES_PACKET ResPacket = response.ResponsePacket(&ReqPacket);

	UINT nLen;
	PARSEHANDLE->PackagePacket(0, &nLen, &ResPacket);
	char *buff = new char[nLen];
	PARSEHANDLE->PackagePacket(buff, &nLen, &ResPacket);

	SendText(pContext, buff, nLen);
	delete []buff;

	char *pIpAddr = inet_ntoa(pContext->addrRemote.sin_addr);
	g_MainDlg.AddConnectNum();
	g_MainDlg.WriteLog(pIpAddr, ReqPacket.szURL,
		CParseHttpPacket::m_ppStatusCode[ResPacket.StatusCode], nLen);

#ifdef _UNICODE
	TCHAR sztIp[16];
	MultiByteToWideChar(CP_ACP, 0, pIpAddr, strlen(pIpAddr)+1, sztIp, strlen(pIpAddr)+1);
	TCHAR sztURL[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, ReqPacket.szURL, strlen(ReqPacket.szURL)+1,
		sztURL, strlen(ReqPacket.szURL)+1);
	TCHAR sztStatusCode[STATUSCODE_LEN];
	MultiByteToWideChar(CP_ACP, 0, CParseHttpPacket::m_ppStatusCode[ResPacket.StatusCode],
		STATUSCODE_LEN, sztStatusCode, STATUSCODE_LEN);
	g_MainDlg.AddItemToList(sztIp, sztURL, sztStatusCode, nLen);
#else
	g_MainDlg.AddItemToList(pIpAddr, ReqPacket.szURL,
		CParseHttpPacket::m_ppStatusCode[ResPacket.StatusCode], nLen);
#endif
}

void CMyServer::OnWriteCompleted(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{

}

bool CMyServer::SplitPacket(CIOCPContext *pContext, CIOCPBuffer *pBuffer)
{
	return true;
}

void CMyServer::OnPacketError()
{
}
