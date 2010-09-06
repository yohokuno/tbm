// SunExeption.cpp: SunExeption クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunExeption.h"
#include "SunLog.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SunExeption::SunExeption(const char* fmt,...)
{
	char str[512];	//決めうちｗ
	va_list arg;
	va_start(arg,fmt);
	vsprintf(str, fmt, arg);
	va_end(arg);

	m_str = str;
}

SunExeption::~SunExeption()
{

}

int SunExeption::operator ()()
{
	SunLog.LogOut(m_str.c_str());
	::MessageBox(NULL,m_str.c_str(),"エラー",MB_OK | MB_ICONERROR);
	return 1;
}