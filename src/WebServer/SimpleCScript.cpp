/*************************************************
Copyright
File name: SimpleCScript.h
Author: �ų���
Version: 
Date: 2012/11/24
*************************************************/
#include "StdAfx.h"
#include "SimpleCScript.h"
#include "IOCP.h"
#include "Expr.h"
#include <cctype>
#include <fstream>
using std::ifstream;
using std::ofstream;


char CSimpleCScript::m_ppKeyWord[MAX_KEYWORD_NUM][KEYWORD_LEN] = 
{"int", "for", "while", "if", "write", "read"};

char CSimpleCScript::m_szSingleWord[MAX_SINGLEWORD_NUM] = "+-*/;(){}\"";

char CSimpleCScript::m_szPreDoubleWord[MAX_PREDOUBLEWORD_NUM] = "<>!=";

char CSimpleCScript::m_ppBoolWord[MAX_BOOLWORD_NUM][3] = {">","<",">=","<=","==","!="};

CSimpleCScript::CSimpleCScript(void)
{
}

CSimpleCScript::~CSimpleCScript(void)
{
}

void CSimpleCScript::ExecuteScript(const char *pFileName,
								   vector<string> vecParm, 
								   RES_PACKET *packet)
{
	ifstream in(pFileName);
	if (!in)
		return;

	ofstream Err(ERROR_FILE);

	packet->pEntityBody = new char[BUFFER_SIZE];
	memset(packet->pEntityBody, 0, BUFFER_SIZE);
	char *pTem = packet->pEntityBody;

	in.seekg(0, std::ios::end);
	int FileLen = in.tellg();
	in.seekg(0, std::ios::beg);

	int curPos = 0, prevPos = 0;
	int cSimpleCCode = 0;
	in.getline(pTem, packet->pEntityBody+BUFFER_SIZE-pTem);
	while (in && pTem < packet->pEntityBody+BUFFER_SIZE)
	{
		prevPos = curPos;
		
		if (strcmp(pTem, BEGIN_SYMBOL) == 0)
		{
			m_mapVar.clear();
			m_vecLable.clear();
			m_strExpr.clear();

			cSimpleCCode++;
			Err << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << '\n';
			Err << "��" << cSimpleCCode << "��simpleC����:" << '\n';

			bool bSuccess = ScriptWordAnalyse(in);
			if (bSuccess)
			{
				m_ObjCode.open(OBJCODE_FILE, 
					std::ios::in | std::ios::out | std::ios::trunc);
				bSuccess = ScriptProgramAnalyse(Err);
				m_ObjCode.close();
			}
			else
			{
				Err << "�ű��к��зǷ��ַ�" << '\n';
				Err << "�ű�����ʧ��" << '\n';
			}

			const int nMaxLen = 1024;
			char buff[nMaxLen];
			int nLen;
			if (bSuccess)
			{
				ScriptProgramExecute(vecParm, buff, &nLen);
			}
			else 
			{
				char szErr[25] = "-server script error-";
				nLen = strlen(szErr);
				memcpy(buff, szErr, nLen);
			}

			Err << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << "\n\n\n";

			memcpy(pTem, buff, nLen);
			pTem += nLen;

			curPos = in.tellg();
			in.getline(pTem, packet->pEntityBody+BUFFER_SIZE-pTem);
			continue;
		}

		curPos = in.tellg();
		if (curPos > 0)
		{
			pTem += curPos - prevPos;
			if (curPos != FileLen) //��curPos == FileLen ����һ�����ļ�β
				 pTem -= 2;  //δ�����һ�м���\r\n
		}

		in.getline(pTem, packet->pEntityBody+BUFFER_SIZE-pTem);
	}
	Err.close();
	in.close();

	packet->nBodyBytes = pTem - packet->pEntityBody;
	packet->verson = VERSON_1_1;
	packet->StatusCode = STATUSCODE_202;
	char szLen[10];
	sprintf_s(szLen, sizeof(szLen), "%d", packet->nBodyBytes);
	packet->HeadLine.insert(std::pair<string,string>("Content-length", szLen));
	packet->HeadLine.insert(std::pair<string,string>("Content-type", "text/html"));
}

bool CSimpleCScript::ScriptWordAnalyse(ifstream &in)
{
	ofstream out(TEMP_FILE);
	const int nTokenMaxLen = 100;
	char szToken[nTokenMaxLen] = "";
	int cTokenCurSize = 0;
	char ch;
	UINT state = STATE_INIT;
	bool bHaveNum = false;     //�������к������֣��Ͳ�������ؼ��ֺͽ�����־�Ƚ�
	bool bSimpleCCode = true;  //��ǰ�Ƿ�Ϊsimple c ������
	bool bText = false;        //�Ƿ�Ϊ""��Χ���ı�
		
	while (bSimpleCCode && in.get(ch))
	{
		switch(state)
		{
		case STATE_INIT:
			if (ch == ' ' || ch == '\n')
				continue;

			cTokenCurSize = 0;
			if (isalpha(ch))
			{
				state = STATE_ID_KEYWORD;
				bHaveNum = false;
			}
			else if (isdigit(ch))
			{
				state = STATE_NUM;
			}
			else if (strchr(m_szSingleWord, ch) != 0)
			{
				//�ж��Ƿ�Ϊע��
				if (ch == '/')
				{
					char chTem;
					in.get(chTem);
					if (chTem == '*')
					{
						state = STATE_COMMENT;
						break;
					}
					in.putback(chTem);
				}
				state = STATE_SINGLEWORD;
			}
			else if (strchr(m_szPreDoubleWord, ch) != 0)
			{
				char chTem;
				in.get(chTem);
				if (chTem == '=')
					state = STATE_DOUBLEWORD;
				else
					state = STATE_SINGLEWORD;
					
				in.putback(chTem);
			}
			else
			{
				state = STATE_ERROR;
				break;
			}
			szToken[cTokenCurSize++] = ch;
			break;
		case STATE_ID_KEYWORD: //����������--��ĸ{��ĸ|����}
			if (isalpha(ch))
			{
				szToken[cTokenCurSize++] = ch;
			}
			else if (isdigit(ch))
			{
				if (!bHaveNum)
					bHaveNum = true;
				szToken[cTokenCurSize++] = ch;
			}
			else //��������
			{
				szToken[cTokenCurSize] = '\0';
				if (!bHaveNum)
				{
					//�ж��Ƿ�Ϊ������־
					if (strcmp(szToken, END_SYMBOL) == 0)
					{
						bSimpleCCode = false;
						break;
					}
					//�ж��Ƿ�Ϊ�ؼ���
					int index = 0;
					while (index != MAX_KEYWORD_NUM)
					{
						if (strcmp(szToken, m_ppKeyWord[index]) == 0)
						{
							out << szToken << '\t' << szToken << '\n';
							state = STATE_INIT;
							in.putback(ch);
							break;
						}
						index++;
					}
					if (index != MAX_KEYWORD_NUM)
						break;
				}
				out << "ID" << '\t' << szToken << '\n';
				state = STATE_INIT;
				in.putback(ch);
			}
			break;
		case STATE_NUM:
			if (isdigit(ch))
			{
				szToken[cTokenCurSize++] = ch;
			}
			else
			{
				szToken[cTokenCurSize] = '\0';
				out << "NUM" << '\t' << szToken << '\n';
				state = STATE_INIT;
				in.putback(ch);
			}
			break;
		case STATE_SINGLEWORD:
			out << szToken[0] << '\t' << szToken[0] << '\n';
			if (szToken[0] == '\"')
			{
				if (bText)
				{
					bText = false;
					state = STATE_INIT;
				}
				else
				{
					bText = true;
					state = STATE_TEXT;
					cTokenCurSize = 0;
				}
			}
			else
			{
				state = STATE_INIT;
			}
			in.putback(ch);
			break;
		case STATE_DOUBLEWORD:
			szToken[cTokenCurSize++] = ch;
			szToken[cTokenCurSize] = '\0';
			out << szToken << '\t' << szToken << '\n';
			state = STATE_INIT;
			break;
		case STATE_TEXT:
			if (ch != '\"')
			{
				szToken[cTokenCurSize++] = ch;
			}
			else
			{
				szToken[cTokenCurSize] = '\0';
				out << "TEXT" << '\t' << szToken << '\n';

				in.putback(ch);
				state = STATE_INIT;
			}
			break;
		case STATE_COMMENT:
			if (ch == '*')
			{
				in.get(ch);
				if (ch == '/')
					state = STATE_INIT;
			}
			break;
		case STATE_ERROR:
			in.getline(szToken, nTokenMaxLen);
			while (in && strcmp(szToken, END_SYMBOL) != 0)
				in.getline(szToken, nTokenMaxLen);
			out.close();
			return false;
		}
	}
	out.close();
	return true;
}

bool CSimpleCScript::ScriptProgramAnalyse(ofstream &Err)
{
	ifstream in(TEMP_FILE);
	if (!in)
		return false;

	m_mapVar.clear();

	int error = Program(in, Err);
	
	in.close();

	if (error > 0)
		return false;
	return true;
}

bool CSimpleCScript::ScriptProgramExecute(vector<string> &vecParm, char *buff, int *pLen)
{
	char *pTem = buff;
	CExpr expr;
	char szExpr[MAX_EXPR_LEN];
	ifstream in(OBJCODE_FILE);
	if (!in)
	{
		*pLen = 0;
		return false;
	}

	char token[MAX_VARNAME_LEN];
	int temPos = in.tellg();
	int begPos, endPos;
	bool bCondition = false; //��ǰ����Ƿ�Ϊ�������

	in >> token;
	while (in)
	{
		if (strcmp(token, "=") == 0) //��ֵ���
		{
			char szLVar[MAX_VARNAME_LEN];
			in >> szLVar;
			GenerateExpr(szExpr, in);
			int val;
			expr.Caculate(&val, szExpr);
			m_mapVar[szLVar] = val;

			if (bCondition)
			{
				bCondition = false;
				if (!val)
				{
					endPos = FindEndLable(begPos);
					in.seekg(endPos);
				}
			}
		}
		else if (strcmp(token, "write") == 0) //������
		{
			char ch;
			in >> ch;
			if (ch == '\"')
			{
				char szText[100];
				in.getline(szText, 100, '\"');
				EndOfCurLine(in);
				memcpy(pTem, szText, strlen(szText));
				pTem += strlen(szText);
			}
			else
			{
				in.putback(ch);
				GenerateExpr(szExpr, in);
				int val;
				expr.Caculate(&val, szExpr);
				char szNum[10];
				sprintf_s(szNum, 10, "%d", val);
				strcpy_s(pTem, 10, szNum);
				pTem += strlen(szNum);
			}
			
		}
		else if (strcmp(token, "read") == 0) //�������
		{
			string strVal;
			in >> strVal;
			vector<string>::const_iterator vecIter = vecParm.begin();
			if (vecIter != vecParm.end())
			{
				int val = atoi(vecIter->c_str());
				m_mapVar[strVal] = val;
				vecParm.erase(vecIter);
			}
			else
			{
				char szErr[] = "�ű�����(readʧ��)";
				int nLen = strlen(szErr);
				memcpy(pTem, szErr, nLen);
				pTem += nLen;
			}

			EndOfCurLine(in);
		}
		else if (strcmp(token, "loop") == 0) //ѭ�����
		{
			begPos = temPos;
			bCondition = true;
			EndOfCurLine(in);
		}
		else if (strcmp(token, "if") == 0) //�ж����
		{
			begPos = temPos;
			bCondition = true;
			EndOfCurLine(in);
		}
		else if (strcmp(token, "endloop") == 0)
		{
			EndOfCurLine(in);
			endPos = in.tellg();
			begPos = FindBegLable(endPos);
			in.seekg(begPos);
		}
		else if (strcmp(token , "endif") == 0)
		{
			EndOfCurLine(in);
		}
		else //һ����ʽ
		{
			in.seekg(temPos);
			GenerateExpr(szExpr, in);
			int val;
			expr.Caculate(&val, szExpr);

			if (bCondition)
			{
				bCondition = false;
				if (!val)
				{
					endPos = FindEndLable(begPos);
					in.seekg(endPos);
				}
			}
		}

		temPos = in.tellg();
		in >> token;
	}

	*pLen = pTem - buff;

	in.close();
	return true;
}

void CSimpleCScript::GenerateExpr(char *szExpr, ifstream &in)
{
	char *pTem = szExpr;
	char szTempVar[MAX_VARNAME_LEN];
	in >> szTempVar;
	int tempos = 0;

	while (strcmp(szTempVar, "#") != 0)
	{
		map<string,int>::const_iterator iter = m_mapVar.find(szTempVar);
		//��δ�ҵ������������������
		if (iter == m_mapVar.end()) 
		{
			strcpy_s(pTem, MAX_EXPR_LEN-(pTem-szExpr), szTempVar);
			pTem += strlen(szTempVar);
		}
		else
		{
			char szNum[10];
			sprintf_s(szNum, 10, "%d", iter->second);
			strcpy_s(pTem, MAX_EXPR_LEN-(pTem-szExpr), szNum);
			pTem += strlen(szNum);
		}

		tempos = in.tellg();
		in >> szTempVar;
	}

	//��ֹin����������/r/n�����lable���
	in.seekg(tempos);
	EndOfCurLine(in);
}

int CSimpleCScript::FindBegLable(int end)
{
	vector<LABLE>::iterator iter = m_vecLable.begin();
	while (iter != m_vecLable.end())
	{
		if (iter->end == end)
		{
			return iter->beg;
		}
		iter++;
	}

	return -1;
}

int CSimpleCScript::FindEndLable(int beg)
{
	vector<LABLE>::iterator iter = m_vecLable.begin();
	while (iter != m_vecLable.end())
	{
		if (iter->beg == beg)
		{
			return iter->end;
		}
		iter++;
	}

	return -1;
}

void CSimpleCScript::EndOfCurLine(ifstream &in)
{
	char ch;
	while (in.get(ch) && ch != '\n');
}

int CSimpleCScript::Program(ifstream &in, ofstream &Err)
{
	int error = 0;

	error = DeclarationList(in, Err);
	if (error > 0)
	{
		Err << "�ű����󣬱���ʧ��" << '\n';
		return error;
	}

	error = StatementList(in, Err);
	if (error > 0)
		Err << "�ű����󣬱���ʧ��" << '\n';
	else
		Err << "�ű�����ɹ�" << '\n';

	return error;
}

int CSimpleCScript::DeclarationList(ifstream &in, ofstream &Err)
{
	int error = 0;

	while ((error = DeclarationStat(in, Err)) == 0);

	return error;
}

int CSimpleCScript::DeclarationStat(ifstream &in, ofstream &Err)
{
	char ch;
	int error = 0;
	int FilePos;
	char token[KEYWORD_LEN];

	FilePos = in.tellg();
	in.getline(token, KEYWORD_LEN, '\t');

	int j = m_iDeclBeg;
	while(j != m_iDeclEnd)
	{
		if (strcmp(token,m_ppKeyWord[j]) == 0)
		{
			break;
		}
		j++;
	}
	if (j == m_iDeclEnd) // �������ֽ���
	{
		error = -1; //����-1��ʾ�������ֽ���
		in.seekg(FilePos);
	}
	else
	{
		EndOfCurLine(in);

		in.getline(token, KEYWORD_LEN, '\t');

		if (strcmp("ID", token) == 0)
		{
			//��¼�ű�����
			const int nMaxVarNameLen = 10;
			char szVarName[nMaxVarNameLen];
			in.getline(szVarName, nMaxVarNameLen);
			if (m_mapVar.find(szVarName) != m_mapVar.end())
			{
				Err << "����" << szVarName << "�ض���" << '\n';
				return 1;
			}
			m_mapVar.insert(std::pair<string,int>(szVarName, 0));
			
			in.get(ch);
			if (ch == ';')
			{
				error = 0;
				EndOfCurLine(in);
			}
			else
			{
				Err << "ȱ��;��" << '\n';
				error = 1;
			}
		}
		else
		{
			Err << "ȱ�ٱ�����" << '\n';
			error = 1;
		}
	}
	return error;
}

int CSimpleCScript::StatementList(ifstream &in, ofstream &Err)
{
	int error = 0;

	while ((error = Statement(in, Err)) == 0);

	return error;
}

int CSimpleCScript::Statement(ifstream &in, ofstream &Err)
{
	int eror = 0;
	char ch;

	in.get(ch);
	in.putback(ch);
	switch (ch)
	{
	case 'i': //if
		eror = IfStat(in, Err);
		break;
	case 'w': //while or write
		{
			char szTem[3];
			int filepos = in.tellg();
			in.get(szTem, 3);
			in.seekg(filepos);
			if (strcmp(szTem, "wh") == 0)
				eror = WhileStat(in, Err);
			else
				eror = WriteStat(in, Err);
		}
		break;
	case 'r': //read
		eror = ReadStat(in, Err);
		break;
	case 'f': //for
		eror = ForStat(in, Err);
		break;
	case ';':
	case 'I':
	case 'N':
		eror = ExpressionStat(in, Err);
		break;
	case '{':
		eror = CompoundStat(in, Err);
		break;
	default: // ���岿�ֽ���
		eror = -1;
	}
	return eror;
}

int CSimpleCScript::IfStat(ifstream &in, ofstream &Err)
{
	int error = 0;
	int filepos;
	char token[KEYWORD_LEN], ch;

	filepos = in.tellg();
	in.getline(token, KEYWORD_LEN, '\t');
	if (strcmp(token, "if") == 0)
	{
		LABLE lable;
		lable.beg = m_ObjCode.tellp();
		m_ObjCode << "if" << '\n';

		EndOfCurLine(in);
		in.get(ch);
		if (ch == '(')
		{
			EndOfCurLine(in);
			error = Expression(in, Err);
			if (error > 0)
				return error;

			m_ObjCode << '#' << '\n';
		}
		else
		{
			Err << "ȱ�� ( ����" << '\n';
			error = 1;
		}

		in.get(ch);
		if (ch == ')')
		{
			EndOfCurLine(in);
			error = Statement(in, Err);
			if (error > 0)
				return error;

			m_ObjCode << "endif" << '\n';
			lable.end = m_ObjCode.tellp();
			m_vecLable.push_back(lable);
		}
		else
		{
			Err << "ȱ�� ) ����" << '\n';
			error = 1;
		}
	}
	else
	{
		in.seekg(filepos); //���岿�ֽ���
		error = -1;
	}
	return error;
}

int CSimpleCScript::WhileStat(ifstream &in, ofstream &Err)
{
	int error = 0;
	int filepos;
	char token[KEYWORD_LEN];
	char ch;

	filepos = in.tellg();
	in.getline(token, KEYWORD_LEN, '\t');
	if (strcmp(token, "while") == 0)
	{
		LABLE lable;

		lable.beg = m_ObjCode.tellp();
		m_ObjCode << "loop" << '\n';

		EndOfCurLine(in);
		in.get(ch);
		if (ch == '(')
		{
			EndOfCurLine(in);
			error = Expression(in, Err);
			if (error > 0)
				return error;

			m_ObjCode << '#' << '\n';
		}
		else
		{
			Err << "ȱ��(����" << '\n';
			error = 1;
		}

		in.get(ch);
		if (ch == ')')
		{
			EndOfCurLine(in);
			error = Statement(in, Err);
			if (error > 0)
				return error;

			m_ObjCode << "endloop" << "\n";
			lable.end = m_ObjCode.tellp();
			m_vecLable.push_back(lable);
		}
		else
		{
			Err << "ȱ��)����" << '\n';
			error = 1;
		}
	} 
	else
	{
		in.seekg(filepos);
		error = -1;
	}
	return error;
}

int CSimpleCScript::ForStat(ifstream &in, ofstream &Err)
{
	int error = 0;
	int filepos;
	char token[KEYWORD_LEN];
	char ch;

	filepos = in.tellg();
	in.getline(token, KEYWORD_LEN, '\t');
	if (strcmp(token, "for") == 0)
	{
		EndOfCurLine(in);
		in.get(ch);
		if (ch == '(')
		{
			EndOfCurLine(in);
			error = Expression(in, Err);
			if (error > 0)
				return error;
			m_ObjCode << '#' << '\n';

			in.get(ch);
			if (ch == ';')
			{
				LABLE lable;
				lable.beg = m_ObjCode.tellp();
				m_ObjCode << "loop" << '\n';

				EndOfCurLine(in);
				error = Expression(in, Err);
				if (error > 0)
					return error;
				m_ObjCode << '#' << '\n';

				in.get(ch);
				if (ch == ';')
				{
					//for�Ĵ˱��ʽҪ�������
					int pos = m_ObjCode.tellg();
					EndOfCurLine(in);
					error = Expression(in, Err);
					if (error > 0)
						return error;
					m_ObjCode << '#' << '\n';
					char szTemExpr[MAX_EXPR_LEN];
					m_ObjCode.seekg(pos);
					m_ObjCode.getline(szTemExpr, MAX_EXPR_LEN);

					in.get(ch);
					if (ch == ')')
					{
						m_ObjCode.seekp(pos);
						EndOfCurLine(in);
						error = Statement(in, Err);
						if (error > 0)
							return error;
						m_ObjCode << szTemExpr << '\n';
						m_ObjCode << "endloop" << '\n';
						lable.end = m_ObjCode.tellp();
						m_vecLable.push_back(lable);
					} 
					else
					{
						Err << "ȱ��)����" << '\n';
						error  = 1;
					}
				} 
				else
				{
					Err << "ȱ��;" << '\n';
					error  = 1;
				}
			} 
			else
			{
				Err << "ȱ��;" << '\n';
				error  = 1;
			}
		}
		else
		{
			Err << "ȱ��(����" << '\n';
			error = 1;
		}
	}
	else
	{
		in.seekg(filepos);
		error = -1;
	}

	return error;
}

int CSimpleCScript::WriteStat(ifstream &in, ofstream &Err)
{
	int error = 0;
	int filepos;
	char token[KEYWORD_LEN];
	char ch;

	filepos = in.tellg();
	in.getline(token, KEYWORD_LEN, '\t');
	if (strcmp(token, "write") == 0)
	{
		m_ObjCode << token << ' ';

		EndOfCurLine(in);
		in.get(ch);
		if (ch == '\"')
		{
			m_ObjCode << ch;
			EndOfCurLine(in);
			in.getline(token, KEYWORD_LEN, '\t');
			if (strcmp(token, "TEXT") == 0)
			{
				char szText[100];
				in.getline(szText, 100);
				m_ObjCode << szText;
				in.get(ch);
				if (ch != '\"')
				{
					Err << "ȱ��\"" << '\n';
					return 1;
				}
				m_ObjCode << ch;
				EndOfCurLine(in);
			}
			else if (strcmp(token, "\"") == 0)
			{
				m_ObjCode << ch;
				EndOfCurLine(in);
			}
			else
			{
				Err << "ȱ��\"" << '\n';
				return 1;
			}
			
		}
		else 
		{
			in.putback(ch);
			error = Expression(in, Err);
			if (error > 0)
				return error;
		}
		
		in.get(ch);
		EndOfCurLine(in);
		if (ch != ';')
		{
			Err << "ȱ��;" << '\n';
			error = 1;
		}

		m_ObjCode << '#' << '\n';
	} 
	else
	{
		in.seekg(filepos);
		error = -1;
	}
	return error;
}

int CSimpleCScript::ReadStat(ifstream &in, ofstream &Err)
{
	int error = 0;
	int filepos;
	char token[MAX_VARNAME_LEN];
	char ch;

	filepos = in.tellg();
	in.getline(token, KEYWORD_LEN, '\t');
	if (strcmp(token, "read") == 0)
	{
		m_ObjCode << token << ' ';

		EndOfCurLine(in);
		in.getline(token, KEYWORD_LEN, '\t');
		if (strcmp(token, "ID") != 0)
		{
			Err << "ȱ�ٲ���ID" << '\n';
			return 1;
		}

		in.getline(token, MAX_VARNAME_LEN);
		if (m_mapVar.find(token) == m_mapVar.end())
		{
			Err << "����δ����" << '\n';
			return 1;
		}
		m_ObjCode << token << ' ';

		in.get(ch);
		EndOfCurLine(in);
		if (ch != ';')
		{
			Err << "ȱ��;" << '\n';
			return 1;
		}
		m_ObjCode << '#' << '\n';
	} 
	else
	{
		in.seekg(filepos);
		error = -1;
	}
	return error;
}

int CSimpleCScript::CompoundStat(ifstream &in, ofstream &Err)
{
	int error = 0;
	char ch;

	in.get(ch);
	if (ch != '{')
	{
		Err << "ȱ��{����" << "\n";
		return 1;
	}

	EndOfCurLine(in);
	error = StatementList(in, Err);
	if (error > 0)
		return error;

	in.get(ch);
	EndOfCurLine(in);
	if (ch != '}')
	{
		Err << "ȱ��}����" << "\n";
		return 1;
	}

	return 0;
}

int CSimpleCScript::ExpressionStat(ifstream &in, ofstream &Err)
{
	int error = 0;
	char ch;

	in.get(ch);
	if (ch != ';')
	{
		in.putback(ch);
		error = Expression(in, Err);
		if (error <= 0)
		{
			in.get(ch);
			EndOfCurLine(in);
			if (ch != ';')
			{
				Err << "ȱ��;" << '\n';
				error = 1;
			}
			else
			{
				m_ObjCode << '#' << '\n';
			}
		}
	} 
	return error;
}

int CSimpleCScript::Expression(ifstream &in, ofstream &Err)
{
	int error = 0;
	int file_pos;
	char token[KEYWORD_LEN];

	file_pos = in.tellg();
	in.getline(token, KEYWORD_LEN, '\t');
	if (strcmp(token, "ID") == 0)
	{
		//������������Ա�����Ŀ�����
		const int nVarNameLen = 10;
		char szVarName[nVarNameLen];
		in.getline(szVarName, nVarNameLen);
		if (m_mapVar.find(szVarName) == m_mapVar.end())
		{
			Err << "����" << szVarName << "δ����" << '\n';
			return 1;
		}
		
		in.getline(token, KEYWORD_LEN, '\t');
		if (strcmp(token, "=") == 0)
		{
			m_ObjCode << '=' << ' ' << szVarName << ' ';

			EndOfCurLine(in);
			error = BoolExpr(in, Err);
		} 
		else
		{
			in.seekg(file_pos);
			error = BoolExpr(in, Err);
		}
	} 
	else 
	{
		in.seekg(file_pos);
		error = BoolExpr(in, Err);
	}

	return error;
}

int CSimpleCScript::BoolExpr(ifstream &in, ofstream &Err)
{
	int error = 0;
	int filepos;
	char token[3];
	

	error = AdditiveExpr(in, Err);
	if (error > 0)
		return error;

	filepos = in.tellg();
	in.getline(token, 3, '\t');
	int i = 0;
	while (i != MAX_BOOLWORD_NUM )
	{
		if (strcmp(token, m_ppBoolWord[i]) == 0)
			break;
		i++;
	}
	if (i != MAX_BOOLWORD_NUM)
	{
		m_ObjCode << token << ' ';

		EndOfCurLine(in);
		error = AdditiveExpr(in, Err);
		if (error > 0)
			return error;
	}
	else
	{
		in.seekg(filepos);
	}

	return error;
}

int CSimpleCScript::AdditiveExpr(ifstream &in, ofstream &Err)
{
	int error = 0;
	char ch;

	error = Term(in, Err);
	if (error <= 0)
	{
		in.get(ch);
		while (ch == '+' || ch == '-')
		{
			m_ObjCode << ch << ' ';

			EndOfCurLine(in);
			error = Term(in, Err);
			if (error > 0)
				return error;
			in.get(ch);
		}
		in.putback(ch);
	}

	return error;
}

int CSimpleCScript::Term(ifstream &in, ofstream &Err)
{
	int error = 0;
	char ch;

	error = Factor(in, Err);
	if (error <= 0)
	{
		in.get(ch);
		while (ch == '*' || ch == '/')
		{
			m_ObjCode << ch << ' ';

			EndOfCurLine(in);
			error = Factor(in, Err);
			if (error > 0)
				return error;
			in.get(ch);
		}
		in.putback(ch);		
	}

	return error;
}

int CSimpleCScript::Factor(ifstream &in, ofstream &Err)
{
	int error = 0;
	char token[KEYWORD_LEN];
	char ch;

	in.getline(token, KEYWORD_LEN, '\t');
	if (strcmp(token, "(") == 0)
	{
		m_ObjCode << '(' << ' ';
		EndOfCurLine(in);
		error = Expression(in, Err);
		if (error > 0)
			return error;
		in.get(ch);
		if (ch != ')')
		{
			Err << "ȱ��)����" << '\n';
			return 1;
		}
		m_ObjCode << ')' << ' ';
		EndOfCurLine(in);
	}
	else if (strcmp(token, "ID") == 0)
	{
		const int nVarNameLen = 10;
		char szVarName[nVarNameLen];
		in.getline(szVarName, nVarNameLen);
		m_ObjCode << szVarName << ' ';
	}
	else if (strcmp(token, "NUM") == 0)
	{
		const int nNumLen = 10;
		char szNum[nNumLen];
		in.getline(szNum,nNumLen);
		m_ObjCode << szNum << ' ';
	}
	else
	{
		error = 1;
	}
	return error;
}