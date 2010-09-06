/**
	@file SunSprite.cpp
	@brief SunSprite クラスのインプリメンテーション
*/
#include "stdafx.h"
#include "SunUtility.h"
#include "SunLog.h"
#include "SunApp.h"
#include "SunConfig.h"
#include "Sun3D.h"
#include "SunTexture.h"
#include "SunSprite.h"
#include "SunExeption.h"

const DWORD VXSPRITE::FVF = ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE| D3DFVF_TEX1 );
double SunSprite::s_x = 0;
double SunSprite::s_y = 0;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
SunSprite::SunSprite(SunTexture* ptex)
{
	this->Init();
	m_ptex		= ptex;
	RECT rect = {0,0,ptex->m_d3dsd.Width,ptex->m_d3dsd.Height};
	this->CreateRect(rect);
	this->SetSourceRect(rect);
}
SunSprite::~SunSprite()
{

}
int SunSprite::Init()
{
//	m_ptex	= NULL;
	::ZeroMemory(m_vertex,sizeof(m_vertex));
	for(int i=0;i<4;++i)
	{
		m_vertex[i].c = xrgb(255,255,255);
		m_vertex[i].rhw = 1;
	}
	return 1;
}
/**
	@brief 描画
*/
int SunSprite::Draw()
{
	if(m_ptex == NULL)
		return 0;
	if(!Sun3D.SetTexture(m_ptex->m_pd3dtex))
		return 0;
	//FVF指定
	if(!Sun3D.SetFVF(VXSPRITE::FVF))	//テンプレート引数のＦＶＦ指定
		return 0;
	//いよいよ描画
	if(FAILED(Sun3D.m_pd3ddev->DrawPrimitiveUP(
										D3DPT_TRIANGLEFAN,
										2,
										&this->m_vertex[0],
										sizeof(VXSPRITE)
										)))
	{
		throw SunExeption("プリミティブ描画失敗\n");
	}
	return 1;
}
/**
	@brief 描画
*/
int SunSprite::DrawEx()
{
	Move( s_x, s_y );
	return Draw();
}
/**
	@brief テクスチャ設定
*/
int SunSprite::SetTexture(SunTexture* ptex)
{
	m_ptex = ptex;
	return 1;
}
/**
	@brief ＲＥＣＴ作成
*/
int SunSprite::CreateRect(RECT &rect)
{
	//左上
	m_vertex[0].x = -0.5 + rect.left;
	m_vertex[0].y = -0.5 + rect.top;
	//右上
	m_vertex[1].x = -0.5 + rect.right;
	m_vertex[1].y = -0.5 + rect.top;
	//右下
	m_vertex[2].x = -0.5 + rect.right;
	m_vertex[2].y = -0.5 + rect.bottom;
	//左下
	m_vertex[3].x = -0.5 + rect.left;
	m_vertex[3].y = -0.5 + rect.bottom;
	return 1;
}
/**
	@brief ＲＥＣＴ作成
*/
int SunSprite::CreateRectEx(float x,float y,float w,float h)
{
	//左上
	m_vertex[0].x = -0.5 + x;
	m_vertex[0].y = -0.5 + y;
	//右上
	m_vertex[1].x = -0.5 + x + w;;
	m_vertex[1].y = -0.5 + y;
	//右下
	m_vertex[2].x = -0.5 + x + w;
	m_vertex[2].y = -0.5 + y + h;
	//左下
	m_vertex[3].x = -0.5 + x;
	m_vertex[3].y = -0.5 + y + h;
	return 1;
}
/**
	@brief 頂点色の設定
*/
int SunSprite::SetColor(D3DCOLOR color)
{
	for(int i=0;i<4;++i)
		m_vertex[i].c = color;
	return 1;
}
/**
	@brief 頂点色の設定
*/
int SunSprite::SetGradationColor(D3DCOLOR under,D3DCOLOR top)
{
	m_vertex[0].c = top;
	m_vertex[1].c = top;
	m_vertex[2].c = under;
	m_vertex[3].c = under;
	return 1;
}
/**
	@brief テクスチャ領域設定

	いわゆるＵＶ座標の設定です。
	テクスチャがセットされている必要があります。
*/
int SunSprite::SetSourceRect(RECT &rcSrc)
{
	//テクスチャがセットされていなければ失敗
	if(m_ptex == NULL)
		return 0;
	//情報
	float width		= m_ptex->m_d3dsd.Width;
	float height	= m_ptex->m_d3dsd.Height;
	//UV設定
	//---左上
	m_vertex[0].u = (float)rcSrc.left	/ width;
	m_vertex[0].v = (float)rcSrc.top	/ height;
	//---右上
	m_vertex[1].u = (float)rcSrc.right	/ width;
	m_vertex[1].v = (float)rcSrc.top	/ height;
	//---右下
	m_vertex[2].u = (float)rcSrc.right	/ width;
	m_vertex[2].v = (float)rcSrc.bottom	/ height;
	//---左下
	m_vertex[3].u = (float)rcSrc.left	/ width;
	m_vertex[3].v = (float)rcSrc.bottom	/ height;
	return 1;
}
/**
	@brief テクスチャ繰り返し設定

	画像全体を使うことを前提に、繰り返しテクスチャを設定します。
	テクスチャがセットされている必要があります。
*/
int SunSprite::SetRepeat(float rx,float ry)
{
	//テクスチャがセットされていなければ失敗
	if(m_ptex == NULL)
		return 0;
	//UV設定
	//---左上
	m_vertex[0].u = 0;
	m_vertex[0].v = 0;
	//---右上
	m_vertex[1].u = rx;
	m_vertex[1].v = 0;
	//---右下
	m_vertex[2].u = rx;
	m_vertex[2].v = ry;
	//---左下
	m_vertex[3].u = 0;
	m_vertex[3].v = ry;
	return 1;
}

/**
	@brief Ｚ軸回転

	中心点は絶対座標です。
*/
int SunSprite::Rotate(float a,float x,float y)
{
	for(int i=0;i<4;++i)
	{
		float dx = m_vertex[i].x - x;
		float dy = m_vertex[i].y - y;
		float d = sqrt(dx*dx + dy*dy);	//距離
		float ang = atan2(dy,dx);		//偏角
		m_vertex[i].x = x + d * cos(a + ang);
		m_vertex[i].y = y + d * sin(a + ang);
	}
	return 1;
}
/**
	@brief 拡大縮小

	中心点は絶対座標です。
*/
int SunSprite::Scale(float mx,float my,float x,float y)
{
	for(int i=0;i<4;++i)
	{
		float lx = m_vertex[i].x - x;
		float ly = m_vertex[i].y - y;
		m_vertex[i].x = x + lx * mx;
		m_vertex[i].y = y + ly * my;
	}
	return 1;
}
/**
	@brief 平行移動
*/
int SunSprite::Move(float x,float y)
{
	for(int i=0;i<4;++i)
	{
		m_vertex[i].x += x;
		m_vertex[i].y += y;
	}
	return 1;
}