
/**
*/

#ifndef ___AYAME_H___
#define ___AYAME_H___


//====================================================================
// INCLUDE
//====================================================================
#include <windows.h>


//====================================================================
// DEFINE
//====================================================================
#define LOOP_INFINITY	(0xFFFFFFFF)


//====================================================================
// TYPEDEF
//====================================================================
// ----互換性のため残していますが、使用されていません。
typedef void *(*AYAME_MEMORY_ALLOC_PROC)( unsigned long Size, const char *pFileName, unsigned long Line );										///< メモリ確保用コールバック関数
typedef void (*AYAME_MEMORY_FREE_PROC)( void *pData );																							///< メモリ解放用コールバック関数
// ----

typedef bool (*AYAME_INITIALIZE_PROC)( HWND hWnd, void **ppDirectSound, AYAME_MEMORY_ALLOC_PROC pAllocProc, AYAME_MEMORY_FREE_PROC pFreeProc );	///< ドライバー初期化
typedef void (*AYAME_UNINITIALIZE_PROC)( void );																								///< ドライバー解放
typedef bool (*AYAME_GETERROR_PROC)( char *pErrStr );
typedef class CAyame *(*AYAME_CREATE_FROM_FILE_PROC)( const char *pFileName, bool IsGlobal );													///< ドライバーからオブジェクト取得
typedef class CAyame *(*AYAME_CREATE_FROM_FILE_EX_PROC)( const char *pFileName, unsigned long Start, unsigned long Size, bool IsGlobal );		///< ドライバーからオブジェクト取得
typedef class CAyame *(*AYAME_CREATE_FROM_MEMORY_PROC)( void *pData, unsigned long Size, bool IsGlobal );										///< ドライバーからオブジェクト取得


//====================================================================
/**
	@brief	OggVorbis再生用クラス

	DLL用に純粋仮想関数だけの中身のないクラスです
*/
//====================================================================
typedef class CAyame
{
public:
	virtual bool __stdcall IsPlay( void )					= 0;	///< 再生チェック
	virtual bool __stdcall Play( unsigned long nLoopCount = 0, unsigned long nLoopInSample = 0 )	= 0;	///< 再生
	virtual bool __stdcall Loop( unsigned long nLoopCount = LOOP_INFINITY, unsigned long nLoopInSample = 0 )	= 0;	///< ループ再生
	virtual bool __stdcall Stop( void )						= 0;	///< 停止
	virtual bool __stdcall Pause( void )					= 0;	///< 一時停止
	virtual bool __stdcall SetVolume( float fParam )		= 0;	///< ボリューム設定
	virtual bool __stdcall SetPan( float fParam )			= 0;	///< パン設定
	virtual bool __stdcall Release( void )					= 0;	///< 解放
	virtual bool __stdcall AddRef( void )					= 0;	///< 参照カウントインクリメント
	virtual void __stdcall WaitEvent( void )				= 0;	///< イベント終了待ち
}
COggVorbis, *PCOggVorbis;


#endif // ___AYAME_H___


