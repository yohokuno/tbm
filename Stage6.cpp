#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//ステージ6
CStage6 *g_pStage6 = NULL;

CStage6::CStage6() :
imgLety("img/lety.png"),
imgEarth("img/earth.png"),
imgEarthFreeze("img/earth_freeze.png"),
imgBack("img/lety_back.png"),
imgTalk("img/lety_talk.png"),
imgBarier("img/barier.png")
{
	g_pStage6 = this;
	g_pBack		= new CBackStage6();

	TSunFunctionCallback<CStage6> fn;
	fn.SetFunction(this,&CStage6::EnemyApear);
	mt.Start(&fn,0x8000);
}
CStage6::~CStage6(){
	g_pStage6 = NULL;
}
int CStage6::Run(){
	return CStage::Run();
}
int CStage6::EnemyApear()
{

	g_pSystem->spell_num = 27;

	PlayBGM("tbm_stage6");
	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("Stage6",stageFontColor1,320,210,2,0));
	g_lEffect3.Add( new CEffectFont("白夜幻想譚",stageFontColor2,320,230,2,0));
	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("♪北極点幻想",bgmFontColor,380,420) );
	mt.Suspend(30);

	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama6(GAME_RIGHT - 40,GAME_TOP - 16,2));
		mt.Suspend(40);
	}
	mt.Suspend(60);
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama6(GAME_RIGHT - 40,GAME_BOTTOM + 16,-2));
		mt.Suspend(40);
	}
	mt.Suspend(60);
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama6(GAME_RIGHT - 40,GAME_TOP - 16,2));
		mt.Suspend(40);
	}
	mt.Suspend(60);
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama6(GAME_RIGHT - 40,GAME_BOTTOM + 16,-2));
		mt.Suspend(40);
	}
	mt.Suspend(60);
	mt.Suspend(240);
	TamaToItem();
	mt.Suspend(60);

	//中ボス
	g_lEnemy.Add( new CBossChirnoMiddle2() );
	while(!g_lEnemy.IsEmpty()) {
		mt.Suspend(1);
	}
	mt.Suspend(240);

	//ボス
	g_lEnemy.Add( new CBossLety() );
	while(!g_lEnemy.IsEmpty()) {
		mt.Suspend(1);
	}

	return 1;
}

//-------------------毛玉6---------------------------------
CEnemyKedama6::CEnemyKedama6(double x,double y,double vy)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 80;
	this->color = 0;
	this->vy = vy;
}
int CEnemyKedama6::Run()
{
	y += vy;
	int interval = 20*(4-GetDifficult());
	if( count%interval == 0) {
		CTamaNormal tama;
		tama.x = GetX();
		tama.y = GetY();
		tama.type = 0;
		tama.color = 0;

		CDanmakuToPlayerLine d(&tama);
		d.sv = 2.0 - 0.2 * GetDifficult();;
		d.ev = 2.0 + 0.2 * GetDifficult();
		d.n = 1;
		if( GetDifficult() == DIFF_LUNATIC) {
			d.m = 16;
		}else {
			d.m = 2*(1+GetDifficult());
		}
		d.Fire();
		g_pMaterial->sndFire.Play(0);
	}
	return CEnemyBase::Run();
}
//アイテム
int CEnemyKedama6::GetItem(){
	return ITEM_SCORE2;
}
//------------チルノ２(宣言はChirno.hで)
CBossChirnoMiddle2 *g_pChirno2 = NULL;

CBossChirnoMiddle2::CBossChirnoMiddle2():
	imgChirno("img/chirno.png"),
	imgBack1("img/chirno_back1.png"),
	imgBack2("img/chirno_back2.png"),
	imgTalk("img/chirno_talk.png")
{
	x = GAME_RIGHT + 80;
	y = GAME_CENTER_Y - GetH()/2;
	g_pChirno2 = this;

	TSunFunctionCallback<CBossChirnoMiddle2> fn;
	fn.SetFunction(this,&CBossChirnoMiddle2::Thread);
	mt3.Start(&fn,0x8000);
}
CBossChirnoMiddle2::~CBossChirnoMiddle2(){
	g_pChirno2 = NULL;
}
int CBossChirnoMiddle2::Draw(){
	int index = (count/7) % 2;
	RECT rect={index*GetW(),0,(index+1)*GetW(),GetH()};
	this->imgChirno.DrawRect(x,y,rect);
	return CBossBase::Draw();
}
int CBossChirnoMiddle2::SpellCard() {
	flag = 1;
	pBack = g_pBack;
	g_pBack = new CBackChirno2();
	RECT rect = {0,0,296,448};
	g_lEffect2.Add( new CEffectCutin( &this->imgTalk, &rect, GAME_CENTER_X, 120));
	return CBossBase::SpellCard();
}
int CBossChirnoMiddle2::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
//全体動作
int CBossChirnoMiddle2::Thread(){
	TSunFunctionCallback<CBossChirnoMiddle2> fn;

	//出現
	flag = 0;
	deffence = 0;
	for( int i = 0; i < 46; i++ ){
		x -= 5;
		mt3.Suspend();
	}

	//会話
	{
		g_pPlayer->Talk();
		pTalk = new CTalkChirno4();
		pTalk->Start();
		while( !pTalk->IsEnd() )
			mt3.Suspend();
		SAFE_DELETE(pTalk);
		g_pPlayer->Resume();
	}

	//通常攻撃
	fn.SetFunction(this,&CBossChirnoMiddle2::NormalAtack);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//井の中の蛙
	SpellCard();
	fn.SetFunction(this,&CBossChirnoMiddle2::Kawazu);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//エフェクト
	flag = 0;
	deffence = 0;
	AddItem( ITEM_1UP, 1 );
	g_lEffect.Add( new CEffectBreak( GetX(), GetY() ));
	mt3.Suspend( 60 );

	//画面外へ
	x=1000;
	mt3.Suspend( 60 );

	return 1;
}

//通常攻撃
int CBossChirnoMiddle2::NormalAtack() {
	SetHP(1000);
	SetTime(20);
	flag = 1;
	deffence = 1;
	while( 1 ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			int s = 1 + GetDifficult();
			int imax = s * s * 8;
			for( int i = 0; i < imax; ++i ) {
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 10;
				tama.color = 3;
				tama.v = 2;

				CDanmakuNonDirectional d(&tama);
				int n = 2 + 2 * GetDifficult();
				d.offset = 360 * i / imax / n * (7-GetDifficult()) * (k*2-1);
				d.n = n;
				d.w = 360;
				d.Fire();

				d.offset = -d.offset;
				d.Fire();

				g_pMaterial->sndFire.Play(0);
				
				mt.Suspend(64.0*3/imax);
				//mt.Suspend(1+DIFF_LUNATIC-GetDifficult());
			}
			//移動
			const int move_count = 30;
			int xv,yv;
			if( !k ) {
				xv = 400 - GetX();
				yv = GAME_TOP + 120 - GetY();
			}
			else {
				xv = 500 - GetX();
				yv = GAME_BOTTOM - 120 - GetY();
			}
			for( int i = 0; i < move_count; i++ ) {
				x += xv / move_count;
				y += yv / move_count;
				mt.Suspend(1);
			}
			mt.Suspend(60 + 20 * GetDifficult());
		}
	}
	return 1;
}
//蛙符「井の中の蛙」
int CBossChirnoMiddle2::Kawazu() {
	SetHP(2000);
	SetTime(40);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 550 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	deffence = 1;
	while( 1 ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			mt.Suspend(60);
			//置き弾
			CTamaSet tama;
			tama.type = 2;
			tama.color = 0;
			tama.v = 3;
			tama.c = 1;
			tama.t = 0;
			tama.set_interval = 23 - 2*GetDifficult()*GetDifficult();

			tama.x = GAME_LEFT;
			tama.y = g_pPlayer->GetY() + 120;
			tama.a = 0;
			g_lTama.Add(tama.Copy());

			tama.x = GAME_RIGHT;
			tama.y = g_pPlayer->GetY() - 120;
			tama.a = 180;
			g_lTama.Add(tama.Copy());

			tama.x = g_pPlayer->GetX() + 120;
			tama.y = GAME_TOP;
			tama.a = 90;
			g_lTama.Add(tama.Copy());

			tama.x = g_pPlayer->GetX() - 120;
			tama.y = GAME_BOTTOM;
			tama.a = -90;
			g_lTama.Add(tama.Copy());

			g_pMaterial->sndFire.Play(0);
			mt.Suspend(240);

			//フリーズ弾
			{
				double angle = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
				CTamaFreeze tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 2;
				tama.color = 5;
				tama.v = 1;
				tama.t = 100;
				tama.c = 120;
				tama.r = 1;

				CDanmakuCircleLine d(&tama);
				d.aa = angle;
				d.vv = 2;
				d.sv = 0.2 + 0.1 * (DIFF_LUNATIC - GetDifficult());
				d.ev = 1;
				if( GetDifficult() == DIFF_LUNATIC) {
					d.n = 32;
					d.m = 7;
				}else {
					d.n = 4 * ( GetDifficult() + 1 );
					d.m = 2 + GetDifficult();
				}
				d.Fire();
				if( GetDifficult() >= DIFF_HARD ) {
					d.m = 1;
					d.n = 8;
					d.vv = 3;
					d.sv = d.ev = 0.2;
					d.aa += 10;
					d.Fire();
					d.aa += -20;
					d.Fire();
				}
				g_pMaterial->sndFire.Play(0);
				mt.Suspend(30);
			}
			mt.Suspend(70);
			g_pMaterial->sndFreeze.Play(0);
			mt.Suspend(30);
			//移動
			const int move_count = 30;
			int xv,yv;
			if( k % 2 ) {
				xv = 500 - GetX();
				yv = GAME_TOP + 120 - GetY();
			}
			else {
				xv = 500 - GetX();
				yv = GAME_BOTTOM - 120 - GetY();
			}
			for( int i = 0; i < move_count; i++ ) {
				x += xv / move_count;
				y += yv / move_count;
				mt.Suspend(1);
			}
			mt.Suspend(60);
			g_pMaterial->sndMelt.Play(0);
		}
		g_pMaterial->sndMelt.Play(0);
		TamaRandom(0.5);
	}
	return 1;
}
