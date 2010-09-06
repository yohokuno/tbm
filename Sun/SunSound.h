// SunSound.h: SunSound クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Ayame.h"
#include "SunFile.h"

#define BGMVOLUME 100
#define SEVOLUME 90

class CSunSound;
class CSunAyame;

extern CSunSound SunSound;
extern CSunAyame	SunAyame;

void PlayBGM(char *pFile);
void ResumeBGM();
void StopBGM();
void PauseBGM();
/**
	サウンドオブジェクト
*/
class CSunSound
{
public:
	LPDIRECTSOUND	m_pDS;
	LPDIRECTSOUND8	m_pDS8;
	int	m_sevolume;
	int m_bgmvolume;
public:
	CSunSound();
	virtual ~CSunSound();
	void Init();
	void ReInit();
	void Release();
};

/**
	効果音クラス
*/
class SunSoundBuffer
{
public:
	SunSoundBuffer();
	SunSoundBuffer(char* pFile);
	virtual ~SunSoundBuffer();
	int Play(DWORD dwFlags);
	int Resume(DWORD dwFlags);
	int Stop();
	int Release();
	int SetVolume( LONG lVolume );
	int SetPan( LONG lPan );

	int Load(char* lpszFileName);
	int LoadFromPack(char* pPack,char* pFile);
	int LoadFromDirectory(char* pFile);
	int CreateBuffer( DWORD dwSize );
protected:
	LPDIRECTSOUNDBUFFER8	m_pdsb;
	LPDIRECTSOUND8			m_pDS;
	WAVEFORMATEX	m_wf;
};
/**
	Ayameクラス
	Ayameライブラリを利用したOggVorbis再生クラス
*/
class CSunAyame
{
public:
	bool							m_IsInitAyame;
	HINSTANCE						m_hAyame;								///< Ayame DLLハンドル
	AYAME_INITIALIZE_PROC			m_pAyameInitialize;						///< ドライバー初期化
	AYAME_UNINITIALIZE_PROC			m_pAyameUninitialize;					///< ドライバー解放
	AYAME_CREATE_FROM_FILE_PROC		m_pAyameCreateFromFile;				///< ドライバーからオブジェクト取得
	AYAME_CREATE_FROM_FILE_EX_PROC	m_pAyameCreateFromFileEx;				///< ドライバーからオブジェクト取得
	AYAME_CREATE_FROM_MEMORY_PROC	m_pAyameCreateFromMemory;				///< ドライバーからオブジェクト取得
	AYAME_GETERROR_PROC				m_pAyameGetError;						///< エラー文字列
	CAyame	*m_pAyame;
	CSunAyame();
	int Init( const char *pDllPath );
	int Release();
	int Play(char* pFile);
	int Pause();
	int Resume();
	int Stop();
};
