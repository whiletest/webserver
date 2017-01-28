#include "StdAfx.h"
#include "MyResHttpPacket.h"
#include "SimpleCScript.h"
#include "MainDlg.h"
#include "Config.h"
#include <fstream>

extern CMainDlg g_MainDlg;

CMyResHttpPacket::CMyResHttpPacket(void)
{
}

CMyResHttpPacket::~CMyResHttpPacket(void)
{
}

void CMyResHttpPacket::ResponseHtml(const char *pFileName, UCHAR verson, RES_PACKET *packet)
{
	std::ifstream in(pFileName, std::ios::in);
	in.seekg(0, std::ios::end);
	packet->nBodyBytes = in.tellg();
	in.seekg(0, std::ios::beg);

	packet->pEntityBody = new char[packet->nBodyBytes];
	memset(packet->pEntityBody, 0, packet->nBodyBytes);
	in.read(packet->pEntityBody, packet->nBodyBytes);
	in.close();

	packet->verson = verson;
	packet->StatusCode = STATUSCODE_202;
	char szLen[10];
	sprintf_s(szLen, sizeof(szLen), "%d", packet->nBodyBytes);
	packet->HeadLine.insert(std::pair<string,string>("Content-length", szLen));
	packet->HeadLine.insert(std::pair<string,string>("Content-type", "text/html"));
}

void CMyResHttpPacket::ResponseSimpleC(const char *pFileName,
									   const vector<string> &vecParm, 
									   RES_PACKET *packet)
{
	CSimpleCScript scs;
	scs.ExecuteScript(pFileName, vecParm, packet);
}

void CMyResHttpPacket::Responde404(const REQ_PACKET *pReqPacket, RES_PACKET *packet)
{
	packet->verson = pReqPacket->verson;
	packet->StatusCode = STATUSCODE_404;
	char szContent[] = 
		"<html>\
		<title>Error</title>\
		<body>\
		<h1>ERROR:404</h1>\
		<p><h3>Sorry!can not find this file..</h3></p>\
		</body>\
		</html>";
	packet->nBodyBytes = strlen(szContent);
	packet->pEntityBody = new char[packet->nBodyBytes];
	char szLen[10];
	sprintf_s(szLen, sizeof(szLen), "%d", strlen(szContent));
	packet->HeadLine.insert(std::pair<string,string>("Content-length", szLen));
	packet->HeadLine.insert(std::pair<string,string>("Content-type","text/html"));
	memcpy(packet->pEntityBody, szContent, packet->nBodyBytes);
}

void CMyResHttpPacket::Responde202(const REQ_PACKET *pReqPacket, RES_PACKET *packet)
{
	const int nMaxSuffixLen = 10;
	char szSuffixName[nMaxSuffixLen];
	GetSuffixName(pReqPacket->szURL, szSuffixName);

	UINT nLenBytes;
	GetAbsolutePath(0, &nLenBytes, pReqPacket->szURL);
	char *pDocPath = new char[nLenBytes];
	GetAbsolutePath(pDocPath, &nLenBytes, pReqPacket->szURL);

	if (strcmp(szSuffixName, ".html") == 0)
	{
		if (ISSET_BIT(g_MainDlg.m_pCfg->m_cfg.script, SCRIPT_STATICHTML))
		{
			ResponseHtml(pDocPath, pReqPacket->verson, packet);
		}
		else
		{
			RespondeText(pReqPacket, "服务器暂不支持静态html格式文档", packet);
		}
	}
	else if (strcmp(szSuffixName, ".scs") == 0) //simple c script
	{
		if (ISSET_BIT(g_MainDlg.m_pCfg->m_cfg.script, SCRIPT_SIMPLEC))
		{
			ResponseSimpleC(pDocPath, pReqPacket->vecParm, packet);
		}
		else
		{
			RespondeText(pReqPacket, "服务器暂不支持Simple C 脚本语言", packet);
		}
	}

	delete []pDocPath;
}