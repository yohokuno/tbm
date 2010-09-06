#pragma once

#define KEYCONFIG_NUM 7
#define REPLAY_NUM 50
#define SPELL_DISPLAY 10

struct CTitle  : public CScene
{
	int mode;
	int title_select;
	int difficult_select;
	int stage_select;
	int option_select;
	int config_select;
	int result_select;
	int replay_select;
	int replay_num;
	int replay_exist[REPLAY_NUM];
	int spell_select;
	int spell_difficult;
	int spell_sum;
	int spell_sum_diff;
	int inp;
	int inp_old;
	int otaku;
	int replay_config;
	int talk;
	SunSoundBuffer	sndDecide;
	SunSoundBuffer	sndCancel;
	SunSoundBuffer	sndSelect;
	SunOption optMode;
	SunOption optGraphic;
	SunOption optBGMVol;
	SunOption optSEVol;
	SunOption optInterval;
	SunOption optKeyConfig[KEYCONFIG_NUM];
	SunFont	fontSpellName[SPELL_DISPLAY];
	CTitle();
	virtual ~CTitle();
	virtual int Run();
	virtual int Draw();
	virtual int Process();
	int Resume();
	void StartGame();
	void UpdateSpellFont();
	int DrawNumber(int x,int y,int n,int k,D3DCOLOR color,double scale);
};
extern CTitle *g_pTitle;

struct CTitleMaterial
{
	SunTexture	imgTitle;
	SunTexture	imgTitleMoji;
	SunTexture	imgStageMoji;
	SunTexture	imgDifficult;
	SunTexture	imgLoad;
	SunTexture	imgSelect;
	SunTexture	imgRight;
	SunTexture	imgConfig;
	SunTexture	imgKeyConfig;
	CTitleMaterial() :
		imgTitle("img/title.png"),
		imgTitleMoji("img/titlemoji.png"),
		imgStageMoji("img/stagemoji.png"),
		imgDifficult("img/difficult.png"),
		imgLoad("img/load.png"),
		imgSelect("img/select.png"),
		imgRight("img/right.png"),
		imgConfig("img/config.png"),
		imgKeyConfig("img/keyconfig.png")
	{
		PlayBGM("tbm_title");
	}
	~CTitleMaterial()
	{
		StopBGM();
	}
};
extern CTitleMaterial *g_pTitleMaterial;