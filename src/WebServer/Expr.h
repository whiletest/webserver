/*************************************************
Copyright
File name: Expr.h
Author: �ų���
Version: 
Date: 2012/11/24
Description: 
	����һ������ֵ��׺���ʽ����øñ��ʽ�ļ�����
	֧�ֵ���������������
	֧�ֵ�������У�+ - * / ( ) <(С��) >(����) <= >= != ==
History: 
1. 
Date:
Author:
Modification:
*************************************************/
#pragma once
#include <vector>
#include <map>
#include <string>
using std::vector;
using std::map;
using std::string;

class CExpr
{
public:
	CExpr(void);
	virtual ~CExpr(void);
	/*************************************************
	Function: Caculate
	Description: ����һ������ֵ��׺���ʽ����øñ��ʽ�ļ�����
	Input: szExpr ����ֵ��׺���ʽ����5+(2*3)��'\0'��β
	Output: pVal �ñ��ʽ������ֵ
	Return: 
	*************************************************/
	void Caculate(int *pVal, const char *szExpr);
protected:
	/*************************************************
	Function: TanslateToPost
	Description: ����׺��ʾת��Ϊ��׺���ʽ
	Input: szExpr ����ֵ��׺���ʽ����5+(2*3)��'\0'��β
	Output: ��׺���ʽ�������Ա����m_vecPostExpr��
	Return: 
	*************************************************/
	void TanslateToPost(const char* szExpr);
	/*************************************************
	Function: GetResult
	Description: �����׺���ʽ�����ظñ��ʽ�ļ�����
	Input: ��׺���ʽ����Ա����m_vecPostExpr
	Output: pVal �ñ��ʽ������ֵ
	Return: 
	*************************************************/
	void GetResult(int *pVal);
	/*************************************************
	Function: Operate
	Description: ����һ����Ԫ�������ʽ����������֧������
	Input: 
		left �������
		right �Ҳ�����
		oper �����
	Output:
	Return: �ö�Ԫ���ʽ�ļ����� 
	*************************************************/
	int Operate(int left, int right, const string &oper);
	/*************************************************
	Function: AddToOperStack
	Description: ����׺��ʾת��Ϊ��׺���ʽ���̣�������������׺���ʽ���������ջ��
	Input: strOper ������
	Output: 
	Return: 
	*************************************************/
	void AddToOperStack(string strOper);
	/*************************************************
	Function: OperStackRule
	Description: ���������������ջʱӦ���صĹ���
	Input: 
		lOper �������(ջ��)
		lPri ������������ȼ�
		rOper �Ҳ�����(��׺���ʽ��)
		rPri �Ҳ����������ȼ�
	Output: 
	Return: 
	*************************************************/
	void OperStackRule(string lOper, int lPri, string rOper, int rPri);
	/*************************************************
	Function: IsContiSingleOper
	Description: ���strOper�Ƿ�Ϊ�Ƿ��Ķ��ַ������������ܻ���������������һ��
	Input: strOper �������ַ���
	Output:
	Return: �Ƕ�����ַ�����������һ�𣬷���true,���򷵻�false 
	*************************************************/
	bool IsContiSingleOper(string strOper); 
protected:
	vector<string> m_vecOp; //������ջ
	vector<string> m_vecPostExpr; //��׺���ʽջ
	vector<int> m_vecValue; //�����׺���ʽ�Ĵ�ֵջ
	map<string, int> m_mapOperPri; //���������ȼ�ӳ��
};
