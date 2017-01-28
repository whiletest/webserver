/*************************************************
Copyright
File name: SimpleCScript.h
Author: 张朝旭
Version: 
Date: 2012/11/24
Description: 
	对于动态文档中服务器脚本语言进行执行。
	实现了服务器脚本语言的词法检查，语法编译，执行的接口。
	实现的是简单C语言(simple C)的基本语法
History: 
1. 
Date:2012.12.02
Author:zcx
Modification:
	ExecuteScript参数修改，添加了脚本执行时参数mapParm
*************************************************/
#pragma once
#include "scriptengine.h"
#include <vector>
using std::vector;
using std::fstream;

class CSimpleCScript :
	public CScriptEngine
{
#define TEMP_FILE    ".\\temp.dat"    //中间代码文件
#define ERROR_FILE   ".\\error.txt"   //错误输出文件
#define OBJCODE_FILE ".\\objcode.dat" //目标代码生成文件
//////////////////////////////////////////////////////////////////////////
#define BEGIN_SYMBOL "BEGINC" 
#define END_SYMBOL   "ENDC"    //字符{字符|数字}
//////////////////////////////////////////////////////////////////////////
	enum //用于词法分析时的状态转换
	{
		STATE_INIT,       //初始化状态
		STATE_ID_KEYWORD, //变量或者关键字
		STATE_NUM,        //数字字面值
		STATE_SINGLEWORD, //单字符操作符
		STATE_DOUBLEWORD, //双字符操作符
		STATE_TEXT,       //字符串
		STATE_COMMENT,    //注释
		STATE_ERROR       //错误
	};
	//用于记录for,while,if等具有条件判断的语句的起始位置，结束位置
	struct LABLE 
	{
		int beg;
		int end;
	};
public:
	CSimpleCScript(void);
	virtual ~CSimpleCScript(void);
    /*************************************************
	Function: ExecuteScript
	Description: 执行指定的动态文件中的服务器脚本语言
	Input: 
		pFileName 文档完整绝对路径
		vecParm 脚本运行时参数
	Output: packet 服务器响应客户端的响应包
	Return: 
	*************************************************/
	virtual void ExecuteScript(const char *pFileName, vector<string> vecParm, RES_PACKET *packet);
protected:
	/*************************************************
	Function: ScriptWordAnalyse
	Description: 对于指定的动态文档中的脚本语言进行词法分析
	Input: in 指定文档的输入流对象
	Output: 
	Return: 词法分析成功返回true,否则false
	*************************************************/
	virtual bool ScriptWordAnalyse(ifstream &in);
	/*************************************************
	Function: ScriptProgramAnalyse
	Description: 对于词法分析成功后的中间代码文件进行语言分析，并且同时生成目标代码
	Input: Err 语法分析错误的输出流对象
	Output: 
	Return: 语法分析成功返回true,否则返回false
	*************************************************/
	virtual bool ScriptProgramAnalyse(ofstream &Err);
	/*************************************************
	Function: ScriptProgramExecute
	Description: 执行目标代码文件中编译成功的脚本语言目标代码
	Input: 
	Output: 
		buff 目标代码的执行结果缓冲区
		pLen 缓冲区大小
	Return: 目标代码执行成功返回true,否则false
	*************************************************/
	virtual bool ScriptProgramExecute(vector<string> &vecParm, char *buff, int *pLen);
protected:
	/*************************************************
	Function: GenerateExpr
	Description: 将目标代码中指定的表达式提取出来
	Input: in 在目标代码文件中的输入流，保证in当前指向语句首(首字母前不含转义字符)
	Output: szExpr 提取出来的表达式
	Return:
	*************************************************/
	void GenerateExpr(char *szExpr, ifstream &in);
	/*************************************************
	Function: FindEndLable
	Description: 找到while,for,if等含有条件判断的语句的结束标志
	Input: beg 起始标志
	Output: 
	Return: 结束标志，未找到返回-1
	*************************************************/
	int FindEndLable(int beg);
	/*************************************************
	Function: FindBegLable
	Description: 找到while,for,if等含有条件判断的语句的起始标志
	Input: end 结束标志
	Output: 
	Return: 起始标志,未找到返回-1
	*************************************************/
	int FindBegLable(int end);
//////////////////////////////////////////////////////////////////////////
//语法分析模版帮助函数
	/*************************************************
	Function: EndOfCurLine
	Description: 将输入流移动到下一行开始字符
	Input: in 输入流
	Output: 
	Return:
	*************************************************/
	void EndOfCurLine(ifstream &in);
	/*************************************************
	Function: Program
	Description: 
		语言分析入口函数
		语法规则--<program>::=<declaration_list><statement_list>
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int Program(ifstream &in, ofstream &Err);
	/*************************************************
	Function: DeclarationList
	Description: 
		变量定义模块
		语法规则--<declaration_list>::=<declaration_list><declaration_stat> |ε
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int DeclarationList(ifstream &in, ofstream &Err);
	/*************************************************
	Function: DeclarationStat
	Description: 
		变量定义语句
		语法规则--<declaration_stat>::=int ID;
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数，返回-1表明变量定义模块结束
	*************************************************/
	int DeclarationStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: StatementList
	Description: 
		表达式语句模块
		语法规则--<statement_list>::=<statement_list><statement>| ε
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int StatementList(ifstream &in, ofstream &Err);
	/*************************************************
	Function: Statement
	Description: 
		表达式语句
		语法规则--<statement>::=<if_stat>|<while_stat>|<for_stat>|
								<write_stat>|<command_stat> |<expression_stat>
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数，返回-1表示表达式模块结束
	*************************************************/
	int Statement(ifstream &in, ofstream &Err);
	/*************************************************
	Function: IfStat
	Description: 
		if语句
		语法规则--<if_stat>::= if (<expr>) <statement >
		注：()为终结符号
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int IfStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: WhileStat
	Description: 
		while语句
		语法规则--<while_stat>::= while (<expr >) < statement >
		注：()为终结符号
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int WhileStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: ForStat
	Description: 
		for语句
		语法规则--<for_stat>::= for(<expr>;<expr>;<expr>)<statement>
		注：()为终结符号
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int ForStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: WriteStat
	Description: 
		write语句
		语法规则--<write_stat>::=write <expression>|"{字符}";
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int WriteStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: ReadStat
	Description: 
		read语句
		语法规则--<read_stat>::=read ID;
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int ReadStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: CompoundStat
	Description: 
		语句块
		语法规则--<compound_stat>::={<statement_list>}
		注：{}为终结符号
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int CompoundStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: ExpressionStat
	Description: 
		表达式语句
		语法规则--<expression_stat>::=< expression >;|;
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int ExpressionStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: Expression
	Description: 
		表达式
		语法规则--< expression >::= ID=<bool_expr>|<bool_expr>
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int Expression(ifstream &in, ofstream &Err);
	/*************************************************
	Function: BoolExpr
	Description: 
		布尔表达式
		语法规则--<bool_expr>::=<additive_expr>
								|<additive_expr>(>|<|>=|<=|==|!=)< additive_expr > 
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int BoolExpr(ifstream &in, ofstream &Err);
	/*************************************************
	Function: AdditiveExpr
	Description: 
		加减法表达式
		语法规则--< additive_expr>::=<term>{(+|-)< term >}
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int AdditiveExpr(ifstream &in, ofstream &Err);
	/*************************************************
	Function: Term
	Description: 
		乘除法表达式
		语法规则--< term >::=<factor>{(*| /)< factor >}
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int Term(ifstream &in, ofstream &Err);
	/*************************************************
	Function: Factor
	Description: 
		因子
		语法规则--< factor >::=(< expression >)|ID|NUM
	Input: 
		in 目标代码文件输入流
		Err 错误输出流
	Output: 
	Return: 此模块语句分析错误个数
	*************************************************/
	int Factor(ifstream &in, ofstream &Err);
//////////////////////////////////////////////////////////////////////////
protected:
	map<string, int> m_mapVar; //脚本变量索引表
	string m_strExpr;          //表达式临时存储
	fstream m_ObjCode;         //目标代码文件输出流
	vector<LABLE> m_vecLable;  //用于while,for,if的起始结束记录
//////////////////////////////////////////////////////////////////////////
#define MAX_KEYWORD_NUM  6
#define KEYWORD_LEN      10
	static char m_ppKeyWord[MAX_KEYWORD_NUM][KEYWORD_LEN];
	static const int m_iDeclBeg = 0;
	static const int m_iDeclEnd = 1;
	static const int m_iStatBeg = 1;
	static const int m_iStatEnd = MAX_KEYWORD_NUM;
#define MAX_SINGLEWORD_NUM 10+1
	static char m_szSingleWord[MAX_SINGLEWORD_NUM];
#define MAX_PREDOUBLEWORD_NUM 4+1
	static char m_szPreDoubleWord[MAX_PREDOUBLEWORD_NUM];
#define MAX_BOOLWORD_NUM 6
	static char m_ppBoolWord[MAX_BOOLWORD_NUM][3];
#define MAX_VARNAME_LEN 20
#define MAX_EXPR_LEN    50
};
