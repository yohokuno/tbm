/**
	@file Sun3D.h
	@brief ３Ｄ関連
*/
#pragma once

//ブレンディングモード
enum eBlendingType
{
	BLEND_NONE,			///< アルファブレンド無効（透過もなし）
	BLEND_NORMAL,		///< 通常
	BLEND_ADD,			///< 加算
	BLEND_MUL,			///< 乗算
	BLEND_REVERSE,		///< 反転
	BLEND_BRILLIANT,	///< 覆い焼き
	BLEND_DARKSIDE,		///< 焼き込み
	BLEND_MAX,			///< 比較（明）
	BLEND_MIN,			///< 比較（暗）

	BLEND_TEST,			///< 開発用
	BLEND_TEST2,		///< 開発用
	BLEND_TRANS,		///< 透過
};
///Direct3Dオブジェクト生成関数用関数ポインタ型
typedef IDirect3D9 * (WINAPI *PDIRECT3DCREATE9)( unsigned int );
/**
	@class CSun3D
	@brief	３Ｄ統括クラス
	
	Ｄｉｒｅｃｔ３Ｄ関連のクラス。
	Ｄｉｒｅｃｔ３ＤをＤＬＬから動的に読み込む。
	３Ｄデバイスの「選択」「作成」「能力のログ出力」などデバイス関連も扱う。
	内部でコンフィグを読み込む。

    @todo	------------------------
	現在は２Ｄ用の機能のみ実装してあります。
	３Ｄ用のライトなどの設定機能はあとで実装する必要があります。
	テクスチャの多重設定をはじく機能とか。
    @bug	------------------------
*/
class CSun3D
{
	friend class SunSurface;
	friend class SunTexture;
	friend class SunSpriteBase;
	friend class SunImage;
public:
//データメンバ
	HINSTANCE			m_hDll;		///<DLLのハンドル
	LPDIRECT3D9			m_pd3d;		///<D3D
	LPDIRECT3DDEVICE9	m_pd3ddev;	///<DEVICE
	int	m_bitmode;	///<画面のビットモード
	int	m_texbit;	///<テクスチャのビットモード
	int	m_vsync;	///<垂直同期
	int	m_square;	///<正方形テクスチャ
	int	m_skip;		///<描画スキップモード
	int	m_interval;	///<描画スキップ数
	int	m_minimum;	///<描画スキップFPS
	int	m_scount;	///<描画スキップカウント
	int	m_count;	///<カウント数
	int m_time;		///<前フレームのtimeGetTimeの値
	double	m_obfps;	///<ＦＰＳ（毎フレーム観測値）

	D3DPRESENT_PARAMETERS	m_d3dpp;	///<初期化のときだけ必要？
	D3DCAPS9	m_d3dcaps;	///<デバイス能力
	D3DFORMAT	m_format;	///<使用するテクスチャフォーマット
	DWORD	m_renderState[0xFF];	///< レンダリングステート
	DWORD	m_textureState[0xFF];	///< テクスチャステート
	DWORD	m_fvfState;				///<FVFのフラグ
	D3DXIMAGE_FILEFORMAT	m_fileformat;	///<スナップショットの形式
public:
//基本機能
	CSun3D();						///<メンバの初期化
	virtual ~CSun3D();				///<Ｅｘｉｔ（）を呼びます。
	int Init();						///<初期化
	int Exit();						///<終了
	int ReInit();					///<再初期化
	int LoadConfig();				///<コンフィグロード
	int Clear(D3DCOLOR color);		///<バックバッファをクリア
	int GetUpdate();				///<画面を更新するかどうか
	int Present();					///<バックバッファをプレゼンテーション
	int BeginScene();				///<シーン開始
	int EndScene();					///<シーン終了
	int	GetTextureSize();			///<最大テクスチャサイズ取得
//内部使用
	int CreateDirect3D();			///<Ｄｉｒｅｃｔ３Ｄ作成
	int CreateDevice();				///<デバイス作成
	int DecideDisplay();			///<m_d3dpp初期化関係
	int DecideRefreshRate();		///<m_d3dpp初期化関係
	int DecideTextureFormat();		///<m_d3dpp初期化関係
	int DecideOtherParameters();	///<m_d3dpp初期化関係
	int InitState();				///<各種ステートの初期化
//ログ関連	
	int CheckAdapterInformation();	///<ディスプレイログ出力
	int InitCaps();					///<ＣＡＰＳ初期化
	int	DeviceLog();				///<ＣＡＰＳログ出力
	int ParameterLog();				///<決定されたパラメータの出力
	int GetBackBufferFormatRank(D3DFORMAT format);	///<バックバッファフォーマットの評価
	int GetTextureFormatRank(D3DFORMAT format);		///<テクスチャフォーマットの評価
	string GetFormatString(D3DFORMAT Format);		///<ピクセルフォーマット文字列取得
	string GetErrorString(HRESULT hr);				///<エラー文字列取得
//テクスチャ関連
	int SetTexture(LPDIRECT3DTEXTURE9 pd3dtex);		///<テクスチャ設定
	int SetColorkeyEnable(bool flag);				///<カラーキー設定
	int SetRenderState( D3DRENDERSTATETYPE State, DWORD Param );		///<レンダーステート
	int SetTextureState(D3DTEXTURESTAGESTATETYPE State, DWORD Param );	///<テクスチャステート
	int SetBlendingType( eBlendingType BlendingType );					//ブレンディングモード
	int SetFVF( DWORD fvfState);						///<FVFフラグセット
	int GetVsync(){return m_vsync;}
	int SetViewport(D3DVIEWPORT9& viewport);
//3D関連
	int MoveView(float x,float y,float z);
	int SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX *pMatrix);
	int SetLight(DWORD Index,CONST D3DLIGHT9 *pLight);
};

extern CSun3D Sun3D;