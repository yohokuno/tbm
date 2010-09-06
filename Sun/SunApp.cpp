/**
	@file CSunApp
	@brief CSunApp クラスのインプリメンテーション
*/
#include "stdafx.h"
#include "resource.h"
#include "SunLog.h"
#include "SunApp.h"
#include "SunConfig.h"
#include "SunUtility.h"
#include "SunExeption.h"
#include "SunSound.h"
//非アクティブ中はBGM停止のため、SunSound依存。

CSunApp SunApp;
///////////////////////////////////////////////
// 構築/消滅
///////////////////////////////////////////////

CSunApp::CSunApp()
{
	InitMember();
}
/**
	@brief メンバ変数初期化
*/
int CSunApp::InitMember()
{
	//更新必須メンバ
	this->m_appname	= "Sun";
	this->m_appver	= 1.0;
	this->m_hInst		= NULL;
	//アプリケーション関係
	this->m_hMutex	= NULL;
	this->m_bActive	= true;
	this->m_width		= 640;
	this->m_height	= 480;
	this->m_bAllowDouble	= 0;
	this->m_vsync	= 1;
	this->m_finish	= LOOP_MAINONLY;
	//ウィンドウ関係
	this->m_hWnd		= NULL;
	this->m_hAccel	= NULL;
	this->m_window	= true;
	::SetRect(&this->m_rcClient,0,0,0,0);
	::SetRect(&this->m_rcWindow,0,0,0,0);
	this->m_bSmoothing = false;
	::ZeroMemory( &m_stickykeys, sizeof( m_stickykeys ));
	//ＦＰＳ／タイマー関係
	this->m_fps		= 60;
	this->m_obfps		= 60;
	this->m_oldtime	= 0;
	this->m_time		= 0;
	this->m_count		= 0;
	this->m_fpsshow	= 1;
	this->m_pfreq.QuadPart = 0;
	this->m_ptime.QuadPart = 0;
	this->m_timer = 0;
	return 1;
}
/**
	@brief デストラクタ
*/
CSunApp::~CSunApp()
{
	this->Exit();
}
/*
	@brief アプリケーションを初期化
*/
int CSunApp::Init()
{
	//メモリリークチェック
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COMの初期化
	::CoInitialize( NULL );

	//再起動時に残っているメッセージを捨てる
	this->DumpMessage();

	//アプリ情報出力
	this->GetAppInformation();

	//コンフィグ読み込み
	this->LoadConfig();

/*	//２重起動防止
	if(!this->CheckMutex())
	{
		return 0;
	}
*/
	//ウィンドウ初期化
	if(!this->InitWindow())
	{
		return 0;
	}
	//ＦＰＳ・タイマー関係
	if(!this->InitTimer())
	{
		return 0;
	}
/*	//アンチエイリアス無効
	::SystemParametersInfo( SPI_GETFONTSMOOTHING, 0, &m_bSmoothing,0);
	if(m_bSmoothing)
		::SystemParametersInfo( SPI_SETFONTSMOOTHING, FALSE, NULL, 0 );
*/
	//固定キー無効
	::SystemParametersInfo( SPI_GETSTICKYKEYS, sizeof( STICKYKEYS ), &m_stickykeys, 0 );
	m_stickykeys.cbSize = sizeof( STICKYKEYS );
	STICKYKEYS stickykeys = m_stickykeys;
	//stickykeys.dwFlags &= ~SKF_STICKYKEYSON;
	//stickykeys.dwFlags &= ~SKF_AVAILABLE;
	//stickykeys.dwFlags &= ~SKF_HOTKEYACTIVE;
	stickykeys.dwFlags &= ~SKF_CONFIRMHOTKEY;
	//stickykeys.dwFlags &= ~SKF_INDICATOR;
	::SystemParametersInfo( SPI_SETSTICKYKEYS, sizeof( STICKYKEYS ), &stickykeys, 0 );
	return 1;
}
/**
	@brief アプリケーションを終了
*/
int CSunApp::Exit()
{
	try
	{
		//タイマーの解除
		::timeEndPeriod(m_wTimerRes);
		//ミューテックスの解放
		if ( this->m_hMutex != NULL )
		{
			::CloseHandle( this->m_hMutex );
			this->m_hMutex	= NULL;
		}
		//ウィンドウ破棄
		::DestroyWindow(this->m_hWnd);
		::UnregisterClass(this->m_appname.data(),this->m_hInst);
		//ＣＯＭを解放
		::CoUninitialize();
/*		//アンチエイリアス設定を復元
		if(m_bSmoothing)
			SystemParametersInfo( SPI_SETFONTSMOOTHING, TRUE, NULL, 0 );
*/
		//固定キー設定を復元
		::SystemParametersInfo( SPI_SETSTICKYKEYS, sizeof( STICKYKEYS ), &m_stickykeys, 0 );

	}catch(...){MsgBox("アプリケーションの解放で不明な例外が発生しました。");}
	return 1;
}
/**
	@brief 二重起動確認
*/
int CSunApp::CheckMutex()
{
	if(this->m_bAllowDouble == 0)
	{
		if(this->m_hMutex != NULL)	//呼ばれるの２回目
			return 1;
		this->m_hMutex = ::CreateMutex( NULL, FALSE, this->m_appname.data() );
		if ( ::WaitForSingleObject( this->m_hMutex, 0 ) != WAIT_OBJECT_0 )
		{
			throw SunExeption("２重起動しないでください。\n");
		}
	}
	return 1;
}
/**
	@brief メッセージ処理
*/
int CSunApp::Message()
{
	if(this->m_finish == LOOP_FINISH || this->m_finish == LOOP_RESTART)
		return 0;

	do
	{
		MSG	   msg;
		while(::PeekMessage(&msg,0,0,0,PM_REMOVE))	//メッセージループ(メッセージがなくなるまでループ）
		{
			if(msg.message==WM_QUIT)
			{
				this->m_finish	= LOOP_FINISH;	//終了フラグを立てる
				return 0;			//終了のときは０を返す
			}
			if( 0 == TranslateAccelerator( this->m_hWnd, this->m_hAccel, &msg ) )
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		//アクティブかどうかを取得
		this->m_bActive = (GetActiveWindow()!=NULL);

		//CPU消費対策
		Sleep(1);
	}
	while( !this->m_bActive);
	return 1;
}
/**
	@brief WaitTime関数

	Message() + SyncFPS() + ObserveFPS()
*/
int CSunApp::WaitTime()
{
	//自動スキップ機能のため、垂直同期の有無にかかわらずウェイトを入れます
//	if( m_vsync )
//		return this->Message();
//	else
		if( m_timer )
			return this->Message() && this->SyncFPS2();
		else
			return this->Message() && this->SyncFPS();
}
/**
	@brief タイマー初期化
*/
int CSunApp::InitTimer()
{
	//TGTタイマー関連
	this->m_oldtime	= (int)::timeGetTime();
//	this->m_time		= (int)::timeGetTime();
	TIMECAPS tc;
 	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
		return 0;
	m_wTimerRes = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
	::timeBeginPeriod(m_wTimerRes); 

	//QPCタイマー関連
	if( m_timer == 1 )
	{
		::QueryPerformanceFrequency( &m_pfreq );
		if( m_pfreq.QuadPart == 0 )
			m_timer = 0;
		::QueryPerformanceCounter( &m_ptime );
	}
	return 1;
}
/**
	@brief ＦＰＳ同期
*/
int CSunApp::SyncFPS()
{
	if(this->m_finish == LOOP_FINISH || this->m_finish == LOOP_RESTART)
		return 0;
	if(this->m_fps <= 0)
		return 1;

	int time = ::timeGetTime();
	while(true)
    {
		//FPS測定（空ループすべきか？）
		if( time < this->m_oldtime)
		{//経過時間変数がオーバーフロー
			this->m_oldtime = time;	//現在の起動からの経過時間を取得
		}

		//前回のループ処理からの経過時間を取得
		double t=fabs(time-this->m_oldtime);	//前回ループからの経過時間

		int _fps = 1000/this->m_fps;
		//経過時間によって実行する処理分岐
		if( t >= _fps )
		{//前回ループから経過時間が取り決め値より経過している
			do
			{//取り決め時間になるまで空ループ
				t -= _fps;				//経過時間を進める
				this->m_oldtime+=_fps;	//経過時間を進める
			}
			while( t >= _fps );			//■経過時間が取り決め時間を過ぎるまでループ
			break;	//タイムループ脱出
		}
		//メッセージを１回処理
		if(!this->Message())
			return 0;
	}
	return 1;
}
/**
	@brief ＦＰＳ同期2
*/
int CSunApp::SyncFPS2()
{
	if(this->m_finish == LOOP_FINISH || this->m_finish == LOOP_RESTART)
		return 0;
	if(this->m_fps <= 0)
		return 1;
	while(true)
    {
		LARGE_INTEGER time = {0,0};

		QueryPerformanceCounter( &time );
//		QueryPerformanceFrequency( &m_pfreq );

		//FPS測定（空ループすべきか？）
		if( time.QuadPart < this->m_ptime.QuadPart)
		{//経過時間変数がオーバーフロー
			this->m_ptime.QuadPart = time.QuadPart;	//現在の起動からの経過時間を取得
		}

		//前回のループ処理からの経過時間を取得
		LONGLONG t=abs(time.QuadPart-this->m_ptime.QuadPart);	//前回ループからの経過カウント

		LONGLONG _fps = m_pfreq.QuadPart/this->m_fps; //理想の1フレームのカウント数

		//経過時間によって実行する処理分岐
		if( t >= _fps )
		{//前回ループから経過時間が取り決め値より経過している
			do
			{//取り決め時間になるまで空ループ
				t -= (double)_fps;				//経過時間を進める
				this->m_ptime.QuadPart += _fps;	//経過時間を進める
			}
			while( t >= _fps );			//経過時間が取り決め時間を過ぎるまでループ
			break;	//タイムループ脱出
		}
		//メッセージを１回処理
		if(!this->Message())
			return 0;
	}
	return 1;
}
/**
	@brief ＦＰＳ観測
*/
int CSunApp::ObserveFPS()
{
	int time = (int)::timeGetTime();
	this->m_count++;

	//１フレーム目
	if( this->m_time == 0 ) {
		m_time = time;
	}
	//１秒おきに更新（１秒以上おきに更新）
	if(time - this->m_time >= 1000)
	{
		this->m_obfps		= 1000.0 * this->m_count / (time-this->m_time);	//ＦＰＳはdouble型です。
		this->m_count		= 0;
		this->m_time		= (int)::timeGetTime();
		this->m_fpsshow	= 1;	//ＦＰＳ表示更新フラグ
	}

	return 1;
}
/**
	@brief ＦＰＳ表示（タイトルバー使用）
*/
int CSunApp::ShowFPS()
{
	if(this->m_fpsshow > 0)
	{
		this->SetCaption("%s fps=%d",this->m_appname.data(),(int)this->m_obfps);
		this->m_fpsshow --;
	}
	return 1;
}
/**
	@brief ＦＰＳ文字列取得
*/
string CSunApp::GetFPSString()
{
	char str[32];
	sprintf(str,"%dFPS",(int)this->m_obfps);
	return str;
}
/**
	@brief メッセージボックス

	printfの書式でメッセージボックス表示
*/
int CSunApp::MsgBox(const char *fmt,...)
{
	if(fmt == NULL)
		return 0;
	char str[512];	//決めうちｗ
	va_list arg;
	va_start(arg,fmt);
	vsprintf(str, fmt, arg);
	SunLog(str,fmt,arg);
	va_end(arg);

	::MessageBox(NULL,str,this->m_appname.data(),MB_ICONERROR);
	return 1;
}
/**
	@brief ウィンドウキャプション

	printfの書式でウィンドウキャプション表示
*/
int CSunApp::SetCaption(const char *fmt,...)
{
	//フルスクリーンだったらウィンドウキャプションは表示できない
	if(this->m_window == false)
		return 0;
	if(fmt == NULL)
		return 0;
	char str[512];	//決めうちｗ
	va_list arg;
	va_start(arg,fmt);
	vsprintf(str, fmt, arg);
	va_end(arg);

	::SetWindowText(this->m_hWnd,str);
	return 1;
}
/**
	@brief デバッガ出力

	Ouptputをprintf拡張
*/
int CSunApp::Output(const char* fmt,...)
{
	if(fmt == NULL)
		return 0;
	char str[512];	//決めうちｗ
	va_list arg;
	va_start(arg,fmt);
	vsprintf(str, fmt, arg);
	va_end(arg);

	OutputDebugString(str);
	return 1;
}
/**
	@brief 再初期化
*/
int CSunApp::ReInit()
{
	//任意にＥｘｉｔを呼ばなくても済むように（２度読んでもかまわない
	this->Exit();

	//まだＷＭ＿ＱＵＩＴが残ってる可能性があるのでメッセージを捨てる
	this->DumpMessage();

	//終了・エラー関係のデータメンバを初期化
	this->m_finish	= LOOP_MAINONLY;

	//初期化関数は同じ
	if(!this->Init())
		return 0;

	return 1;
}
/**
	@brief メッセージを捨てる

	再初期化用関数
*/
int CSunApp::DumpMessage()
{
    MSG	   msg;
	while(::PeekMessage(&msg,0,0,0,PM_REMOVE))	//メッセージループ(メッセージがなくなるまでループ）
	{
	    if( 0 == TranslateAccelerator( this->m_hWnd, this->m_hAccel, &msg ) )
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	return 1;
}
/**
	@brief アプリケーション情報をログアウト
*/
int CSunApp::GetAppInformation()
{
	//アプリ名とバージョンを出力
	SunLog.LogOut("%s Ver %g\n",this->m_appname.data(),(double)this->m_appver);

	// 起動時間
	SYSTEMTIME SystemTime;
	::GetLocalTime( &SystemTime );
	SunLog.LogOut("起動時間				[%u年%u月%u日 %u時%u分%u秒]\n",
		SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
		SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond );
	return 1;
}
/**
	@brief コンフィグ読み込み
*/
int CSunApp::LoadConfig()
{
	m_window	= SunConfig.GetInt2("system","window",0);
	m_fps		= SunConfig.GetInt2("system","fps",60);
	m_timer		= SunConfig.GetInt2("system","timer",1);
	return 1;
}
/**
	@brief 終了フラグを立てる
*/
int CSunApp::Finish()
{
	m_finish = LOOP_FINISH;
	return 1;
}
/**
	@brief 再起動フラグを立てる
*/
int CSunApp::ReStart()
{
	//終了フラグが立ってたらそっち優先
	if(m_finish == LOOP_FINISH)
		return 0;
	m_finish = LOOP_RESTART;
	return 1;
}
/**
	@brief 再起動フラグが立っているか？
*/
int CSunApp::GetRestart()
{
	if(m_finish == LOOP_RESTART)
	{
		m_finish = LOOP_MAINONLY;
		return 1;
	}
	return 0;
}
/**
	マウスカーソルの移動
*/
int CSunApp::MoveMouse(int x,int y)
{
	RECT rect;
	::GetWindowRect(m_hWnd,&rect);
	MoveMouse(rect.left + x, rect.right + y);
	return 1;
}
/**
	@brief 内部使用関数

	ウィンドウ初期化
*/
int CSunApp::InitWindow()
{
	//キーボードアクセラレーター
    this->m_hAccel = ::LoadAccelerators( this->m_hInst, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

	//ウィンドウクラスの登録
    WNDCLASSEX wc;

    wc.cbSize        = sizeof(wc);
    wc.lpszClassName = this->m_appname.data();
    wc.lpfnWndProc   = WndProc;
    wc.style         = CS_VREDRAW | CS_HREDRAW;
    wc.hInstance     = this->m_hInst;
    wc.hIcon         = LoadIcon( this->m_hInst, MAKEINTRESOURCE(IDI_MAIN_ICON) );
    wc.hIconSm       = LoadIcon( this->m_hInst, MAKEINTRESOURCE(IDI_MAIN_ICON) );
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground= (HBRUSH)GetStockObject( BLACK_BRUSH );
//    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName  = "";
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;

    if( ::RegisterClassEx( &wc ) == 0 )
	{
		throw SunExeption("ウィンドウクラスの登録に失敗");
	}

	DWORD x = 0;
	DWORD y = 0;
	DWORD w = this->m_width;
	DWORD h = this->m_height;
	DWORD flag = WS_POPUP;

	//ウィンドウサイズを計算
	if (this->m_window)
	{//ウインドウモードではウィンドウサイズがちょっと大きい
		flag |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

		w +=
			::GetSystemMetrics(SM_CXBORDER)		+
			::GetSystemMetrics(SM_CXEDGE)		+
			::GetSystemMetrics(SM_CXDLGFRAME);

		h +=
			::GetSystemMetrics(SM_CYBORDER)		+
			::GetSystemMetrics(SM_CYEDGE)		+
			::GetSystemMetrics(SM_CYDLGFRAME)	+
			::GetSystemMetrics(SM_CYCAPTION);

		x = ::GetSystemMetrics(SM_CXSCREEN)/2 - w/2;
		y = ::GetSystemMetrics(SM_CYSCREEN)/2 - h/2;
	}

	//ウィンドウを作成・表示
    this->m_hWnd = ::CreateWindowEx( 0,
							this->m_appname.data(), 
							this->m_appname.data(),
							flag,
							x, y, w, h,
							NULL,
							NULL,
							this->m_hInst,
							NULL );
    if( this->m_hWnd == NULL )
	{
		throw SunExeption("ウィンドウ作成に失敗\n");
	}
    ::ShowWindow( this->m_hWnd, 1 );
    ::UpdateWindow( this->m_hWnd );

	//ウィンドウ領域取得
    ::GetWindowRect( this->m_hWnd, &this->m_rcWindow );

	return 1;
}
/**
	@brief WndProc関数

	ウィンドウプロシージャ
	ウィンドウメッセージを処理
*/
LRESULT CALLBACK CSunApp::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	HINSTANCE hInst = SunApp.m_hInst;
    switch (msg)
    {
        case WM_COMMAND:
            switch( LOWORD(wParam) )
            {
				case IDM_SCREENMODE:
					//Ｆ１
					//フル←→窓切り替え切り替え用だったが
					//切り替えはコンフィグのみになったので使わない
					return 0L;

                case IDM_EXIT:
					//ＥＳＣ（終了）
            	    ::PostMessage( hWnd, WM_CLOSE, 0, 0 );
                    return 0L;
            }
            break;
		case WM_PAINT:
			//ここでSun3D.Presentをやってもバックバッファが更新されずに転送されるので
			//緑やピンクの画面が出るだけ
			break;
        case WM_GETMINMAXINFO:
            {
				//ウィンドウサイズ変更を許さない。
                MINMAXINFO* pMinMax = (MINMAXINFO*) lParam;

                DWORD dwFrameWidth    = ::GetSystemMetrics( SM_CXSIZEFRAME );
                DWORD dwFrameHeight   = ::GetSystemMetrics( SM_CYSIZEFRAME );
                DWORD dwCaptionHeight = ::GetSystemMetrics( SM_CYCAPTION );

                pMinMax->ptMinTrackSize.x = (DWORD)SunApp.m_width  + dwFrameWidth * 2;
                pMinMax->ptMinTrackSize.y = (DWORD)SunApp.m_height + dwFrameHeight * 2 + 
                                            dwCaptionHeight;

                pMinMax->ptMaxTrackSize.x = pMinMax->ptMinTrackSize.x;
                pMinMax->ptMaxTrackSize.y = pMinMax->ptMinTrackSize.y;
            }
            return 0L;

        case WM_MOVE:
            return 0L;
        case WM_SIZE:
            break;
        case WM_SETCURSOR:
			//フルスクリーンモードでは、カーソルを隠す
            if( !SunApp.m_window )
            {
                SetCursor( NULL );
                return TRUE;
            }
            break;
        case WM_EXITMENULOOP:
			//メニューを開いてる間、ゲームの進行を止める
            SunApp.m_time = (int)::timeGetTime();
            SunApp.m_oldtime = (int)::timeGetTime();
			SunApp.m_count = 0;
			SunApp.m_obfps = 0;
            break;
        case WM_EXITSIZEMOVE:
			//サイズを変更しようとしている間、ゲームの進行をとめる
            SunApp.m_time = (int)::timeGetTime();
            SunApp.m_oldtime = (int)::timeGetTime();
			SunApp.m_count = 0;
			SunApp.m_obfps = 0;
            break;
		case WM_ACTIVATE:
			//非アクティブ中はBGM停止
			//SunSoundに依存してるのはここだけ。
			switch( LOWORD( wParam )) {
			case WA_INACTIVE:	//非アクティブ
				PauseBGM();
				break;
			case WA_ACTIVE:		//マウスクリック以外によりアクティブ
			case WA_CLICKACTIVE://マウスクリックによりアクティブ
				ResumeBGM();
				break;
			}
			break;
        case WM_SYSCOMMAND:
            switch( wParam ) {
				//フルスクリーンではサイズ変更・最大化等が不可能
                case SC_MOVE:			//ウィンドウ移動
				case SC_SIZE:			//ウィンドウサイズ変更
				case SC_MAXIMIZE:		//ウィンドウ最大化
                    if( !SunApp.m_window ) {
                        return TRUE;
					}
					break;
				case SC_MONITORPOWER:	//省電力モード抑制
				case SC_SCREENSAVE:		//スクリーンセーバー抑制
					return 1;
            }
            break;
        case WM_DESTROY:
            ::PostQuitMessage( 0 );
            return 0L;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}