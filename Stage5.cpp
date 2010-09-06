#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//ステージ5
CStage5 *g_pStage5 = NULL;

CStage5::CStage5() :
imgChen("img/chen.png"),
imgBack1("img/chen_back1.png"),
imgBack3("img/chen_back3.png"),
imgTalk("img/chen_talk.png")
{
	g_pStage5 = this;
	g_pBack		= new CBackStage5();

	TSunFunctionCallback<CStage5> fn;
	fn.SetFunction(this,&CStage5::EnemyApear);
	mt.Start(&fn,0x8000);
}
CStage5::~CStage5(){
	g_pStage5 = NULL;
}
int CStage5::Run(){
	return CStage::Run();
}
int CStage5::EnemyApear()
{
	g_pSystem->spell_num = 20;

	PlayBGM("tbm_stage5");
	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("Stage5",stageFontColor1,320,210,2,0));
	g_lEffect3.Add( new CEffectFont("破れた障子の裏側で",stageFontColor2,320,230,2,0));
	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("♪夜の矢上を往く",bgmFontColor,380,420) );
	mt.Suspend(30);

	for( int i = 0; i < 4; i++ ) {
		g_lEnemy.Add( new CEnemyFairy18(GAME_RIGHT + 24,GAME_CENTER_Y + rand(-180,180), 0) );
		mt.Suspend(60);
	}
	mt.Suspend(120);
	for( int i = 0; i < 4; i++ ) {
		g_lEnemy.Add( new CEnemyFairy18(GAME_RIGHT + 24,GAME_CENTER_Y + rand(-180,180), 0) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy18(GAME_RIGHT + 24,GAME_CENTER_Y + rand(-180,180), 1) );
		mt.Suspend(60);
	}
	mt.Suspend(240);

	for( int i = 0; i < 4; i++ ) {
		double yy = rand(-180,180);
		g_lEnemy.Add( new CEnemyFairy21(GAME_RIGHT + 24,GAME_CENTER_Y + yy, irand(0,1)));
		mt.Suspend(60);
	}
	mt.Suspend(120);
	for( int i = 0; i < 4; i++ ) {
		double yy = rand(-180,180);
		g_lEnemy.Add( new CEnemyFairy21(GAME_RIGHT + 24,GAME_CENTER_Y + yy, irand(0,1)));
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy18(GAME_RIGHT + 24,GAME_CENTER_Y + rand(-180,180), 0) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy18(GAME_RIGHT + 24,GAME_CENTER_Y + rand(-180,180), 1) );
		mt.Suspend(60);
	}

	mt.Suspend(240);
	TamaToItem();
	mt.Suspend(60);

	//中ボス
	g_lEnemy.Add( new CBossChenMiddle() );
	while(!g_lEnemy.IsEmpty()) {
		mt.Suspend(1);
	}

	mt.Suspend(240);

	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y + 40, -1) );
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y + 80, -1) );
	mt.Suspend(20);
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y + 40, -1) );
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y + 80, -1) );
	mt.Suspend(60);

	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -80, 1) );
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -120, 1) );
	mt.Suspend(20);
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -80, 1) );
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -120, 1) );
	mt.Suspend(60);

	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y + 80, -1) );
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y + 120, -1) );
	mt.Suspend(20);
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y + 80, -1) );
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y + 120, -1) );
	mt.Suspend(60);

	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -40, 1) );
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -80, 1) );
	mt.Suspend(20);
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -40, 1) );
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -80, 1) );
	mt.Suspend(120);

	for( int i = 0; i < 8; i++ ) {
		double xx = rand(0,120);
		g_lEnemy.Add( new CEnemyKedama5(GAME_CENTER_X+xx, GAME_TOP,rand(30,120),1));
		mt.Suspend(60);
	}
	mt.Suspend(240);

	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -40, 1) );
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -80, 1) );
	mt.Suspend(20);
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -40, 1) );
	g_lEnemy.Add( new CEnemyFairy20(GAME_RIGHT + 24,GAME_CENTER_Y -80, 1) );
	mt.Suspend(60);
	double xx = rand(0,120);
	g_lEnemy.Add( new CEnemyKedama5(GAME_CENTER_X+xx, GAME_TOP,rand(30,120),1));
	mt.Suspend(60);
	xx = rand(0,120);
	g_lEnemy.Add( new CEnemyKedama5(GAME_CENTER_X+xx, GAME_TOP,rand(30,120),1));
	mt.Suspend(60);

	mt.Suspend(240);

	for( int i = 0; i < 8; i++ ) {
		xx = rand(0,120);
		g_lEnemy.Add( new CEnemyFairy23(GAME_CENTER_X+xx, GAME_BOTTOM,rand(30,200),0));
		mt.Suspend(30);
	}
	mt.Suspend(120);
	for( int i = 0; i < 4; i++ ) {
		xx = rand(0,120);
		g_lEnemy.Add( new CEnemyFairy23(GAME_CENTER_X+xx, GAME_BOTTOM,rand(30,200),0));
		mt.Suspend(30);
		xx = rand(0,120);
		g_lEnemy.Add( new CEnemyKedama5(GAME_CENTER_X+xx, GAME_TOP,rand(30,120),1));
		mt.Suspend(30);
	}


	mt.Suspend(500);
	TamaToItem();
	mt.Suspend(60);

	//ボス
	g_lEnemy.Add( new CBossChen() );
	while(!g_lEnemy.IsEmpty()) {
		mt.Suspend(1);
	}
	mt.Suspend(100);
	return 1;
}
//-------------------妖精18（行列弾）---------------------------------
CEnemyFairy18::CEnemyFairy18(double x,double y,int t)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 150;
	this->color = t ? 1:0;
	this->t = t;
}
int CEnemyFairy18::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		if( count == 60) {
			CTamaTurn tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 12;
			tama.color = 1+2*t;
			tama.stop_count = 20;
			tama.stop_time = 30;
			tama.turn = 90;

			int n = GetDifficult() + 1;
			for( int i = 0; i < n; i++ ) {
				tama.v2 = 1 + 0.2*i;
				CDanmakuNonDirectionalLine d(&tama);
				d.offset = 90*t;
				d.sv = 6;
				d.ev = 1;
				d.w = 90;
				d.n = 2;
				if( GetDifficult() == DIFF_LUNATIC) {
					d.m = 8;
				}else {
					d.m = 2*(1+GetDifficult());
				}
				tama.turn = +90;
				d.Fire();
				tama.turn = -90;
				d.Fire();
			}
			g_pMaterial->sndFire.Play(0);

		}
	}
	else{
		x += v;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy18::GetItem(){
	return t ? ITEM_POWER2 : ITEM_SCORE2;
}
//-------------------妖精20（妖気）---------------------------------
CEnemyFairy20::CEnemyFairy20(double x,double y,double t)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 20;
	this->color = 4;
	this->t = t;
	this->v = -5.0;
}
int CEnemyFairy20::Run()
{
	//移動
	const double a = 0.1;
	v += a;
	x += v;
	y += 2*t;

	//攻撃
	int interval = 5*(4-GetDifficult());
	if( count%interval == 0) {
		CTamaIncrease tama;
		tama.x = GetX();
		tama.y = GetY();
		tama.type = 12;
		tama.color = 2;
		tama.v = -(double)(count-30)*(count-30)/50;
		tama.v = Sigmoid(tama.v,3,0.5,2);
		tama.a = 0;
		tama.end = 20;
		tama.inc = -0.3;

		g_lTama.Add( tama.Copy() );
		g_pMaterial->sndFire.Play(0);

	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy20::GetItem(){
	return ITEM_POWER2;
}
//-------------------妖精21(うずまき)---------------------------------
CEnemyFairy21::CEnemyFairy21(double x,double y,int t)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 150;
	this->color = t ? 7 : 6;
	this->t = t;
}
int CEnemyFairy21::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		int interval = 2*(4-GetDifficult());
		if( GetDifficult() == DIFF_LUNATIC ) interval = 1;
		if( count%interval == 0) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 12;
			tama.color = t ? 6 : 7;
			tama.v = 1.0 + 0.2 * GetDifficult();
			tama.a = (double)count*interval*4*(t*2-1);
			g_lTama.Add(tama.Copy());

			g_pMaterial->sndFire.Play(0);

		}
	}
	else{
		x += v;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy21::GetItem(){
	return ITEM_SCORE2;
}
//-------------------妖精22(人魂)---------------------------------
CEnemyFairy22::CEnemyFairy22(double x,double y,double c,double t)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 150;
	this->color = t?1:2;
	this->c = c;
	this->t = t;
}
int CEnemyFairy22::Run()
{
	const v = 2;
	if( count < c ) {
		y += v;
	}
	else if( count < c+60 ) {
		int interval = 2*(4-GetDifficult());
		if( GetDifficult() == DIFF_LUNATIC ) interval = 1;
		if( count%interval == 0) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 12;
			tama.color = t?1:3;
			tama.v = 1.0 + 0.2 * GetDifficult();
			tama.a = (double)count*interval*8*(2*t-1);
			g_lTama.Add(tama.Copy());

			g_pMaterial->sndFire.Play(0);
		}
	}
	else{
		y -= v;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy22::GetItem(){
	return ITEM_POWER2;
}
//-------------------妖精23(謎）---------------------------------
CEnemyFairy23::CEnemyFairy23(double x,double y,double c,double t)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 150;
	this->color = 2;
	this->c = c;
	this->t = t;
}
int CEnemyFairy23::Run()
{
	const v = 2;
	if( count < c ) {
		y -= v;
	}
	else if( count < c+60 ) {
		int interval = 1*(4-GetDifficult());
		if( count%interval == 0) {
			CTamaTaikyoku tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 12;
			tama.color = 3;
			tama.t1 = 120;
			tama.t2 = 0;
			tama.aa = -1.0-(double)count/10.0;
			
			CDanmakuNonDirectionalLine d(&tama);
			d.offset = 90+rand(-30,30);
			d.w = 0;
			d.n = 1;
			d.sv = 1.0;
			d.ev = 2.0 + 0.1 * GetDifficult();
			if( GetDifficult() == DIFF_LUNATIC) {
				d.m = 2;
			}else {
				d.m = 1;
			}
			d.Fire();
			g_pMaterial->sndFire.Play(0);

		}
	}
	else{
		y += v;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy23::GetItem(){
	return ITEM_SCORE2;
}
//-------------------毛玉5---------------------------------
CEnemyKedama5::CEnemyKedama5(double x,double y,double c,double t)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 50;
	this->color = t?1:3;
	this->c = c;
	this->t = t;
}
int CEnemyKedama5::Run()
{
	const v = 2;
	if( count < c ) {
		if( t )
			y += v;
		else
			y -= v;
	}
	else if( count < c+30 ) {
	}
	else{
		if( t )
			y -= v;
		else
			y += v;
	}
	int interval = 2*(4-GetDifficult());
	if( GetDifficult() == DIFF_LUNATIC ) interval = 1;
	if( count%interval == 0) {
		CTamaNormal tama;
		tama.x = GetX();
		tama.y = GetY();
		tama.type = 0;
		tama.color = t?1:3;
		tama.v = 1.0 + 0.2 * GetDifficult();
		tama.a = (double)count*interval*8*(2*t-1);
		g_lTama.Add(tama.Copy());

		g_pMaterial->sndFire.Play(0);
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyKedama5::GetItem(){
	return ITEM_POWER2;
}

//-------------------七福神---------------------------------
CEnemy7God::CEnemy7God(double x,double y,int c)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 500;
	this->color = 4;
	this->c = c;
}
int CEnemy7God::Run()
{
	if( count % 360 > 60 ) {
		int interval = 32*(4-GetDifficult());
		if( GetDifficult() == DIFF_LUNATIC ) interval = 8;
		if( count%interval == 0) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 12;
			tama.color = c;
			tama.v = 1.0 + 0.2 * GetDifficult();

			CDanmakuToPlayerLine d(&tama);
			d.w = 60;
			d.m = 1;
			d.sv = d.ev = tama.v;
			d.offset = 10*Sin(count*3);
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 3;
			}else {
				d.n = 1;
			}
			d.Fire();
			g_pMaterial->sndFire.Play(0);
		}
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemy7God::GetItem(){
	return ITEM_NONE;
}