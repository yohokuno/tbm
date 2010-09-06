#pragma once

#define MARISA_TALK_COLOR D3DCOLOR_XRGB(255,80,120)
#define ENEMY_TALK_COLOR D3DCOLOR_XRGB(255,80,0)
#define ENEMY_NAME_COLOR D3DCOLOR_XRGB(255,80,0)

struct CTalk
{
	SunMicroThread mt;
	char* text;
	int chara;
	int face;
	int manfu;
	int enemy_face;
	SunFont font;
	SunFont font2;
	SunFont fntEnemy;

	CTalk();
	virtual int Start() = 0;
	virtual void DrawEnemy() = 0;
	int IsEnd();
	int IsSuspended();
	void Resume();
	void Talk();
};

struct CTalkRumia1 : public CTalk
{
	virtual int Start();
	virtual void DrawEnemy();
	int Thread();
};

struct CTalkRumia2 : public CTalk
{
	virtual int Start();
	virtual void DrawEnemy();
	int Thread();
};

struct CTalkWriggle1 : public CTalk
{
	virtual int Start();
	virtual void DrawEnemy();
	int Thread();
};

struct CTalkWriggle2 : public CTalk
{
	virtual int Start();
	virtual void DrawEnemy();
	int Thread();
};

struct CTalkChirno1 : public CTalk
{
	virtual int Start();
	virtual void DrawEnemy();
	int Thread();
};

struct CTalkChirno2 : public CTalkChirno1
{
	virtual int Start();
	int Thread();
};

struct CTalkChirno3 : public CTalkChirno1
{
	virtual int Start();
	int Thread();
};

struct CTalkMystia1 : public CTalk
{
	virtual int Start();
	virtual void DrawEnemy();
	int Thread();
};
struct CTalkMystia2 : public CTalkMystia1
{
	virtual int Start();
	int Thread();
};
struct CTalkMystia3 : public CTalkMystia1
{
	virtual int Start();
	int Thread();
};
struct CTalkChen1 : public CTalk
{
	virtual int Start();
	virtual void DrawEnemy();
	int Thread();
};
struct CTalkChen2 : public CTalkChen1
{
	virtual int Start();
	int Thread();
};
struct CTalkChen3 : public CTalkChen1
{
	virtual int Start();
	int Thread();
};
struct CTalkLety1 : public CTalk
{
	virtual int Start();
	virtual void DrawEnemy();
	int Thread();
};
struct CTalkLety2 : public CTalkLety1
{
	virtual int Start();
	int Thread();
};
struct CTalkLety3 : public CTalkLety1
{
	virtual int Start();
	int Thread();
};


//台詞
struct CSerif
{
	SunFont font;
	D3DCOLOR c;
	int x,y;
};
#define SERIF_NUM 4
#define NARRATION_COLOR D3DCOLOR_XRGB(255,255,255)

//エンディング
struct CEnding : public CTalk
{
	SunTexture	imgBack;
	CSerif		serif[SERIF_NUM];

	CEnding();
	int Init();
	void Talk();
	virtual int Start();
	virtual void DrawEnemy();
	int Thread();
};

//6面中ボスチルノ
struct CTalkChirno4 : public CTalk
{
	virtual int Start();
	virtual void DrawEnemy();
	int Thread();
};