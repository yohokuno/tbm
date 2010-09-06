#pragma once

//ƒXƒe[ƒW1
struct CStage1 : public CStage
{
	SunTexture	imgRumia;
	SunTexture	imgYuki;
	SunTexture	imgBack1;
	SunTexture	imgBack2;
	SunTexture	imgCutin;
	CStage1();
	virtual ~CStage1();
	virtual int Run();
	virtual int EnemyApear();
};
extern CStage1 *g_pStage1;

//—d¸1(UŒ‚‚È‚µj
struct CEnemyFairy1 : public CEnemyFairyBase
{
	double v,s;
	CEnemyFairy1(double x,double y);
	virtual int Run();
};

//—d¸2i©‹@‘_‚¢’ej
struct CEnemyFairy2 : public CEnemyFairyBase
{
	int c;
	CEnemyFairy2(double x,double y);
	virtual int Run();
	virtual int GetItem();
};

//—d¸3i‘S•ûˆÊ’ej
struct CEnemyFairy3 : public CEnemyFairyBase
{
	CEnemyFairy3(double x,double y);
	virtual int Run();
	virtual int GetItem();
};

//—d¸4i‹ô”’ej
struct CEnemyFairy4 : public CEnemyFairyBase
{
	int c;
	CEnemyFairy4(double x,double y,int c);
	virtual int Run();
	virtual int GetItem();
};

//–Ñ‹Ê1
struct CEnemyKedama1 : public CEnemyKedamaBase
{
	int c;
	CEnemyKedama1(double x,double y,int c);
	virtual int Run();
	virtual int Death();
};