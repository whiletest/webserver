/*************************************************
Copyright
File name: ScriptEngine.h
Author: 张朝旭
Version: 
Date: 2012/11/24
Description: 
	对于动态文档中服务器脚本语言进行执行。
	抽象类，提供对服务器脚本语言的词法检查，语法编译，执行的接口。
History: 
1. 
Date:2012.12.02
Author:zcx
Modification:
	ExecuteScript参数修改，添加了脚本执行时参数vecParm
*************************************************/
#pragma once
#include <fstream>
using std::ifstream;
using std::ofstream;
#include "packet.h"

class CScriptEngine
{
public:
	CScriptEngine(void);
	virtual ~CScriptEngine(void);
	/*************************************************
	Function: ExecuteScript
	Description: 执行指定的动态文件中的服务器脚本语言
	Input: pFilePath 动态文档的路径
	Output: packet 服务器响应客户端的响应包
	Return: 
	*************************************************/
	virtual void ExecuteScript(const char *pFileName, vector<string> vecParm, RES_PACKET *packet) = 0;
protected:
	/*************************************************
	Function: ScriptWordAnalyse
	Description: 对于指定的动态文档中的脚本语言进行词法分析
	Input: in 指定文档的输入流对象
	Output: 
	Return: 词法分析成功返回true,否则false
	*************************************************/
	virtual bool ScriptWordAnalyse(ifstream &in) = 0;
	/*************************************************
	Function: ScriptProgramAnalyse
	Description: 对于词法分析成功后的中间代码文件进行语言分析，并且同时生成目标代码
	Input: Err 语法分析错误的输出流对象
	Output: 
	Return: 语法分析成功返回true,否则返回false
	*************************************************/
	virtual bool ScriptProgramAnalyse(ofstream &Err) = 0;
	/*************************************************
	Function: ScriptProgramExecute
	Description: 执行目标代码文件中编译成功的脚本语言目标代码
	Input: 
	Output: 
		buff 目标代码的执行结果缓冲区
		pLen 缓冲区大小
	Return: 目标代码执行成功返回true,否则false
	*************************************************/
	virtual bool ScriptProgramExecute(vector<string> &vecParm, char *buff, int *pLen) = 0;
};
