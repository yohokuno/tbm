#pragma once

#define STAGE_NUM 6

const D3DCOLOR stageFontColor1 = xrgb(255,120,200);
const D3DCOLOR stageFontColor2 = xrgb(180,140,255);
const D3DCOLOR bgmFontColor = xrgb(200,50,255);

//インターフェース＋基本機能
struct CStage : public SunObject
{
	SunMicroThreadEx	mt;
	int count;

	CStage();
	virtual ~CStage();
	virtual int Run();
	virtual int Draw();
	virtual int EnemyApear() = 0;
	void Stop();
	void Resume();
};
extern CStage *g_pStage;

//敵インターフェース
struct CEnemy : public SunObject
{
	CEnemy(){}
	virtual ~CEnemy(){}
	virtual int Run() = 0;
	virtual int Draw() = 0;
};

extern SunList<CEnemy> g_lEnemy;

//ザコ敵共通
struct CEnemyBase : public CEnemy
{
	int count;
	double hp;
	double x,y;
	CEnemyBase();
	virtual ~CEnemyBase();
	virtual int Run();
	virtual int Draw() = 0;
	virtual int GetItem() = 0;	///< アイテムの種類
	virtual double GetW() = 0;
	virtual double GetH() = 0;
	virtual int Damage( double damage );	///<ダメージをくらう
	virtual int Death();	///< HPがゼロになったとき
	virtual int Hantei();	///< 自機との当たり判定
	double GetX(){return x + GetW()/2;}
	double GetY(){return y + GetH()/2;}
	double GetSize(){return (GetW()+GetH())/4;}
	int Run2();
};

//妖精共通
struct CEnemyFairyBase : public CEnemyBase
{
	int color;
	virtual int Draw();
	virtual int GetItem();
	virtual double GetW(){return 48;}
	virtual double GetH(){return 48;}
};

//毛玉共通
struct CEnemyKedamaBase : public CEnemyBase
{
	int color;
	virtual int Draw();
	virtual int GetItem();
	virtual double GetW(){return 32;}
	virtual double GetH(){return 32;}
};

//フード共通
struct CEnemyHoodBase : public CEnemyBase
{
	int color;
	virtual int Draw();
	virtual int GetItem();
	virtual double GetW(){return 48;}
	virtual double GetH(){return 48;}
};