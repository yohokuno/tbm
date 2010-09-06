#pragma once

#define GAME_LEFT	24
#define GAME_TOP	48
#define GAME_RIGHT	600
#define GAME_BOTTOM	448
#define GAME_WIDTH	(GAME_RIGHT - GAME_LEFT)
#define GAME_HEIGHT	(GAME_BOTTOM - GAME_TOP)
#define GAME_CENTER_X ( GAME_LEFT + GAME_WIDTH / 2 )
#define GAME_CENTER_Y ( GAME_TOP + GAME_HEIGHT / 2 )

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

struct CScene : public SunObject
{
	virtual int Run() = 0;
	virtual int Draw() = 0;
	virtual int Process() = 0;
};

struct CGame  : public CScene
{
	int stop;		///< ゲームの進行を止めるフラグ（ポーズ、ゲームオーバー）
	int mode;		///< モード（0:Start,1:Practice,2:Replay)
	int diff;		///< 難易度（0:Easy,1:Normal,2:Hard,3:Lunatic)
	int stage;		///< ステージ（1～6）

	CGame(int mode,int diff,int stage);
	virtual ~CGame();
	virtual int Run();
	virtual int Draw();
	virtual int Process();
	void Stop();
	void Resume();
	void CreateStage();
};

extern CGame *g_pGame;
extern CScene *g_pScene;

#define DIFF_EASY		0
#define DIFF_NORMAL		1
#define DIFF_HARD		2
#define DIFF_LUNATIC	3

inline int GetDifficult(){return g_pGame->diff;}