#pragma once

#define SYSTEM_NORMAL 0
#define SYSTEM_PAUSE 1
#define SYSTEM_GAMEOVER 2
#define SYSTEM_REPLAY 3
#define SYSTEM_RESULT 4
#define SYSTEM_RESUME 5

struct CSystem : SunObject
{
	SunTexture	imgSystem1;
	SunTexture	imgSystem2;
	SunTexture	imgMaho;
	SunTexture	imgMoji;
	SunTexture	imgMoji2;
	SunTexture	imgLife;
	SunTexture	imgBomb;
	SunSoundBuffer sndPause;
	int	count;	//カウント
	int life;	//残機
	int bomb;	//残ボム
	int power;	//パワー
	int graze;	//グレイズ
	LONGLONG score;		//スコア
	LONGLONG hiscore;	//ハイスコア
	int spellcard;		//スペルカード
	double rank;			//倍率
	int bullet;				//最大弾数
	int item;				//アイテム取った数

	int state;				//SYSTEM_**
	int pause_select;		//ポーズメニュー
	int gameover_select;	//ゲームオーバーメニュー
	int replay_select;		//リプレイメニュー
	int fullpower_count;	//フルパワー表示
	int spellbonus_count;	//スペルボーナス表示
	int spellbonus;			//0ならSPELLCARD FAILED
	int continue_dirty;		//コンティニューしたか？
	int continue_rest;		//残りコンティニュー回数
	int result_mode;		//リザルト表示
	int result_count;		//リザルト表示
	int extend_count;		//エクステンド回数
	int extend_interval;	//エクステンド間隔
	int extend_score;		//次のエクステンドスコア
	int resume_count;		//再開カウント
	int spell_num;			//現在のスペル番号
	int draw_system;		//システム描画する？

	CSystem();
	virtual ~CSystem();
	virtual int Run();
	virtual int Draw();
	int Damage();
	int GameOver();
	int AddScore(LONGLONG s);
	int AddPower(int p);
	int AddLife(int l);
	int AddBomb(int b);
	int AddGraze(int g);
	int DrawNumber(int x,int y,LONGLONG n,int k,D3DCOLOR color = alpha(255),int scale = 1);
	void ReturnTitle();
	void Restart();
	void SaveReplay();
	void DrawResult(int x,int y,D3DCOLOR color);
	void ResumeGame();
};

extern CSystem *g_pSystem;
