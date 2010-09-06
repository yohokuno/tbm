#pragma once

//ステージ5
struct CStage5 : public CStage
{
	SunTexture imgChen;
	SunTexture imgBack1;
	SunTexture imgBack3;
	SunTexture imgTalk;
	CStage5();
	virtual ~CStage5();
	virtual int Run();
	virtual int EnemyApear();
};
extern CStage5 *g_pStage5;
//妖精18（行列弾）
struct CEnemyFairy18 : public CEnemyHoodBase
{
	int t;
	CEnemyFairy18(double x,double y,int t);
	virtual int Run();
	virtual int GetItem();
};
//妖精19（行列弾）
struct CEnemyFairy19 : public CEnemyHoodBase
{
	CEnemyFairy19(double x,double y);
	virtual int Run();
	virtual int GetItem();
};
//妖精20（妖気）
struct CEnemyFairy20 : public CEnemyFairyBase
{
	double v,t;
	CEnemyFairy20(double x,double y,double t);
	virtual int Run();
	virtual int GetItem();
};
//妖精21（うずまき弾）
struct CEnemyFairy21 : public CEnemyFairyBase
{
	int t;
	CEnemyFairy21(double x,double y,int t);
	virtual int Run();
	virtual int GetItem();
};
//妖精22（未使用）
struct CEnemyFairy22 : public CEnemyFairyBase
{
	double c,t;
	CEnemyFairy22(double x,double y,double c,double t);
	virtual int Run();
	virtual int GetItem();
};

//妖精23（水しぶき）
struct CEnemyFairy23 : public CEnemyFairyBase
{
	double c,t;
	CEnemyFairy23(double x,double y,double c,double t);
	virtual int Run();
	virtual int GetItem();
};

//毛玉5（うずまき）
struct CEnemyKedama5 : public CEnemyKedamaBase
{
	double c,t;
	CEnemyKedama5(double x,double y,double c,double t);
	virtual int Run();
	virtual int GetItem();
};
//七福神
struct CEnemy7God : public CEnemyHoodBase
{
	int c;
	CEnemy7God(double x,double y,int c);
	virtual int Run();
	virtual int GetItem();
};