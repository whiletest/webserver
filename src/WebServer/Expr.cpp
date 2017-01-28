/*************************************************
Copyright
File name: Expr.cpp
Author: 张朝旭
Version: 
Date: 2012/11/24
*************************************************/
#include "StdAfx.h"
#include "Expr.h"
#include <cctype>

CExpr::CExpr(void)
{
	m_mapOperPri.insert(std::pair<string,int>("<",1));
	m_mapOperPri.insert(std::pair<string,int>(">",1));
	m_mapOperPri.insert(std::pair<string,int>("<=",1));
	m_mapOperPri.insert(std::pair<string,int>(">=",1));
	m_mapOperPri.insert(std::pair<string,int>("==",1));
	m_mapOperPri.insert(std::pair<string,int>("!=",1));
	m_mapOperPri.insert(std::pair<string,int>("+",2));
	m_mapOperPri.insert(std::pair<string,int>("-",2));
	m_mapOperPri.insert(std::pair<string,int>("*",3));
	m_mapOperPri.insert(std::pair<string,int>("/",3));
	m_mapOperPri.insert(std::pair<string,int>("(",6));
	m_mapOperPri.insert(std::pair<string,int>(")",0));
}

CExpr::~CExpr(void)
{
}


//输入中缀表达式，如(1-2)*(12+345)
void CExpr::Caculate(int *pVal, const char *szExpr)
{
	m_vecOp.clear();
	m_vecPostExpr.clear();
	m_vecValue.clear();

	TanslateToPost(szExpr);

	GetResult(pVal);
}

void CExpr::OperStackRule(string lOper, int lPri, string rOper, int rPri)
{
	while (lPri >= rPri && lOper != "(" ) 
	{
		m_vecPostExpr.push_back(lOper);
		m_vecOp.pop_back();
		if (m_vecOp.empty())
			break;
		lOper = m_vecOp.back();
		map<string, int>::const_iterator iter = m_mapOperPri.find(lOper);
		lOper = iter->first;
		lPri = iter->second;
	}

	if (rOper == ")")
	{
		m_vecOp.pop_back();
	}
	else
	{
		m_vecOp.push_back(rOper);
	}
}

void CExpr::AddToOperStack(string strOper)
{
	if (m_vecOp.empty())
	{
		m_vecOp.push_back(strOper);
	}
	else
	{
		string strLeftOper = m_vecOp.back();
		map<string, int>::const_iterator LeftOperIter = m_mapOperPri.find(strLeftOper);
		map<string, int>::const_iterator RightOperIter = m_mapOperPri.find(strOper);
		OperStackRule(strLeftOper, LeftOperIter->second,
			strOper, RightOperIter->second);
	}
}

void CExpr::TanslateToPost(const char* szExpr)
{
	string strOper; //操作符
	string strNum; //数字
	while (*szExpr)
	{
		if (isdigit(*szExpr))
		{
			strNum.push_back(*szExpr);

			if (!strOper.empty())
			{
				AddToOperStack(strOper);
				strOper.clear();
			}
		}
		else 
		{
			if (!strNum.empty())
			{
				m_vecPostExpr.push_back(strNum);
				strNum.clear();
			}
			
			strOper.push_back(*szExpr);
			if (IsContiSingleOper(strOper))
			{
				string strTem;
				strTem.push_back(strOper[0]);
				AddToOperStack(strTem);

				strTem[0] = strOper[1];
				AddToOperStack(strTem);

				strOper.clear();
			}
		}

		szExpr++;
	}

	if (!strNum.empty())
		m_vecPostExpr.push_back(strNum);

	if (strOper == ")")
	{
		AddToOperStack(strOper);
		strOper.clear();
	}

	while (!m_vecOp.empty())
	{
		m_vecPostExpr.push_back(m_vecOp.back());
		m_vecOp.pop_back();
	}
}

void CExpr::GetResult(int *pVal)
{
	for (vector<string>::iterator iter = m_vecPostExpr.begin();
		iter != m_vecPostExpr.end();iter++)
	{
		if (isdigit((*iter)[0]))
		{
			int val = atoi(iter->c_str());
			m_vecValue.push_back(val);
		}
		else
		{
			int rightVal = m_vecValue.back();
			m_vecValue.pop_back();
			int leftVal = 0;
			if (!m_vecValue.empty())
			{
				leftVal = m_vecValue.back();
				m_vecValue.pop_back();
			}
			m_vecValue.push_back(Operate(leftVal, rightVal, *iter));
		}
	}

	*pVal = m_vecValue.back();
}

int CExpr::Operate(int left, int right, const string &oper)
{
	if (oper == "+")
		return (left + right);
	else if (oper == "-")
		return (left - right);
	else if (oper == "*")
		return (left * right);
	else if (oper == "/")
		return (left / right);
	else if (oper == "<")
		return (left < right ? 1 : 0);
	else if (oper == ">")
		return (left > right ? 1 : 0);
	else if (oper == "<=")
		return (left <= right ? 1 : 0);
	else if (oper == ">=")
		return (left >= right ? 1 : 0);
	else if (oper == "==")
		return (left == right ? 1 : 0);
	else if (oper == "!=")
		return (left != right ? 1 : 0);

	return 0;
}

bool CExpr::IsContiSingleOper(string strOper)
{
	if (strOper.size() == 1)
		return false;
	if (strOper == "<=" || strOper == ">=" 
		|| strOper == "==" || strOper == "!=")
		return false;
	return true;
}