#pragma once

#define MARISA_WIDTH	48
#define MARISA_HEIGHT	48
#define MARISA_CLIP		10

struct CMarisa
{
	SunTexture	imgMarisa;
	SunTexture	imgHakka;
	SunTexture	imgAtari;
	SunFont		font;
	SunEffectLaser2	effect;
	SunEffectLaser2	laser1;
	SunEffectLaser	laser2;
	SunEffectLaser	laser3;
	SunSoundBuffer	sndLaser;
	SunSoundBuffer	sndShot;
	SunSoundBuffer	sndDamage;
	SunSoundBuffer	sndGraze;
	double x,y;
	int count;
	int	state;	///< 0:通常、1:ボム中、2:喰らいボム受付中、3:出現中
	int atack;	///< 0:攻撃しない、1:ショット
	int slow;	///< 0:高速、1:低速
	int muteki;	///< 0:通常、0以上：無敵
	int dcount;	///< 汎用ダウンカウンタ
	int kurai;	///< 喰らいボム受付時間（喰らいボムを使うたびに減っていく）
	int renda;	///< 連打カウンタ（ボタン2用）
	int forbit;	///< ショット・ボム禁止
	double rx,ry;	///< マスタースパーク用ランダム変数
	double si;		///< マスタースパーク用サイン値
	int max_kurai;	///< 喰らいボムの受付時間
	int dec_kurai;	///< 喰らいボムの受付時間減少分

	CMarisa();
	virtual ~CMarisa();
	int Run();
	int Move();
	int Attack();
	int Bomb();
	int Draw();
	int Draw2();

	double GetMasterHantei(int x,int y);	///< マスタースパークの攻撃判定
	int GetMasterHantei2(int x,int y);		///< マスタースパークの弾消し判定
	int GetLaserHantei(RECT *pRect);		///< レーザーの攻撃判定
	double GetLaserDamage(){return 1;}
	double GetSize();		///< 弾との当たり判定用
	double GetGrazeSize();	///< グレイズの判定サイズ
	int GetMuteki();	///< 無敵かどうか
	void Damage();		///< エフェクトとか
	void Damage2();		///< 実際のダメージ（喰らいボム用）
	void Stop();
	double GetX();
	double GetY();
	void GetItem(int type);
	int Graze();		///< グレイズ
	void Talk();		///< 会話ON
	void Resume();		///< 会話OFF
	void ResetState();	///< ステージクリア時用
	void ResetPosition();	///<位置初期化
};

extern CMarisa *g_pPlayer;