#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//ステージ4
CStage4 *g_pStage4 = NULL;

CStage4::CStage4() :
imgMystia("img/mystia.png"),
imgBack1("img/mystia_back1.png"),
imgBack2("img/mystia_back2.png"),
imgTalk("img/mystia_talk.png")
{
	g_pStage4 = this;
	g_pBack		= new CBackStage4();

	TSunFunctionCallback<CStage4> fn;
	fn.SetFunction(this,&CStage4::EnemyApear);
	mt.Start(&fn,0x8000);
}
CStage4::~CStage4(){
	g_pStage4 = NULL;
}
int CStage4::Run(){
	return CStage::Run();
}
int CStage4::EnemyApear()
{

	g_pSystem->spell_num = 14;
	PlayBGM("tbm_stage4");

	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("Stage4",stageFontColor1,320,210,2,0));
	g_lEffect3.Add( new CEffectFont("闇に潜む影",stageFontColor2,320,230,2,0));
	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("♪レフトウィングス",bgmFontColor,380,420) );
	mt.Suspend(60);

	//開始
	g_lEnemy.Add( new CEnemyFairy14(GAME_RIGHT + 24,GAME_CENTER_Y) );
	mt.Suspend(60);
	for( int i = 0; i < 4; i++ ) {
		g_lEnemy.Add( new CEnemyFairy14(GAME_RIGHT + 24,GAME_CENTER_Y - (i+1) * 30) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy14(GAME_RIGHT + 24,GAME_CENTER_Y + (i+1) * 30) );
		mt.Suspend(60);
	}
	mt.Suspend(120);

	for( int i = 0; i < 2; i++ ) {
		g_lEnemy.Add( new CEnemyFairy15(GAME_RIGHT + 24,GAME_CENTER_Y - (5-i) * 30, 0) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy15(GAME_RIGHT + 24,GAME_CENTER_Y + (5-i) * 30, 1) );
		mt.Suspend(60);
	}
	for( int i = 0; i < 2; i++ ) {
		g_lEnemy.Add( new CEnemyFairy14(GAME_RIGHT + 24,GAME_CENTER_Y - (i+1) * 30) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy14(GAME_RIGHT + 24,GAME_CENTER_Y + (i+1) * 30) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy15(GAME_RIGHT + 24,GAME_CENTER_Y - (5-i) * 30, 0) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy15(GAME_RIGHT + 24,GAME_CENTER_Y + (5-i) * 30, 1) );
		mt.Suspend(60);
	}
	mt.Suspend(240);
	TamaToItem();
	mt.Suspend(60);

	//中ボス
	g_lEnemy.Add( new CBossMystiaMiddle() );
	while(!g_lEnemy.IsEmpty()) {
		mt.Suspend(1);
	}

	mt.Suspend(240);

	for( int i = 0; i < 2; i++ ) {
		g_lEnemy.Add( new CEnemyFairy17(GAME_RIGHT + 24,GAME_CENTER_Y - (5-i) * 10) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy17(GAME_RIGHT + 24,GAME_CENTER_Y + (5-i) * 50) );
		mt.Suspend(60);
	}
	mt.Suspend(240);
	for( int i = 0; i < 2; i++ ) {
		g_lEnemy.Add( new CEnemyFairy16(GAME_RIGHT + 24,GAME_CENTER_Y - (5-i) * 30) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy16(GAME_RIGHT + 24,GAME_CENTER_Y + (5-i) * 30) );
		mt.Suspend(60);
	}

	mt.Suspend(500);
	TamaToItem();
	mt.Suspend(60);

	//ボス
	g_lEnemy.Add( new CBossMystia() );
	while(!g_lEnemy.IsEmpty()) {
		mt.Suspend(1);
	}
	mt.Suspend(100);
	return 1;
}
//-------------------妖精14---------------------------------
CEnemyFairy14::CEnemyFairy14(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 150;
	this->color = 4;
}
int CEnemyFairy14::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		if( count > 120 ) {
			if( count % (20-5*GetDifficult()) == 0 ) {
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 13;
				tama.color = 0;
				tama.v = 2.0 + 0.5 * GetDifficult();

				CDanmakuToPlayer d(&tama);
				d.w = 360;
				if( GetDifficult() == DIFF_LUNATIC) {
					d.n = 8;
				}else {
					d.n = GetDifficult() + 1;
				}
				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
		}
	}
	else{
		x += v;
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyFairy14::GetItem(){
	return ITEM_POWER;
}
//打ち返し弾
int CEnemyFairy14::Death() {
	CTamaNormal tama;
	tama.x = GetX();
	tama.y = GetY();
	tama.type = 13;
	tama.color = 0;
	tama.v = 1.0 + 0.2 * GetDifficult();

	CDanmakuHorizon d(&tama);
	d.offset = rand(0, 360);
	if( GetDifficult() == DIFF_LUNATIC) {
		d.n = 16;
		d.m = 8;
		d.w = 1.1;
	}else {
		d.n = 4 * ( GetDifficult() + 1 );
		d.m = 4 + GetDifficult();
		d.w = 2.2 - 0.5 * GetDifficult();
	}
	d.Fire();
	g_pMaterial->sndFire.Play(0);

	return CEnemyBase::Death();
}
//-------------------妖精15---------------------------------
CEnemyFairy15::CEnemyFairy15(double x,double y,int c)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 200;
	this->color = 5;
	this->c = c;
}
int CEnemyFairy15::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		if( count == 60) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = 0;
			tama.v = 1.0 + 0.5 * GetDifficult();

			CDanmakuHorizon d(&tama);
			d.w = 2;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 16;
				d.m = 8;
			}else {
				d.n = 4 * ( GetDifficult() + 1 );
				d.m = 4 + GetDifficult();
			}
			double angle = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			d.offset = angle + c * 360/d.m/2;
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
int CEnemyFairy15::GetItem(){
	return ITEM_POWER;
}
//-------------------妖精16---------------------------------
CEnemyFairy16::CEnemyFairy16(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 150;
	this->color = 4;
}
int CEnemyFairy16::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		if( count%10 == 0) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = 0;
			tama.v = 1.0 + 0.3 * GetDifficult();

			CDanmakuHorizon d(&tama);
			d.w = 1;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 4;
				d.m = 4;
			}else {
				d.n = 1 * ( GetDifficult() + 1 );
				d.m = 1 * ( GetDifficult() + 1 );
			}
			double angle = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			d.offset = angle + 360/d.m/4;
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
int CEnemyFairy16::GetItem(){
	return ITEM_POWER2;
}
//打ち返し弾
int CEnemyFairy16::Death() {
	CTamaNormal tama;
	tama.x = GetX();
	tama.y = GetY();
	tama.type = 13;
	tama.color = 0;
	tama.v = 1.0 + 0.2 * GetDifficult();

	CDanmakuToPlayerLine d(&tama);
	d.sv = 0.5;
	d.ev = 2.0;
	d.w = 30;
	if( GetDifficult() == DIFF_LUNATIC) {
		d.n = 16;
		d.m = 16;
	}else {
		d.m = 2*(1+GetDifficult());
		d.m = 2*(1+GetDifficult());
	}
	d.Fire();
	g_pMaterial->sndFire.Play(0);

	return CEnemyBase::Death();
}
//-------------------妖精17---------------------------------
CEnemyFairy17::CEnemyFairy17(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 150;
	this->color = 5;
}
int CEnemyFairy17::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		if( count == 60) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = 0;
			tama.v = 1.0 + 0.2 * GetDifficult();

			CDanmakuToPlayerLine d(&tama);
			d.w = 30;
			d.sv = 1;
			d.ev = 5;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 3;
				d.m = 32;
			}else {
				d.n = 3;
				d.m = 4*(1+GetDifficult());
			}
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
int CEnemyFairy17::GetItem(){
	return ITEM_SCORE2;
}
//打ち返し弾
int CEnemyFairy17::Death() {
	CTamaNormal tama;
	tama.x = GetX();
	tama.y = GetY();
	tama.type = 13;
	tama.color = 0;
	tama.v = 1.0 + 0.2 * GetDifficult();

	CDanmakuHorizon d(&tama);
	d.offset = rand(0, 360);
	if( GetDifficult() == DIFF_LUNATIC) {
		d.n = 16;
		d.m = 8;
		d.w = 1.1;
	}else {
		d.n = 4 * ( GetDifficult() + 1 );
		d.m = 4 + GetDifficult();
		d.w = 2.2 - 0.5 * GetDifficult();
	}
	d.Fire();
	g_pMaterial->sndFire.Play(0);

	return CEnemyBase::Death();
}
