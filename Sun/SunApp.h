
#pragma once

/**
	@file Sun.h
	CSunAppクラスの宣言。
	実際には唯一のグローバル変数Sunを用います。
*/

//メインループ・再起動ループフラグ
enum eLoopFlag
{
	LOOP_MAINONLY,				///<メインループ
	LOOP_FINISH,				///<終了
	LOOP_RESTART,				///<再起動
};
/**
	@class CSunApp
	@brief	アプリケーション管理クラス

	アプリケーションの管理を担当する。
	具体的には「ウィンドウ管理」「ＦＰＳ管理」「ログファイル管理」
	
	コンフィグ変更による再起動のイメージとしてはこんな感じ
	if(!Config())
		Sun.ReStart();
*/
class CSunApp
{
public:
//データメンバ（かったるいのでpublic）
//このうち更新必須メンバは
//m_appname,m_appver,m_hInstの三つとする。（あとで増える可能性あり）
//また、コンフィグによる更新メンバはいまのところ
//m_bWindow,m_fpsである。
	//更新必須メンバ
	string		m_appname;	///<アプリケーション名
	double		m_appver;	///<バージョン情報
	HINSTANCE	m_hInst;	///<インスタンスハンドル
	//アプリケーション関係
	HANDLE	m_hMutex;	///<ミューテックスハンドル
	HACCEL	m_hAccel;	///<キーボードアクセラレータ
	int		m_width;	///<幅
	int		m_height;	///<高さ
	int		m_bAllowDouble;	///<２重起動を許すか？
	int		m_vsync;		
	eLoopFlag	m_finish;	///<終了フラグ（０：メインループ、１：ループ終了、２：再起動）
	//ウィンドウ関係
	int		m_window;		///<ウィンドウモードフラグ(0:window,1:fullscreen)
	HWND	m_hWnd;			///<ウィンドウハンドル
	bool	m_bActive;		///<アクティブフラグ
	RECT	m_rcClient;		///<クライアント領域
	RECT	m_rcWindow;		///<ウィンドウ領域
	bool	m_bSmoothing;	///<アンチエイリアスフラグ
	STICKYKEYS	m_stickykeys;	///<固定キー情報構造体
	//ＦＰＳ／タイマー関係
	int		m_fps;		///<ＦＰＳ（設定値）
	double	m_obfps;	///<ＦＰＳ（毎秒観測値）
	int		m_oldtime;	///<（同期用）１フレ前のtimeGetTimeの値
	int		m_time;		///<（観測用）１秒前のtimeGetTimeの値
	int		m_count;	///<（観測用）１秒おき更新のためのフレームカウント
	int		m_fpsshow;	///<（表示用）更新フラグ
	int		m_timer;	///<タイマーの種類（0：timeGetTime、1:QueryPerformanceCounter)
	UINT	m_wTimerRes;	///<タイマーの分解能
	LARGE_INTEGER m_pfreq;	///<QueryPerformanceFrequency
	LARGE_INTEGER m_ptime;	///<QueryPerformanceCounter

public:
//基本機能
	CSunApp();					///<コンストラクタ。メンバの初期化。
	virtual ~CSunApp();		///<デストラクタ。内部でExit()を呼ぶ。グローバルだが問題なし。
	int Init();				///<いろいろ初期化（内部でInitLogなど)。外部からデータメンバを必要なだけ更新してから呼ぶ
	int InitWindow();		///<ウィンドウ初期化。Init()のあとコンフィグを初期化してから呼ぶ
	int Exit();				///<エラー表示とMutex解放
	int ReInit();			///<再初期化。このとき更新必須データメンバは前回のまま残る
	int WaitTime();			///<Message() + SyncFPS() + ObserveFPS()
	int Message();			///<メッセージ処理（内部使用）
	int Finish();			///<アプリケーション終了
	int ReStart();			///<アプリケーション再起動
	int GetRestart();		///<再起動フラグが立っているか？
	//ＦＰＳ／タイマー関係
	int InitTimer();
	int SyncFPS();		///<ＦＰＳ同期(timeGetTime使用）
	int SyncFPS2();		///<ＦＰＳ同期(QueryPerformanceCounter使用)
	int ObserveFPS();	///<ＦＰＳ観測
	int ShowFPS();		///<ＦＰＳ表示（タイトルバー使用）
	string GetFPSString();
	//ログ・デバッガ出力など表示関連
	int MsgBox(const char *fmt,...);	///<printf風メッセージボックス
	int SetCaption(const char *fmt,...);///<printf風ウィンドウキャプション
	int Output(const char* fmt,...);	///<printf風デバッガ出力
	int MoveMouse(int x,int y);			///<マウスカーソル移動
	//ウィンドウプロシージャ
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );	///<ウィンドウプロシージャ
//セット・ゲット系
	void SetAppname(char *appname)	{ m_appname	= appname;}
	void SetVersion(double appver)	{ m_appver	= appver;}
	void SetInst(HINSTANCE hInst)	{ m_hInst	= hInst;}
	double	GetVersion(){return m_appver;}
	HWND GetWindow(){return m_hWnd;}
	HINSTANCE GetInst(){return m_hInst;}
	void SetVsync(int vsync){ m_vsync = vsync; }
//内部使用関数
	int InitMember();			///<メンバ変数初期化
	int DumpMessage();			///<メッセージを全て捨てる
	int CheckMutex();			///<２重起動防止用
	int GetAppInformation();	///<アプリ情報（Ｓｕｎメンバ）出力
	int LoadConfig();			///<コンフィグ読み込み
};

extern CSunApp SunApp;