#pragma once

/**
	@brief リプレイ1入力
*/
struct SunReplayData
{
	LONGLONG flame;		//フレーム（時間）
	unsigned char inp;		//入力番号
	unsigned char change;	//0：離した、1：押した
	unsigned char dammy1;
	unsigned char dammy2;
	unsigned int dammy3;
	SunReplayData(){
		dammy1 = 0xff;
		dammy2 = 0xff;
		dammy3 = 0xffffffff;
	}
};
/**
	@brief リプレイファイルヘッダ
*/
struct SunReplayHeader
{
	char guid[4];		///< 'r' 'p' 'l' 'y'
	unsigned int seed;	//ランダムシード
	unsigned char mode;
	unsigned char diff;
	unsigned char stage;
	unsigned char dammy;
	unsigned int dammy2;
	SunReplayHeader(){
		dammy = 0xff;
		dammy2 = 0xffffffff;
	}
};
/**
	@brief リプレイクラス
*/
class CSunReplay
{
public:
	LONGLONG m_count;		//フレームカウント
	unsigned int m_seed;	//ランダムシード
	int m_mode;				//再生/記録モード
	int	m_old[INP_MAX];		///<１フレ前の状態
	int	m_state[INP_MAX];	///<現在の状態
	SunReplayHeader m_header;		//ヘッダー
	list<SunReplayData> m_lstData;	//データ
	int m_pause;			//ポーズ許可フラグ
public:
	CSunReplay();
	virtual ~CSunReplay();
	void Record();
	int Run();
	void Save( char* file );
	void Save();
	void Play( char* file  );
	void Exit();
	int GetInputState(int inp);	///< その瞬間の入力状態
	int GetInputEvent(int inp);	///< 押された瞬間を検出
	int GetInputHanasi(int inp);///< 離しを検出
};
#define REPLAY_NONE		0
#define REPLAY_RECORD	1
#define REPLAY_PLAY		2

extern CSunReplay SunReplay;

//その瞬間の入力状態
int GetInputState(int inp);
//押された瞬間を検出
int GetInputEvent(int inp);
//離しに反応
int GetInputHanasi(int inp);
//決定ボタン
int GetInputDecide();
//キャンセルボタン
int GetInputCancel();
//上下キー
int GetInputUD();
//左右キー
int GetInputLR();
//左右キー状態
int GetInputLRState();
//ポーズボタン
int GetInputPause();
