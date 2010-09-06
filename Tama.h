#pragma once

//すべての弾をアイテムに変える
int TamaToItem();
//フリーズ
int TamaFreeze();
//ランダムに動く
int TamaRandom(double v);
//移動再開
int TamaResume(double v);

//基本クラス
struct CTama : public SunObject
{
	double x,y;
	double v,a;
	int type,color;
	virtual int Run() = 0;
	virtual int Draw() = 0;
	virtual int Item() = 0;		//アイテムに変わる
	virtual int Freeze() = 0;	//凍る
	virtual CTama *Copy() = 0;
};
extern SunList<CTama> g_lTama;
//普通クラス（等速直線運動）
struct CTamaNormal : public CTama
{
	int graze;
	int out;
	int count;
	CTamaNormal();
	virtual int Run();
	virtual int Run2();
	virtual int Draw();
	virtual CTama *Copy();
	virtual int Item();
	virtual int Freeze();
	double GetX();
	double GetY();
	int Vanish();
	double GetVX();
	double GetVY();
	void SetVelocityXY(double vx,double vy);
};
//加速度運動
struct CTamaIncrease : public CTamaNormal
{
	int end;
	double inc;
	CTamaIncrease();
	virtual int Run();
	virtual CTama *Copy();
	void SetIncrease( double sv,double ev,int end );
};
//折れ弾
//等速直線→停止→向きを変えて再開
struct CTamaTurn : public CTamaNormal
{
	int stop_count;
	int stop_time;
	double turn;
	double v2;
	CTamaTurn();
	virtual int Run();
	virtual CTama *Copy();
};
//自機狙い折れ弾
//等速直線→停止→自機狙い
struct CTamaTurnToPlayer : public CTamaNormal
{
	int stop_count;
	int stop_time;
	double turn;
	double v2;
	CTamaTurnToPlayer();
	virtual int Run();
	virtual CTama *Copy();
};
//蛍弾
struct CTamaHotaru : public CTamaNormal
{
	double s,sa;
	CTamaHotaru();
	virtual int Run();
	virtual CTama *Copy();
};
//折れ弾２（角度絶対指定）
struct CTamaTurn2 : public CTamaNormal
{
	int turn_count;
	double turn;
	double v2;
	CTamaTurn2();
	virtual int Run();
	virtual CTama *Copy();
};
//分裂弾
struct CTamaBurst : public CTamaNormal
{
	int burst_count;
	int t,m,n;
	double sv,ev;
	CTamaBurst();
	virtual int Run();
	virtual CTama *Copy();
};

//ムカデ弾
struct CTamaMukade : public CTamaNormal
{
	int burst_start;
	int burst_interval;
	int burst_end;
	int t;
	CTamaMukade();
	virtual int Run();
	virtual CTama *Copy();
};
//ムカデ弾（足）
struct CTamaMukadeLeg : public CTamaNormal
{
	int t,tt;
	double xx,yy,s;
	CTamaMukadeLeg();
	virtual int Run();
	virtual CTama *Copy();
};

//置き弾
struct CTamaSet : public CTamaNormal
{
	int t;
	double c;
	int set_interval;
	CTamaSet();
	virtual int Run();
	virtual CTama *Copy();
};

//フリーズ弾
struct CTamaFreeze : public CTamaNormal
{
	int r;
	int t;
	int c;
	double vv;
	virtual int Run();
	virtual CTama *Copy();
};

//分裂弾2
struct CTamaBurst2 : public CTamaNormal
{
	int burst_start;
	int burst_end;
	int burst_interval;
	int m,n;
	double w;
	double sv,ev;
	double offset;
	virtual int Run();
	virtual CTama *Copy();
};

//霜弾
struct CTamaFrost : public CTamaNormal
{
	int mode;
	int freeze;
	double frost;
	CTamaFrost();
	virtual ~CTamaFrost();
	virtual int Run();
	virtual CTama *Copy();
	void Stop();
	static list<CTamaFrost*> lTamaFrost;
};

//結晶弾
struct CTamaCristal : public CTamaNormal
{
	int mode;
	CTamaCristal();
	virtual ~CTamaCristal();
	virtual int Run();
	virtual CTama *Copy();
	void Stop();
	static list<CTamaCristal*> lTamaCristal;
};

//ピンポイント弾
struct CTamaPinPoint : public CTamaNormal
{
	int mode;
	int t;
	int c;
	double vv;
	virtual int Run();
	virtual CTama *Copy();
};

//ボイド弾
struct CTamaBoid : public CTamaNormal
{
	double w1,w2,w3;
	int loop;
	double vmax;
	CTamaBoid();
	virtual ~CTamaBoid();
	virtual int Run();
	virtual CTama *Copy();
	static list<CTamaBoid*> lTamaBoid;
	static double gx1,gy1;
	static double gx2,gy2;
	static double gx3,gy3;
	static void UpdateGoal();
};

//太極図
struct CTamaTaikyoku : public CTamaNormal
{
	int t1,t2;
	double aa;
	CTamaTaikyoku();
	virtual int Run();
	virtual CTama *Copy();
};

//タイフーン
struct CTamaTyphoon : public CTamaNormal
{
	CTamaTyphoon();
	virtual ~CTamaTyphoon();
	virtual int Run();
	virtual CTama *Copy();
	static list<CTamaTyphoon*> lTamaTyphoon;
};
//ハリケーン
struct CTamaHurricane : public CTamaNormal
{
	int t;
	double c;
	CTamaHurricane();
	virtual int Run();
	virtual CTama *Copy();
};

//ツナミ
struct CTamaTsunami : public CTamaNormal
{
	int end;
	double ax;
	double ay;
	CTamaTsunami();
	virtual int Run();
	virtual CTama *Copy();
};

//反射弾
struct CTamaReflect : public CTamaNormal
{
	int num;
	CTamaReflect();
	virtual int Run();
	virtual CTama *Copy();
};
