#pragma once

//ƒŒƒeƒB‹¤’Ê
struct CBossLetyBase : public CBossBase
{
	int barier;
	CBossLetyBase();
	virtual int Draw();
	virtual int SpellCard();
	void Suspend2(double t);
};
struct CBossLety : public CBossLetyBase
{
	int freeze;
	CBossLety();
	virtual ~CBossLety();
	virtual int Run();
	virtual int Draw();
	int Thread();
	int NormalAtack1();
	int NormalAtack2();
	int WhiteAlbum();
	int AbsoluteZero();
	int Typhoon();
	int Hurricane();
	int Tsunami();
	int EarthQuake();
	int OzoneHole();
	int SeptemberEleven();
	int GlobalWorming();
	int NuclearWinter();
};

extern CBossLety *g_pLety;