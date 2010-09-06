#pragma once

//レーザーインターフェース
//実は普通の弾と同じ。
//描画順序を弾より後ろにするためだけに分けた。
struct CLaser : public SunObject
{
	virtual int Run() = 0;
	virtual int Draw() = 0;
	virtual int Item() = 0;
};
extern SunList<CLaser> g_lLaser;

//四角いレーザー
struct CLaserSquare : public CLaser
{
	int count;
	int start_count;
	int end_count;
	int vanish_count;
	int graze_count;
	double iw;
	double ow;
	double a; //角度
	double w; //角速度
	SunEffectLineLaser laser;
	CLaserSquare();
	virtual int Run();
	virtual int Draw();
	virtual int Item();
};