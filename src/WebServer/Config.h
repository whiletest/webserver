/*************************************************
Copyright
File name: config.h
Author: 张朝旭
Version: 
Date: 2012/11/26
Description: 
	对配置文件config.txt进行管理。
History: 
1. 
Date:
Author:
Modification:
*************************************************/
#pragma once

enum
{
	RECORD_NONE = 0x00,   //无选中项
	RECORD_ONLIST = 0x01, //显示到列表
	RECORD_LOG = 0x02,    //记录日志
	RECORD_CONNECTNUM = 0x04 //显示历史连接数
};

enum
{
	LIST_NONE = 0x00, //无选中项
	LIST_URL = 0x01, //显示URL
	LIST_STATUS = 0x02, //显示状态码
	LIST_REMOTEIP = 0x04, //显示客户端ip
	LIST_BYTES = 0x08, //显示传输字节数
	LIST_DATE = 0x10 //显示日期时间
};

enum
{
	SCRIPT_NONE = 0x00, //无选中项
	SCRIPT_SIMPLEC = 0x01, //启用simple C脚本
	SCRIPT_STATICHTML = 0x02 //启用静态html文档
};

#define RESET_WHOLE(x)    ((x) &= 0x00)
#define RESET_BIT(x,y)    ((x) &= ~(y))
#define SET_BIT(x,y)      ((x) |= (y))
#define ISSET_BIT(x,y)    ((x) & (y))

struct CONFIG
{
	UINT record;               //记录配置
	UINT list;                 //列表显示配置
	UINT script;               //支持脚本配置
	char szRootPath[MAX_PATH]; //根目录配置
};

class CConfig
{
//配置文件路径
#define CONFIG_FILE "config.txt"
public:
	CConfig(void);
	~CConfig(void);
	/*************************************************
	Function: WriteConfig
	Description: 将配置数据写入配置文件
	Input: 
	Output: 
	Return: 
	*************************************************/
	void WriteConfig();
	/*************************************************
	Function: ReadConfig
	Description: 将配置数据从配置文件中读出
	Input: 
	Output: 
	Return: 读取成功返回true,否则返回false
	*************************************************/
	bool ReadConfig(); 
public:
	CONFIG m_cfg; //配置数据
};
