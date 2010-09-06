/**
	@file Sun3D.cpp
	@brief CSun3D クラスのインプリメンテーション
*/
#include "stdafx.h"
#include "SunUtility.h"
#include "SunLog.h"
#include "SunConfig.h"
#include "SunExeption.h"
#include "SunApp.h"
#include "Sun3D.h"

CSun3D Sun3D;
/**
	@brief バックバッファフォーマット
*/
static const D3DFORMAT BackBufferFormats[] = {
	//16bit
	D3DFMT_R5G6B5,			D3DFMT_A1R5G5B5,		D3DFMT_X1R5G5B5,
	D3DFMT_X4R4G4B4,		D3DFMT_A4R4G4B4,		D3DFMT_A8R3G3B2,
	//24bit
	D3DFMT_R8G8B8,
	//32bit
	D3DFMT_A8R8G8B8,		D3DFMT_X8R8G8B8,		D3DFMT_A2R10G10B10,
	D3DFMT_A8B8G8R8,		D3DFMT_X8B8G8R8,		D3DFMT_A2B10G10R10,
	//64bit
	D3DFMT_A16B16G16R16,	D3DFMT_A16B16G16R16F,
	//UNKNOWN
	D3DFMT_UNKNOWN,
};

/**
	 @briefテクスチャフォーマット
*/
static D3DFORMAT TextureFormats[] = {
	//16bit
	D3DFMT_R5G6B5,			D3DFMT_A1R5G5B5,		D3DFMT_X1R5G5B5,
	D3DFMT_X4R4G4B4,		D3DFMT_A4R4G4B4,		D3DFMT_A8R3G3B2,
	//24bit
	D3DFMT_R8G8B8,
	//32bit
	D3DFMT_A8R8G8B8,		D3DFMT_X8R8G8B8,		D3DFMT_A2R10G10B10,
	D3DFMT_A8B8G8R8,		D3DFMT_X8B8G8R8,		D3DFMT_A2B10G10R10,
	//64bit
	D3DFMT_A16B16G16R16,	D3DFMT_A16B16G16R16F,
	//UNKNOWN
	D3DFMT_UNKNOWN
};
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CSun3D::CSun3D()
{
	this->m_hDll	= NULL;
	this->m_pd3d	= NULL;
	this->m_pd3ddev	= NULL;
	this->m_bitmode	= 32;
	this->m_vsync	= 1;
	this->m_texbit	= 0;
	this->m_square	= 0;
	this->m_skip	= 2;
	this->m_interval= 3;
	this->m_minimum	= 30;
	this->m_scount	= 0;
	this->m_count	= 0;
	this->m_format	= D3DFMT_UNKNOWN;
	this->m_obfps = 60;
	::ZeroMemory(&this->m_d3dpp,sizeof D3DPRESENT_PARAMETERS);
	::ZeroMemory(&this->m_d3dcaps,sizeof D3DCAPS9);
	memset( this->m_renderState, 0xFF, sizeof(DWORD[0xFF]) );
	memset( this->m_textureState, 0xFF, sizeof(DWORD[0xFF]) );
	m_fvfState = ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE| D3DFVF_TEX1 );	//２Ｄ用頂点フォーマット
	m_fileformat = D3DXIFF_JPG;
}

CSun3D::~CSun3D()
{
	this->Exit();
}
/**
	@brief ３Ｄ全初期化
	
	 これを行う前にＳｕｎが初期化されてる必要がある
*/
int CSun3D::Init()
{
	//コンフィグロード
	if(!this->LoadConfig())
		return 0;
	//Ｄｉｒｅｃｔ３Ｄ初期化
	if(!this->CreateDirect3D())
		return 0;
	//ディスプレイログ出力
	if(!this->CheckAdapterInformation())
		return 0;
	//Caps初期化
	if(!this->InitCaps())
		return 0;
	//Capsログ出力
	if(!this->DeviceLog())
		return 0;
	//フォーマトの決定
	if(!Sun3D.DecideDisplay())
		return 0;
	//リフレッシュレートの決定
	if(!Sun3D.DecideRefreshRate())
		return 0;
	//テクスチャフォーマットの決定
	if(!Sun3D.DecideTextureFormat())
		return 0;
	//その他のパラメータの決定
	if(!Sun3D.DecideOtherParameters())
		return 0;
	//決定されたパラメータのログ出力
	if(!Sun3D.ParameterLog())
		return 0;
	//デバイス作成
	if(!this->CreateDevice())
		return 0;
	//各種ステート初期化
	if(!this->InitState())
		return 0;
	//SunAppに送る
	SunApp.SetVsync( this->m_vsync );
	this->m_time = ::timeGetTime();
	return 1;
}
/**
	@brief 解放処理
*/
int CSun3D::Exit()
{
	try
	{
		SAFE_RELEASE(m_pd3ddev);
		SAFE_RELEASE(m_pd3d);
		::FreeLibrary(m_hDll);
		m_hDll = NULL;
	}catch(...){MsgBox("Direct3Dの解放で不明な例外しました。");}
	return 1;
}
/**
	@brief 再初期化
*/
int CSun3D::ReInit()
{
	if(!this->Exit())
		return 0;
	if(!this->Init())
		return 0;
	return 1;
}
/**
	@brief 各種ステート初期化
*/
int CSun3D::InitState()
{
	D3DXMATRIX	proj;
	D3DXMATRIX	view;
	D3DXMATRIX	world;

	// プロジェクション行列
	D3DXMatrixPerspectiveFovLH( &proj,
		D3DX_PI / 4,	//y方向への視野
		1.0f,			//アスペクト比
		10.0f,			//近ビュー平面
		5000.0f );		//遠ビュー平面
	
	// ビュー行列を設定
	D3DXVECTOR3 vEye( 0, 0, -50 );	//視点
	D3DXVECTOR3 vAt( 0, 0, 0 );		//注視点
	D3DXVECTOR3 vUp( 0, 1, 0 );		//上方向
	D3DXMatrixLookAtLH( &view, &vEye, &vAt, &vUp );

	// ワールドマトリックス作成
	D3DXMatrixRotationYawPitchRoll( &world, 0, 0, 0 );

	// プロジェクション行列設定
	m_pd3ddev->SetTransform( D3DTS_PROJECTION, &proj );

	// ビュー行列を設定
	m_pd3ddev->SetTransform( D3DTS_VIEW, &view );

	// ワールドマトリックスを設定
	m_pd3ddev->SetTransform( D3DTS_WORLD, &world );

	// 色の算出方法の設定
	this->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );		// 転送元の色の算出方法
	this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );	// 転送先の色の算出方法

	//　レンダリングステート
	this->SetRenderState( D3DRS_LIGHTING, false );			//ライティング
	this->SetRenderState( D3DRS_ZENABLE, false );
	this->SetRenderState( D3DRS_ZWRITEENABLE, false );
	this->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	this->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	this->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );	//裏も描画
	this->SetRenderState( D3DRS_CLIPPING , true );			//クリッピング
	this->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );

	//アンチェリ
//	if( m_d3dcaps.LineCaps & D3DLINECAPS_ANTIALIAS )
//		this->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );

	m_pd3ddev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	m_pd3ddev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	m_pd3ddev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);


	// テクスチャのブレンディング方法の設定
	this->SetTextureState(  D3DTSS_ALPHAOP, D3DTOP_MODULATE );	// 以下の引数の成分を乗算する
	this->SetTextureState(  D3DTSS_COLORARG1, D3DTA_TEXTURE );	// テクスチャの色
	this->SetTextureState(  D3DTSS_COLORARG2, D3DTA_DIFFUSE );	// ディフーズの色

	//頂点フォーマットをDirectXへ通知
	m_pd3ddev->SetFVF(( D3DFVF_XYZRHW | D3DFVF_DIFFUSE| D3DFVF_TEX1 ));

	//デバイス能力によって正方形テクスチャを強制使用
	if( m_d3dcaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY )
		m_square = 1;
	return 1;
}
/**
	@brief コンフィグロード
*/
int CSun3D::LoadConfig()
{
	//画面のビットモード
	switch(SunConfig.GetInt2("direct3d","bitmode",32))
	{
	case 16:
		m_bitmode = 16;
		break;
	case 24:
		m_bitmode = 24;
		break;
	case 32:
		m_bitmode = 32;
		break;
	case 64:
		m_bitmode = 64;
		break;
	default:
		SunApp.MsgBox("ビットモードの設定が狂っています。\n一度コンフィグファイルを削除してください。\n");
		break;
	}
	//テクスチャのビットモード
	switch(SunConfig.GetInt2("direct3d","texbit",0))
	{
	case 0:	//画面のビットモードと同じ
		m_texbit = 0;
		break;
	case 16:
		m_texbit = 16;
		break;
	case 24:
		m_texbit = 24;
		break;
	case 32:
		m_texbit = 32;
		break;
	case 64:
		m_texbit = 64;
		break;
	default:
		SunApp.MsgBox("ビットモードの設定が狂っています。\n一度コンフィグファイルを削除してください。\n");
		break;
	}

	//垂直同期待ちの設定
	if( m_vsync != -1 ) {
		m_vsync	= SunConfig.GetInt2("direct3d","vsync",1);
	}
	else {
		m_vsync = 0;
	}

	//正方形テクスチャの強制使用
	m_square = SunConfig.GetInt2("direct3d","square",0);

	//スナップショットのファイル形式
	string s;
	s = SunConfig.GetString2("direct3d","snapshot","jpg");
	if( s == "bmp" ) {
		m_fileformat = D3DXIFF_BMP;
	}
	else if( s == "jpg" ) {
		m_fileformat = D3DXIFF_JPG;
	}
	else if( s == "png" ) {
		m_fileformat = D3DXIFF_PNG;
	}
	else if( s == "tga" ) {
		m_fileformat = D3DXIFF_TGA;
	}
	else {
		m_fileformat = (D3DXIMAGE_FILEFORMAT)SunConfig.GetInt("direct3d","snapshot");
	}

	//描画スキップ
	m_skip = SunConfig.GetInt2("config.ini","direct3d","skip",2);
	m_interval = SunConfig.GetInt2("config.ini","direct3d","interval",1);
	m_minimum = SunConfig.GetInt2("config.ini","direct3d","minimum",30);
	m_scount = 0;
	return 1;
}
/**
	@brief GetUpdate
*/
int CSun3D::GetUpdate()
{
	return m_skip == 0 ||
		m_skip == 1 && ( m_obfps >= m_minimum || m_scount >= m_interval-1 ) ||
		m_skip == 2 && m_count % m_interval == 0;
}
/**
	@brief Present
*/
int CSun3D::Present()
{
	int time = ::timeGetTime();
	//毎フレーム更新
	this->m_obfps = 1000.0 / (time-this->m_time);
	this->m_time = time;
	this->m_count++;

	//描画スキップで飛ばされる部分
	if( this->GetUpdate() ) {
		this->m_scount = 0;
		if(m_pd3ddev == NULL)
			return 0;
		HRESULT result;
		while( FAILED( result = m_pd3ddev->Present(NULL,NULL,NULL,NULL))) {
			SunLog.LogOut("Present失敗 : %d\n",result);
			result = m_pd3ddev->Reset(&m_d3dpp);
			if(FAILED(result)) {
				SunLog.LogOut("Reset失敗 : %d\n",result);
				return 0;
			}
		}
	}
	else {
		this->m_scount++;
	}
	return 1;
}
/**
	@brief クリアー
*/
int CSun3D::Clear(D3DCOLOR color)
{
	if(FAILED(m_pd3ddev->Clear(0,NULL,D3DCLEAR_TARGET ,color, 1.0f, 0)))
		return 0;
	return 1;
}
/**
	@brief BeginScene()
*/
int CSun3D::BeginScene()
{
	if(FAILED(m_pd3ddev->BeginScene()))
		return 0;
	return 1;
}
/**
	@brief EndScene()
*/
int CSun3D::EndScene()
{
	if(FAILED(m_pd3ddev->EndScene()))
		return 0;
	return 1;
}
/**
	@brief テクスチャサイズは？
*/
int CSun3D::GetTextureSize()
{
	return floor2(min(m_d3dcaps.MaxTextureWidth,m_d3dcaps.MaxTextureHeight));
}
/**
	@brief DLLロード、D3Dオブジェクト取得
*/
int CSun3D::CreateDirect3D()
{
	PDIRECT3DCREATE9 pProcDirect3DCreate9 = NULL;

	// Load DLL
	this->m_hDll = ::LoadLibrary( "d3d9.dll" );
	if ( this->m_hDll == NULL )
	{
		throw SunExeption("DLLの読み込みに失敗\nDirectX9が正しくインストールされているか\n確認して下さい\n");
	}

	// Load Direct3DCreate from DLL
	pProcDirect3DCreate9 = (PDIRECT3DCREATE9)::GetProcAddress( this->m_hDll, "Direct3DCreate9" );
	if ( pProcDirect3DCreate9 == NULL )
	{
		SunApp.MsgBox("DLLからの関数呼び出しに失敗\nDirectX9が正しくインストールされているか\n確認して下さい" );
		return 0;
	}

	// Create Direct3D Object
	this->m_pd3d = pProcDirect3DCreate9( D3D_SDK_VERSION );
	if ( this->m_pd3d == NULL )
	{
		SunApp.MsgBox("Direct3Dオブジェクトの生成に失敗\nDirectX9が正しくインストールされているか\n確認して下さい" );
		return 0;
	}
	return 1;
}

/**
	@brief デバイス能力取得
*/
int CSun3D::InitCaps()
{

	//デバイスの能力の調査
	//デフォルトのデバイスのみ調査します。（ビデオカードが２つ以上ある場合は考慮しない）

	//---アダプタのハードウェア情報を取得します
	if( m_pd3d->GetDeviceCaps(0,D3DDEVTYPE_HAL,&m_d3dcaps) != D3D_OK )
	{//ＨＡＬデバイスとしてのハードウェアがまったく存在しない
		SunLog.LogOut("HALデバイスを生成できません\n");
		if( m_pd3d->GetDeviceCaps(0,D3DDEVTYPE_REF,&m_d3dcaps) != D3D_OK )
		{//ＲＥＦデバイスとしてのハードウェアがまったく存在しない
			//この処理が実行される時、アダプタのハードウェアはまったくもってゲーム動作に適さないと認識できる
			//しかし、プログラム終了する程の権限を持たない例外である為に処理を継続する
			//これによって、アダプタのハードウェア情報は取得できていない
			SunApp.MsgBox("デバイスがDirect3Dに対応していません。\n");
			return 0;
		}
	}
	return 1;
}
/**
	@brief デバイス作成
*/
int CSun3D::CreateDevice()
{
	//ハードウェア頂点処理（頂点バッファ）はしない方針で（２Dだから。TWINKBELL参照）
	//ＨＡＬデバイス生成を試みる
	if( FAILED(m_pd3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,SunApp.m_hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3ddev)))
	{
		SunLog.LogOut("HALデバイスを生成できませんでした、REFで動作します\n");
		//ＲＥＦデバイス生成を試みる
		if( FAILED(m_pd3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,SunApp.m_hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3ddev)))
		{
			//--どのデバイスも取得できなかった
			SunApp.MsgBox("DirectXデバイスを取得できませんでした\n");
			SAFE_RELEASE(m_pd3d);						//DirectX3Dオブジェクトを解放

			return FALSE;
		}

	}

	if( m_pd3ddev == NULL )
	{//オブジェクト生成失敗
		SunApp.MsgBox("DirectXD3D9デバイス生成失敗\n");
		return 0;
	}

	SunLog.LogOut("使用できるビデオメモリ：[%d] MB\n", m_pd3ddev->GetAvailableTextureMem()/(1024*1024) );

	return 1;
}
/**
	@brief ディスプレイモードを決定する
*/
int CSun3D::DecideDisplay()
{
	D3DDISPLAYMODE displayMode;
	m_pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&displayMode);		//現在のディスプレイモードデータ取得
	if(SunApp.m_window)
	{//ウィンドウモードではフォーマットが固定される
		//バックバッファフォーマットをディスプレイと等価に
		m_d3dpp.BackBufferFormat = displayMode.Format;
		//フォーマットを見てビットモード決定・・だる
		if(displayMode.Format == D3DFMT_R5G6B5 ||
			displayMode.Format == D3DFMT_A1R5G5B5 ||
			displayMode.Format == D3DFMT_X1R5G5B5 ||
			displayMode.Format == D3DFMT_X4R4G4B4 ||
			displayMode.Format == D3DFMT_A4R4G4B4 ||
			displayMode.Format == D3DFMT_A8R3G3B2)
		{
			m_bitmode = 16;
		}
		else if(displayMode.Format == D3DFMT_R8G8B8)
		{//24は少ないだろう。
			m_bitmode = 24;
		}
		else if(displayMode.Format == D3DFMT_A8R8G8B8 ||
			displayMode.Format == D3DFMT_X8R8G8B8 ||
			displayMode.Format == D3DFMT_A2R10G10B10 ||
			displayMode.Format == D3DFMT_A8B8G8R8 ||
			displayMode.Format == D3DFMT_X8B8G8R8 ||
			displayMode.Format == D3DFMT_A2B10G10R10)
		{//32は重くなるのが怖いけど、なんとかするべき。
			m_bitmode = 32;
		}
		else if(displayMode.Format == D3DFMT_A16B16G16R16 ||
			displayMode.Format == D3DFMT_A16B16G16R16F)
		{//64は、将来的に普及するかも
			m_bitmode = 64;
		}
		else
		{//謎の画面モード
			SunApp.MsgBox("現在の画面モードが判別できません。\n");
			return 0;
		}
		return 1;
	}
	else
	{//フルスクリーンモードでは自分でディスプレイモードを決められる
		for ( long i = 0; BackBufferFormats[i] != D3DFMT_UNKNOWN; i++ )
		{
			if(FAILED(m_pd3d->CheckDeviceType( 0 , D3DDEVTYPE_HAL , BackBufferFormats[i], BackBufferFormats[i], SunApp.m_window)))
			{
				continue;
			}
			if(GetBackBufferFormatRank(m_d3dpp.BackBufferFormat) < GetBackBufferFormatRank(BackBufferFormats[i]))
			{
				m_d3dpp.BackBufferFormat = BackBufferFormats[i];
			}
		}
		if(this->m_d3dpp.BackBufferFormat == D3DFMT_UNKNOWN)
		{
			SunApp.MsgBox("このデバイスは利用可能なフォーマットがありません。\n");
			return 0;
		}
	}
	return 1;
}
/**
	@brief バックバッファフォーマットの評価

	基本的にアルファ値なしが優先
*/
int CSun3D::GetBackBufferFormatRank(D3DFORMAT format)
{
	int ret=0;
	if(m_bitmode == 16)
	{
		switch(format)
		{
		case D3DFMT_R5G6B5:
			ret = 6;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 4;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 3;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 2;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(m_bitmode == 24)
	{
		switch(format)
		{
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_R5G6B5:
			ret = 6;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 4;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 3;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 2;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(m_bitmode == 32)
	{
		switch(format)
		{
		case D3DFMT_X8R8G8B8:
			ret = 13;
			break;
		case D3DFMT_X8B8G8R8:
			ret = 12;
			break;
		case D3DFMT_A2R10G10B10:
			ret = 11;
			break;
		case D3DFMT_A2B10G10R10:
			ret = 10;
			break;
		case D3DFMT_A8R8G8B8:
			ret = 9;
			break;
		case D3DFMT_A8B8G8R8:
			ret = 8;
			break;
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_R5G6B5:
			ret = 6;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 4;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 3;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 2;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(m_bitmode == 64)
	{
		switch(format)
		{
		case D3DFMT_A16B16G16R16F:
			ret = 15;
			break;
		case D3DFMT_A16B16G16R16:
			ret = 14;
			break;
		case D3DFMT_X8R8G8B8:
			ret = 13;
			break;
		case D3DFMT_X8B8G8R8:
			ret = 12;
			break;
		case D3DFMT_A2R10G10B10:
			ret = 11;
			break;
		case D3DFMT_A2B10G10R10:
			ret = 10;
			break;
		case D3DFMT_A8R8G8B8:
			ret = 9;
			break;
		case D3DFMT_A8B8G8R8:
			ret = 8;
			break;
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_R5G6B5:
			ret = 6;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 4;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 3;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 2;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else
	{
		SunApp.MsgBox("ビットモードの設定が狂っています。\n一度コンフィグファイルを消してください。\n");
	}
	return ret;
}
/**
	@brief リフレッシュレートの決定
*/
int CSun3D::DecideRefreshRate()
{
	//ウィンドウモード時にリフレッシュレートを設定するとバグる（;д
	if(SunApp.m_window == 1)
		return 1;
	//同じく垂直同期を取らない設定のとき
	if(this->m_vsync == 0)
		return 1;
	HRESULT hr;
	int refreshRate = 0;
	// モードを列挙
	D3DDISPLAYMODE DisplayMode;
	DWORD ModeCount = this->m_pd3d->GetAdapterModeCount( D3DADAPTER_DEFAULT, m_d3dpp.BackBufferFormat);
	for ( DWORD j = 0; j < ModeCount; j++ )
	{
		hr = this->m_pd3d->EnumAdapterModes( D3DADAPTER_DEFAULT, m_d3dpp.BackBufferFormat, j, &DisplayMode );
		if SUCCEEDED( hr )
		{
			//640*480だったら
			if(DisplayMode.Width == SunApp.m_width && DisplayMode.Height == SunApp.m_height)
			{
				//リフレッシュレートがより60に近かったら
				if(abs(DisplayMode.RefreshRate-60) < abs(refreshRate-60))
				{
					refreshRate = DisplayMode.RefreshRate;
				}
			}
		}
	}
	m_d3dpp.FullScreen_RefreshRateInHz = refreshRate;
	return 1;
}
/**
	@brief テクスチャフォーマットの決定
*/
int CSun3D::DecideTextureFormat()
{
	HRESULT hr;
	//テクスチャフォーマットを列挙
	for ( long j = 0; TextureFormats[j] != D3DFMT_UNKNOWN; j++ )
	{
		hr = this->m_pd3d->CheckDeviceFormat(
						D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						m_d3dpp.BackBufferFormat,
						0,
						D3DRTYPE_TEXTURE,
						TextureFormats[j] );
		if SUCCEEDED( hr )
		{
			//現在のテクスチャフォーマットと評価を比較
			if(GetTextureFormatRank(m_format) < GetTextureFormatRank(TextureFormats[j]))
			{
				m_format = TextureFormats[j];
			}
		}
	}
	return 1;
}
/**
	@brief テクスチャフォーマットの評価

  基本的にアルファ値つきが優先
*/
int CSun3D::GetTextureFormatRank(D3DFORMAT format)
{
	int ret = 0;
	int bitmode = ( m_texbit != 0 ? m_texbit : m_bitmode );
	if(bitmode == 16)
	{
		switch(format)
		{
		case D3DFMT_A4R4G4B4:
			ret = 6;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 4;
			break;
		case D3DFMT_R5G6B5:
			ret = 3;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 2;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(bitmode == 24)
	{
		switch(format)
		{
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 6;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 4;
			break;
		case D3DFMT_R5G6B5:
			ret = 3;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 2;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(bitmode == 32)
	{
		switch(format)
		{
		case D3DFMT_A8R8G8B8:
			ret = 13;
			break;
		case D3DFMT_A8B8G8R8:
			ret = 12;
			break;
		case D3DFMT_A2R10G10B10:
			ret = 11;
			break;
		case D3DFMT_A2B10G10R10:
			ret = 10;
			break;
		case D3DFMT_X8R8G8B8:
			ret = 9;
			break;
		case D3DFMT_X8B8G8R8:
			ret = 8;
			break;
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 6;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 4;
			break;
		case D3DFMT_R5G6B5:
			ret = 3;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 2;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(m_bitmode == 64)
	{
		switch(format)
		{
		case D3DFMT_A16B16G16R16F:
			ret = 15;
			break;
		case D3DFMT_A16B16G16R16:
			ret = 14;
			break;
		case D3DFMT_A8R8G8B8:
			ret = 13;
			break;
		case D3DFMT_A8B8G8R8:
			ret = 12;
			break;
		case D3DFMT_A2R10G10B10:
			ret = 11;
			break;
		case D3DFMT_A2B10G10R10:
			ret = 10;
			break;
		case D3DFMT_X8R8G8B8:
			ret = 9;
			break;
		case D3DFMT_X8B8G8R8:
			ret = 8;
			break;
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 6;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 4;
			break;
		case D3DFMT_R5G6B5:
			ret = 3;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 2;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else
	{
		SunApp.MsgBox("ビットモードの設定が狂っています。\n一度コンフィグファイルを消してください。\n");
	}
	return ret;
}
/**
	@brief その他のパラメータを決定
*/
int CSun3D::DecideOtherParameters()
{
	//---表示領域の設定	
	m_d3dpp.BackBufferWidth			= SunApp.m_width;				//横ドット幅設定
	m_d3dpp.BackBufferHeight		= SunApp.m_height;				//縦ドット幅設定
	m_d3dpp.BackBufferCount			= 1;						//バックバッファの数
	m_d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;		//
	m_d3dpp.MultiSampleQuality		= D3DMULTISAMPLE_NONE;		//
//	m_d3dpp.SwapEffect				= D3DSWAPEFFECT_FLIP;	//フリップレベル
	m_d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	//フリップレベル
	m_d3dpp.Windowed				= SunApp.m_window;				//ウインドウモードで動作？
	m_d3dpp.hDeviceWindow			= SunApp.m_hWnd;				//ウィンドウハンドル
	m_d3dpp.EnableAutoDepthStencil	= false;					//
	m_d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				//ステンシルフォーマット
//	m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;			//デバッグ用にバックバッファをロック可能にする
//	m_d3dpp.FullScreen_RefreshRateInHz;							//こいつは決定済み
//	m_d3dpp.PresentationInterval;								//あとでコンフィグにより決定
	if(SunApp.m_window) //ウィンドウモードで垂直同期は取らないことにする
		m_vsync = 0;
	//ここからコンフィグに影響される
//	m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;
	switch(m_vsync)
	{
	//VSYNC非同期の場合
	case 0:
		if(m_d3dcaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE)
		{
			m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
		}
		//デバイスがD3DPRESENT_INTERVAL_IMMEDIATEに対応していないとき
		else
		{
			m_vsync	= 1;
			m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;
		}
		break;
	//VSYNC同期の場合
	case 1:
		m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;
		break;
	default:
		SunApp.MsgBox("垂直同期待ちの設定が狂っています。\n一度コンフィグファイルを削除してください。\n");
		return 0;
	}
	return 1;
}
/**
	@brief 決定されたパラメーター出力
*/
int CSun3D::ParameterLog()
{
	SunLog.LogOut("決定されたパラメータ\n");
	SunLog.LogOut("	BackBufferFormat[ %s ]\n",this->GetFormatString(m_d3dpp.BackBufferFormat).data());
	SunLog.LogOut("	WindowMode[ %d ]\n",this->m_d3dpp.Windowed);
	SunLog.LogOut("	RefreshRate[ %d ]\n",this->m_d3dpp.FullScreen_RefreshRateInHz);
	SunLog.LogOut("	PresentationInterval[ %d ]\n",this->m_d3dpp.PresentationInterval);
	SunLog.LogOut("	Vsync[ %d ]\n",this->m_vsync);
	SunLog.LogOut("	TextureFormat[ %s ]\n",this->GetFormatString(m_format).data());
	return 1;
}
/**
	@brief ピクセルフォーマット文字列取得
*/
string CSun3D::GetFormatString(D3DFORMAT Format)
{
	string ret;
	switch ( Format )
	{
	case D3DFMT_UNKNOWN:			ret = "UNKNOWN" ;		break;
	case D3DFMT_R8G8B8:				ret = "R8G8B8" ;		break;
	case D3DFMT_A8R8G8B8:			ret = "A8R8G8B8" ;		break;
	case D3DFMT_X8R8G8B8:			ret = "X8R8G8B8" ;		break;
	case D3DFMT_R5G6B5:				ret = "R5G6B5" ;		break;
	case D3DFMT_X1R5G5B5:			ret = "X1R5G5B5" ;		break;
	case D3DFMT_A1R5G5B5:			ret = "A1R5G5B5" ;		break;
	case D3DFMT_A4R4G4B4:			ret = "A4R4G4B4" ;		break;
	case D3DFMT_R3G3B2:				ret = "R3G3B2" ;		break;
	case D3DFMT_A8:					ret = "A8" ;			break;
	case D3DFMT_A8R3G3B2:			ret = "A8R3G3B2" ;		break;
	case D3DFMT_X4R4G4B4:			ret = "X4R4G4B4" ;		break;
	case D3DFMT_A2B10G10R10:		ret = "A2B10G10R10" ;	break;
	case D3DFMT_A8B8G8R8:			ret = "A8B8G8R8" ;		break;
	case D3DFMT_X8B8G8R8:			ret = "X8B8G8R8" ;		break;
	case D3DFMT_G16R16:				ret = "G16R16" ;		break;
	case D3DFMT_A2R10G10B10:		ret = "A2R10G10B10" ;	break;
	case D3DFMT_A16B16G16R16:		ret = "A16B16G16R16" ;	break;
	case D3DFMT_A8P8:				ret = "A8P8" ;			break;
	case D3DFMT_P8:					ret = "P8" ;			break;
	case D3DFMT_L8:					ret = "L8" ;			break;
	case D3DFMT_L16:				ret = "L16" ;			break;
	case D3DFMT_A8L8:				ret = "A8L8" ;			break;
	case D3DFMT_A4L4:				ret = "A4L4" ;			break;
	case D3DFMT_V8U8:				ret = "V8U8" ;			break;
	case D3DFMT_Q8W8V8U8:			ret = "Q8W8V8U8" ;		break;
	case D3DFMT_V16U16:				ret = "V16U16" ;		break;
	case D3DFMT_Q16W16V16U16:		ret = "Q16W16V16U16" ;	break;
	case D3DFMT_CxV8U8:				ret = "CxV8U8" ;		break;
	case D3DFMT_L6V5U5:				ret = "L6V5U5" ;		break;
	case D3DFMT_X8L8V8U8:			ret = "X8L8V8U8" ;		break;
	case D3DFMT_A2W10V10U10:		ret = "A2W10V10U10" ;	break;
	case D3DFMT_G8R8_G8B8:			ret = "G8R8_G8B8" ;		break;
	case D3DFMT_R8G8_B8G8:			ret = "R8G8_B8G8" ;		break;
	case D3DFMT_DXT1:				ret = "DXT1" ;			break;
	case D3DFMT_DXT2:				ret = "DXT2" ;			break;
	case D3DFMT_DXT3:				ret = "DXT3" ;			break;
	case D3DFMT_DXT4:				ret = "DXT4" ;			break;
	case D3DFMT_DXT5:				ret = "DXT5" ;			break;
	case D3DFMT_UYVY:				ret = "UYVY" ;			break;
	case D3DFMT_YUY2:				ret = "YUY2" ;			break;
	case D3DFMT_D16_LOCKABLE:		ret = "D16_LOCKABLE" ;	break;
	case D3DFMT_D32:				ret = "D32" ;			break;
	case D3DFMT_D15S1:				ret = "D15S1" ;			break;
	case D3DFMT_D24S8:				ret = "D24S8" ;			break;
	case D3DFMT_D24X8:				ret = "D24X8" ;			break;
	case D3DFMT_D24X4S4:			ret = "D24X4S4" ;		break;
	case D3DFMT_D32F_LOCKABLE:		ret = "D32F_LOCKABLE" ;	break;
	case D3DFMT_D24FS8:				ret = "D24FS8" ;		break;
	case D3DFMT_D16:				ret = "D16" ;			break;
	case D3DFMT_VERTEXDATA:			ret = "VERTEXDATA" ;	break;
	case D3DFMT_INDEX16:			ret = "INDEX16" ;		break;
	case D3DFMT_INDEX32:			ret = "INDEX32" ;		break;
	case D3DFMT_R16F:				ret = "R16F" ;			break;
	case D3DFMT_G16R16F:			ret = "G16R16F" ;		break;
	case D3DFMT_A16B16G16R16F:		ret = "A16B16G16R16F" ;	break;
	case D3DFMT_R32F:				ret = "R32F" ;			break;
	case D3DFMT_G32R32F:			ret = "G32R32F" ;		break;
	case D3DFMT_A32B32G32R32F:		ret = "A32B32G32R32F" ;	break;
	}

	return ret;
}
/**
	@brief エラー文字列取得
*/
string CSun3D::GetErrorString(HRESULT hr)
{
	string ret;
	switch ( hr )
	{
	case D3D_OK:
		ret = "エラーは発生していない。" ;
		break;
	case D3DERR_CONFLICTINGRENDERSTATE:
		ret = "現在設定されているレンダリング ステートは一緒に使用できない。" ;
		break;
	case D3DERR_CONFLICTINGTEXTUREFILTER:
		ret = "現在のテクスチャ フィルタは一緒に使用できない。" ;
		break;
	case D3DERR_CONFLICTINGTEXTUREPALETTE:
		ret = "現在のテクスチャは同時に使用できない。一般にマルチテクスチャ デバイスにおいて、同時に有効にされたパレット化テクスチャで同じパレットを共有する必要がある場合に発生する。" ;
		break;
	case D3DERR_DEVICELOST:
		ret = "デバイスが失われていて、現時点では復元できないため、レンダリングは不可能である。" ;
		break;
	case D3DERR_DEVICENOTRESET:
		ret = "デバイスはリセットできない。" ;
		break;
	case D3DERR_DRIVERINTERNALERROR:
		ret = "内部ドライバ エラー。" ;
		break;
	case D3DERR_INVALIDCALL:
		ret = "メソッドの呼び出しが無効である。たとえば、メソッドのパラメータに無効な値が設定されている場合など。" ;
		break;
	case D3DERR_INVALIDDEVICE:
		ret = "要求されたデバイスの種類が有効でない。" ;
		break;
	case D3DERR_MOREDATA:
		ret = "指定されたバッファ サイズに保持できる以上のデータが存在する。" ;
		break;
	case D3DERR_NOTAVAILABLE:
		ret = "このデバイスは、照会されたテクニックをサポートしていない。" ;
		break;
	case D3DERR_NOTFOUND:
		ret = "要求された項目が見つからなかった。" ;
		break;
	case D3DERR_OUTOFVIDEOMEMORY:
		ret = "Direct3D が処理を行うのに十分なディスプレイ メモリがない。" ;
		break;
	case D3DERR_TOOMANYOPERATIONS:
		ret = "デバイスがサポートしている以上のテクスチャ フィルタリング処理を、アプリケーションが要求している。" ;
		break;
	case D3DERR_UNSUPPORTEDALPHAARG:
		ret = "アルファ チャネルに対して指定されているテクスチャ ブレンディング引数を、デバイスがサポートしていない。" ;
		break;
	case D3DERR_UNSUPPORTEDALPHAOPERATION:
		ret = "アルファ チャネルに対して指定されているテクスチャ ブレンディング処理を、デバイスがサポートしていない。" ;
		break;
	case D3DERR_UNSUPPORTEDCOLORARG:
		ret = "カラー値に対して指定されているテクスチャ ブレンディング引数を、デバイスがサポートしていない。" ;
		break;
	case D3DERR_UNSUPPORTEDCOLOROPERATION:
		ret = "カラー値に対して指定されているテクスチャ ブレンディング処理を、デバイスがサポートしていない。" ;
		break;
	case D3DERR_UNSUPPORTEDFACTORVALUE:
		ret = "デバイスが指定されたテクスチャ係数値をサポートしていない。" ;
		break;
	case D3DERR_UNSUPPORTEDTEXTUREFILTER:
		ret = "デバイスが指定されたテクスチャ フィルタをサポートしていない。" ;
		break;
	case D3DERR_WRONGTEXTUREFORMAT:
		ret = "テクスチャ サーフェスのピクセル フォーマットが有効でない。" ;
		break;
	case E_FAIL:
		ret = "Direct3D サブシステム内で原因不明のエラーが発生した。" ;
		break;
	case E_INVALIDARG:
		ret = "無効なパラメータが戻ってくる関数に渡された。" ;
		break;
	case E_OUTOFMEMORY:
		ret = "Direct3D が呼び出しを完了するための十分なメモリを割り当てることができなかった。" ;
		break;
	case D3DXERR_CANNOTATTRSORT:
		ret = "最適化テクニックとして属性のソート (D3DXMESHOPT_ATTRSORT) はサポートされていない。 " ;
		break;
	case D3DXERR_CANNOTMODIFYINDEXBUFFER:
		ret = "インデックス バッファを変更できない。 " ;
		break;
	case D3DXERR_INVALIDMESH:
		ret = "メッシュが無効である。 " ;
		break;
	case D3DXERR_SKINNINGNOTSUPPORTED:
		ret = "スキニングはサポートされていない。 " ;
		break;
	case D3DXERR_TOOMANYINFLUENCES:
		ret = "指定された影響が多すぎる。 " ;
		break;
	case D3DXERR_INVALIDDATA:
		ret = "データが無効である。" ;
		break;
	default:
		ret = "未知のエラー。" ;
		break;
	}

	return ret;
}
/**
	@brief デバイス能力を調べてログを出力
*/
int CSun3D::DeviceLog()
{
	SunLog.LogOut("グラフィックデバイス能力の列挙\n");

	//ドライバはD3DPRESENT_INTERVAL_IMMEDIATEをサポートしているか？
	SunLog.LogCaps( m_d3dcaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE,"	ドライバは、D3DPRESENT_INTERVAL_IMMEDIATEをサポート");

	//ドライバはD3DPRESENT_INTERVAL_ONEをサポートしているか？
	SunLog.LogCaps( m_d3dcaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE,"	ドライバは、D3DPRESENT_INTERVAL_ONEをサポート");

	//バックバッファがスワップ不能
	if(m_d3dcaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE == 0 && m_d3dcaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE == 0)
	{
		SunApp.MsgBox("このデバイスではスワップができません\n");
		return 0;
	}
	//テクスチャサイズは２の累乗か？
	SunLog.LogCaps( m_d3dcaps.TextureCaps & D3DPTEXTURECAPS_POW2 , "	テクスチャサイズは２の累乗か？");

	//テクスチャサイズは常に正方形か？
	SunLog.LogCaps( m_d3dcaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY , "	テクスチャサイズは常に正方形か？" );

	//テクスチャの最大サイズは？
	SunLog.LogOut("	テクスチャの最大サイズ：[%d] × [%d]\n", m_d3dcaps.MaxTextureWidth , m_d3dcaps.MaxTextureHeight);

	//加算合成
	SunLog.LogCaps( m_d3dcaps.TextureOpCaps & D3DTEXOPCAPS_ADD ,"	加算合成");

	//アルファブレンド
	SunLog.LogCaps( m_d3dcaps.TextureOpCaps & D3DTEXOPCAPS_BLENDCURRENTALPHA ,"	アルファブレンド");

	//プリミティブの描画可能数
	SunLog.LogOut("	プリミティブの描画可能最大数:[%d]\n",m_d3dcaps.MaxPrimitiveCount);

	//---デバイスの性能指標（D3DDEVTYPE 列挙型
	SunLog.LogCaps( m_d3dcaps.DeviceType == D3DDEVTYPE_HAL,"	ハードウェアアクセラレーションレイヤー");

	//システムメモリバッファからビデオメモリバッファへのイメージ転送ができるか？
	SunLog.LogCaps( m_d3dcaps.DevCaps  & D3DDEVCAPS_CANBLTSYSTONONLOCAL,"	システム→ビデオのイメージ転送");

	//---レンダリングコマンドを高速に送信できるか？
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_CANRENDERAFTERFLIP,"	高速レンダリング");
	
	//---システムメモリにある実行バッファを処理できるか？
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_EXECUTESYSTEMMEMORY,"	システムメモリの実行バッファ");

	//---ビデオメモリにある実行バッファを処理できるか？
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_EXECUTEVIDEOMEMORY,"	ビデオメモリの実行バッファ");
	
	//---デバイスには、シーンのラスタ化用のハードウェア アクセラレーションがあるか？
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_HWRASTERIZATION,"	シーンラスタのハードウェアアクセラレーション");
	
	//---デバイスは、ハードウェアでトランスフォーム & ライティングをサポートできるか？
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT, "	行列変換、光源処理ハードウェア機構" );
	
	//---デバイスは、５次ベジェ、スプラインをサポートしているか？
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_QUINTICRTPATCHES,"	５次ベジェ、スプライン曲線のサポート");

	//---ビデオメモリ以外にテクスチャを格納できるか？
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_TEXTURENONLOCALVIDMEM ,"	ビデオメモリ以外にテクスチャを格納できるか？");

	//---システムメモリからテクスチャを利用できるか？
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_TEXTURESYSTEMMEMORY ,"	システムメモリからテクスチャを利用できるか？");

	//---頂点演算済みのデータ格納先にシステムメモリのバッファを利用できる
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_TLVERTEXSYSTEMMEMORY ,"	システムメモリバッファに頂点データを配置");
	
	//---頂点演算済みのデータ格納先にビデオメモリのバッファを利用できる
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_TLVERTEXVIDEOMEMORY ,"	ビデオメモリバッファに頂点データを配置");
	
	//アンチエイリアスを利用できる
	SunLog.LogCaps( m_d3dcaps.LineCaps & D3DLINECAPS_ANTIALIAS , "	アンチエイリアスを利用できる");

	//ソースブレンドを利用できる
	SunLog.LogCaps( m_d3dcaps.LineCaps & D3DLINECAPS_BLEND , "	ソースブレンドを利用できる");

	//フォグを利用できる
	SunLog.LogCaps( m_d3dcaps.LineCaps & D3DLINECAPS_FOG , "	フォグを利用できる");

	//テクスチャマッピングを利用できる
	SunLog.LogCaps( m_d3dcaps.LineCaps & D3DLINECAPS_TEXTURE , "	テクスチャマッピングを利用できる");
	return 1;
}
/**
	デバイスログ関数
*/
int CSun3D::CheckAdapterInformation()
{
	D3DADAPTER_IDENTIFIER9 Identifier;
	HRESULT hr;

	//
	// アダプター情報
	//
	SunLog.LogOut( "アダプター情報\n" );
	

	if(FAILED(m_pd3d->GetAdapterIdentifier( D3DADAPTER_DEFAULT, 0, &Identifier )))
	{
		SunApp.MsgBox("アダプタ情報を得られませんでした。３Ｄデバイスが有効になっているか確認してください。\n");
		return 0;
	}
	{
		SunLog.LogOut( "	Name [ %s ]\n", Identifier.Description );
		SunLog.LogOut( "	Driver [ %s ]\n", Identifier.Driver );
		SunLog.LogOut( "	Product [ %u ]\n", HIWORD(Identifier.DriverVersion.HighPart) );

		SunLog.LogOut( "　　Version [ %u.%u.%u ]\n",
			LOWORD(Identifier.DriverVersion.HighPart),
			HIWORD(Identifier.DriverVersion.LowPart),
			LOWORD(Identifier.DriverVersion.LowPart) );
		
		SunLog.LogOut( "	Windows Hardware Quality Lab (WHQL) 認証レベル\n" );

		if ( Identifier.WHQLLevel == 0 )
		{
			SunLog.LogOut( "	認証されていない。\n" );			
		}
		else if ( Identifier.WHQLLevel == 1 )
		{
			SunLog.LogOut( "	WHQL の認証は受けているが、利用できる日付情報はない。\n" );
		}
		else
		{
			SunLog.LogOut( "	%u.%u.%u\n",
				((Identifier.WHQLLevel>>16)&0xFF), ((Identifier.WHQLLevel>>8)&0x0F), ((Identifier.WHQLLevel>>0)&0x0F) );
			
		}
	}

	//
	// 現在の画面モード取得
	//
	SunLog.LogOut( "現在の画面情報\n" );
	D3DDISPLAYMODE WindowMode;
	if(FAILED( m_pd3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &WindowMode ) ) )
	{
		SunApp.MsgBox("現在の画面情報を得られませんでした。３Ｄデバイスが有効になっているか確認してください。\n");
		return 0;
	}
	{
		SunLog.LogOut( "　　　Width [ %u ]\n", WindowMode.Width );
		SunLog.LogOut( "　　　Height [ %u ]\n", WindowMode.Height );
		SunLog.LogOut( "　　　RefreshRate [ %u ]\n", WindowMode.RefreshRate );
		SunLog.LogOut( "　　　Format [ %s ]\n", this->GetFormatString(WindowMode.Format).data() );
	}

	//
	// フルスクリーン時に使用可能な画面モードを列挙する
	//
	SunLog.LogOut( "使用可能な[%d]×[%d]画面モードを列挙\n" ,SunApp.m_width, SunApp.m_height);

	//　バックバッファフォーマットで列挙
	int devFlag = 0;	//ゲームを実行できるデバイスか？
	for ( long i = 0; BackBufferFormats[i] != D3DFMT_UNKNOWN; i++ )
	{
		// ディスプレイモード数の取得
		D3DDISPLAYMODE DisplayMode;
		DWORD ModeCount = this->m_pd3d->GetAdapterModeCount( D3DADAPTER_DEFAULT, BackBufferFormats[i] );
		if ( ModeCount > 0 )
		{
			int modeFlag = 0;	//このモードはゲームに使用できる？
			SunLog.LogOut( "　BackBuffer Format [ %s ]\n", this->GetFormatString(BackBufferFormats[i]).data() );
			// モードを列挙
			for ( DWORD j = 0; j < ModeCount; j++ )
			{
				hr = this->m_pd3d->EnumAdapterModes( D3DADAPTER_DEFAULT, BackBufferFormats[i], j, &DisplayMode );
				if SUCCEEDED( hr )
				{
					//640*480だったら
					if(DisplayMode.Width == SunApp.m_width && DisplayMode.Height == SunApp.m_height)
					{
						modeFlag += 1;
						SunLog.LogOut("		RefreshRate[%d]\n",DisplayMode.RefreshRate);
					}
				}
			}
			//テクスチャフォーマットを列挙
			for ( long j = 0; TextureFormats[j] != D3DFMT_UNKNOWN; j++ )
			{
				hr = this->m_pd3d->CheckDeviceFormat(
								D3DADAPTER_DEFAULT,
								D3DDEVTYPE_HAL,
								BackBufferFormats[i],
								0,
								D3DRTYPE_TEXTURE,
								TextureFormats[j] );
				if SUCCEEDED( hr )
				{
					modeFlag += 1;
					SunLog.LogOut( "　	　　TextureFormat[ %s ]\n", this->GetFormatString(TextureFormats[j]).data() );
				}
			}
			//このモードでは640*480で使用できるテクスチャフォーマットが存在する
			devFlag = 1;
		}
	}
	//万が一に備えて・・
	if(devFlag == 0)
	{
		SunApp.MsgBox("このデバイスでは[%d]×[%d]で使用できるテクスチャフォーマットがありません\n",SunApp.m_width, SunApp.m_height);
		return 0;
	}

	return 1;
}
/**
	テクスチャ設定
*/
int CSun3D::SetTexture(LPDIRECT3DTEXTURE9 pd3dtex)
{
	if(FAILED(this->m_pd3ddev->SetTexture(0,pd3dtex)))
		return 0;
	return 1;
}
/**
	@brief レンダリングステート

	現在と同じステートならなにもしません。
*/
int CSun3D::SetRenderState( D3DRENDERSTATETYPE State, DWORD Param )
{
//	if ( m_renderState[State] != Param )
	{
		this->m_renderState[State] = Param;
		if(FAILED(this->m_pd3ddev->SetRenderState( State, Param )))
			return 0;
	}
	return 1;
}
/**
	@brief テクスチャステート
	
	現在と同じステートならなにもしません。
*/
int CSun3D::SetTextureState( D3DTEXTURESTAGESTATETYPE State, DWORD Param )
{
//	if ( this->m_textureState[State] != Param )
	{
		this->m_textureState[State] = Param;
		if(FAILED(this->m_pd3ddev->SetTextureStageState( 0, State, Param )))
			return 0;
	}
	return 1;
}
/*
//FVFフラグのセット
//現在と同じステートならなにもしません。
*/
int CSun3D::SetFVF(DWORD fvfState)
{
//	if ( this->m_fvfState != fvfState )
	{
		m_fvfState = fvfState;
		if(FAILED(this->m_pd3ddev->SetFVF(fvfState)))
			return 0;
	}
	return 1;
}
/**
//カラーキー
//現在と同じステートならなにもしません。
*/
int CSun3D::SetColorkeyEnable( bool Flag )
{
	if ( Flag )
	{
		this->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
		this->SetRenderState( D3DRS_ALPHATESTENABLE, true );
		this->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
		this->SetRenderState( D3DRS_ALPHAREF, 0 );
	}
	else
	{
		this->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
		this->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	}
	return 1;
}
/**
//ブレンディングタイプ
//現在と同じステートならなにもしません。
*/
int CSun3D::SetBlendingType( eBlendingType BlendingType )
{
	switch ( BlendingType )
	{
	case BLEND_NONE:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_NORMAL:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_ADD:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_MUL:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_REVERSE:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_BRILLIANT:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_DARKSIDE:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_MAX:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_MAX );
		break;

	case BLEND_MIN:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_MIN );
		break;

	case BLEND_TEST:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_TEST2:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCCOLOR );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_TRANS:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;
	}
	return 1;
}

int CSun3D::SetViewport(D3DVIEWPORT9& viewport)
{
/*	if( FAILED( m_pd3ddev->SetViewport( &viewport ) ) ) {
		SunLog.LogOut("SetViewportに失敗\n");
		return 0;
	}
*/	return 1;
}
int CSun3D::MoveView(float x,float y,float z)
{
	D3DXMATRIX trans,view;
	D3DXMatrixTranslation(&trans,x,y,z);
	m_pd3ddev->GetTransform( D3DTS_VIEW, &view );
	D3DXMatrixMultiply(&view,&view,&trans);
	m_pd3ddev->SetTransform( D3DTS_VIEW, &view );

	return 1;
}
int CSun3D::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX *pMatrix)
{
	if(FAILED( m_pd3ddev->SetTransform( State, pMatrix ) ))
		return 0;
	return 1;
}
int CSun3D::SetLight(DWORD Index,CONST D3DLIGHT9 *pLight)
{
	if(FAILED( m_pd3ddev->SetLight(Index,pLight)))
		return 0;
	if(FAILED( m_pd3ddev->LightEnable(Index,true)))
		return 0;
	return 1;
}