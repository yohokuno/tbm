#pragma once

//žò‹¤’Ê
struct CBossChenBase : public CBossBase
{
	CBossChenBase();
	virtual int Draw();
	virtual int SpellCard();
	int NormalAtack1();
};
//’†ƒ{ƒX
struct CBossChenMiddle : public CBossChenBase
{
	CBossChenMiddle();
	virtual int Run();
	virtual int Draw();
	int Thread();
	int Taikyoku();
};
struct CBossChen : public CBossChenBase
{
	CBossChen();
	virtual int Run();
	virtual int Draw();
	int Thread();
	int Num3();
	int Num4();
	int Num5();
	int Num6();
	int Num7();
	int Num8();
	int Magata();
	int Kagome();
};
