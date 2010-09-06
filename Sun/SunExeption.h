// SunExeption.h: SunExeption クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

/**
	@brief 例外クラス

	例外クラスです。
	ログファイルを出力し、メッセージボックスを表示します。
*/
class SunExeption
{
private:
	string m_str;	///<エラー文字列
public:
	SunExeption(const char* fmt,...);
	virtual ~SunExeption();
	int operator()();
};

