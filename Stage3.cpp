#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"
//ステージ3
CStage3 *g_pStage3 = NULL;

CStage3::CStage3() :
imgChirno("img/chirno.png"),
imgBack1("img/chirno_back1.png"),
imgBack2("img/chirno_back2.png"),
imgTalk("img/chirno_talk.png")
{
	g_pStage3 = this;
	g_pBack		= new CBackStage3();

	TSunFunctionCallback<CStage3> fn;
	fn.SetFunction(this,&CStage3::EnemyApear);
	mt.Start(&fn,0x8000);
}
CStage3::~CStage3(){
	g_pStage3 = NULL;
}
int CStage3::Run(){
	return CStage::Run();
}
int CStage3::EnemyApear()
{
	g_pSystem->spell_num = 8;
	PlayBGM("tbm_elf");
	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("Stage3",stageFontColor1,320,210,2,0));
	g_lEffect3.Add( new CEffectFont("湖上の積雪",stageFontColor2,320,230,2,0));
	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("♪ゴースト・イン・ザ・シェル",bgmFontColor,380,420) );
	mt.Suspend(60);

	//開始
	g_lEnemy.Add( new CEnemyFairy9(GAME_RIGHT + 24,GAME_CENTER_Y) );
	mt.Suspend(60);
	g_lEnemy.Add( new CEnemyFairy9(GAME_RIGHT + 24,GAME_CENTER_Y - 80) );
	g_lEnemy.Add( new CEnemyFairy9(GAME_RIGHT + 24,GAME_CENTER_Y + 80) );
	mt.Suspend(60);
	g_lEnemy.Add( new CEnemyFairy9(GAME_RIGHT + 24,GAME_CENTER_Y) );
	g_lEnemy.Add( new CEnemyFairy9(GAME_RIGHT + 24,GAME_CENTER_Y - 120) );
	g_lEnemy.Add( new CEnemyFairy9(GAME_RIGHT + 24,GAME_CENTER_Y + 120) );
	mt.Suspend(600);

	for( int i = 0; i < 4; ++i ) {
		g_lEnemy.Add( new CEnemyKedama4(GAME_RIGHT - 120,GAME_TOP - 16,1));
		mt.Suspend(20);
	}
	for( int i = 0; i < 4; ++i ) {
		g_lEnemy.Add( new CEnemyKedama4(GAME_RIGHT - 40,GAME_BOTTOM + 16,-1));
		mt.Suspend(20);
	}
	mt.Suspend(60);
	for( int i = 0; i < 4; ++i ) {
		g_lEnemy.Add( new CEnemyKedama4(GAME_RIGHT - 120,GAME_TOP - 16,1));
		mt.Suspend(20);
	}
	for( int i = 0; i < 4; ++i ) {
		g_lEnemy.Add( new CEnemyKedama4(GAME_RIGHT - 40,GAME_BOTTOM + 16,-1));
		mt.Suspend(20);
	}
	mt.Suspend(60);

	for( int i = 0; i < 8; i++ ) {
		g_lEnemy.Add( new CEnemyFairy10(GAME_RIGHT + 24,GAME_CENTER_Y + rand(-180,180)) );
		mt.Suspend(60);
	}
	mt.Suspend(300);

	g_lEnemy.Add( new CEnemyFairy11(GAME_RIGHT + 24,GAME_CENTER_Y) );
	mt.Suspend(800);

	for( int i = 4; i >= 2; i-- ) {
		g_lEnemy.Add( new CEnemyFairy9(GAME_RIGHT + 24,GAME_CENTER_Y + i*180/4));
		g_lEnemy.Add( new CEnemyFairy9(GAME_RIGHT + 24,GAME_CENTER_Y - i*180/4));
		mt.Suspend(10);
	}
	mt.Suspend(0);
	for( int i = 1; i >= 0; i-- ) {
		g_lEnemy.Add( new CEnemyFairy10(GAME_RIGHT + 24,GAME_CENTER_Y + i*180/4));
		g_lEnemy.Add( new CEnemyFairy10(GAME_RIGHT + 24,GAME_CENTER_Y - i*180/4));
		mt.Suspend(10);
	}
	mt.Suspend(0);
	for( int i = 0; i <= 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama4(GAME_RIGHT-80,GAME_TOP - 16,1));
		g_lEnemy.Add( new CEnemyKedama4(GAME_RIGHT-80,GAME_BOTTOM + 16,-1));
		mt.Suspend(5);
	}
	mt.Suspend(180);

	g_pMaterial->sndFreeze.Play(0);
	TamaFreeze();
	mt.Suspend(120);
	TamaToItem();
	g_lEnemy.DeleteAll();
	mt.Suspend(240);

	//中ボス
	g_lEnemy.Add( new CBossChirnoMiddle() );
	while(1) {
		if( g_lEnemy.IsEmpty() )
			break;
		mt.Suspend(1);
	}
	mt.Suspend(240);

	for( int i = 0; i < 4; ++i ) {
		g_lEnemy.Add( new CEnemyKedama4(GAME_RIGHT - 120,GAME_TOP - 16,1));
		mt.Suspend(20);
	}
	for( int i = 0; i < 4; ++i ) {
		g_lEnemy.Add( new CEnemyKedama4(GAME_RIGHT - 40,GAME_BOTTOM + 16,-1));
		mt.Suspend(20);
	}
	mt.Suspend(240);
	for( int i = 0; i < 2; i++ ) {
		g_lEnemy.Add( new CEnemyFairy10(GAME_RIGHT + 24,GAME_CENTER_Y - 120) );
		g_lEnemy.Add( new CEnemyFairy9(GAME_RIGHT + 24,GAME_CENTER_Y + 120) );
		mt.Suspend(120);
		g_lEnemy.Add( new CEnemyFairy10(GAME_RIGHT + 24,GAME_CENTER_Y + 80) );
		g_lEnemy.Add( new CEnemyFairy9(GAME_RIGHT + 24,GAME_CENTER_Y - 80) );
		mt.Suspend(120);
	}
	mt.Suspend(240);

	g_lEnemy.Add( new CEnemyFairy12(GAME_RIGHT + 24,GAME_CENTER_Y) );
	g_lEnemy.Add( new CEnemyFairy13(GAME_RIGHT + 24,GAME_CENTER_Y) );
	mt.Suspend(60);
	g_lEnemy.Add( new CEnemyFairy12(GAME_RIGHT + 24,GAME_CENTER_Y - 120) );
	g_lEnemy.Add( new CEnemyFairy12(GAME_RIGHT + 24,GAME_CENTER_Y + 120) );
	g_lEnemy.Add( new CEnemyFairy13(GAME_RIGHT + 24,GAME_CENTER_Y) );
	mt.Suspend(60);
	g_lEnemy.Add( new CEnemyFairy12(GAME_RIGHT + 24,GAME_CENTER_Y - 160) );
	g_lEnemy.Add( new CEnemyFairy12(GAME_RIGHT + 24,GAME_CENTER_Y + 160) );
	g_lEnemy.Add( new CEnemyFairy13(GAME_RIGHT + 24,GAME_CENTER_Y) );
	mt.Suspend(60);

	mt.Suspend(240);
	TamaToItem();
	mt.Suspend(60);

	//ボス
	g_lEnemy.Add( new CBossChirno() );
	while(1) {
		if( g_lEnemy.IsEmpty() )
			break;
		mt.Suspend(1);
	}
	mt.Suspend(100);
	return 1;
}

//-------------------妖精9---------------------------------
CEnemyFairy9::CEnemyFairy9(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 200;
	this->color = 2;
}
int CEnemyFairy9::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		if( count == 120 ) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = 3;

			CDanmakuNonDirectionalLine d(&tama);
			d.offset = rand(0, 360);
			d.sv = 2;
			d.ev = 1;
			d.w = 360;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 64;
				d.m = 4;
			}else {
				d.n = 4 * ( GetDifficult() + 1 );
				d.m = 1 + GetDifficult();
			}
			g_pMaterial->sndFire.Play(0);
			d.Fire();
		}
	}
	else{
		x += v;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy9::GetItem(){
	return ITEM_SCORE;
}
//-------------------妖精10---------------------------------
CEnemyFairy10::CEnemyFairy10(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 200;
	this->color = 1;
}
int CEnemyFairy10::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		if( count == 120 ) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 2;
			tama.color = 3;
			tama.v = 1;

			CDanmakuCircleToPlayer d(&tama);
			d.offset = rand(0, 360);
			d.w = 360;
			d.aa = 0;
			d.vv = 2;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 32;
			}else {
				d.n = 4 * ( GetDifficult() + 1 );
			}
			g_pMaterial->sndFire.Play(0);
			d.Fire();
		}
	}
	else{
		x += v;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy10::GetItem(){
	return ITEM_SCORE;
}
//-------------------妖精11---------------------------------
CEnemyFairy11::CEnemyFairy11(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 800;
	this->color = 2;
}
int CEnemyFairy11::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 600 ) {
		if( count == 120 || count == 360 ) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 10;
			tama.color = 3;
			tama.v = 0.5;

			CDanmakuCircleNonDirectional d(&tama);
			d.vv = 1;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 64;
				d.m = 16;
			}else {
				d.n = 8 * ( GetDifficult() + 1 );
				d.m = 4 + GetDifficult();
			}
			g_pMaterial->sndFire.Play(0);
			d.Fire();
		}
	}
	else{
		x += v;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy11::GetItem(){
	return ITEM_BOMB;
}
//-------------------妖精12---------------------------------
CEnemyFairy12::CEnemyFairy12(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 200;
	this->color = 2;
}
int CEnemyFairy12::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		if( count == 120 ) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 2;
			tama.color = 3;
			tama.v = 1;

			CDanmakuCircle d(&tama);
			d.offset = rand(0, 360);
			d.w = 360;
			d.aa = 0;
			d.vv = 2;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 32;
			}else {
				d.n = 4 * ( GetDifficult() + 1 );
			}
			d.Fire();
			d.aa = 180;
			d.Fire();
			g_pMaterial->sndFire.Play(0);
		}
	}
	else{
		x += v;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy12::GetItem(){
	return ITEM_SCORE2;
}
//-------------------妖精13---------------------------------
CEnemyFairy13::CEnemyFairy13(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 200;
	this->color = 2;
}
int CEnemyFairy13::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		if( count == 120 ) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 2;
			tama.color = 3;
			tama.v = 1;

			CDanmakuCircle d(&tama);
			d.offset = rand(0, 360);
			d.w = 360;
			d.aa = 90;
			d.vv = 2;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 32;
			}else {
				d.n = 4 * ( GetDifficult() + 1 );
			}
			d.Fire();
			d.aa = -90;
			d.Fire();
			g_pMaterial->sndFire.Play(0);
		}
	}
	else{
		x += v;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy13::GetItem(){
	return ITEM_POWER2;
}
//-------------------毛玉4---------------------------------
CEnemyKedama4::CEnemyKedama4(double x,double y,int c)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 10;
	this->color = 0;
	this->c = c;
}
int CEnemyKedama4::Run()
{
	if( count < 30 || count > 60 )
		this->y += c * 5;
	return CEnemyBase::Run();
}
//打ち返し弾
int CEnemyKedama4::Death() {
	CTamaNormal tama;
	tama.x = GetX();
	tama.y = GetY();
	tama.type = 0;
	tama.color = 0;
	tama.v = 2;

	CDanmakuNonDirectional d(&tama);
	d.offset = rand(0, 360);
	d.w = 360;
	if( GetDifficult() == DIFF_LUNATIC) {
		d.n = 64;
	}else {
		d.n = 4 * ( GetDifficult() + 1 );
	}
	g_pMaterial->sndFire.Play(0);
	d.Fire();
	return CEnemyBase::Death();
}
