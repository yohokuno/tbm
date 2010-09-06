#pragma once


//再利用可能な弾幕生成ルーチン集

//簡易な弾幕関数
//自機狙い小弾
int Kodama( double x, double y );


//弾幕インターフェース
struct CDanmaku
{
	virtual int Fire() = 0;
};

//基本クラス
struct CDanmakuBase : public CDanmaku
{
	CTama *pTama;
	virtual int Fire() = 0;
};

//自機狙いnWay弾
struct CDanmakuToPlayer : public CDanmakuBase
{
	int n;
	double w;		//上下に開く角度
	CDanmakuToPlayer(CTama *pTama);
	virtual int Fire();
};
//自機狙いランダム弾
struct CDanmakuToPlayerRandom : public CDanmakuBase
{
	int n;
	double w,offset;
	double sv,ev;
	CDanmakuToPlayerRandom(CTama *pTama);
	virtual int Fire();
};

//自機狙いnWay m列弾
struct CDanmakuToPlayerLine : public CDanmakuBase
{
	int n, m;
	double offset;
	double w;
	double sv, ev;	//開始速度、終了速度
	CDanmakuToPlayerLine(CTama *pTama);
	virtual int Fire();
};

//全方位弾
struct CDanmakuNonDirectional : public CDanmakuBase
{
	int n;
	double w;
	double offset;	//角度のオフセット
	CDanmakuNonDirectional(CTama *pTama);
	virtual int Fire();
};
//全方位m列弾
struct CDanmakuNonDirectionalLine : public CDanmakuBase
{
	int n, m;
	double w;
	double offset;	//角度のオフセット
	double sv, ev;	//開始速度、終了速度
	CDanmakuNonDirectionalLine(CTama *pTama);
	virtual int Fire();
};
//てんとう虫
struct CDanmakuTentou : public CDanmaku
{
	int n, m;
	double offset;	//向き
	double sv, ev;	//開始速度、終了速度
	CTamaNormal *pTama;
	CDanmakuTentou(CTamaNormal *pTama);
	virtual int Fire();
};
//クモの巣
struct CDanmakuSpyder : public CDanmakuBase
{
	int n, m;
	double offset;	//向き
	double sv, ev;	//開始速度、終了速度
	CDanmakuSpyder(CTama *pTama);
	virtual int Fire();
};

//方向つき円弾
struct CDanmakuCircle : public CDanmakuBase
{
	int n;
	double w;
	double offset;	//角度のオフセット
	double aa,vv;	//速度のオフセット
	CDanmakuCircle(CTama *pTama);
	virtual int Fire();
};
//自機狙い円弾
struct CDanmakuCircleToPlayer : public CDanmakuBase
{
	int n;
	double w;
	double offset;	//角度のオフセット
	double aa,vv;	//速度のオフセット
	CDanmakuCircleToPlayer(CTama *pTama);
	virtual int Fire();
};
//全方位円弾
struct CDanmakuCircleNonDirectional : public CDanmakuBase
{
	int n,m;
	double w;
	double offset;	//角度のオフセット
	double aa,vv;	//速度のオフセット
	CDanmakuCircleNonDirectional(CTama *pTama);
	virtual int Fire();
};
//n行m列円弾
struct CDanmakuCircleLine : public CDanmakuBase
{
	int n,m;
	double w;
	double offset;	//角度のオフセット
	double aa,vv;	//速度のオフセット
	double sv, ev;	//開始速度、終了速度
	CDanmakuCircleLine(CTama *pTama);
	virtual int Fire();
};

//線分
struct CDanmakuSegment : public CDanmakuBase
{
	int n;
	double a1,a2,v1,v2;
	CDanmakuSegment(CTama *pTama);
	virtual int Fire();
	void Set( double t );	//密度で弾数指定（長さの考慮が不要になる）
};

//線分列
struct CDanmakuSegmentLine : public CDanmakuBase
{
	int n,m;
	double a1,a2,v1,v2,ev;
	CDanmakuSegmentLine(CTama *pTama);
	virtual int Fire();
	void Set( double t );	//密度で弾数指定（長さの考慮が不要になる）
};

//多角形
struct CDanmakuPolygons : public CDanmakuBase
{
	int n,m;
	double sv,ev;
	double offset;
	double t;
	CDanmakuPolygons(CTama *pTama);
	virtual int Fire();
};

//横型列弾
struct CDanmakuHorizon : public CDanmakuBase
{
	int n,m;
	double w;
	double offset;	//角度のオフセット
	CDanmakuHorizon(CTama *pTama);
	virtual int Fire();
};
