/**
	@file SunSprite3D.h
	@brief SunSprite3D クラスのインターフェイス
*/

#pragma once

#include "SunTexture.h"
/**
	@brief 3Dスプライト用頂点構造
*/
typedef struct VXSPRITE3D
{
	float x, y, z;
	float nx, ny, nz;
	D3DCOLOR c;
	float u, v;
	//頂点フォーマットフラグ
	static const DWORD FVF;
} VXSPRITE3D;

/**
	@brief 3Dスプライトクラス

	内部に頂点構造体（４頂点）とSunTextureポインタを持つ。
*/

class SunSprite3D
{
public:
	//データメンバ
	VXSPRITE3D		m_vertex[4];
	SunTexture*		m_ptex;		///<テクスチャポインタ
public:
	SunSprite3D(SunTexture* ptex);
	virtual ~SunSprite3D();
	int Draw();										///<描画
	int SetTexture(SunTexture* ptex);				///<テクスチャ設定
	int CreateRectXY(RECT &rect,float z);			///<XY平面に平行な矩形設定
	int CreateRectYZ(RECT &rect,float x);			///<YZ平面に平行な矩形設定
	int CreateRectZX(RECT &rect,float y);			///<ZX平面に平行な矩形設定
	int SetSourceRect(RECT &rcSrc);					///<描画元領域設定
	int SetRepeat(float rx,float ry);				///<描画元繰り返し数
	int SetColor(D3DCOLOR color);					///<ディヒューズ色設定
	int SetGradationColor(D3DCOLOR under,D3DCOLOR top);	///<グラデーション色設定
	int Move(float x,float y,float z);				///<平行移動
	int Rotate(float a,float x,float y);			///<Ｚ軸回転
	int Scale(float mx,float my,float x,float y);	///<拡大縮小
//内部使用関数
	int Init();
};
