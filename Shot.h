#pragma once

//インターフェース
struct CShot : public SunObject
{
	double x,y;
	virtual int Run() = 0;
	virtual int Draw() = 0;
	virtual void GetHantei(RECT *pRect) = 0;
	virtual double GetDamage() = 0;
	virtual double GetH() = 0;
};

//ミサイル
struct CMainShot : public CShot
{
	double v;
	CMainShot(double x,double y);
	virtual ~CMainShot();
	virtual int Run();
	virtual int Draw();
	virtual void GetHantei(RECT *pRect);
	virtual double GetDamage();
	virtual double GetH();
};

//ショット
struct CSubShot : public CShot
{
	double s;

	CSubShot(double x,double y,double s);
	virtual ~CSubShot();
	virtual int Run();
	virtual int Draw();
	virtual void GetHantei(RECT *pRect);
	virtual double GetDamage();
	double GetH();
};

extern SunList<CShot> g_lShot;