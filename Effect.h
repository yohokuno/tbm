#pragma once

//エフェクトのインターフェース
struct CEffect : public SunObject
{
	CEffect(){};
	virtual ~CEffect(){};
	virtual int Run() = 0;
	virtual int Draw() = 0;
};

extern SunList<CEffect> g_lEffect;	//下位レイヤー（加算）
extern SunList<CEffect> g_lEffect2;	//上位レイヤー（加算）
extern SunList<CEffect> g_lEffect3;	//上位レイヤー（通常）

//画面全体でないエフェクトの基礎クラス
struct CEffectBase : public CEffect
{
	int count;
	CEffectBase();
	virtual int Run();
	virtual int Draw() = 0;
};

//リングエフェクト（ザコを倒したとき）
struct CEffectRing : public CEffectBase
{
	SunEffectRing effect;
	CEffectRing(double x,double y);
	virtual int Run();
	virtual int Draw();
};

//ダメージエフェクト
struct CEffectDamage : public CEffectBase
{
	SunEffectRing effect;
	CEffectDamage(double x,double y);
	virtual int Run();
	virtual int Draw();
};


//カットインエフェクト
struct CEffectCutin : public CEffectBase
{
	SunTexture	*pTex;
	RECT		rect;
	double		x,y;
	CEffectCutin(SunTexture *p,RECT *pRect,double x,double y);
	virtual int Run();
	virtual int Draw();
};

//フォントエフェクト
struct CEffectFont : public CEffectBase
{
	int c1,c2,c3;
	double x,y;
	double vx,vy;
	SunFont	font;
	D3DCOLOR color;
	int a;
	CEffectFont(char *text,D3DCOLOR color,double x,double y,double vx=-2,double vy=0,int c1=30,int c2=120,int c3=30);
	virtual int Run();
	virtual int Draw();
};

//グレイズエフェクト
struct CEffectGraze : public CEffectBase
{
	double x,y;
	double a,v;
	CEffectGraze(double x,double y);
	virtual int Run();
	virtual int Draw();
};

//スターエフェクト
struct CEffectStar : public CEffectBase
{
	double x,y;
	double a,v;
	CEffectStar(double x,double y);
	virtual int Run();
	virtual int Draw();
};

//ヒットエフェクト
struct CEffectHit : public CEffectBase
{
	double x,y;
	double a,v;
	CEffectHit(double x,double y);
	virtual int Run();
	virtual int Draw();
};

//ホリックエフェクト
struct CEffectHolic : public CEffectBase
{
	double x,y;
	CEffectHolic(double x,double y);
	virtual int Run();
	virtual int Draw();
};

//ボス破壊エフェクト
struct CEffectBreak : public CEffectBase
{
	double x,y;
	CEffectBreak(double x,double y);
	virtual int Run();
	virtual int Draw();
};
//鳥目エフェクト
struct CEffectTorime : public CEffectBase
{
	double x,y;
	CEffectTorime();
	virtual int Run();
	virtual int Draw();
};
