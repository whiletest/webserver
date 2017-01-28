/*************************************************
Copyright
File name: packet.h
Author: 张朝旭
Version: 
Date: 2012/11/22
Description: 
实现客户端发来的http协议解析的请求包REQ_PACKET的定义
实现服务器对客户端请求进行响应的响应包RES_PACKET的定义
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

#define METHOD_SIZE 10 //方法最多的字符个数
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
	UCHAR method; //方法
	char szURL[MAX_PATH]; //文件路径
	vector<string> vecParm; //执行脚本是传递的参数(静态html没有)
	UCHAR verson; //协议版本
	map<string, string> HeadLine; //首部行
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
	UCHAR verson; //协议版本
	UCHAR StatusCode; //状态码
	map<string, string> HeadLine; //首部行
	char *pEntityBody; //实体主体
	UINT nBodyBytes; //EntityBody字节数
	//////////////////////////////////////////////////////////////////////////
	_RES_PACKET():pEntityBody(0),nBodyBytes(0){}
	//拷贝构造函数，防止浅拷贝
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
	//重载赋值操作符，防止浅拷贝
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
	//析构
	~_RES_PACKET()
	{
		if (pEntityBody)
		{
			delete []pEntityBody;
			pEntityBody = 0;
		}
	}
	//获得首部行字节
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


#define CR '\r' //回车
#define LF '\n' //换行