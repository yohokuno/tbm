#pragma once

//ƒXƒe[ƒW4
struct CStage4 : public CStage
{
	SunTexture imgMystia;
	SunTexture imgBack1;
	SunTexture imgBack2;
	SunTexture imgCutin;
	SunTexture imgTalk;
	CStage4();
	virtual ~CStage4();
	virtual int Run();
	virtual int EnemyApear();
};
extern CStage4 *g_pStage4;
//—d¸14ic‰¡‘Å‚¿•Ô‚µ’ej
struct CEnemyFairy14 : public CEnemyHoodBase
{
	CEnemyFairy14(double x,double y);
	virtual int Run();
	virtual int GetItem();
	virtual int Death();
};

//—d¸15iã‰ºŠú‘_‚¢’ej
struct CEnemyFairy15 : public CEnemyFairyBase
{
	int c;
	CEnemyFairy15(double x,double y,int c);
	virtual int Run();
	virtual int GetItem();
};
//—d¸16i‘Å‚¿•Ô‚µ©‹@‘_‚¢3Way’ej
struct CEnemyFairy16 : public CEnemyHoodBase
{
	CEnemyFairy16(double x,double y);
	virtual int Run();
	virtual int GetItem();
	virtual int Death();
};
//—d¸17iüŒ^’ej
struct CEnemyFairy17 : public CEnemyFairyBase
{
	CEnemyFairy17(double x,double y);
	virtual int Run();
	virtual int GetItem();
	virtual int Death();
};