/**
	@file SunLog.h
	@brief CSunLog クラスのインターフェイス
*/

#pragma once

/**
	@class CSunLog
	@brief ログファイル出力クラス

	ログ出力ユーティリティとしての機能と、
	ＣＰＵ情報などの具体的なログ出力をメンバなしでできる範囲でカバーします。
	もともとＳｕｎクラスに入ってたのですが
	見苦しかったので分割しましたｗ
	このクラスはほかのどのクラスにも依存しません(SunUtilityにも)。
*/
class CSunLog  
{
private:
	string	m_logfilename;	//ログファイル名
public:
//基本機能
	CSunLog();
	CSunLog(const char* file);
	virtual ~CSunLog();
	int LogStart();						//stderrのリダイレクト
	int LogOut(const char *fmt,...);	//printf風ログファイル出力
	int LogCaps(int i,char *text);
	int operator()(const char *fmt,...);
//内部使用関数
//以下の情報ログ関係の関数はほぼＬｕｎａのコピペ
	int GetAllInformation();
	int GetCPUInformation();
	int GetOSInformation();
	int GetMemoryInformation();
	int GetSystemInformation();
};

extern CSunLog		SunLog;
