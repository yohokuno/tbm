#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"


CBossLety *g_pLety = NULL;

//-------------------レティ共通---------------------------------

CBossLetyBase::CBossLetyBase(){
	x = GAME_RIGHT + 80;
	y = GAME_CENTER_Y - GetH()/2;
	barier = 0;
}
int CBossLetyBase::Draw(){
	if( barier ) {
		Sun3D.SetBlendingType( BLEND_ADD );
		g_pStage6->imgBarier.Draw(GetX()-128,GetY()-128);
		Sun3D.SetBlendingType( BLEND_NORMAL );
	}
	int index = (count/7) % 2;
	RECT rect={index*GetW(),0,(index+1)*GetW(),GetH()};
	g_pStage6->imgLety.DrawRect(x,y,rect);
	return CBossBase::Draw();
}
//スペルカード
int CBossLetyBase::SpellCard() {
	flag = 1;
	pBack = g_pBack;
	g_pBack = new CBackLety();
	RECT rect = {0,0,512,512};
	g_lEffect2.Add( new CEffectCutin( &g_pStage6->imgTalk, &rect, GAME_CENTER_X, 120));
	return CBossBase::SpellCard();
}
void CBossLetyBase::Suspend2(double t){this->Suspend(t);}
//----------------------------------------------------
CBossLety::CBossLety(){
	TSunFunctionCallback<CBossLety> fn;
	fn.SetFunction(this,&CBossLety::Thread);
	mt3.Start(&fn,0x8000);
	g_pLety = this;
	freeze = 0;
}
CBossLety::~CBossLety(){
	g_pLety = NULL;
	SunSprite::SetOffset(0,0);
	if( g_pMaterial != NULL )
		g_pMaterial->sndGogo.Stop();
}
int CBossLety::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossLetyBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossLety::Draw(){
	CBossLetyBase::Draw();
	return 1;
}
//全体動作（出現～会話～通常攻撃～スペルカード～撃破）
int CBossLety::Thread(){
	TSunFunctionCallback<CBossLety> fn;
	TSunFunctionCallback<CBossLetyBase> fnb;

	//出現
	flag = 0;
	deffence = 0;

	//会話
	mt3.Suspend();
	g_pPlayer->Talk();
	pTalk = new CTalkLety2();
	pTalk->Start();
	while( !pTalk->IsEnd() )
		mt3.Suspend();
	SAFE_DELETE(pTalk);
	g_pPlayer->Resume();
	fntBoss.CreateList("Lety",12);

	//通常攻撃
	fn.SetFunction(this,&CBossLety::NormalAtack1);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//ホワイトアルバム
	g_pSystem->spell_num = 28;
	SpellCard();
	fn.SetFunction(this,&CBossLety::WhiteAlbum);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();
	barier = 0;

	//通常攻撃
	fn.SetFunction(this,&CBossLety::NormalAtack1);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//アブソリュート・ゼロ
	g_pSystem->spell_num = 29;
	SpellCard();
	fn.SetFunction(this,&CBossLety::AbsoluteZero);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();
	freeze = 0;

	//通常攻撃
	fn.SetFunction(this,&CBossLety::NormalAtack1);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//ハリケーン
	g_pStage6->imgEarth.LoadTexture("img/earth2.png");
	g_pSystem->spell_num = 30;
	SpellCard();
	fn.SetFunction(this,&CBossLety::Hurricane);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//通常攻撃2
	fn.SetFunction(this,&CBossLety::NormalAtack2);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//ツナミパニック
	g_pSystem->spell_num = 31;
	SpellCard();
	fn.SetFunction(this,&CBossLety::Tsunami);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//通常攻撃2
	fn.SetFunction(this,&CBossLety::NormalAtack2);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//アースクエイク
	g_pSystem->spell_num = 32;
	SpellCard();
	fn.SetFunction(this,&CBossLety::EarthQuake);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();
	SunSprite::SetOffset(0,0);
	if( g_pMaterial != NULL )
		g_pMaterial->sndGogo.Stop();

	//通常攻撃2
	fn.SetFunction(this,&CBossLety::NormalAtack2);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//オゾンホール
	g_pStage6->imgEarth.LoadTexture("img/earth3.png");
	g_pSystem->spell_num = 33;
	SpellCard();
	fn.SetFunction(this,&CBossLety::OzoneHole);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//幕間
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt3.Suspend(60);

	//グローバルワーミング
	g_pStage6->imgEarth.LoadTexture("img/earth4.png");
	g_pSystem->spell_num = 34;
	SpellCard();
	fn.SetFunction(this,&CBossLety::GlobalWorming);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//幕間
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt3.Suspend(60);
/*
	//セプテンバーイレブン
	SpellCard();
	fn.SetFunction(this,&CBossLety::SeptemberEleven);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//幕間
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt3.Suspend(60);
*/
	//ニュークリアウィンター
	g_pStage6->imgEarth.LoadTexture("img/earth5.png");
	g_pSystem->spell_num = 35;
	SpellCard();
	fn.SetFunction(this,&CBossLety::NuclearWinter);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//エフェクト
	mt.Stop();
	mt2.Stop();
	flag = 0;
	deffence = 0;
	g_lEffect.Add( new CEffectBreak( GetX(), GetY() ));
	g_pPlayer->Talk();
	mt3.Suspend( 60 );

	//画面外へ
	x=1000;

	mt3.Suspend( 300 );

	//エンディング
	if( g_pGame->mode == 0 ) {
		SAFE_DELETE(g_pBack);
		pTalk = new CEnding();
		pTalk->Start();
		while( !pTalk->IsEnd() )
			mt3.Suspend();
		SAFE_DELETE(pTalk);
	}
	return 1;
}
//通常攻撃1
int CBossLety::NormalAtack1() {
	SetHP(1000);
	SetTime(20);
	flag = 1;
	deffence = 1;
	while( 1 ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			int s = 1 + GetDifficult() + ( GetDifficult() == DIFF_LUNATIC );
			int imax = s * s * 4;
			for( int i = 0; i < imax; ++i ) {
				CTamaTurn tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 10;
				tama.color = 3;
				tama.v = 2;
				tama.v2 = 1;
				tama.stop_count = 60;
				tama.stop_time = 30;
				tama.turn = 360.0*5*i/imax;

				CDanmakuNonDirectional d(&tama);
				int n = 2 + 2 * GetDifficult();
				d.offset = 360.0 * 2 * i / imax / n * (7-GetDifficult()) * (k*2-1);
				d.n = n;
				d.w = 360;
				d.Fire();

				g_pMaterial->sndFire.Play(0);
				mt.Suspend(64.0*3/imax);
			}
			mt.Suspend(60);
			const int move_count = 30;
			int xv,yv;
			if( k ) {
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
			mt.Suspend(60);
		}
	}
	return 1;
}

//通常攻撃2
int CBossLety::NormalAtack2() {
	SetHP(1400);
	SetTime(30);
	flag = 1;
	deffence = 1;
	while( 1 ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			int s = 1 + GetDifficult() + ( GetDifficult() == DIFF_LUNATIC );
			int imax = s * s * 6;
			for( int i = 0; i < imax; ++i ) {
				CTamaTurn tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 13;
				tama.color = 3;
				tama.v = 2;
				tama.v2 = 1;
				tama.stop_count = 60;
				tama.stop_time = 30;
				tama.turn = 360.0*5*i/imax;

				CDanmakuNonDirectional d(&tama);
				int n = 2 + 2 * GetDifficult();
				d.offset = 360.0 * 2 * i / imax / n * (7-GetDifficult()) * (k*2-1);
				d.n = n;
				d.w = 360;
				d.Fire();

				g_pMaterial->sndFire.Play(0);
				mt.Suspend(64.0*3/imax);
			}
			mt.Suspend(60);
			const int move_count = 30;
			int xv,yv;
			if( k ) {
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
			mt.Suspend(60);
		}
	}
	return 1;
}
//ホワイトアルバム
int CBossLety::WhiteAlbum()
{
	deffence = 0;
	SetHP(1500);
	SetTime(50);
	Suspend(60);

	for( int i = 0; i < 60; ++i ) {
		x += ( 450 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	deffence = 1;
	for(int j=0;;j++){
		barier = 1;
		g_pMaterial->sndFire.Play(0);
		int imax = 40 * (1+GetDifficult());
		for( int i = 0;i<imax; i++){
			//ショットとの当たり判定
			RECT myRect = {GetX()-32,GetY()-64,GetX(),GetY()+64};
			list<CShot*>::iterator itr;
			for( itr = g_lShot.begin(); itr != g_lShot.end(); itr++ ) {
				if( *itr != NULL ) {
					RECT shotRect;
					(*itr)->GetHantei(&shotRect);
					if( GetHantei( &shotRect, &myRect )) {
						CTamaTurn tama;
						tama.x = (*itr)->x;
						tama.y = (*itr)->y;
						tama.type = 2;
						tama.stop_count = 0;
						tama.stop_time = 30;
						tama.turn = 0;
						tama.v = 0;
						tama.v2 = 4;
						tama.color = 0;
						double angle = Atan2( g_pPlayer->GetX() - tama.GetX(), g_pPlayer->GetY() - tama.GetY());
						tama.a = angle;
						g_lTama.Add( tama.Copy() );
						g_pMaterial->sndFreeze.Play(0);
						//g_pMaterial->sndFire.Play(0);

						SAFE_DELETE( *itr );
					}
				}
			}
			this->Suspend((double)imax/120);
		}
		barier = 0;
		g_pMaterial->sndMelt.Play(0);
		this->Suspend(60);
		imax = 120 * (1+GetDifficult());
		for( int i = 0;i<imax; i++){
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = 3;
			tama.a = (2*!(j%2)-1)*360.0*2*i/imax;
			tama.v = 1.0+abs(mod((int)tama.a,60)-40+10*(j%3==1))/30.0;
			g_lTama.Add( tama.Copy() );
		}
		g_pMaterial->sndFire.Play(0);
		this->Suspend(60);
		double xx = GAME_RIGHT - rand(120,240);
		double yy = GAME_CENTER_Y + rand( -120,120);
		for( int i = 0; i < 120; ++i ) {
			x += ( xx - GetX() ) / 60;
			y += ( yy - GetY() ) / 60;
			mt.Suspend(1);
		}
	}
	return 1;
}

//アブソリュート・ゼロ
int CBossLety::AbsoluteZero()
{
	deffence = 0;
	SetHP(1000);
	SetTime(40);
	Suspend(60);

	double xx = GetX();
	double yy = GetY();
	for( int i = 0; i < 60; ++i ) {
		x += ( 450 - xx ) / 60;
		y += ( GAME_CENTER_Y - yy ) / 60;
		Suspend(1);
	}
	deffence = 1;
	while(1){
		int imax = (1+GetDifficult());
		for( int i = 0; i < imax; i++ ){
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 16;
			tama.color = 0;
			tama.v = 2.0 + 4.0*i/imax;

			CDanmakuNonDirectional d(&tama);
			int n = 16* (1+GetDifficult());
			if( GetDifficult() == DIFF_LUNATIC )
				n = 120;
			d.offset = 0;
			d.n = n;
			d.w = 360;
			d.Fire();
			g_pMaterial->sndFire.Play(0);

			Suspend(30);
			if( i == 0 ) {
				g_pMaterial->sndFreeze.Play(0);
				freeze = 1;
				deffence = 0;
			}
			TamaFreeze();
		}
		Suspend(60);

		g_pMaterial->sndMelt.Play(0);
		freeze = 0;
		deffence = 1;
		TamaResume(2);
		Suspend(60);

		double xx = GAME_RIGHT - rand(120,240);
		double yy = GAME_CENTER_Y + rand( -120,120);
		for( int i = 0; i < 120; ++i ) {
			x += ( xx - GetX() ) / 60;
			y += ( yy - GetY() ) / 60;
			mt.Suspend(1);
		}

		CTamaNormal tama;
		tama.x = GetX();
		tama.y = GetY();
		tama.type = 4;
		tama.v = 1;
		tama.color = 2;
		double angle = Atan2( g_pPlayer->GetX() - tama.GetX(), g_pPlayer->GetY() - tama.GetY());
		tama.a = angle;
		g_lTama.Add( tama.Copy() );
		g_pMaterial->sndFire.Play(0);
		Suspend(60);

	}
	return 1;
}
/*
//タイフーン
int CBossLety::Typhoon()
{
	deffence = 0;
	SetHP(2500);
	SetTime(60);
	Suspend(60);

	for( int i = 0; i < 60; ++i ) {
		x += ( 450 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	deffence = 1;
	CTamaTyphoon::lTamaTyphoon.clear();
	while(1){
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			int s = 1 + GetDifficult() + ( GetDifficult() == DIFF_LUNATIC );
			int imax = s * s * 8;
			for( int i = 0; i < imax; ++i ) {
				CTamaTyphoon tama;
				tama.x = GetX() + rand(-30,30);
				tama.y = GetY() + rand(-30,30);
				tama.type = 16;
				tama.color = 0;
				tama.v = 0;
				tama.a = 0;
				g_lTama.Add( tama.Copy() );

				g_pMaterial->sndFire.Play(0);
				mt.Suspend(64.0/imax);
			}
			mt.Suspend(60);
			const int move_count = 30;
			int xv,yv;
			if( k ) {
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
			mt.Suspend(60);
			CTamaTyphoon::lTamaTyphoon.clear();
		}
	}
	return 1;
}*/


//ハリケーン
int CBossLety::Hurricane()
{
	deffence = 0;
	SetHP(2000);
	SetTime(60);
	Suspend(60);

	for( int i = 0; i < 60; ++i ) {
		x += ( 450 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	deffence = 1;
	while(1){
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			int s = 1 + GetDifficult() + ( GetDifficult() == DIFF_LUNATIC );
			int imax = s * s;
			for( int i = 0; i < imax; ++i ) {
				CTamaHurricane tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 16;
				tama.color = 0;
				tama.v = 2.0+0.2*GetDifficult();
				tama.c = 10 - 2 * GetDifficult();
				tama.t = k*2-1;

				CDanmakuNonDirectional d(&tama);
				d.n = 4*( 1+GetDifficult());
				d.Fire();
				g_pMaterial->sndFire.Play(0);

				mt.Suspend(120.0/imax);
			}
			mt.Suspend(60);
			{
				CTamaNormal t;
				t.x = GetX();
				t.y = GetY();
				t.type = 4;
				t.color = 2;
				t.v = 2.0;

				CDanmakuNonDirectionalLine d(&t);
				d.n = 8*(1+GetDifficult());
				if( GetDifficult() == DIFF_LUNATIC )
					d.n = 48;
				d.m = 1+GetDifficult();
				d.sv = 2.0-0.4*GetDifficult();
				d.ev = 2.0+0.4*GetDifficult();
				d.w = 360;
				d.offset = 0;
				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
			mt.Suspend(60);
			double xx = GAME_RIGHT - rand(120,240);
			double yy = GAME_CENTER_Y + rand( -120,120);
			for( int i = 0; i < 120; ++i ) {
				x += ( xx - GetX() ) / 60;
				y += ( yy - GetY() ) / 60;
				mt.Suspend(1);
			}
			mt.Suspend(60);
		}
	}
	return 1;
}

//ツナミ
int CBossLety::Tsunami()
{
	deffence = 0;
	SetHP(2500);
	SetTime(70);
	Suspend(60);

	for( int i = 0; i < 60; ++i ) {
		x += ( 450 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	deffence = 1;
	while(1){
		int s = 1 + GetDifficult() + ( GetDifficult() == DIFF_LUNATIC );
		int imax = s * s * 4;
		for( int j = 0; j < 3; j ++ ){
			for( int i = 0; i < imax; i++){
				CTamaTsunami t;
				t.x = GetX();
				t.y = GetY();
				t.type = 16;
				t.color = 3;
				t.SetVelocityXY( 2.5+rand(0,1), ((double)i/imax-0.5)*5.0 );
				t.end = 60;
				t.ax = -0.1;
				t.ay = 0.0;
				g_lTama.Add( t.Copy() );
			}
			g_pMaterial->sndFire.Play(0);
			mt.Suspend(60);
		}
		mt.Suspend(120);
		{
			CTamaNormal t;
			t.x = GetX();
			t.y = GetY();
			t.type = 4;
			t.color = 2;
			t.v = 2.0;

			CDanmakuNonDirectionalLine d(&t);
			d.n = 8*(1+GetDifficult());
			if( GetDifficult() == DIFF_LUNATIC )
				d.n = 56;
			d.m = 1+GetDifficult();
			d.sv = 2.0-0.4*GetDifficult();
			d.ev = 2.0+0.4*GetDifficult();
			d.w = 360;
			d.offset = 0;
			d.Fire();
			g_pMaterial->sndFire.Play(0);
		}
		mt.Suspend(120);
		{
			CTamaNormal t;
			t.x = GetX();
			t.y = GetY();
			t.type = 4;
			t.color = 2;
			t.v = 2.0;

			CDanmakuNonDirectionalLine d(&t);
			d.n = 8*(1+GetDifficult());
			if( GetDifficult() == DIFF_LUNATIC )
				d.n = 56;
			d.m = 1+GetDifficult();
			d.sv = 2.0-0.4*GetDifficult();
			d.ev = 2.0+0.4*GetDifficult();
			d.w = 360;
			d.offset = 0;
			d.Fire();
			g_pMaterial->sndFire.Play(0);
		}
		mt.Suspend(120);
		double xx = GAME_RIGHT - rand(120,240);
		double yy = GAME_CENTER_Y + rand( -120,120);
		for( int i = 0; i < 120; ++i ) {
			x += ( xx - GetX() ) / 60;
			y += ( yy - GetY() ) / 60;
			mt.Suspend(1);
		}
		mt.Suspend(120);
	}
	return 1;
}

//アースクエイク
int CBossLety::EarthQuake()
{
	deffence = 0;
	SetHP(3000);
	SetTime(80);
	Suspend(60);

	for( int i = 0; i < 60; ++i ) {
		x += ( 450 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	deffence = 1;
	while(1){
		if( g_pMaterial != NULL )
			g_pMaterial->sndGogo.Play(DSBPLAY_LOOPING);
		int imax = 480;
		for( int i = 0; i < imax; i++){
			//ランダムに画面を揺らす
			double rx = rand(-5,5);
			double ry = rand(-5,5);
			SunSprite::SetOffset(rx,ry);
			this->Suspend(1);
			if( i > 60 && i % 60 == 0 ) {
				CTamaTurn tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 16;
				tama.color = 3;
				tama.v = 2*(1.0+(double)i/imax);
				tama.v2 = 1.0+(double)i/imax;
				tama.stop_count = 60;
				tama.stop_time = 30;
				tama.turn = 0;

				CDanmakuNonDirectional d(&tama);
				int n = 24*(1+ GetDifficult());
				d.offset = rand(0,360);
				d.n = n;
				d.w = 360;
				d.Fire();

				g_pMaterial->sndFire.Play(0);
			}
		}
		SunSprite::SetOffset(0,0);
		if( g_pMaterial != NULL )
			g_pMaterial->sndGogo.Stop();
		mt.Suspend(120);
		{
			CTamaNormal t;
			t.x = GetX();
			t.y = GetY();
			t.type = 4;
			t.color = 2;
			t.v = 2.0;

			CDanmakuNonDirectionalLine d(&t);
			d.n = 8*(1+GetDifficult());
			if( GetDifficult() == DIFF_LUNATIC )
				d.n = 56;
			d.m = 1+GetDifficult();
			d.sv = 2.0-0.4*GetDifficult();
			d.ev = 2.0+0.4*GetDifficult();
			d.w = 360;
			d.offset = 0;
			d.Fire();
			g_pMaterial->sndFire.Play(0);
		}
		double xx = GAME_RIGHT - rand(120,240);
		double yy = GAME_CENTER_Y + rand( -120,120);
		for( int i = 0; i < 120; ++i ) {
			x += ( xx - GetX() ) / 60;
			y += ( yy - GetY() ) / 60;
			mt.Suspend(1);
		}
		{
			CTamaNormal t;
			t.x = GetX();
			t.y = GetY();
			t.type = 4;
			t.color = 2;
			t.v = 2.0;

			CDanmakuNonDirectionalLine d(&t);
			d.n = 8*(1+GetDifficult());
			if( GetDifficult() == DIFF_LUNATIC )
				d.n = 56;
			d.m = 1+GetDifficult();
			d.sv = 2.0-0.4*GetDifficult();
			d.ev = 2.0+0.4*GetDifficult();
			d.w = 360;
			d.offset = 0;
			d.Fire();
			g_pMaterial->sndFire.Play(0);
		}
		mt.Suspend(120);
	}
	return 1;
}

//オゾンホール
int CBossLety::OzoneHole()
{
	deffence = 0;
	SetHP(2000);
	SetTime(60);
	Suspend(60);

	double xx = 450 - GetX();
	double yy = GAME_CENTER_Y - GetY();
	for( int i = 0; i < 60; ++i ) {
		x += xx / 60;
		y += yy / 60;
		mt.Suspend(1);
	}
	deffence = 1;
	while(1){
		for( int i = 0;; i++){
			{
				CTamaNormal t;
				t.x = GetX();
				t.y = GetY();
				t.type = 16;
				t.color = 3;
				t.v = 2.0;

				CDanmakuToPlayerLine d(&t);
				d.n = 48*(1+GetDifficult());
				d.m = 1+i;
				if( i > 4 )
					d.m = 4;
				d.sv = 2.0-0.2*GetDifficult();
				d.ev = 2.0+0.2*GetDifficult();
				d.w = 180 - 10 / (1+GetDifficult());
				d.offset = 180;
				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
			this->Suspend(60+30*GetDifficult());
			{
				CTamaNormal t;
				t.x = GetX();
				t.y = GetY();
				t.type = 4;
				t.color = 1;
				t.v = 2.0;

				CDanmakuToPlayerLine d(&t);
				d.n = 1;
				d.m = 2*(1+GetDifficult());
				d.sv = 2.0-0.2*GetDifficult();
				d.ev = 2.0+0.2*GetDifficult();
				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
			this->Suspend(180-30*GetDifficult());
		}
	}
	return 1;
}
/*
//セプテンバーイレブン
int CBossLety::SeptemberEleven()
{
	deffence = 0;
	SetHP(1500);
	SetTime(40);
	Suspend(60);

	for( int i = 0; i < 60; ++i ) {
		x += ( 450 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	deffence = 1;
	while(1){
		for( int i = 0;; i++){
			this->Suspend(120);
		}
	}
	return 1;
}*/

//グローバルワーミング
int CBossLety::GlobalWorming()
{
	deffence = 0;
	SetHP(3000);
	SetTime(80);
	Suspend(60);

	double xx = 450 - GetX();
	double yy = GAME_CENTER_Y - GetY();
	for( int i = 0; i < 60; ++i ) {
		x += xx / 60;
		y += yy / 60;
		mt.Suspend(1);
	}
	deffence = 1;
	//温室効果ガス
	int t = 5*(5-GetDifficult());
	CTamaNormal tama;
	tama.type = 2;
	tama.v = 0;
	tama.color = 0;
	for( int i = 0; i < GAME_WIDTH/t+1; i++ ){

		tama.x = GAME_LEFT + i * t;
		tama.y = GAME_TOP;
		tama.a = 90;
		g_lTama.Add( tama.Copy() );

		tama.x = GAME_LEFT + i * t;
		tama.y = GAME_BOTTOM;
		tama.a = -90;
		g_lTama.Add( tama.Copy() );
	}
	for( int i = 0; i < GAME_HEIGHT/t+1; i++ ){
		tama.x = GAME_RIGHT;
		tama.y = GAME_TOP + i * t;
		tama.a = 180;
		g_lTama.Add( tama.Copy() );

		tama.x = GAME_LEFT;
		tama.y = GAME_TOP + i * t;
		tama.a = 0;
		g_lTama.Add( tama.Copy() );
	}
	g_pMaterial->sndFire.Play(0);
	Suspend(120);
	//反射弾
	while(1){
		for( int i = 0;; i++){
			{
				CTamaReflect t;
				t.x = GetX();
				t.y = GetY();
				t.type = 3;
				t.color = 1;

				CDanmakuToPlayerLine d(&t);
				d.w = 180;
				d.n = 8 * (1+GetDifficult()) - 1;
				if( GetDifficult() == DIFF_LUNATIC )
					d.n = 47;
				d.m = 1;// * (1+GetDifficult());
				d.sv = 1.0;//-0.2*GetDifficult();
				d.ev = 1.0;//+0.2*GetDifficult();
				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
			this->Suspend(120);
		}
	}
	return 1;
}

//ニュークリアウィンター
int CBossLety::NuclearWinter()
{
	deffence = 0;
	SetHP(5000);
	SetTime(99);
	Suspend(60);

	double xx = 350 - GetX();
	double yy = GAME_CENTER_Y - GetY();
	for( int i = 0; i < 60; ++i ) {
		x += xx / 60;
		y += yy / 60;
		mt.Suspend(1);
	}
	Suspend(120);
	deffence = 1;
	while(1){
		for( int i = 0;; i++){
			int kmax = 4 + 2*GetDifficult();
			for( int k = 0; k < kmax; k++){
				int jmax = 3;
				for( int j = 0; j < jmax; j++){
					CTamaNormal t;
					t.x = GetX();
					t.y = GetY();
					t.type = 2;
					t.color = 0;

					CDanmakuNonDirectionalLine d(&t);
					d.n = 6*(1+GetDifficult());
					d.m = 1;
					d.sv = 2.0-0.2*GetDifficult();
					d.ev = 2.0+0.2*GetDifficult();
					d.w = 180.0 / jmax / 2;
					d.offset = 120 * j + 60 * i;
					d.Fire();
					g_pMaterial->sndFire.Play(0);
				}
				this->Suspend(100.0/kmax);
			}
			if( i > 3 ) {
				CTamaNormal t;
				t.x = GetX();
				t.y = GetY();
				t.type = 4;
				t.color = 1;
				t.v = 2.0;

				CDanmakuToPlayerLine d(&t);
				d.w = 120;
				d.n = 3;
				d.m = 2*(1+GetDifficult());
				d.sv = 2.0-0.2*GetDifficult();
				d.ev = 2.0+0.2*GetDifficult();
				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
		}
	}
	return 1;
}
