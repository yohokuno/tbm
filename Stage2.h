#pragma once

//ƒXƒe[ƒW2
struct CStage2 : public CStage
{
	SunTexture imgWriggle;
	SunTexture imgBack1;
	SunTexture imgBack2;
	SunTexture imgCutin;
	SunTexture	imgTalk;
	CStage2();
	virtual ~CStage2();
	virtual int Run();
	virtual int EnemyApear();
};
extern CStage2 *g_pStage2;
//—d¸5i“ñŸŠÖ”E©‹@‘_‚¢’ej
struct CEnemyFairy5 : public CEnemyFairyBase
{
	int c,t;
	double vx,vy;
	CEnemyFairy5(double x,double y,int c,int t);
	virtual int Run();
	virtual int GetItem();
};

//—d¸6i‹üÜ©‹@‘_‚¢’ej
struct CEnemyFairy6 : public CEnemyFairyBase
{
	int t;
	CEnemyFairy6(double x,double y,int t);
	virtual int Run();
	virtual int GetItem();
};
//—d¸7(‰ñ“]ˆÚ“®j
struct CEnemyFairy7 : public CEnemyFairyBase
{
	double tx,ty;
	int t;
	double a;
	CEnemyFairy7(double tx,double ty,double a,int t);
	virtual int Run();
	virtual int GetItem();
};

//—d¸8(‰ò©‹@‘_‚¢’ej
struct CEnemyFairy8 : public CEnemyFairyBase
{
	CEnemyFairy8(double x,double y);
	virtual int Run();
	virtual int GetItem();
};

//–Ñ‹Ê2iˆê’â~j
struct CEnemyKedama2 : public CEnemyKedamaBase
{
	int c;
	CEnemyKedama2(double x,double y,int c);
	virtual int Run();
	virtual int Death();
};

//–Ñ‹Ê3iOŸŠÖ”j
struct CEnemyKedama3 : public CEnemyKedamaBase
{
	int c;
	CEnemyKedama3(double x,double y,int c);
	virtual int Run();
	virtual int Death();
};