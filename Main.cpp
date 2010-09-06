/**
	@file ThkgmMain.cpp
	@brief メインファイル
*/
#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"
#include "Title.h"

inline int SunInit();
inline int SunExit();

CScene *g_pScene = NULL;

/**
	@brief メイン関数

	WinMainです。ここに直接ループを書いていきます。


*/
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow )
{
	//アプリケーション情報をリセット
	SunApp.SetInst(hInst);
	SunApp.SetAppname("とびまりさ");	//アプリ名
	SunApp.SetVersion(1.00); 			//バージョン
	SunInput.UseJoystick(1);
	SunInput.UseKeyboard(1);
	SunInput.UseMouse(0);
	SunRand2.Randomize();

	//例外が発生した場合、グローバル変数を解放して終了します。
	//例外はSunTaskのインターフェースです。
	try{
		//ライブラリ初期化
		if(!SunInit())
			return 0;

		//ユーザーオブジェクト初期化
		g_pTitle = new CTitle();
		g_pScene = g_pTitle;
		SunSoundBuffer sndCamera("snd/camera.wav");

		//メインループ
		while(1) {
	
			//FPS観測
			if(!SunApp.ObserveFPS())
				break;
			
			//シーン開始
			if(!Sun3D.BeginScene())
				break;

			//描画
			if(Sun3D.GetUpdate()) {
				if(!g_pScene->Draw())
					break;
			}
			//実行＆描画
			if(!g_pScene->Process())
				break;

			//スクリーンショット
			if(SunInput.GetInputEvent( INP_B5 )) {
				if( g_pTitle == NULL || g_pTitle->mode != 7 ) {
					sndCamera.Play(0);
					SaveSnapShot();
				}
			}

			//シーン終了
			if(!Sun3D.EndScene())
				break;

			//処理
			if(!g_pScene->Run())
				break;

			//ｎ倍速
			for( int i = 0; i < 10; i++ ) {
				if( SunInput.GetKeyState( DIK_F1+i ) & 0x80 )
					SunApp.m_fps = 60 * (i+1);
			}

			//描画間隔
			for( int i = 0; i < 10; i++ ) {
				if( SunInput.GetKeyState( DIK_1+i ) & 0x80 ) {
					Sun3D.m_skip = 2;
					Sun3D.m_interval = i+1;
				}
			}

			//プレゼンテーション
			if(!Sun3D.Present())
				break;

			//フレーム調整
			if(!SunApp.WaitTime())
				break;

			//入力デバイス更新
			if(!SunInput.UpdateInput())
				break;
		}

		sndCamera.Release();
		SAFE_DELETE(g_pGame);
		SAFE_DELETE(g_pTitle);
		SunExit();

		//再起動
		if( SunApp.GetRestart() ) {
			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			ZeroMemory(&si,sizeof(si));
			si.cb=sizeof(si);
			char buffer[MAX_PATH];
			::GetModuleFileName( NULL, buffer, MAX_PATH );
			CreateProcess(NULL,(LPTSTR)buffer,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi);
		}
	}
	catch(SunExeption exeption){
		exeption();
		return 1;
	}
	return 0;
}

//初期化補助関数
inline int SunInit()
{
	//カレントの設定
	ResetCurrent();

	//ログファイルの初期化
	SunLog.LogStart();

	//ログファイル出力
	SunLog.GetAllInformation();

	//コンフィグファイルのバージョン確認
	//カレントディレクトリに左右されることに注意
	SunConfig.Read();

	//SunApp初期化
	if(!SunApp.Init())
		return 0;

	//Sun3D初期化
	if(!Sun3D.Init())
		return 0;

	//SunInput初期化
	if(!SunInput.ReInit())
		return 0;

	//SunSound初期化
	SunSound.Init();

	return 1;
}
//終了関数
inline int SunExit()
{
	SunSound.Release();
	SunInput.Release();
	Sun3D.Exit();
	SunApp.Exit();
	return 1;
}