#pragma once

//チルノ共通
struct CBossChirnoBase : public CBossBase
{
	CBossChirnoBase();
	virtual int Draw();
	virtual int SpellCard();
	int DontCome();
	int NormalAtack1();
};
//中ボス
struct CBossChirnoMiddle : public CBossChirnoBase
{
	CBossChirnoMiddle();
	virtual int Run();
	virtual int Draw();
	int Thread();
	int FrogRefrigerator();
};
//3面ボス
struct CBossChirno : public CBossChirnoBase
{
	CBossChirno();
	virtual int Run();
	virtual int Draw();
	int Thread();
	int NormalAtack2();
	int Sesi();
	int IceSword();
	int Frost();
	int Flower();
	int Freeze();
	int Cristal();
	int Koch();
	int KochRecursion( int n );
	int KochDanmaku( int n, double v, double a1, double a2, double t );
	list<double> lKoch;
};

//6面中ボスチルノ
struct CBossChirnoMiddle2 : public CBossBase
{
	SunTexture imgChirno;
	SunTexture imgBack1;
	SunTexture imgBack2;
	SunTexture imgTalk;

	CBossChirnoMiddle2();
	virtual ~CBossChirnoMiddle2();
	virtual int Draw();
	virtual int SpellCard();
	virtual int Run();
	int Thread();
	int NormalAtack();
	int Kawazu();
};
extern CBossChirnoMiddle2 *g_pChirno2;