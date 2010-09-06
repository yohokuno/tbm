// SunFile.cpp: SunFile クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunFile.h"
#include "SunExeption.h"
#include "SunUtility.h"
/**
	@brief ファイル読み込み	

	static関数
*/
int ReadFile(char* pFileName,void* pBuffer,DWORD dwSize)
{
	SunFileBase file;
	if( file.OpenFileRead(pFileName) == 0)
		return 0;
	return file.ReadFile(pBuffer,dwSize);
}
/**
	@brief ファイル書き込み

	static関数
*/
int WriteFile(char* pFileName,void* pBuffer,DWORD dwSize)
{
	SunFileBase file;
	if( file.OpenFileWrite(pFileName) == 0)
		return 0;
	return file.WriteFile(pBuffer,dwSize);
}
/**
	@brief ファイルサイズ取得

	static関数
*/
DWORD GetFileSize(char* pFileName)
{
	SunFileBase file;
	if( file.OpenFileRead(pFileName) == 0)
		return 0;
	return file.GetFileSize();
}
/**
	@class SunFileBase
*/
SunFileBase::SunFileBase()
{
	m_hFile = INVALID_HANDLE_VALUE;
}

SunFileBase::~SunFileBase()
{
	this->CloseFile();
}
/**
	@brief ファイルを読み書き可能にして開きます。

	ファイルが存在しないときのみ作成します。
*/
int SunFileBase::OpenFileReadWrite(char* pFileName)
{
	m_hFile = ::CreateFile(
		pFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL );
	return (m_hFile!=INVALID_HANDLE_VALUE);
}

/**
	@brief ファイルを読み込み可能にして開きます。

	ファイルが存在しないと失敗します。
*/
int SunFileBase::OpenFileRead(char* pFileName)
{
	m_hFile = ::CreateFile(
		pFileName, GENERIC_READ,
		FILE_SHARE_READ,
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL );
	return (m_hFile!=INVALID_HANDLE_VALUE);
}
/**
	@brief ファイルを書き込み可能にして開きます。

	常にファイルを作成します。
	ファイルがすでに存在していると上書きします。
*/
int SunFileBase::OpenFileWrite(char* pFileName)
{
	m_hFile = ::CreateFile(
		pFileName, GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL );
	return (m_hFile!=INVALID_HANDLE_VALUE);
}
/**
	@brief ファイルを閉じます。
*/
int SunFileBase::CloseFile()
{
	if(m_hFile != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
	return 1;
}
/**
	@brief ファイルサイズを取得します。
*/
DWORD SunFileBase::GetFileSize()
{
	return ::GetFileSize(m_hFile,NULL);
}
/**
	@brief ファイルを読みます。
*/
DWORD SunFileBase::ReadFile(void* pBuffer,DWORD dwSize)
{
	if ( m_hFile == INVALID_HANDLE_VALUE )
		return 0;
	DWORD dwReadBytes;
	::ReadFile( m_hFile, pBuffer, dwSize, &dwReadBytes, NULL);
	return dwReadBytes;
}
/**
	@brief ファイルに書きます。
*/
DWORD SunFileBase::WriteFile(void* pBuffer,DWORD dwSize)
{
	if ( m_hFile == INVALID_HANDLE_VALUE )
		return 0;
	DWORD dwWriteBytes;
	::WriteFile( m_hFile, pBuffer, dwSize, &dwWriteBytes, NULL);
	return dwWriteBytes;
}
/**
	シーク（ファイルポインタの移動）
*/
DWORD SunFileBase::SeekFile(LONG lDistanceToMove,DWORD dwMoveMethod)
{
	return ::SetFilePointer(m_hFile,lDistanceToMove,NULL,dwMoveMethod);
}
/**
	@brief	ファイルデータヘッダ

	ファイル内容に関するデータを格納しているヘッダ
*/
struct FILE_DATA
{
	char FileName[64];			///< ファイル名
	unsigned long Address;		///< ファイル開始位置
	unsigned long Size;			///< ファイルサイズ
};

/**
	@brief	バイナリデータヘッダ

	バイナリデータに関するデータを格納しているヘッダ
*/
struct BINARY_HEADER
{
	char Guid[4];				///< 'P' 'A' 'C' 'K'
	unsigned long FileCount;	///< ファイル数
};

/**
	@class SunFile
*/
// 構築/消滅

SunFile::SunFile()
{
	m_dwStart = 0;
	m_dwSize = 0;
	m_strPack[0] = '\0';
	m_strFile[0] = '\0';
}

SunFile::~SunFile()
{
}

/**
	指定ファイルのファイルハンドルを取得します。<BR>
	取得したファイルハンドルは読み込み・書き込みができます。

	@param pFile	[in] ファイル名
*/
int SunFile::OpenFile( char *pFile )
{
	if(pFile == NULL)
		return 0;
	this->CloseFile();
	// ファイルオープン
	if( this->OpenFileRead( pFile ) == 0)
		return 0;
	// パックファイルではないのでアドレスは０
	m_dwStart = 0;

	// ファイルサイズ取得
	m_dwSize = this->GetFileSize();

	return 1;
}

/**
	パックファイル内の指定ファイルのファイルハンドルを取得します。<BR>
	取得したファイルハンドルは読み込み・書き込みができます。

	@param pFile	[in] ファイル名
	@param pPack	[in] パックファイル名

*/
SunFile::OpenFileFromPack( char *pFile, char *pPack )
{
	BINARY_HEADER BinHead;	//ファイルヘッダ構造体
	FILE_DATA FileData;		//ファイルデータ構造体
	unsigned long i;
	//-----------------------------------------------------------
	// ファイルオープン
	//-----------------------------------------------------------
	if( this->OpenFile(pPack) == 0)
		return 0;

	//-----------------------------------------------------------
	// ヘッダ読み込み
	//-----------------------------------------------------------
	this->ReadFile(&BinHead, sizeof(BINARY_HEADER) );

	//-----------------------------------------------------------
	// ヘッダチェック
	//-----------------------------------------------------------
	if ( BinHead.Guid[0] != 'P' ) return 0;
	if ( BinHead.Guid[1] != 'A' ) return 0;
	if ( BinHead.Guid[2] != 'C' ) return 0;
	if ( BinHead.Guid[3] != 'K' ) return 0;

	//-----------------------------------------------------------
	// 該当ファイル検索
	//-----------------------------------------------------------
	for ( i = 0; i < BinHead.FileCount; i++ )
	{
		// ファイルデータ読み込み
		this->ReadFile(&FileData, sizeof(FILE_DATA) );
		// ファイル名チェック（大文字・小文字を区別しません）
		if ( !stricmp( FileData.FileName, pFile ) )
		{
			// 該当アドレスまで移動
			this->SeekFile( FileData.Address, FILE_BEGIN );

			// ファイルデータ
			m_dwStart = FileData.Address;
			m_dwSize = FileData.Size;

			return 1;
		}
	}

	return 0;
}
/**
	ディレクトリ内のファイルを指定して、存在しなかったらディレクトリ名と同じ名前のパックファイルから探します。
	取得したファイルハンドルは読み込み・書き込みができます。
	
	@param pFile	[in] ファイル名
	@param pPack	[in] パックファイル名

*/
int SunFile::OpenFileFromDirectory( char *pFile)
{
	//フルパスだったりして・・（汗）
	//いや、DirectMusicのコールバック関数の関係で必要なんです
	string current = GetCurrent();
	if(strstr(pFile,current.c_str())!=NULL)
	{
		//相対パスを渡す
		return this->OpenFileFromDirectory( &pFile[strlen(current.c_str())+1]);
	}
	//普通のファイルを試す
	if( this->OpenFile(pFile) == 0)
	{
		char* ptr;
		//ディレクトリ名とファイル名の境目
		{
			ptr = strchr(pFile,'\\');
			if(ptr == NULL )
				ptr = strchr(pFile,'/');
			if(ptr == NULL || *ptr == '\0')	
				return 0;
		}
		//ディレクトリ名を切り出す
		{
			size_t size = abs(ptr - pFile);
			if(size > 255)
				return 0;
			strncpy(m_strPack,pFile,size);
			strcpy(&m_strPack[size],".bin");
		}
		//ファイル名を切り出す
		{
			ptr++;
			if(ptr == NULL || *ptr == '\0')	
				return 0;
			strcpy(m_strFile,ptr);
		}
		if( this->OpenFileFromPack(m_strFile,m_strPack) == 0)
			return 0;
	}
	else {
		strcpy( m_strFile, pFile );
	}
	return 1;
}
/**
	オープンしたファイルへの読み込み処理を行います。
	バッファサイズ以上の読み込みを行おうとした場合、
	読み込みは行われません。

	@param Size		[in] 読み込み込むデータサイズ
	@param pData	[in] 読み込むデータのポインタ
*/
int SunFile::FileRead(void *pData, DWORD dwSize)
{
	if(m_hFile == INVALID_HANDLE_VALUE)
		return 0;

	// ファイルの現在位置を取得
	unsigned long Pos = SeekFile( 0, FILE_CURRENT );

	// オーバーチェック
	if ( Pos + dwSize > m_dwStart + m_dwSize )
		return 0;

	// ファイル読み込み
	if( this->ReadFile( pData, dwSize ) == 0)
		return 0;

	return 1;
}

/**
	@class SunFileEx
*/
// 構築/消滅

SunFileEx::SunFileEx()
{
	m_pBuffer = NULL;
}

SunFileEx::~SunFileEx()
{
	this->Free();
}
/**
	解放
*/
int SunFileEx::Free()
{
	if(m_pBuffer != NULL)
		free(m_pBuffer);
	m_pBuffer = NULL;
	return 1;
}
/**
	読み込み
*/
int SunFileEx::Read()
{
	this->Free();
	m_pBuffer = malloc(m_dwSize);
	if(this->ReadFile(this->m_pBuffer,this->m_dwSize) == 0)
		return 0;;
	return 1;
}