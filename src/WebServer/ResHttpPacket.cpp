/*************************************************
Copyright
File name: ResHttpPacket.cpp
Author: ’≈≥Ø–Ò
Version: 
Date: 2012/11/22
*************************************************/
#include "StdAfx.h"
#include "ResHttpPacket.h"
#include "Config.h"
#include "IOCP.h"
#include "MainDlg.h"
#include <fstream>
using std::ifstream;

extern CMainDlg g_MainDlg;

CResHttpPacket::CResHttpPacket(void)
{
}

CResHttpPacket::~CResHttpPacket(void)
{
}

RES_PACKET CResHttpPacket::ResponsePacket(const REQ_PACKET *pReqPacket)
{
	RES_PACKET ResPacket;
	if (pReqPacket)
	{
		if (pReqPacket->method == METHOD_GET)
		{
			GetDocument(pReqPacket, &ResPacket);
		}
	}

	return ResPacket;
}

bool CResHttpPacket::CheckURL(const char *pURL)
{
	UINT nLenBytes;
	GetAbsolutePath(0, &nLenBytes, pURL);
	char *pDocPath = new char[nLenBytes];
	GetAbsolutePath(pDocPath, &nLenBytes, pURL);
	std::ifstream in(pDocPath, std::ios::in);
	delete []pDocPath;
	if (in)
	{
		in.close();
		return true;
	}

	return false;
}

void CResHttpPacket::GetAbsolutePath(char *pAbsolutePath, UINT *pLen, const char *pURL)
{
	if (pAbsolutePath)
	{
		memcpy(pAbsolutePath, g_MainDlg.m_pCfg->m_cfg.szRootPath,
			strlen(g_MainDlg.m_pCfg->m_cfg.szRootPath));
		strcpy_s(pAbsolutePath+strlen(g_MainDlg.m_pCfg->m_cfg.szRootPath), 
			strlen(pURL)+1, pURL);
	}
	else
	{
		*pLen = strlen(g_MainDlg.m_pCfg->m_cfg.szRootPath) + strlen(pURL) + 1;
	}
}

void CResHttpPacket::GetSuffixName(const char *pURL, char *pSuffixName)
{
	while (*pURL && *pURL != '.')
		pURL++;
	while (*pURL)
		*pSuffixName++ = *pURL++;
	*pSuffixName = '\0';
}

void CResHttpPacket::GetDocument(const REQ_PACKET *pReqPacket, RES_PACKET *pResPacket)
{
	if (pResPacket)
	{
		if (CheckURL(pReqPacket->szURL))
			Responde202(pReqPacket, pResPacket);
		else
			Responde404(pReqPacket, pResPacket);
	}
}

void CResHttpPacket::RespondeText(const REQ_PACKET *pReqPacket, const char *pText, RES_PACKET *packet)
{
	packet->verson = pReqPacket->verson;
	packet->StatusCode = STATUSCODE_404;
	char szContent[BUFFER_SIZE];
	sprintf_s(szContent, BUFFER_SIZE,
		"<html>\
		<title>Error</title>\
		<body>\
		<h1>ERROR:404</h1>\
		<p><h3>%s</h3></p>\
		</body>\
		</html>",
		pText);
	packet->nBodyBytes = strlen(szContent);
	packet->pEntityBody = new char[packet->nBodyBytes];
	char szLen[10];
	sprintf_s(szLen, sizeof(szLen), "%d", strlen(szContent));
	packet->HeadLine.insert(std::pair<string,string>("Content-length", szLen));
	packet->HeadLine.insert(std::pair<string,string>("Content-type","text/html"));
	memcpy(packet->pEntityBody, szContent, packet->nBodyBytes);
}