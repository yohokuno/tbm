// SunSound.cpp: SunSound クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunLog.h"
#include "SunConfig.h"
#include "SunUtility.h"
#include "SunExeption.h"
#include "SunFile.h"
#include "SunApp.h"
#include "SunSound.h"
#include "Ayame.h"


CSunSound	SunSound;
CSunAyame	SunAyame;

#define BGM_OGG	2
#define BGM_AYM 3

static int bgm_mode = BGM_AYM;

//ラッパ関数
void PlayBGM(char *pFile){
	char buffer[MAX_PATH];
	switch( bgm_mode ) {
	case BGM_OGG:
		sprintf( buffer, "ogg\\%s.ogg", pFile );
		SunAyame.Play( buffer );
		break;
	case BGM_AYM:
		sprintf( buffer, "bgm\\%s.wav", pFile );
		SunAyame.Play( buffer );
		break;
	}
}
void ResumeBGM(){
//	SunAyame.Resume();
}
void StopBGM(){
	SunAyame.Stop();
}
void PauseBGM(){
//	SunAyame.Pause();
}
/**
	サウンドオブジェクト
*/
CSunSound::CSunSound()
{
	CoInitialize(NULL);
	m_pDS = NULL;
	m_pDS8 = NULL;
	m_sevolume = 0;
	m_bgmvolume = 0;
}

CSunSound::~CSunSound()
{
	Release();
	CoUninitialize();
}

void CSunSound::Init()
{
	//DirectSound8オブジェクトを取得
	if(FAILED(DirectSoundCreate8(NULL, &m_pDS8, NULL)))
		throw SunExeption("DirectSoundの初期化に失敗しました。\n");

	//強調レベルを設定
	if(FAILED(m_pDS8->SetCooperativeLevel(SunApp.m_hWnd, DSSCL_NORMAL)))
		throw SunExeption("DirectSoundの初期化に失敗しました。\n");

	//DirectSoundオブジェクトを取得
	if(FAILED(m_pDS8->QueryInterface(IID_IDirectSound,(void**)&m_pDS)))
		throw SunExeption("IDirectSoundへのQueryInterfaceに失敗しました\n");

	//コンフィグロード
	m_sevolume = SunConfig.GetInt2("config.ini","sound","sevolume",SEVOLUME);
	m_bgmvolume = SunConfig.GetInt2("config.ini","sound","bgmvolume",BGMVOLUME);

	//Ayameの初期化
	SunAyame.Init( "Ayame.dll" );
}
void CSunSound::Release()
{
	SunAyame.Release();

	SAFE_RELEASE( m_pDS8 );
	SAFE_RELEASE( m_pDS );
}
/**
	SunSoundBufferクラス
*/
SunSoundBuffer::SunSoundBuffer()
{
	m_pdsb = NULL;
	m_pDS = SunSound.m_pDS8;
}
SunSoundBuffer::SunSoundBuffer(char* pFile)
{
	m_pdsb = NULL;
	m_pDS = SunSound.m_pDS8;
	LoadFromDirectory(pFile);
}
SunSoundBuffer::~SunSoundBuffer()
{
	Release();
}
int SunSoundBuffer::Release()
{
	SAFE_RELEASE(m_pdsb);
	return 1;
}
//再生
int SunSoundBuffer::Play(DWORD dwFlags)
{
	if( m_pdsb != NULL ) {
		m_pdsb->SetCurrentPosition(0);
		m_pdsb->Play(0,0,dwFlags);
	}
	return 1;
}
//再開
int SunSoundBuffer::Resume(DWORD dwFlags)
{
	if( m_pdsb != NULL ) {
		m_pdsb->Play(0,0,dwFlags);
	}
	return 1;
}
//停止
int SunSoundBuffer::Stop()
{
	DWORD dwFlags;
	if( m_pdsb != NULL ){
		if(FAILED(m_pdsb->GetStatus(&dwFlags)))
			return 0;
		else
			if(dwFlags & DSBSTATUS_PLAYING)
				m_pdsb->Stop();
	}
	return 1;
}
//ボリューム設定
int SunSoundBuffer::SetVolume( LONG lVolume )
{
//	LONG volume = lVolume + log10((double)(SunSound.m_sevolume+1)/101.0)*2000;
	LONG volume = lVolume + DSBVOLUME_MIN + ( DSBVOLUME_MAX - DSBVOLUME_MIN ) * SunSound.m_sevolume / 100;
	volume = min( DSBVOLUME_MAX, max( DSBVOLUME_MIN, volume ) );
	if( m_pdsb != NULL ){
		if(FAILED(m_pdsb->SetVolume( volume )))
			return 0;
	}
	return 1;
}
//パン設定
int SunSoundBuffer::SetPan( LONG lPan )
{
	if( FAILED( m_pdsb->SetPan( lPan )))
		return 0;
	return 1;
}
//ファイルからロード
int SunSoundBuffer::Load(char* lpszFileName)
{
	//RIFF構造のファイルを読み込むのに必要な構造体です。
	MMCKINFO oya_chunk, ko_chunk;
	
	//waveファイルのオープン
	HMMIO hmmio = mmioOpen(lpszFileName, NULL, MMIO_READ|MMIO_ALLOCBUF);
	if(!hmmio) {
		//throw SunExeption("%sのロードに失敗しました\n",lpszFileName);
		return 0;
	}
	
	//waveファイルかどうか調べる
	oya_chunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if(mmioDescend(hmmio, &oya_chunk, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
		mmioClose(hmmio, 0);
		return 0;
	}
	
	//fmtチャンクへ移動する
	ko_chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if(mmioDescend(hmmio, &ko_chunk, &oya_chunk, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
		mmioClose(hmmio, 0);
		return 0;
	}
	
	//fmtチャンクを読み取ります。
	//WAVEFORMATEX構造体に必要な情報がコピーされます。
	if(mmioRead(hmmio, (HPSTR)&m_wf, (LONG)ko_chunk.cksize) != (LONG)ko_chunk.cksize) {
		mmioClose(hmmio, 0);
		return 0;
	}
	
	//fmtサブチャンクの外部に移動する
	//チャンクを潜ったので外に出ます。
	//ああ、なんてセンスの無い処理なんでしょう(^^;
	//もうちょっとスマートに出来るハズだろうと言ってやりたいですね。
	mmioAscend(hmmio, &ko_chunk, 0);
	
	//dataチャンクに移動
	ko_chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(hmmio, &ko_chunk, &oya_chunk, MMIO_FINDCHUNK)) {
		mmioClose(hmmio, 0);
		return 0;
	}

	//バッファ作成
	this->CreateBuffer( ko_chunk.cksize );

	LPVOID pmem;
	DWORD size;

	//サウンドバッファをロック
	//ロックしたサイズとバッファの先頭アドレスが取得出来ます。
    if( FAILED( m_pdsb->Lock(0, ko_chunk.cksize, &pmem, &size, NULL, 0, 0))) {
		m_pdsb->Restore();
		if( FAILED( m_pdsb->Lock(0, ko_chunk.cksize, &pmem, &size, NULL, 0, 0))) {
			SunLog.LogOut( "サウンドバッファのロックに失敗\n");
			return 0;
		}
	}
	
	//dataチャンクを読み取る
	//ここが画像ファイルで言うと一番欲しい画像の部分ですね。
	//丸ごとごっそりいただく事にします。
	if(mmioRead(hmmio, (HPSTR)pmem, (LONG)ko_chunk.cksize) != (LONG)ko_chunk.cksize){
		SunLog.LogOut("mmioReadに失敗\n");
	}
	//バッファをアンロック
	m_pdsb->Unlock(pmem, size, NULL, 0);
	
	//ウェーブファイルを閉じる
	mmioClose(hmmio, 0);
	
	return 1;
}

//パックファイルからロード
int SunSoundBuffer::LoadFromPack(char* pPack,char* pFile)
{
	unsigned long		DataSize		= 0;

	//=====================================================
	//	WAVEデータ検索
	//=====================================================

	FILE *fp = fopen( pPack, "rb" );
	if ( fp == NULL ) 
		throw SunExeption( "%s:パックファイルのオープンに失敗\n", pPack );

	// パックファイルの確認
	char guid[4] = "";
	guid[0] = (char)fgetc( fp );
	guid[1] = (char)fgetc( fp );
	guid[2] = (char)fgetc( fp );
	guid[3] = (char)fgetc( fp );
	if ( strcmp( guid, "SND" ) )
		throw SunExeption( "%s:パックファイルではありません\n", pPack );

	// それでは探しましょうかね
	while ( TRUE )
	{
		char Name[32];

		// データ取得
		fread( &Name, sizeof(char), 32, fp );

		// ダメなら終了
		if ( feof( fp ) ) break;

		// 正解？
		if ( strcmp( Name, pFile ) == 0 )
		{
			//ビバ
			fread( &DataSize, sizeof(long), 1, fp );
			break;
		}
		else
		{
			// 次
			long size;
			fread( &size, sizeof(long), 1, fp );
			fseek( fp, size+sizeof(WAVEFORMATEX), SEEK_CUR );
		}
	}

	if ( DataSize == 0 )
		throw SunExeption( "%s: データの取得に失敗\n", pFile );

	// WAVEフォーマット取得
	fread( &m_wf, sizeof(WAVEFORMATEX), 1, fp );

	//バッファ作成
	this->CreateBuffer( DataSize );

	//先ほど読み込んだwaveのデータ部分をサウンドバッファにコピーします。
	LPVOID pmem;
	DWORD size;
	
	//サウンドバッファをロック
	//ロックしたサイズとバッファの先頭アドレスが取得出来ます。
	m_pdsb->Lock(0, DataSize, &pmem, &size, NULL, 0, 0);
	
	// サウンドデータ読み込み
	fread( pmem, size, 1, fp );
	
	//バッファをアンロック
	//Drawの時と同じです。
	m_pdsb->Unlock(pmem, size, NULL, 0);


	return 1;
}
//ディレクトリもしくはその名前のパックファイルからロード
int SunSoundBuffer::LoadFromDirectory(char* pFile)
{
	this->Release();
//	if( this->Load( pFile ) == 0 )
	{
		char* ptr;
		char strPack[MAX_PATH];
		char strFile[MAX_PATH];
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
			strncpy(strPack,pFile,size);
			strcpy(&strPack[size],".bin");
		}
		//ファイル名を切り出す
		{
			ptr++;
			if(ptr == NULL || *ptr == '\0')	
				return 0;
			strcpy(strFile,ptr);
		}
		if( this->LoadFromPack(strPack,strFile) == 0)
			return 0;
	}
	return 1;
}
//バッファ作成
int SunSoundBuffer::CreateBuffer( DWORD dwSize )
{
	DSBUFFERDESC dsbdesk;

	ZeroMemory(&dsbdesk, sizeof(DSBUFFERDESC));
	dsbdesk.dwSize = sizeof(DSBUFFERDESC);

	//バッファのフラグを指定
	dsbdesk.dwFlags =
		DSBCAPS_CTRLPAN|
		DSBCAPS_CTRLVOLUME|
		DSBCAPS_CTRLFREQUENCY|
		DSBCAPS_CTRLPOSITIONNOTIFY|
		DSBCAPS_GETCURRENTPOSITION2|
		DSBCAPS_GLOBALFOCUS;

	//バッファのサイズを指定
	dsbdesk.dwBufferBytes = dwSize;

	//WAVEFORMATEXをセット
	dsbdesk.lpwfxFormat = &m_wf; 
	
	//オフスクリーンバッファの生成
	LPDIRECTSOUNDBUFFER pdsb = NULL;
	if(FAILED(m_pDS->CreateSoundBuffer(&dsbdesk, &pdsb, NULL))){		
		SunLog.LogOut("CreateSoundBufferに失敗しました\n");
		return 0;
	}
	if(FAILED(pdsb->QueryInterface(IID_IDirectSoundBuffer8,(void**)&m_pdsb))){
		SunLog.LogOut("IDirectSoundBuffer8へのQueryInterfaceに失敗しました\n");
		return 0;
	}
	SAFE_RELEASE( pdsb );
	return 1;
}

/**
	Ayame・OggVorbis管理クラス
*/
CSunAyame::CSunAyame()
{
	m_IsInitAyame = false;
	m_hAyame = NULL;
	m_pAyameInitialize = NULL;
	m_pAyameUninitialize = NULL;
	m_pAyameCreateFromFile	= NULL;
	m_pAyameCreateFromFileEx = NULL;
	m_pAyameCreateFromMemory = NULL;
	m_pAyameGetError = NULL;
	m_pAyame = NULL;
}
static void *AyameAlloc( unsigned int Size, const char *pFileName, unsigned int Line ) {
	return malloc( Size );
}

static void AyameFree( void *pData ) {
	free( pData );
}
//DLLの初期化
int CSunAyame::Init( const char *pDllPath )
{
	if ( m_hAyame != NULL ) return true;

	// DLLのロード
	m_hAyame = ::LoadLibrary( pDllPath );
	if ( m_hAyame == NULL ) return false;

	// 関数のロード
	m_pAyameInitialize			= (AYAME_INITIALIZE_PROC)::GetProcAddress( m_hAyame, "Ayame_Initialize" );
	m_pAyameUninitialize		= (AYAME_UNINITIALIZE_PROC)::GetProcAddress( m_hAyame, "Ayame_Uninitialize" );
	m_pAyameCreateFromFile		= (AYAME_CREATE_FROM_FILE_PROC)::GetProcAddress( m_hAyame, "Ayame_CreateInstanceFromFile" );
	m_pAyameCreateFromFileEx	= (AYAME_CREATE_FROM_FILE_EX_PROC)::GetProcAddress( m_hAyame, "Ayame_CreateInstanceFromFileEx" );
	m_pAyameCreateFromMemory	= (AYAME_CREATE_FROM_MEMORY_PROC)::GetProcAddress( m_hAyame, "Ayame_CreateInstanceFromMemory" );
	m_pAyameGetError			= (AYAME_GETERROR_PROC)::GetProcAddress( m_hAyame, "Ayame_GetLastError" );

	// ちゃんとロードできてり？
	if ( m_pAyameInitialize			== NULL ) return false;
	if ( m_pAyameUninitialize		== NULL ) return false;
	if ( m_pAyameCreateFromFile		== NULL ) return false;
	if ( m_pAyameCreateFromFileEx	== NULL ) return false;
	if ( m_pAyameCreateFromMemory	== NULL ) return false;
	if ( m_pAyameGetError			== NULL ) return false;

	// ドライバーの初期化
	m_IsInitAyame = m_pAyameInitialize( NULL, (void**)&SunSound.m_pDS, (AYAME_MEMORY_ALLOC_PROC)AyameAlloc, AyameFree );

	// OggかWavかの判定
	SunFile file;
	if( file.OpenFile("bgm.bin") != 0)
		bgm_mode = BGM_AYM;
	else if( file.OpenFile("ogg.bin") != 0)
		bgm_mode = BGM_OGG;
	else if( SetCurrentDirectory( "bgm" ) != 0 )
		bgm_mode = BGM_AYM;
	else if( SetCurrentDirectory( "ogg" ) != 0 )
		bgm_mode = BGM_OGG;
	ResetCurrent();
	file.CloseFile();

	return m_IsInitAyame;
}

int CSunAyame::Release()
{
	// ドライバーの解放
	if( m_hAyame != NULL ) {
		if( m_pAyame != NULL ) {
			m_pAyame->Stop();
			m_pAyame->Release();
			m_pAyame = NULL;
		}
		if ( m_pAyameUninitialize != NULL ) {
			m_pAyameUninitialize();
		}
		::FreeLibrary( m_hAyame );
		m_hAyame = NULL;
	}
	return 1;
}

//再生
int CSunAyame::Play( char *pFile )
{
	if( m_pAyameCreateFromFileEx == NULL )
		return 0;
	if( m_pAyameCreateFromFile == NULL )
		return 0;
	if( m_pAyame != NULL ) {
		m_pAyame->Stop();
		m_pAyame->Release();
		m_pAyame = NULL;
	}
	SunFile file;
	file.OpenFileFromDirectory( pFile );
	unsigned long Start = file.GetStart();
	unsigned long Size = file.GetSize();
	char strPack[MAX_PATH];
	char strFile[MAX_PATH];
	strcpy( strPack, file.GetPack() );
	strcpy( strFile, file.GetFile() );
	file.CloseFile();

	if( strPack[0] != '\0' )
		m_pAyame = m_pAyameCreateFromFileEx( strPack, Start, Size, false );
	else
		m_pAyame = m_pAyameCreateFromFile( strFile, false );
	if( m_pAyame == NULL )
		return 0;
	m_pAyame->SetVolume( SunSound.m_bgmvolume );
	return m_pAyame->Play(-1,0);
}
//一時停止
int CSunAyame::Pause()
{
	if( m_pAyame == NULL )
		return 0;
	m_pAyame->Pause();
	return 1;
}
//停止
CSunAyame::Stop()
{
	if( m_pAyame == NULL )
		return 0;
	m_pAyame->Stop();
	return 1;
}
//再開
int CSunAyame::Resume()
{
	if( m_pAyame == NULL )
		return 0;
	m_pAyame->Play(-1,0);
	return 1;
}
