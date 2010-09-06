/**
	@file SunFile.h
	@brief SunFile クラスのインターフェイス
*/

#pragma once

// 内部でSunFileBaseを使う関数群
DWORD GetFileSize(char* pFileName);
int ReadFile(char* pFileName,void* pBuffer,DWORD dwSize);
int WriteFile(char* pFileName,void* pBuffer,DWORD dwSize);
/**
	@brief ファイルクラス基礎

	なにかと無駄の多いＳＤＫのファイル関数を単純にラップしたもの。
	本当はＭＦＣかＷＴＬから該当部分だけ取ってこれればいいんだけど。。
	
	これを直接使う用途は、スコアファイルやリプレイファイル（書き込みが必要なもの）くらい。
*/
class SunFileBase
{
protected:
	HANDLE	m_hFile;	
public:
	SunFileBase();
	virtual ~SunFileBase();
	int OpenFileReadWrite(char* pFileName);
	int OpenFileRead(char* pFileName);
	int OpenFileWrite(char* pFileName);
	int CloseFile();
	DWORD GetFileSize();
	DWORD ReadFile(void* pBuffer,DWORD dwSize);
	DWORD WriteFile(void* pBuffer,DWORD dwSize);
	DWORD SeekFile(LONG lDistanceToMove,DWORD dwMoveMethod);
};
/**
	@brief ファイルクラス

	バイナリファイルの読み込みを行うクラスです。
	書き込みはできません。
	ＬＰＡＣＫによるパックファイルも読めます。
	（直接ファイルがなければパックファイルから読み込む）
	ＬＺＳＳ圧縮にも対応・・しないかも。（ぉ
	なお、基底クラスのメンバに外部からアクセスしてはいけません。
*/
class SunFile  : public SunFileBase
{
protected:
	DWORD	m_dwStart;		///< ファイル開始位置
	DWORD	m_dwSize;		///< ファイル終了位置
	char	m_strPack[MAX_PATH];
	char	m_strFile[MAX_PATH];
public:
	SunFile();
	virtual ~SunFile();
	int OpenFile( char *pFile );						///< ファイルを開く
	int OpenFileFromPack( char *pFile, char *pPack );	///< パックファイルから開く
	int OpenFileFromDirectory( char *pFile);			///< ディレクトリから開く
	int FileRead(void *pData, DWORD dwSize);			///< ファイル読み込み
	int GetSize(){return (int)m_dwSize;}				///< サイズ取得
	HANDLE GetHandle(){return m_hFile;}	///<ストリーミング用にシーク済みファイルハンドル取得
	DWORD GetStart(){return m_dwStart;}	///<ストリーミング用
	char	*GetPack(){return m_strPack;}
	char	*GetFile(){return m_strFile;}
};
/**
	@brief ファイルデータクラス

	SunFileクラスはファイルの読み込み先のバッファは自作して渡す必要があり、
	このときにvoidポインタを使う必要があり危険です。
	このクラスはファイルの読込先であるデータまで管理します。
	読み込みはファイル全体（パックファイルなら目的のファイルのみ）です。
	ただし書き込みはできません。
	なお、基底クラスのメンバに外部からアクセスしてはいけません。
*/
class SunFileEx : protected SunFile
{
protected:
	void* m_pBuffer;	///<ファイル読み込みバッファ
public:
	SunFileEx();
	virtual ~SunFileEx();
	int Free();								///<バッファ解放
	int Read();								///<全読み込み
	void* GetBuffer(){return m_pBuffer;}	///<バッファポインタ取得
	int GetSize(){return (int)m_dwSize;}	///< サイズ取得
	int Open(char* pFile){return OpenFileFromDirectory(pFile);}
};