/*************************************************
Copyright
File name: SimpleCScript.h
Author: �ų���
Version: 
Date: 2012/11/24
Description: 
	���ڶ�̬�ĵ��з������ű����Խ���ִ�С�
	ʵ���˷������ű����ԵĴʷ���飬�﷨���룬ִ�еĽӿڡ�
	ʵ�ֵ��Ǽ�C����(simple C)�Ļ����﷨
History: 
1. 
Date:2012.12.02
Author:zcx
Modification:
	ExecuteScript�����޸ģ�����˽ű�ִ��ʱ����mapParm
*************************************************/
#pragma once
#include "scriptengine.h"
#include <vector>
using std::vector;
using std::fstream;

class CSimpleCScript :
	public CScriptEngine
{
#define TEMP_FILE    ".\\temp.dat"    //�м�����ļ�
#define ERROR_FILE   ".\\error.txt"   //��������ļ�
#define OBJCODE_FILE ".\\objcode.dat" //Ŀ����������ļ�
//////////////////////////////////////////////////////////////////////////
#define BEGIN_SYMBOL "BEGINC" 
#define END_SYMBOL   "ENDC"    //�ַ�{�ַ�|����}
//////////////////////////////////////////////////////////////////////////
	enum //���ڴʷ�����ʱ��״̬ת��
	{
		STATE_INIT,       //��ʼ��״̬
		STATE_ID_KEYWORD, //�������߹ؼ���
		STATE_NUM,        //��������ֵ
		STATE_SINGLEWORD, //���ַ�������
		STATE_DOUBLEWORD, //˫�ַ�������
		STATE_TEXT,       //�ַ���
		STATE_COMMENT,    //ע��
		STATE_ERROR       //����
	};
	//���ڼ�¼for,while,if�Ⱦ��������жϵ�������ʼλ�ã�����λ��
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
	Description: ִ��ָ���Ķ�̬�ļ��еķ������ű�����
	Input: 
		pFileName �ĵ���������·��
		vecParm �ű�����ʱ����
	Output: packet ��������Ӧ�ͻ��˵���Ӧ��
	Return: 
	*************************************************/
	virtual void ExecuteScript(const char *pFileName, vector<string> vecParm, RES_PACKET *packet);
protected:
	/*************************************************
	Function: ScriptWordAnalyse
	Description: ����ָ���Ķ�̬�ĵ��еĽű����Խ��дʷ�����
	Input: in ָ���ĵ�������������
	Output: 
	Return: �ʷ������ɹ�����true,����false
	*************************************************/
	virtual bool ScriptWordAnalyse(ifstream &in);
	/*************************************************
	Function: ScriptProgramAnalyse
	Description: ���ڴʷ������ɹ�����м�����ļ��������Է���������ͬʱ����Ŀ�����
	Input: Err �﷨������������������
	Output: 
	Return: �﷨�����ɹ�����true,���򷵻�false
	*************************************************/
	virtual bool ScriptProgramAnalyse(ofstream &Err);
	/*************************************************
	Function: ScriptProgramExecute
	Description: ִ��Ŀ������ļ��б���ɹ��Ľű�����Ŀ�����
	Input: 
	Output: 
		buff Ŀ������ִ�н��������
		pLen ��������С
	Return: Ŀ�����ִ�гɹ�����true,����false
	*************************************************/
	virtual bool ScriptProgramExecute(vector<string> &vecParm, char *buff, int *pLen);
protected:
	/*************************************************
	Function: GenerateExpr
	Description: ��Ŀ�������ָ���ı��ʽ��ȡ����
	Input: in ��Ŀ������ļ��е�����������֤in��ǰָ�������(����ĸǰ����ת���ַ�)
	Output: szExpr ��ȡ�����ı��ʽ
	Return:
	*************************************************/
	void GenerateExpr(char *szExpr, ifstream &in);
	/*************************************************
	Function: FindEndLable
	Description: �ҵ�while,for,if�Ⱥ��������жϵ����Ľ�����־
	Input: beg ��ʼ��־
	Output: 
	Return: ������־��δ�ҵ�����-1
	*************************************************/
	int FindEndLable(int beg);
	/*************************************************
	Function: FindBegLable
	Description: �ҵ�while,for,if�Ⱥ��������жϵ�������ʼ��־
	Input: end ������־
	Output: 
	Return: ��ʼ��־,δ�ҵ�����-1
	*************************************************/
	int FindBegLable(int end);
//////////////////////////////////////////////////////////////////////////
//�﷨����ģ���������
	/*************************************************
	Function: EndOfCurLine
	Description: ���������ƶ�����һ�п�ʼ�ַ�
	Input: in ������
	Output: 
	Return:
	*************************************************/
	void EndOfCurLine(ifstream &in);
	/*************************************************
	Function: Program
	Description: 
		���Է�����ں���
		�﷨����--<program>::=<declaration_list><statement_list>
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int Program(ifstream &in, ofstream &Err);
	/*************************************************
	Function: DeclarationList
	Description: 
		��������ģ��
		�﷨����--<declaration_list>::=<declaration_list><declaration_stat> |��
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int DeclarationList(ifstream &in, ofstream &Err);
	/*************************************************
	Function: DeclarationStat
	Description: 
		�����������
		�﷨����--<declaration_stat>::=int ID;
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������������-1������������ģ�����
	*************************************************/
	int DeclarationStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: StatementList
	Description: 
		���ʽ���ģ��
		�﷨����--<statement_list>::=<statement_list><statement>| ��
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int StatementList(ifstream &in, ofstream &Err);
	/*************************************************
	Function: Statement
	Description: 
		���ʽ���
		�﷨����--<statement>::=<if_stat>|<while_stat>|<for_stat>|
								<write_stat>|<command_stat> |<expression_stat>
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������������-1��ʾ���ʽģ�����
	*************************************************/
	int Statement(ifstream &in, ofstream &Err);
	/*************************************************
	Function: IfStat
	Description: 
		if���
		�﷨����--<if_stat>::= if (<expr>) <statement >
		ע��()Ϊ�ս����
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int IfStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: WhileStat
	Description: 
		while���
		�﷨����--<while_stat>::= while (<expr >) < statement >
		ע��()Ϊ�ս����
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int WhileStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: ForStat
	Description: 
		for���
		�﷨����--<for_stat>::= for(<expr>;<expr>;<expr>)<statement>
		ע��()Ϊ�ս����
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int ForStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: WriteStat
	Description: 
		write���
		�﷨����--<write_stat>::=write <expression>|"{�ַ�}";
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int WriteStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: ReadStat
	Description: 
		read���
		�﷨����--<read_stat>::=read ID;
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int ReadStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: CompoundStat
	Description: 
		����
		�﷨����--<compound_stat>::={<statement_list>}
		ע��{}Ϊ�ս����
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int CompoundStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: ExpressionStat
	Description: 
		���ʽ���
		�﷨����--<expression_stat>::=< expression >;|;
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int ExpressionStat(ifstream &in, ofstream &Err);
	/*************************************************
	Function: Expression
	Description: 
		���ʽ
		�﷨����--< expression >::= ID=<bool_expr>|<bool_expr>
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int Expression(ifstream &in, ofstream &Err);
	/*************************************************
	Function: BoolExpr
	Description: 
		�������ʽ
		�﷨����--<bool_expr>::=<additive_expr>
								|<additive_expr>(>|<|>=|<=|==|!=)< additive_expr > 
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int BoolExpr(ifstream &in, ofstream &Err);
	/*************************************************
	Function: AdditiveExpr
	Description: 
		�Ӽ������ʽ
		�﷨����--< additive_expr>::=<term>{(+|-)< term >}
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int AdditiveExpr(ifstream &in, ofstream &Err);
	/*************************************************
	Function: Term
	Description: 
		�˳������ʽ
		�﷨����--< term >::=<factor>{(*| /)< factor >}
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int Term(ifstream &in, ofstream &Err);
	/*************************************************
	Function: Factor
	Description: 
		����
		�﷨����--< factor >::=(< expression >)|ID|NUM
	Input: 
		in Ŀ������ļ�������
		Err ���������
	Output: 
	Return: ��ģ���������������
	*************************************************/
	int Factor(ifstream &in, ofstream &Err);
//////////////////////////////////////////////////////////////////////////
protected:
	map<string, int> m_mapVar; //�ű�����������
	string m_strExpr;          //���ʽ��ʱ�洢
	fstream m_ObjCode;         //Ŀ������ļ������
	vector<LABLE> m_vecLable;  //����while,for,if����ʼ������¼
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
