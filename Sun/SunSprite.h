/**
	@file SunSprite.h
	@brief SunSprite クラスのインターフェイス
*/

#pragma once

#include "SunTexture.h"
/**
	@brief 単純な２Ｄスプライト用頂点構造
*/
typedef struct VXSPRITE
{
	float x, y, z, rhw;
	D3DCOLOR c;
	float u, v;
	//頂点フォーマットフラグ
	static const DWORD FVF;
} VXSPRITE;

/**
	@brief スプライト基礎クラス

	内部に頂点構造体（４頂点）とSunTextureポインタを持つ。
	ポインタはスマートポインタである。
	実際にはこれ自体ではなく分割テクスチャ用のスプライトを使用する。
*/

class SunSprite
{
private:
	//データメンバ
	VXSPRITE		m_vertex[4];
	SunTexture*		m_ptex;		///<テクスチャポインタ
	static double	s_x, s_y;
public:
	SunSprite(SunTexture* ptex);
	virtual ~SunSprite();
	int Draw();										///<描画
	int DrawEx();									///<描画(画面揺れ対応)
	int SetTexture(SunTexture* ptex);				///<テクスチャ設定
	int CreateRect(RECT &rect);						///<描画先領域設定
	int CreateRectEx(float x,float y,float w,float h );			///<描画先領域設定（小数対応）
	int SetSourceRect(RECT &rcSrc);					///<描画元領域設定
	int SetRepeat(float rx,float ry);				///<描画元繰り返し数
	int SetColor(D3DCOLOR color);					///<ディヒューズ色設定
	int SetGradationColor(D3DCOLOR under,D3DCOLOR top);	///<グラデーション色設定
	int Move(float x,float y);						///<平行移動
	int Rotate(float a,float x,float y);			///<Ｚ軸回転
	int Scale(float mx,float my,float x,float y);	///<拡大縮小
	static void SetOffset(float x,float y){s_x=x;s_y=y;}
//内部使用関数
	int Init();
};
