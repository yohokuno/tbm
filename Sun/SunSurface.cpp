// SunSurface.cpp: SunSurface クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunUtility.h"
#include "SunLog.h"
#include "SunTask.h"
#include "SunExeption.h"
#include "SunFile.h"
#include "SunApp.h"
#include "Sun3D.h"
#include "SunSurface.h"

SunFpsSurface SunFpsShow;
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SunSurface::SunSurface()
{
	m_pd3ds = NULL;
	::ZeroMemory(&m_d3dii,sizeof D3DXIMAGE_INFO);
	::ZeroMemory(&m_d3dsd,sizeof D3DSURFACE_DESC);
}

SunSurface::SunSurface(char* pFile)
{
	m_pd3ds = NULL;
	::ZeroMemory(&m_d3dii,sizeof D3DXIMAGE_INFO);
	::ZeroMemory(&m_d3dsd,sizeof D3DSURFACE_DESC);
	this->LoadFromPack(pFile);
}

SunSurface::~SunSurface()
{
	this->Release();
}

int SunSurface::Release()
{
	SAFE_RELEASE(m_pd3ds);
	::ZeroMemory(&m_d3dii,sizeof D3DXIMAGE_INFO);
	::ZeroMemory(&m_d3dsd,sizeof D3DSURFACE_DESC);
	return 1;
}
//バックバッファを得る
int SunSurface::GetBackBuffer()
{
	this->Release();
	if(FAILED(Sun3D.m_pd3ddev->GetBackBuffer(
		0,
		0,
		D3DBACKBUFFER_TYPE_MONO,
		&m_pd3ds)))
	{
		throw SunExeption("バックバッファサーフェイスを取得できませんでした。\n");
	}
	m_pd3ds->GetDesc(&m_d3dsd);		//サーフェース情報
	return 1;
}
//セーブ
int SunSurface::SaveSurface(LPCTSTR pDestFile,D3DXIMAGE_FILEFORMAT DestFormat,RECT *pSrcRect)
{
	if(FAILED(D3DXSaveSurfaceToFile(pDestFile,DestFormat,m_pd3ds,NULL,pSrcRect))){
		return 0;
	}
	return 1;
}
//D3DSURFACE8からSunSurfaceへの転送
int SunSurface::Blit( LPDIRECT3DSURFACE9 pd3ds,int x,int y,RECT &rcSrc)
{
	if(m_pd3ds == NULL || pd3ds == NULL)
		return 0;
	RECT rcDest={x,y,x+rcSrc.right-rcSrc.left,y+rcSrc.bottom-rcSrc.top};
	if(FAILED(Sun3D.m_pd3ddev->StretchRect(
		pd3ds,			//転送元
		&rcSrc,
		m_pd3ds,		//転送先は自分自身
		&rcDest,
		D3DTEXF_NONE)))
	{
		throw SunExeption("転送失敗\n");
	}
	return 1;
}
//SunSurface同士の転送
int SunSurface::Blit(SunSurface *ps,int x,int y,RECT &rcSrc)
{
	if(m_pd3ds == NULL || ps == NULL)
		return 0;
	return Blit(ps->GetSurface(),x,y,rcSrc);
}
//ソース領域を全体に固定して転送
int SunSurface::Blit(SunSurface *ps,int x,int y)
{
	RECT rcSrc = {0,0,m_d3dsd.Width,m_d3dsd.Height};
	return Blit(ps->GetSurface(),x,y,rcSrc);
}
//塗りつぶし
int SunSurface::Fill( D3DCOLOR color, RECT &rect)
{
	if(FAILED(Sun3D.m_pd3ddev->ColorFill( m_pd3ds, &rect, color)))
	{
		throw SunExeption("塗りつぶし描画失敗\n");
	}
	return 1;
}
//塗りつぶし
int SunSurface::Fill( D3DCOLOR color)
{
	if(FAILED(Sun3D.m_pd3ddev->ColorFill( m_pd3ds, NULL, color)))
	{
		throw SunExeption("塗りつぶし描画失敗\n");
	}
	return 1;
}
//イメージをサーフェイスへロード（このとき幅と高さも求める）
int SunSurface::Load(char *lpszFileName)
{
	this->Release();

	//イメージ情報の取得
	if(FAILED(::D3DXGetImageInfoFromFile(lpszFileName,&this->m_d3dii)))
	{     
		throw SunExeption("%s : ファイルが存在しません。\n",lpszFileName);
	}
	//空のサーフェイス作成
	if(!this->Create(m_d3dii.Width,m_d3dii.Height))
	{
		SAFE_RELEASE(m_pd3ds);
		throw SunExeption("%s : サーフェイスメモリを確保できませんでした。\n",lpszFileName);
	}
	//サーフェイスにＢＭＰを読み込み
	if(FAILED(D3DXLoadSurfaceFromFile(
		m_pd3ds,
		NULL,
		NULL,
		lpszFileName,
		NULL,
		D3DX_DEFAULT,
		0xFF000000,
		NULL)))
	{
		//ファイルになければリソースから
		if(FAILED(D3DXLoadSurfaceFromResource(
			m_pd3ds,
			NULL,
			NULL,
			NULL,
			lpszFileName,
			NULL,
			D3DX_DEFAULT,
			0xFF000000,
			NULL)))
		{
			SAFE_RELEASE(m_pd3ds);
			throw SunExeption("%s : サーフェイスを読み込めませんでした。\n",lpszFileName);
		}
	}

	m_pd3ds->GetDesc(&m_d3dsd);		//サーフェース情報
	//ログ出力
	SunLog.LogOut("%s ( %d × %d ):サーフェイスを読み込みました。\n",lpszFileName,m_d3dii.Width,m_d3dii.Height);
	return 1;
}
/**
	パックファイルからロード
	といっても実ファイルが存在すればそちらを使います
	（ＳｕｎＦiｌｅの仕様）
*/
int SunSurface::LoadFromPack(char *lpszFileName)
{
	SunFileEx file;
	if(file.Open(lpszFileName) == 0)
	{
		throw SunExeption("%s : ファイルが存在しません。\n",lpszFileName);
	}
	file.Read();
	//イメージ情報の取得
	if(FAILED(::D3DXGetImageInfoFromFileInMemory(file.GetBuffer(),file.GetSize(),&this->m_d3dii)))
	{     
		throw SunExeption("%s : ファイルが存在しません。\n",lpszFileName);
	}
	//空のサーフェイス作成
	if(!this->Create(m_d3dii.Width,m_d3dii.Height))
	{
		SAFE_RELEASE(m_pd3ds);
		throw SunExeption("%s : サーフェイスメモリを確保できませんでした。\n",lpszFileName);
	}
	//サーフェイスにＢＭＰを読み込み
	if(FAILED(D3DXLoadSurfaceFromFileInMemory(
		m_pd3ds,
		NULL,
		NULL,
		file.GetBuffer(),
		file.GetSize(),
		NULL,
		D3DX_DEFAULT,
		0xFF000000,
		NULL)))
	{
		SAFE_RELEASE(m_pd3ds);
		throw SunExeption("%s : サーフェイスを読み込めませんでした。\n",lpszFileName);
	}
	return 1;
}
//作成
int	SunSurface::Create(int width,int height,D3DFORMAT format,D3DPOOL pool)
{
	this->Release();
	if(FAILED( Sun3D.m_pd3ddev->CreateOffscreenPlainSurface(
		width,
		height,
		format,
		pool,
		&m_pd3ds,
		NULL)))
	{
		throw SunExeption("オフスクリーンサーフェースを確保できません。\n");
	}
	m_pd3ds->GetDesc(&m_d3dsd);		//サーフェース情報
	m_d3dii.Width = width;
	m_d3dii.Height = height;
	m_d3dii.Format = format;
	return 1;
}

//作成
int	SunSurface::Create(int width,int height,D3DFORMAT format)
{
	return this->Create(width,height,format,D3DPOOL_DEFAULT);
}
//空のサーフェイスを作成
int SunSurface::Create(int width,int height)
{
	//最優先はバックバッファと同じフォーマット（透過はできない仕様）
	if(!this->Create(width,height,Sun3D.m_d3dpp.BackBufferFormat))
	{
		if(Sun3D.m_bitmode == 16)
		{//16bit
			if(!this->Create(width,height,D3DFMT_R5G6B5))
				if(!this->Create(width,height,D3DFMT_X1R5G5B5))
					if(!this->Create(width,height,D3DFMT_X4R4G4B4))
						return 0;
		}
		else if(Sun3D.m_bitmode == 32)
		{//32bit
		if(!this->Create(width,height,Sun3D.m_d3dpp.BackBufferFormat))
			if(!this->Create(width,height,D3DFMT_X8R8G8B8))
				return 0;
		}
		else
		{//else
			return 0;
		}
	}
	return 1;
}
//描画（内部でGetBackBufferする）
int SunSurface::Draw(int x,int y,RECT &rcSrc)
{
	SunSurface back;
	back.GetBackBuffer();
	return back.Blit(this,x,y,rcSrc);
}
//描画（内部でGetBackBufferする）
int SunSurface::Draw(int x,int y)
{
	SunSurface back;
	back.GetBackBuffer();
	RECT rcSrc = {0,0,m_d3dsd.Width,m_d3dsd.Height};
	return back.Blit(this,x,y,rcSrc);
}
//テクスチャから作成
int SunSurface::GetTextureSurface(LPDIRECT3DTEXTURE9 pd3dtex,int width,int height)
{
	if(pd3dtex == NULL)
		return 0;
	this->Release();
	if(FAILED(pd3dtex->GetSurfaceLevel(0,&m_pd3ds)))
	{
		throw SunExeption("テクスチャサーフェースの取得に失敗\n");
	}
	m_d3dii.Width = width;
	m_d3dii.Height = height;
	m_pd3ds->GetDesc(&m_d3dsd);		//サーフェース情報
	return 1;
}
/**
	@brief 領域指定ロード
	この関数はテクスチャから使うのでサーフェースCreateはしない
	当然ロードもしない。
*/
int SunSurface::LoadRect(char* lpszFileName,RECT &rect,D3DCOLOR color)
{
	if(m_pd3ds == NULL)
		return 0;
	int width = rect.right-rect.left;
	int height = rect.bottom - rect.top;
	if(width > m_d3dsd.Width || height > m_d3dsd.Height)
		return 0;
	if(FAILED(D3DXLoadSurfaceFromFile(
		m_pd3ds,
		NULL,
		NULL,			//なぜか失敗するのでDestはNULL
		lpszFileName,
		&rect,
		D3DX_FILTER_NONE,
		color,
		NULL)))
		return 0;
	return 1;
}
/**
	@brief 領域指定ロードFromメモリー
	この関数はテクスチャから使うのでサーフェースCreateはしない
	当然ロードもしない。
*/
int SunSurface::LoadRectFromMemory(void* pBuffer,DWORD dwSize,RECT &rect,D3DCOLOR color)
{
	if(m_pd3ds == NULL)
		return 0;
	int width = rect.right-rect.left;
	int height = rect.bottom - rect.top;
	if(width > m_d3dsd.Width || height > m_d3dsd.Height)
		return 0;
	if(FAILED(D3DXLoadSurfaceFromFileInMemory(
		m_pd3ds,
		NULL,
		NULL,			//なぜか失敗するのでDestはNULL
		pBuffer,
		dwSize,
		&rect,
		D3DX_FILTER_NONE,
		color,
		NULL)))
		return 0;
	return 1;
}
/**
	@breif SunSurfaceSpriteクラスのコンストラクタ
*/
SunSurfaceSprite::SunSurfaceSprite(SunSurface* pSurface,int x,int y)
{
	m_pSurface=pSurface;
	m_x=x;m_y=y;
	SetRect(&m_rect,0,0,pSurface->m_d3dsd.Width,pSurface->m_d3dsd.Height);
}
SunSurfaceSprite::SunSurfaceSprite(SunSurface* pSurface,int x,int y,RECT rect)
{
	m_pSurface=pSurface;
	m_x=x;m_y=y;
	m_rect = rect;
}
/**
	@brief サーフェーススプライトの描画
*/
int SunSurfaceSprite::operator ()()
{
	return m_pSurface->Draw(m_x,m_y);
}
//
int SunFpsSurface::operator ()()
{
	if(SunApp.m_fpsshow > 0)
	{
		m_surf.Fill(xrgb(0,0,0));
		m_surf.Text((char*)SunApp.GetFPSString().c_str(),0,0,15,RGB(250,250,250));
		SunApp.m_fpsshow --;
	}
	m_surf.Draw(295,455);
//	m_surf.Draw(640-100,480-30,100);
	return 1;
}
//文字描画（GetDC利用ｗ
int SunSurface::Text(const char* text,int x,int y,int size,COLORREF color,const char* pFontName)
{
	if(m_pd3ds == NULL)
		return 0;
	HFONT hFont = CreateFont(
		size,0,0,0,800,
		0,
		0,
		0,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		pFontName);
	HDC hdc;
	if(FAILED(m_pd3ds->GetDC(&hdc)))
		throw SunExeption("文字描画失敗：TEXT=%s\n",text);
	SelectObject(hdc, hFont);
	SetTextColor(hdc,color);
	SetBkMode(hdc, TRANSPARENT);
	if(::TextOut(hdc,x,y,text,strlen(text))==NULL)
		throw SunExeption("文字描画失敗：TEXT=%s\n",text);
	m_pd3ds->ReleaseDC(hdc);
	if(FAILED(DeleteObject(hFont)))
		throw SunExeption("文字描画失敗：TEXT=%s\n",text);
	return 1;
}
//スナップショット保存関数
int SaveSnapShot(D3DXIMAGE_FILEFORMAT DestFormat)
{
	::ResetCurrent();
	::CreateDirectory("snapshot");
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char filename[MAX_PATH];
	char kakutyosi[5];
	switch( DestFormat ) {
	case D3DXIFF_BMP:
		strcpy( kakutyosi, ".bmp" );
		break;
	case D3DXIFF_JPG:
		strcpy( kakutyosi, ".jpg" );
		break;
	case D3DXIFF_PNG:
		strcpy( kakutyosi, ".png" );
		break;
	case D3DXIFF_TGA:
		strcpy( kakutyosi, ".tga" );
		break;
	default:
		strcpy( kakutyosi, ".xxx" );
		break;
	}
	for( int i = 0; i < 1000; ++i ) {
		sprintf( filename, "snapshot/tbm%03d%s", i, kakutyosi );
		hFind = FindFirstFile(filename, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) {
			SunSurface surf;
			surf.GetBackBuffer();
			surf.SaveSurface(filename,DestFormat,NULL);
			break;
		}
	}
	return 1;
}
//スナップショット保存関数（コンフィグからファイル形式を使う）
int SaveSnapShot()
{
	return SaveSnapShot( Sun3D.m_fileformat );
}