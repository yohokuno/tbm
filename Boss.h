#pragma once

/*
  ボス共通
*/
struct CBossBase : public CEnemy
{
	int count;
	double x,y;
	double hp_max;
	double hp;
	double deffence;	//防御力（ダメージ係数）
	int spell;	//スペルフラグ
	int time;	//制限時間
	int dirty;	//0:スペカ取得、1:失敗
	int flag;	//時間が進むかどうかのフラグ
	SunFont fntSpell;
	SunFont fntBoss;
	CBack* pBack;	//背景（保存用）
	CTalk* pTalk;	//会話

	SunMicroThreadEx	mt;
	SunMicroThreadEx	mt2;
	SunMicroThreadEx	mt3;

	CBossBase();
	virtual ~CBossBase();
	virtual int Run();
	virtual int Draw();
	virtual int Draw2();		///< ライフゲージ等
	virtual int Draw3();		///< 会話系
	virtual int Draw4();		///< 鳥目エフェクト専用
	virtual int SpellCard();	///< スペルカード開始
	virtual int SpellEnd();		///< スペルカード終了
	int Damage( double damage );	///< ダメージをくらう
	int Death();
	int Hantei();		///< 自機との当たり判定
	void SetHP( double hp );
	void SetTime( int time );
	int IsEnd();

	double GetX(){return x + GetW()/2;}
	double GetY(){return y + GetH()/2;}
	double GetSize(){return (GetW()+GetH())/4;}
	virtual double GetW(){return 64;}
	virtual double GetH(){return 64;}
	virtual int GetItem();	///< アイテムの種類
	virtual void Appear();
	void AddItem(int item,int num);
	void Suspend(double m);
};
extern CBossBase *g_pBoss;
