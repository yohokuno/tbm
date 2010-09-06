#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"
//ステージ2
CStage2 *g_pStage2 = NULL;

CStage2::CStage2() :
imgWriggle("img/wriggle.png"),
imgBack1("img/wriggle_back1.png"),
imgBack2("img/wriggle_back2.png"),
imgCutin("img/wriggle_cutin.png"),
imgTalk("img/wriggle_talk.png")
{
	g_pStage2 = this;
	g_pBack = new CBackStage2();

	TSunFunctionCallback<CStage2> fn;
	fn.SetFunction(this,&CStage2::EnemyApear);
	mt.Start(&fn,0x8000);
}
CStage2::~CStage2(){
	g_pStage2 = NULL;
}
int CStage2::Run() {
	return CStage::Run();
}
int CStage2::EnemyApear()
{
	int imax = 4 + 4 * GetDifficult();
	g_pSystem->spell_num = 3;
	PlayBGM("tbm_gensi");

	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("Stage2",stageFontColor1,320,210,2,0));
	g_lEffect3.Add( new CEffectFont("虫達の冬越",stageFontColor2,320,230,2,0));
	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("♪蟲の煙草、光酒の盃",bgmFontColor,380,420) );
	mt.Suspend(60);

	//開始
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama2(GAME_RIGHT - 40,GAME_TOP - 16,1));
		mt.Suspend(10);
	}
	mt.Suspend(30);
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama2(GAME_RIGHT - 40,GAME_BOTTOM + 16,-1));
		mt.Suspend(10);
	}
	mt.Suspend(240);

	g_lEnemy.Add( new CEnemyFairy5(GAME_RIGHT + 24,GAME_CENTER_Y + 180,-1,1));
	mt.Suspend(60);
	g_lEnemy.Add( new CEnemyFairy5(GAME_RIGHT + 24,GAME_CENTER_Y - 180,1,1));
	mt.Suspend(240);

	g_lEnemy.Add( new CEnemyFairy5(GAME_RIGHT + 24,GAME_CENTER_Y + 180,-1,0));
	mt.Suspend(60);
	g_lEnemy.Add( new CEnemyFairy5(GAME_RIGHT + 24,GAME_CENTER_Y - 180,1,0));
	mt.Suspend(240);

	g_lEnemy.Add( new CEnemyFairy6(GAME_RIGHT + 24,GAME_CENTER_Y,0));
	mt.Suspend(240);
	g_lEnemy.Add( new CEnemyFairy6(GAME_RIGHT + 24,GAME_CENTER_Y-160,0));;
	g_lEnemy.Add( new CEnemyFairy6(GAME_RIGHT + 24,GAME_CENTER_Y+160,0));
	mt.Suspend(240);

	g_lEnemy.Add( new CEnemyFairy6(GAME_RIGHT + 24,GAME_CENTER_Y,0));
	mt.Suspend(60);
	g_lEnemy.Add( new CEnemyFairy5(GAME_RIGHT + 24,GAME_CENTER_Y - 180,1,1));
	g_lEnemy.Add( new CEnemyFairy5(GAME_RIGHT + 24,GAME_CENTER_Y + 180,-1,0));
	mt.Suspend(240);

	TamaToItem();
	mt.Suspend(60);

	//中ボス
	g_lEnemy.Add( new CBossWriggleMiddle() );
	while(1) {
		if( g_lEnemy.IsEmpty() )
			break;
		mt.Suspend(1);
	}
	mt.Suspend(240);

	//後半
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama3(GAME_RIGHT - 120,GAME_TOP - 16,1));
		mt.Suspend(10);
	}
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama3(GAME_RIGHT - 40,GAME_BOTTOM + 16,-1));
		mt.Suspend(10);
	}
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama3(GAME_RIGHT - 120,GAME_TOP - 16,1));
		mt.Suspend(10);
	}
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama3(GAME_RIGHT - 40,GAME_BOTTOM + 16,-1));
		mt.Suspend(10);
	}
	mt.Suspend(240);

	for( int i = 0; i < imax; ++i ) {
		g_lEnemy.Add( new CEnemyFairy7(GAME_RIGHT+24+120 ,GAME_CENTER_Y, i*360/imax, 1));
	}
	mt.Suspend(120);
	for( int i = 0; i < imax; ++i ) {
		g_lEnemy.Add( new CEnemyFairy7(GAME_RIGHT+24+120 ,GAME_CENTER_Y, i*360/imax, -1));
	}
	mt.Suspend(120);
	for( int i = 0; i < imax; ++i ) {
		g_lEnemy.Add( new CEnemyFairy7(GAME_RIGHT+24+120 ,GAME_CENTER_Y, i*360/imax, 1));
	}
	mt.Suspend(120);
	for( int i = 0; i < imax; ++i ) {
		g_lEnemy.Add( new CEnemyFairy7(GAME_RIGHT+24+120 ,GAME_CENTER_Y, i*360/imax, -1));
	}
	mt.Suspend(120);
	for( int i = 0; i < imax; ++i ) {
		g_lEnemy.Add( new CEnemyFairy7(GAME_RIGHT+24+120 ,GAME_CENTER_Y, i*360/imax, 1));
	}
	mt.Suspend(120);
	for( int i = 0; i < imax; ++i ) {
		g_lEnemy.Add( new CEnemyFairy7(GAME_RIGHT+24+120 ,GAME_CENTER_Y, i*360/imax, -1));
	}
	mt.Suspend(360);

	g_lEnemy.Add( new CEnemyFairy8( GAME_RIGHT+24 ,GAME_CENTER_Y + 120 ));
	mt.Suspend(240);
	for( int i = 0; i < imax; ++i ) {
		g_lEnemy.Add( new CEnemyFairy7(GAME_RIGHT+24+120 ,GAME_CENTER_Y, i*360/imax, 1));
	}
	mt.Suspend(120);
	for( int i = 0; i < imax; ++i ) {
		g_lEnemy.Add( new CEnemyFairy7(GAME_RIGHT+24+120 ,GAME_CENTER_Y, i*360/imax, -1));
	}
	mt.Suspend(120);

	g_lEnemy.Add( new CEnemyFairy8( GAME_RIGHT+24 ,GAME_CENTER_Y - 120 ));
	mt.Suspend(240);
	for( int i = 0; i < imax; ++i ) {
		g_lEnemy.Add( new CEnemyFairy7(GAME_RIGHT+24+120 ,GAME_CENTER_Y, i*360/imax, 1));
	}
	mt.Suspend(120);
	for( int i = 0; i < imax; ++i ) {
		g_lEnemy.Add( new CEnemyFairy7(GAME_RIGHT+24+120 ,GAME_CENTER_Y, i*360/imax, -1));
	}
	mt.Suspend(240);

	TamaToItem();
	g_lEnemy.DeleteAll();
	mt.Suspend(60);

	//ボス
	g_lEnemy.Add( new CBossWriggle() );
	while(1) {
		if( g_lEnemy.IsEmpty() )
			break;
		mt.Suspend(1);
	}

	mt.Suspend(100);
	
	return 1;
}
//-------------------妖精5---------------------------------
CEnemyFairy5::CEnemyFairy5(double x,double y,int c,int t)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 60;
	this->color = t ? 2 : 1;
	this->c = c;
	this->t = t;
	this->vx = -8;
	this->vy = c * 2;
}
int CEnemyFairy5::Run()
{
	//移動
	vx += 0.1;
	x += vx;
	y += vy;
	//攻撃
	if( count > 10 && count < 60 ) {
		int c = 8 - 2*GetDifficult();
		if( count % c == 0 ) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = t ? 3 : 1;
			tama.v = 2;

			CDanmakuToPlayer d(&tama);
			d.w = 60;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 8;
			}else {
				d.n = 2 * ( GetDifficult() + 1 );
			}
			d.n -= (t==0);
			g_pMaterial->sndFire.Play(0);
			d.Fire();
		}
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy5::GetItem(){
	return ITEM_SCORE;
}
//-------------------妖精6---------------------------------
CEnemyFairy6::CEnemyFairy6(double x,double y,int t)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 240;
	this->color = t ? 2 : 1;
	this->t = t;
}
int CEnemyFairy6::Run()
{
	if( count < 60 ) {
		x -= 2;
	}
	else if( count < 240 ) {
		if( count == 120 ) {
			CTamaTurnToPlayer tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = t ? 3 : 1;
			tama.stop_count = 20;
			tama.stop_time = 30;

			int n = GetDifficult() + 1;
			for( int i = 0; i < n; i++ ) {
				tama.v2 = 2 + i;
				CDanmakuNonDirectionalLine d(&tama);
				double angle = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
				d.offset = angle;
				d.sv = 10;
				d.ev = 1;
				d.w = 90;
				d.n = 2;
				if( GetDifficult() == DIFF_LUNATIC) {
					d.m = 16;
				}else {
					d.m = 4+4*GetDifficult();
				}
				d.Fire();
			}
			g_pMaterial->sndFire.Play(0);
		}
	}
	else {
		x += 2;
	}
	return CEnemyBase::Run();
}

//アイテム
int CEnemyFairy6::GetItem(){
	return ITEM_POWER2;
}
//-------------------妖精7---------------------------------
const int r7 = 60;

CEnemyFairy7::CEnemyFairy7(double tx,double ty,double a,int t)
{
	this->tx = tx;
	this->ty = ty;
	this->a = a;
	this->t = t;
	this->x = tx - GetW()/2;
	this->y = ty + r7 * t - GetH()/2;
	this->hp = 30;
	this->color = (t==1) ? 2 : 1;
}
int CEnemyFairy7::Run()
{
	tx -= 1;
	a += 2 * t;
	x = tx + 180 * Sin( a );
	y = ty + 180 * Cos( a );
	if( tx + 180 + 24 < GAME_LEFT )
		return 0;
	int s = 60 - 10 * GetDifficult();
	int e = 90 + 20 * GetDifficult();
	if( count > s && count < e )
	{
		int c = 10 - 2 * GetDifficult();
		if( count % c == 0 ) {
			CTamaIncrease tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 3;
			tama.color = (t==1) ? 3 : 1;
			tama.SetIncrease(1,2+GetDifficult()*2,20);

			CDanmakuToPlayer d(&tama);
			d.w = (t==1) ? 15 : 30;
			d.n = (t==1) ? 2 : (GetDifficult()>DIFF_NORMAL) ? 3 : 1;
			d.Fire();

			g_pMaterial->sndFire.Play(0);
		}
	}
	return CEnemyBase::Run2();
}
//アイテム
int CEnemyFairy7::GetItem(){
	return ITEM_POWER;
}
//-------------------妖精8---------------------------------
CEnemyFairy8::CEnemyFairy8(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 240;
	this->color = 3;
}
int CEnemyFairy8::Run()
{
	if( count < 60 ) {
		x -= 2;
	}
	else if( count < 240 ) {
		if( count == 120 ) {
			CTamaIncrease tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = 5;
			tama.SetIncrease( 0, 1, 30 );

			CDanmakuToPlayerRandom danmaku(&tama);
			danmaku.w = 4.0+2.0*GetDifficult();
			danmaku.sv = 2.0;
			danmaku.ev = 3.0+0.5*GetDifficult();
			if( GetDifficult() == DIFF_LUNATIC ) {
				danmaku.n = 64;
			}
			else {
				danmaku.n = 4 + 8 * GetDifficult();
			}
			g_pMaterial->sndFire.Play(0);
			danmaku.Fire();
			danmaku.offset = 30;
			danmaku.Fire();
			danmaku.offset = -30;
			danmaku.Fire();

			g_pMaterial->sndFire.Play(0);

			{
				CTamaIncrease tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 13;
				tama.color = 5;
				tama.SetIncrease( 0, 1, 30 );

				CDanmakuNonDirectionalLine d(&tama);
				d.offset = rand(0, 360);
				d.sv = 0.2;
				d.ev = 2;
				d.w = 360;
				if( GetDifficult() == DIFF_LUNATIC) {
					d.n = 96;
					d.m = 4;
				}else {
					d.n = 4 + 8 * GetDifficult();
					d.m = 1 + GetDifficult();
				}
				d.Fire();
			}
		}
	}
	else {
		x += 2;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy8::GetItem(){
	return rand(0,1) ? ITEM_POWER2 : ITEM_SCORE2;
}
//-------------------毛玉2---------------------------------
CEnemyKedama2::CEnemyKedama2(double x,double y,int c)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 10;
	this->color = 0;
	this->c = c;
}
int CEnemyKedama2::Run()
{
	if( count < 30 || count > 60 )
		this->y += c * 5;
	return CEnemyBase::Run();
}
//打ち返し弾
int CEnemyKedama2::Death() {
	if( GetDifficult() >= DIFF_HARD ) {
		Kodama(GetX(),GetY());
	}
	return CEnemyBase::Death();
}
//-------------------毛玉3---------------------------------
CEnemyKedama3::CEnemyKedama3(double x,double y,int c)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 10;
	this->color = 0;
	this->c = c;
}
int CEnemyKedama3::Run()
{
	this->x += c;
	this->y += (0.003 * ( pow2( count-60) - 120)) * c;
	return CEnemyBase::Run();
}
//打ち返し弾
int CEnemyKedama3::Death() {
	if( GetDifficult() >= DIFF_HARD ) {
		Kodama(GetX(),GetY());
	}
	return CEnemyBase::Death();
}
