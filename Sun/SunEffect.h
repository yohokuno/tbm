
#pragma once

#include "SunTask.h"
/**
	@brief 単純な２Ｄエフェクト用頂点構造
*/
typedef struct VXEFFECT
{
	float x, y, z, w;
	D3DCOLOR c;
	//頂点フォーマットフラグ
	static const DWORD FVF;
} VXEFFECT;
/**
	@brief エフェクトクラス

	単純な頂点構造体のみをメンバに持つクラス。
	基本的にこれ自体を使わず、ラッパーを使う。

    @todo	------------------------
	追加したい形状がたくさん。
	３Ｄリング（防御エフェクト）とか、レーザー系、放射状ポリゴンとか。
	飛散ポリゴンとか、やり方すらわからない雷系エフェクトとか。
*/
class SunEffect
{
public:
	vector<VXEFFECT>	m_vertex;		///<頂点構造体
	D3DPRIMITIVETYPE	m_primType;		///<描画タイプ
public:
	SunEffect();
	virtual ~SunEffect();
	int Resize(int size);		//サイズ変更
	int SetColor(D3DCOLOR color);
	//描画関連
	int DrawBase(D3DPRIMITIVETYPE PrimitiveType, int PrimitiveCount, int offset);	///<描画数指定描画
	int DrawBase2(D3DPRIMITIVETYPE PrimitiveType, int vertexnum, int offset);		///<描画数指定描画
	int DrawBase(D3DPRIMITIVETYPE PrimitiveType);			///<全て描画
	int Draw();
	//頂点変換関連
	int Move(float x,float y);						//平行移動
	int Rotate(float r,float x,float y);			//Ｚ軸回転
	int Scale(float dx,float dy,float x,float y);	//拡大縮小
	int RotateY(float r,float x);			//Ｙ軸回転
	//頂点生成関連
	int CreateRectBase(RECT& rect,D3DCOLOR color,int offset=0,int interval=1);
	int CreateRegularBase(int x,int y,int n,int r,D3DCOLOR color,int offset=0,int interval=1);		///<正多角形（オフセット＋間隔指定）
	int CreateRegularBase(int x,int y,int z,int n,int r,D3DCOLOR color,int offset,int interval);		///<正多角形
	int CreateParabolaBase(int x,int y,int n,float a,int lastx,D3DCOLOR color,int offset=0,int interval=1);

	void SetType(D3DPRIMITIVETYPE type){m_primType=type;}
};
/**
	@brief リングエフェクト

	もっとも汎用性が高いと思われる、リングのエフェクト。
*/
class SunEffectRing
{
public:
	int m_x,m_y;		//中心座標
	int m_inRadius;		//内側半径
	int m_outRadius;	//外側半径
	DWORD m_inColor;	//内色（デフォルトは透明）
	DWORD m_outColor;	//外色（デフォルトは白）
	int m_num;			//頂点数
public:
	SunEffectRing();
	int Draw();
};
/**
	@brief Ｚリングエフェクト

	ガードエフェクト専用ｗ
*/
class SunEffectZRing
{
public:
	int m_x,m_y;		//中心座標
	int m_z;			//外側の奥行き
	float m_angle;		//Ｙ軸回転の角度
	int m_inRadius;		//内側半径
	int m_outRadius;	//外側半径
	DWORD m_inColor;	//内色（デフォルトは透明）
	DWORD m_outColor;	//外色（デフォルトは白）
	int m_num;			//頂点数
public:
	SunEffectZRing();
	int Draw();
};
/**
	@brief 放物線レーザーエフェクト

	永遠の宵闇にマスタースパーク！！
	ちなみに「放物線」の英単語はParabola
	単色なので実際には２のほうを使うか？
*/
class SunEffectLaser
{
public:
	int m_x,m_y;		//中心座標
	float m_a;			//幅（x=a*y*y;)
	int m_num;			//
	int m_lastx;		//最大Ｘ座標（画面外なら640か0）
	DWORD m_color;		//色（デフォルトは白）
	float m_angle;		//Ｚ軸回転核（デフォルト０）
public:
	SunEffectLaser();
	int Draw();
};

/**
	@brief 放物線レーザーエフェクト

	永遠の宵闇にマスタースパーク！！
	ちなみに「放物線」の英単語はParabola
	内殻と外殻をもつ。
*/
class SunEffectLaser2
{
public:
	int m_x,m_y;		//中心座標
	float m_ina;			//内側係数（x=a*y*y;)
	float m_outa;			//外側係数
	int m_num;			//
	int m_lastx;		//最大Ｘ座標（画面外なら640か0）
	DWORD m_inColor;	//色（デフォルトは白）
	DWORD m_outColor;	//色（デフォルトは透明）
	float m_angle;		//Ｚ軸回転核（デフォルト０）
public:
	SunEffectLaser2();
	int Draw();
};
/**
	@breaf 円形ワイヤーフレーム
*/
class SunEffectCircle
{
public:
	int m_x,m_y;		//中心座標
	int m_radius;		//内側半径
	DWORD m_color;		//内色（デフォルトは透明）
	int m_num;			//頂点数
public:
	SunEffectCircle();
	int Draw();
};
/**
	@brief レクトエフェクト

	画面全体をフラッシュするのに使う、RECTエフェクト。
*/
class SunEffectRect
{
public:
	RECT m_rect;
	DWORD m_color;	//色
public:
	SunEffectRect();
	int Draw();
};
/**
	@brief 直線レーザーエフェクト
*/
class SunEffectLineLaser
{
public:
	float m_sx,m_sy;	///< 始点
	float m_ex,m_ey;	///< 終点
	float m_iw;			///< 内幅
	float m_ow;			///< 外幅
	DWORD m_inColor;	///< 内色
	DWORD m_outColor;	///< 外色
public:
	SunEffectLineLaser();
	int Draw();
};
/**
	@brief ゲージエフェクト

	ボスのライフ表示などに使うゲージエフェクト。
*/
class SunEffectGauge
{
public:
	RECT	m_rect;		//全体の領域
	double	m_point;	//０～１の範囲で、ゲージの量
	DWORD	m_upColor;	//上の色
	DWORD	m_downColor;	//下の色
	int		m_yoko;		//タテかヨコか
public:
	SunEffectGauge();
	int Draw();
};
/**
	@brief 鳥目エフェクト

	ミスティアしか使わん（笑）、鳥目エフェクト。
*/
class SunEffectTorime
{
public:
	int m_x,m_y;		//中心座標（＝自機座標を代入）
	int m_inRadius;		//内円半径
	int m_outRadius;		//外円半径
	DWORD m_inColor;		//色（デフォルトは黒）
	DWORD m_outColor;		//色（デフォルトは黒）
	int m_num;			//内円の頂点数
public:
	SunEffectTorime();
	int Draw();
};
