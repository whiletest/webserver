/*************************************************
Copyright
File name: Expr.h
Author: 张朝旭
Version: 
Date: 2012/11/24
Description: 
	接收一个字面值中缀表达式，获得该表达式的计算结果
	支持的运算数仅限整型
	支持的运算符有：+ - * / ( ) <(小于) >(大于) <= >= != ==
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
	Description: 接收一个字面值中缀表达式，获得该表达式的计算结果
	Input: szExpr 字面值中缀表达式，例5+(2*3)，'\0'结尾
	Output: pVal 该表达式计算后的值
	Return: 
	*************************************************/
	void Caculate(int *pVal, const char *szExpr);
protected:
	/*************************************************
	Function: TanslateToPost
	Description: 将中缀表示转换为后缀表达式
	Input: szExpr 字面值中缀表达式，例5+(2*3)，'\0'结尾
	Output: 后缀表达式，放入成员变量m_vecPostExpr中
	Return: 
	*************************************************/
	void TanslateToPost(const char* szExpr);
	/*************************************************
	Function: GetResult
	Description: 计算后缀表达式，返回该表达式的计算结果
	Input: 后缀表达式，成员变量m_vecPostExpr
	Output: pVal 该表达式计算后的值
	Return: 
	*************************************************/
	void GetResult(int *pVal);
	/*************************************************
	Function: Operate
	Description: 计算一个二元操作表达式，操作数仅支持整型
	Input: 
		left 左操作数
		right 右操作数
		oper 运算符
	Output:
	Return: 该二元表达式的计算结果 
	*************************************************/
	int Operate(int left, int right, const string &oper);
	/*************************************************
	Function: AddToOperStack
	Description: 在中缀表示转换为后缀表达式过程，将操作符从中缀表达式放入操作符栈中
	Input: strOper 操作符
	Output: 
	Return: 
	*************************************************/
	void AddToOperStack(string strOper);
	/*************************************************
	Function: OperStackRule
	Description: 操作符放入操作符栈时应遵守的规则
	Input: 
		lOper 左操作符(栈顶)
		lPri 左操作符的优先级
		rOper 右操作符(中缀表达式中)
		rPri 右操作符的优先级
	Output: 
	Return: 
	*************************************************/
	void OperStackRule(string lOper, int lPri, string rOper, int rPri);
	/*************************************************
	Function: IsContiSingleOper
	Description: 检测strOper是否为非法的多字符操作符，可能会多个单操作符连在一起
	Input: strOper 操作符字符串
	Output:
	Return: 是多个单字符操作符连在一起，返回true,否则返回false 
	*************************************************/
	bool IsContiSingleOper(string strOper); 
protected:
	vector<string> m_vecOp; //操作符栈
	vector<string> m_vecPostExpr; //后缀表达式栈
	vector<int> m_vecValue; //计算后缀表达式的存值栈
	map<string, int> m_mapOperPri; //操作符优先级映射
};
