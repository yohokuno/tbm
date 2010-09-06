#pragma once

//ルーミア共通
struct CBossRumiaBase : public CBossBase
{
	SunEffectRing		circle;

	CBossRumiaBase();
	virtual int Draw();
	virtual int SpellCard();
	int DrawEffect();			//闇エフェクト
	int DontCome();	//近付き防止自機狙い弾
};
//中ボス
struct CBossRumiaMiddle : public CBossRumiaBase
{
	CBossRumiaMiddle();
	virtual int Run();
	virtual int Draw();
	int Thread();
	int NormalAtack1();
};
struct CBossRumia : public CBossRumiaBase
{
	CBossRumia();
	virtual int Run();
	virtual int Draw();
	int Thread();

	int NormalAtack2();
	int NightBird();
	int NormalAtackBase(CTamaNormal *pTama, int j); //片翼型弾幕
	int Demarcation();
	int MoonLightRay();
	int NonDirectional();
};
