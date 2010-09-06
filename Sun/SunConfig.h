/**
	@file SunConfig.h
	@brief CSunConfig クラスのインターフェイス
*/

#pragma once

/**
	@class CSunConfig
	@brief コンフィグクラス

	コンフィグユーティリティ。コンフィグそのものではない。
	ファイル名やセクション名をメンバを使って省略する
*/
class CSunConfig  
{
protected:
//メンバ変数（プロパティ）
	string	m_file;		//コンフィグファイル名
	string	m_section;	//コンフィグセクション名
public:
//構築と消滅
	CSunConfig();
	virtual ~CSunConfig();
//セット
	void SetFile(char *file){m_file = file;}
	void SetSection(char *section){m_section = section;}
//メンバ関数
//セクション固定
	string	GetString( const char *key);
	int		GetInt(const char *key);
	double	GetDouble(const char *key);
	int		SetString(const char *key,const char *str);
	int		SetInt(const char *key,int value);
	int		SetDouble(const char *key,double value);
//ファイル固定
	string	GetString(const char *section, const char *key);
	int		GetInt(const char *section, const char *key);
	double	GetDouble(const char *section, const char *key);
	int		SetString(const char *section, const char *key,const char *str);
	int		SetInt(const char *section, const char *key,int value);
	int		SetDouble(const char *section, const char *key,double value);
//完全自由
	string	GetString(const char *file,const char *section, const char *key);
	int		GetInt(const char *file,const char *section, const char *key);
	double	GetDouble(const char *file,const char *section, const char *key);
	int		SetString(const char *file,const char *section, const char *key,const char *str);
	int		SetInt(const char *file,const char *section, const char *key,int value);
	int		SetDouble(const char *file,const char *section, const char *key,double value);
};
/**
	応用的なコンフィグユーティリティ
	指定したキーが存在しない場合、読み込み関数でデフォルト値を書き込む。
*/
class CSunConfig2 : public CSunConfig
{
protected:
	double	m_version;	//コンフィグファイルのバージョン
public:
//構築と消滅
	CSunConfig2();
	virtual ~CSunConfig2();
//ゲット
	double	GetVersion(){return m_version;}
//読み込み・書き込み
	int		Read();		//バージョン情報を読み取る
	int		Write();	//バージョン情報を書き込む・普通はデストラクタが呼んでくれる
//セクション固定
	string	GetString2(const char *key,const char *def);
	int		GetInt2(const char *key,const int def);
//ファイル固定
	string	GetString2(const char *section, const char *key,const char *def);
	int		GetInt2(const char *section, const char *key,const int def);
//完全自由
	string	GetString2(const char *file,const char *section, const char *key,const char *def);
	int		GetInt2(const char *file,const char *section, const char *key,const int def);
};
extern CSunConfig2 SunConfig;

/**
	オプションクラス

	１つのint型のオプションを現す。コンフィグ画面用。
	読み込み時の値を保存しておき、更新されているかどうかを取得できる。
*/
class SunOption
{
public:
	int m_old;	//初期値
	int m_now;	//設定値
	string m_section;	//
	string m_key;		//
public:
	SunOption();
	~SunOption();
	int Load(char* section,char* key,int def);	//ロード
	int Save();							//セーブ（ロードしたあと）
	int GetUpdate(){return m_old!=m_now;}
};