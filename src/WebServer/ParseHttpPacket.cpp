/*************************************************
Copyright
File name: ParseHttpPacket.cpp
Author: 张朝旭
Version: 
Date: 2012/11/22
*************************************************/
#include "StdAfx.h"
#include "ParseHttpPacket.h"

CParseHttpPacket* CParseHttpPacket::m_pThis = 0;
char CParseHttpPacket::m_ppMethod[METHOD_TOTAL][METHOD_SIZE] = {"GET", "POST"};
char CParseHttpPacket::m_ppVerson[VERSON_TOTAL][VERSON_SIZE] = {"HTTP/1.1", "HTTP/1.0"};
char CParseHttpPacket::m_ppStatusCode[STATUSCODE_TOTAL][STATUSCODE_LEN] = {"404", "202"};
char CParseHttpPacket::m_ppScDesc[STATUSCODE_TOTAL][SC_DESC_LEN] = {"Not found", "Accept"};

CParseHttpPacket::CParseHttpPacket(void)
{
}

CParseHttpPacket::~CParseHttpPacket(void)
{
}

UCHAR CParseHttpPacket::CheckMethod(const char *pMethod)
{
	UCHAR method = METHOD_TOTAL;
	int index = 0;
	while (index != METHOD_TOTAL)
	{
		if (strcmp(pMethod, m_ppMethod[index]) == 0)
			break;
		index++;
	}

	if (index != METHOD_TOTAL)
		method = (UCHAR)index;
	return method;
}

UCHAR CParseHttpPacket::CheckVerson(const char *pVerson)
{
	UCHAR verson = VERSON_TOTAL;
	int index = 0;
	while (index != VERSON_TOTAL)
	{
		if (strcmp(pVerson, m_ppVerson[index]) == 0)
			break;
		index++;
	}

	if (index != VERSON_TOTAL)
		verson = (UCHAR)index;
	return verson;
}

void CParseHttpPacket::TranslateURL(REQ_PACKET *packet, const char *pURL)
{
	char *pFilePath = packet->szURL;
	while (*pURL && *pURL != '?')
	{
		if (*pURL == '/')
		{
			*pFilePath++ = '\\';
			pURL++;
			continue;
		}
		*pFilePath++ = *pURL++;
	}
	*pFilePath = '\0';
	//解析参数
	string strVal;
	bool bVal = false;
	while (*pURL)
	{
		pURL++; //firt time skip '?'
		if (*pURL == '=')
		{
			bVal = true;
		}
		else if (*pURL == '&')
		{
			packet->vecParm.push_back(strVal);
			bVal = false;
			strVal.clear();
		}
		else if (bVal)
		{
			strVal += *pURL;
		}
	}
	packet->vecParm.push_back(strVal);
}

REQ_PACKET CParseHttpPacket::ParsePacket(char *buff, int nLen)
{
	REQ_PACKET packet;
	int curLen = 0;
	char *pSegmentBegin = buff;
	char *pSegmentEnd = buff;
	char szSegment[MAX_PATH];

	//方法
	while(*pSegmentEnd != ' ')
	{
		szSegment[pSegmentEnd-pSegmentBegin] = *pSegmentEnd;
		pSegmentEnd++;
	}
	szSegment[pSegmentEnd-pSegmentBegin] = '\0';
	packet.method = CheckMethod(szSegment);

	//URL
	pSegmentBegin = ++pSegmentEnd;
	while (*pSegmentEnd != ' ')
	{
		szSegment[pSegmentEnd-pSegmentBegin] = *pSegmentEnd;
		pSegmentEnd++;
	}
	szSegment[pSegmentEnd-pSegmentBegin] = '\0';
	TranslateURL(&packet, szSegment);

	//版本
	pSegmentBegin = ++pSegmentEnd;
	while (*pSegmentEnd != CR)
	{
		szSegment[pSegmentEnd-pSegmentBegin] = *pSegmentEnd;
		pSegmentEnd++;
	}
	szSegment[pSegmentEnd-pSegmentBegin] = '\0';
	packet.verson = CheckVerson(szSegment);

	return packet;
}

void CParseHttpPacket::PackagePacket(char *buff, UINT *pLenBytes, const RES_PACKET *packet)
{
	if (buff)
	{
		char *pTem = buff;
		//版本
		memcpy(pTem, m_ppVerson[packet->verson], strlen(m_ppVerson[packet->verson]));

		pTem += strlen(m_ppVerson[packet->verson]);
		*pTem++ = ' ';
		//状态码
		memcpy(pTem, m_ppStatusCode[packet->StatusCode],
			strlen(m_ppStatusCode[packet->StatusCode]));

		pTem += strlen(m_ppStatusCode[packet->StatusCode]);
		*pTem++ = ' ';
		//短语
		memcpy(pTem, m_ppScDesc[packet->StatusCode], 
			strlen(m_ppScDesc[packet->StatusCode]));

		pTem += strlen(m_ppScDesc[packet->StatusCode]);
		*pTem++ = CR;
		*pTem++ = LF;
		//首部行
		for (map<string,string>::const_iterator iter = packet->HeadLine.begin();
			iter != packet->HeadLine.end();iter++)
		{
			memcpy(pTem, iter->first.c_str(), iter->first.size());
			pTem += iter->first.size();
			*pTem++ = ':';
			*pTem++ = ' ';
			memcpy(pTem, iter->second.c_str(), iter->second.size());
			pTem += iter->second.size();
			*pTem++ = CR;
			*pTem++ = LF;
		}

		*pTem++ = CR;
		*pTem++ = LF;
		//实体主体
		if (packet->pEntityBody)
		{
			memcpy(pTem, packet->pEntityBody, packet->nBodyBytes);
		}
	}
	else
	{
		UINT TotalBytes = 0;
		TotalBytes += strlen(m_ppVerson[packet->verson]) + 1; //' '
		TotalBytes += strlen(m_ppStatusCode[packet->StatusCode]) + 1; //' '
		TotalBytes += strlen(m_ppScDesc[packet->StatusCode]) + 2; //\r\n
		TotalBytes += packet->GetHeadLineBytes() + 2; //\r\n
		TotalBytes += packet->nBodyBytes;
		*pLenBytes = TotalBytes;
	}
}