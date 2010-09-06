/**
	@file SunSurface.h
	@brief サーフェース
*/
#pragma once

#include "SunTask.h"
/**
	@class SunSurface
	@brief サーフェース
	
	あまり使う機会はありませんが、サーフェースクラスです。
	アルファつきで作成するとＤＣを使ったテキスト描画ができません。
	テクスチャから取得できるサーフェースはアルファつきです。
	普通に作ると透過ができないかわりにテクスチャサイズの制限がないというメリットがいちおうあります。
*/
class SunSurface  
{
	friend class SunSurfaceSprite;
public:
	LPDIRECT3DSURFACE9	m_pd3ds;
	D3DXIMAGE_INFO		m_d3dii;	//オリジナルイメージ情報
	D3DSURFACE_DESC		m_d3dsd;	//テクスチャ情報
public:
	SunSurface();
	SunSurface(char* pFile);
	virtual ~SunSurface();
	int Release();
	int GetBackBuffer();
	int Load(char* lpszFileName);
	int LoadFromPack(char* lpszFileName);
	int	Create(int width,int height);
	int	Create(int width,int height,D3DFORMAT format);
	int	Create(int width,int height,D3DFORMAT format,D3DPOOL pool);
		//短形転送。転送先は自分自身。バックバッファなら描画になる。
		//アルファ値つきで作成した場合は失敗することに注意。
		int Blit(LPDIRECT3DSURFACE9 pd3ds,int x,int y,RECT &rcSrc);
		int Blit(SunSurface *ps,int x,int y,RECT &rcSrc);
		int Blit(SunSurface *ps,int x,int y);
		int Draw(int x,int y,RECT &rcSrc);		///<内部でバックバッファを取得して転送
		int Draw(int x,int y);
		void Draw(int x,int y,int layer);
		void Draw(int x,int y,RECT &rcSrc,int layer);
		//フォント描画
		int Text(const char* text,int x,int y,int size,COLORREF color,const char* pFontName="ＭＳ ゴシック");
		//塗りつぶし
		int Fill( D3DCOLOR color, RECT &rect);
		int Fill( D3DCOLOR color);
		LPDIRECT3DSURFACE9 GetSurface(){return m_pd3ds;}
	//この関数で作るとアルファつきになることに注意。
	int GetTextureSurface(LPDIRECT3DTEXTURE9 pd3dtex,int width,int height);
	//この関数はテクスチャから使うのでサーフェースCreateはしない
	int LoadRect(char* lpszFileName,RECT &rect,D3DCOLOR color);
	int LoadRectFromMemory(void* pBuffer,DWORD dwSize,RECT &rect,D3DCOLOR color);
	//スナップショット用
	int SaveSurface(LPCTSTR pDestFile,D3DXIMAGE_FILEFORMAT DestFormat,RECT *pSrcRect);
};
/**
	サーフェーススプライトクラス

	描画キュー対応のスプライトです。
*/
class SunSurfaceSprite : public SunTask
{
private:
	SunSurface* m_pSurface;
	int m_x,m_y;
	RECT m_rect;
public:
	SunSurfaceSprite(SunSurface* pSurface,int x,int y);
	SunSurfaceSprite(SunSurface* pSurface,int x,int y,RECT rect);
	virtual int operator()();
};

//FPS表示用サーフェース
class SunFpsSurface : public SunTask
{
private:
	SunSurface m_surf;
public:
	SunFpsSurface(){}
	virtual ~SunFpsSurface(){};
	virtual int operator()();
	void Create(){m_surf.Create(48,15);}
};
extern SunFpsSurface SunFpsShow;

//スナップショット保存関数
int SaveSnapShot(D3DXIMAGE_FILEFORMAT DestFormat);
int SaveSnapShot();