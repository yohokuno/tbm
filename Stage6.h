#pragma once

//ステージ6
struct CStage6 : public CStage
{
	SunTexture imgLety;
	SunTexture imgEarth;
	SunTexture imgEarthFreeze;
	SunTexture imgBack;
	SunTexture imgTalk;
	SunTexture	imgBarier;
	CStage6();
	virtual ~CStage6();
	virtual int Run();
	virtual int EnemyApear();
};
extern CStage6 *g_pStage6;

//毛玉6（自機狙い）
struct CEnemyKedama6 : public CEnemyKedamaBase
{
	double vy;
	CEnemyKedama6(double x,double y,double vy);
	virtual int Run();
	virtual int GetItem();
};