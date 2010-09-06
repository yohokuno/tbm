#pragma once

//全てのアイテムを吸い寄せる
int ItemSuiyose();

//アイテムクラス（typeで型指定）
struct CItem : public SunObject
{
	double x,y;
	int type;
	double vx,ax;
	int suiyose;
	int count;
	CItem(double x,double y,int type);
	virtual ~CItem();
	virtual int Run();
	virtual int Draw();
	double GetSize();
};

extern SunList<CItem> g_lItem;

#define ITEM_STAR	0
#define ITEM_POWER	1
#define ITEM_SCORE	2
#define ITEM_POWER2	3
#define ITEM_BOMB	4
#define ITEM_FULLPOWER	5
#define ITEM_1UP	6
#define ITEM_SCORE2 7
#define ITEM_SCORE0 8

#define ITEM_NONE	-1