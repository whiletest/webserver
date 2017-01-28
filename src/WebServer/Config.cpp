/*************************************************
Copyright
File name: config.cpp
Author: ’≈≥Ø–Ò
Version: 
Date: 2012/11/26
*************************************************/
#include "StdAfx.h"
#include "Config.h"
#include <fstream>
using std::ifstream;
using std::ofstream;

CConfig::CConfig(void)
{
	m_cfg.list = LIST_NONE;
	m_cfg.record = RECORD_NONE;
	m_cfg.script = SCRIPT_NONE;
}

CConfig::~CConfig(void)
{
}

ofstream& operator << (ofstream &out, CONFIG &cfg)
{
	out << cfg.record << '\n';
	out << cfg.list << '\n';
	out << cfg.script << '\n';
	out << cfg.szRootPath << '\n';
	return out;
}

ifstream& operator >> (ifstream &in, CONFIG &cfg)
{
	in >> cfg.record;
	in >> cfg.list;
	in >> cfg.script;
	in >> cfg.szRootPath;
	return in;
}

void CConfig::WriteConfig()
{
	ofstream out(CONFIG_FILE);
	out << m_cfg;
	out.close();
}

bool CConfig::ReadConfig()
{
	ifstream in(CONFIG_FILE);
	if (!in)
		return false;
	in >> m_cfg;
	in.close();
	return true;
}


