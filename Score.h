#pragma once

#define DIFF_NUM 4
#define SPELL_NUM 36

struct CScore
{
	LONGLONG score;
};
struct CSpellRecord
{
	unsigned short challenge;
	unsigned short success;
};
struct CScoreData
{
	CScore score[DIFF_NUM];
	CSpellRecord spell[SPELL_NUM][DIFF_NUM];
};
extern char *spell_name[];

struct CRecord
{
	CScoreData data;
	CRecord();
	virtual ~CRecord();
	void Save();
	void Load();
};
extern CRecord *g_pRecord;

#define SCORE_ITEM0 100
#define SCORE_ITEM1 1000
#define SCORE_ITEM2 10000
#define SCORE_ITEMTAMA 1
#define SCORE_UTIKOMI 1
#define SCORE_GRAZE 100
#define SCORE_SPELLCARD 10000