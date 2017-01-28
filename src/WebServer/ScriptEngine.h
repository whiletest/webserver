/*************************************************
Copyright
File name: ScriptEngine.h
Author: �ų���
Version: 
Date: 2012/11/24
Description: 
	���ڶ�̬�ĵ��з������ű����Խ���ִ�С�
	�����࣬�ṩ�Է������ű����ԵĴʷ���飬�﷨���룬ִ�еĽӿڡ�
History: 
1. 
Date:2012.12.02
Author:zcx
Modification:
	ExecuteScript�����޸ģ�����˽ű�ִ��ʱ����vecParm
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
	Description: ִ��ָ���Ķ�̬�ļ��еķ������ű�����
	Input: pFilePath ��̬�ĵ���·��
	Output: packet ��������Ӧ�ͻ��˵���Ӧ��
	Return: 
	*************************************************/
	virtual void ExecuteScript(const char *pFileName, vector<string> vecParm, RES_PACKET *packet) = 0;
protected:
	/*************************************************
	Function: ScriptWordAnalyse
	Description: ����ָ���Ķ�̬�ĵ��еĽű����Խ��дʷ�����
	Input: in ָ���ĵ�������������
	Output: 
	Return: �ʷ������ɹ�����true,����false
	*************************************************/
	virtual bool ScriptWordAnalyse(ifstream &in) = 0;
	/*************************************************
	Function: ScriptProgramAnalyse
	Description: ���ڴʷ������ɹ�����м�����ļ��������Է���������ͬʱ����Ŀ�����
	Input: Err �﷨������������������
	Output: 
	Return: �﷨�����ɹ�����true,���򷵻�false
	*************************************************/
	virtual bool ScriptProgramAnalyse(ofstream &Err) = 0;
	/*************************************************
	Function: ScriptProgramExecute
	Description: ִ��Ŀ������ļ��б���ɹ��Ľű�����Ŀ�����
	Input: 
	Output: 
		buff Ŀ������ִ�н��������
		pLen ��������С
	Return: Ŀ�����ִ�гɹ�����true,����false
	*************************************************/
	virtual bool ScriptProgramExecute(vector<string> &vecParm, char *buff, int *pLen) = 0;
};
