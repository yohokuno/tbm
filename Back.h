#pragma once

//背景インターフェース
struct CBack : public SunObject
{
	virtual int Run() = 0;
	virtual int Draw() = 0;
};
extern CBack *g_pBack;

//ステージ背景(3D)共通
struct CBackStageBase : public CBack
{
	int	count;
	CBackStageBase();
};

//ステージ1
struct CBackStage1 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunMicroThreadEx	mt;
	CBackStage1();
	virtual int Run();
	virtual int Draw();
	int Thread();
};

//ルーミアスペル
struct CBackRumia : public CBack
{
	int	count;
	CBackRumia();
	virtual int Run();
	virtual int Draw();
};


//ステージ2
struct CBackStage2 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunTexture	imgMori2;
	SunTexture	imgTree;
	SunTexture	imgGrass;

	SunMicroThreadEx	mt;
	CBackStage2();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
//リグルスペル
struct CBackWriggle : public CBack
{
	int	count;
	CBackWriggle();
	virtual int Run();
	virtual int Draw();
};

//ステージ3
struct CBackStage3 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunTexture	imgYuki;

	SunMicroThreadEx	mt;
	CBackStage3();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
//チルノスペル
struct CBackChirno : public CBack
{
	int	count;
	CBackChirno();
	virtual int Run();
	virtual int Draw();
};

//ステージ4
struct CBackStage4 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunTexture	imgBack;
	SunTexture	imgMokume;

	CBackStage4();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
//ミスティアスペル
struct CBackMystia : public CBack
{
	int	count;
	CBackMystia();
	virtual int Run();
	virtual int Draw();
};
//ステージ5
struct CBackStage5 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunTexture	imgBack;
	SunTexture	imgSyoji;

	CBackStage5();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
//橙スペル
struct CBackChen : public CBack
{
	int	count;
	CBackChen();
	virtual int Run();
	virtual int Draw();
};

//ステージ6
struct CBackStage6 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunTexture	imgYuki;
	SunTexture	imgHyoga;

	CBackStage6();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
//レティスペル
struct CBackLety : public CBack
{
	int	count;
	CBackLety();
	virtual int Run();
	virtual int Draw();
};

//チルノスペル2
struct CBackChirno2 : public CBack
{
	int	count;
	CBackChirno2();
	virtual int Run();
	virtual int Draw();
};