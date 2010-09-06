#pragma once

//ミスティア共通
struct CBossMystiaBase : public CBossBase
{
	SunEffectTorime		torime;
	int	yami;
	int yami_r;
	int yami_type;

	CBossMystiaBase();
	virtual int Run();
	virtual int Draw();
	virtual int Draw4();
	virtual int SpellCard();
	int NormalAtack1();
};
//中ボス
struct CBossMystiaMiddle : public CBossMystiaBase
{
	CBossMystiaMiddle();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
struct CBossMystia : public CBossMystiaBase
{
	CBossMystia();
	virtual int Run();
	virtual int Draw();
	int Thread();
	int Yoko();
	int Tate();
	int HalfCircle();
	int Yami();
	int Torime();
	int Torii();
};
