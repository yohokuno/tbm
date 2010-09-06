#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//-------------------チルノ共通---------------------------------

CBossChirnoBase::CBossChirnoBase(){
	x = GAME_RIGHT + 80;
	y = GAME_CENTER_Y - GetH()/2;
}
int CBossChirnoBase::Draw(){
	int index = (count/7) % 2;
	RECT rect={index*GetW(),0,(index+1)*GetW(),GetH()};
	g_pStage3->imgChirno.DrawRect(x,y,rect);
	return CBossBase::Draw();
}

//スペルカード
int CBossChirnoBase::SpellCard() {
	flag = 1;
	pBack = g_pBack;
	g_pBack = new CBackChirno();
	RECT rect = {0,0,296,448};
	g_lEffect2.Add( new CEffectCutin( &g_pStage3->imgTalk, &rect, GAME_CENTER_X, 120));
	return CBossBase::SpellCard();
}
//近付き防止
int CBossChirnoBase::DontCome() {
	mt2.Suspend(120);
	while(1){
		if( pow2( g_pPlayer->GetX() - GetX() ) + pow2( g_pPlayer->GetY() - GetY() )
			< pow2( 80 ) ){
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 1;
				tama.color = 3;

				CDanmakuToPlayerLine d(&tama);
				d.w= 360;
				d.n= 16;
				d.m = 5.0;
				d.sv = 5.0;
				d.ev = 1;

				d.Fire();
		}
		mt2.Suspend(30);
	}
	return 1;
}
//-------------------中ボス---------------------------------
CBossChirnoMiddle::CBossChirnoMiddle(){
	TSunFunctionCallback<CBossChirnoMiddle> fn;
	fn.SetFunction(this,&CBossChirnoMiddle::Thread);
	mt3.Start(&fn,0x8000);
}
int CBossChirnoMiddle::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossChirnoBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossChirnoMiddle::Draw(){
	CBossChirnoBase::Draw();
	return 1;
}
//全体動作
int CBossChirnoMiddle::Thread(){
	TSunFunctionCallback<CBossChirnoMiddle> fn;
	TSunFunctionCallback<CBossChirnoBase> fnb;
	//出現
	flag = 0;
	deffence = 0;

	//会話
	{
		g_pPlayer->Talk();
		pTalk = new CTalkChirno1();
		pTalk->Start();
		while( !pTalk->IsEnd() )
			mt3.Suspend();
		SAFE_DELETE(pTalk);
		g_pPlayer->Resume();
	}

	//通常攻撃
	fn.SetFunction(this,&CBossChirnoMiddle::NormalAtack1);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//１／３アタック
	SpellCard();
	fn.SetFunction(this,&CBossChirnoMiddle::FrogRefrigerator);
	mt.Start(&fn,0x8000);
	fnb.SetFunction(this,&CBossChirnoBase::DontCome);
	mt2.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();
	mt2.Stop();

	//エフェクト
	flag = 0;
	deffence = 0;
	g_lEffect.Add( new CEffectBreak( GetX(), GetY() ));
	AddItem( ITEM_BOMB, 1 );
	mt3.Suspend( 60 );

	//退却
	for( int i = 0; i < 46; i++ ){
		x += 5;
		mt3.Suspend();
	}
	return 1;
}

//通常攻撃1
int CBossChirnoBase::NormalAtack1() {
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
				d.offset = 360 * i / imax / n / 3 * 3 * (7-GetDifficult()) * (k*2-1);
				d.n = n;
				d.w = 360;
				d.Fire();

				d.offset = -d.offset;
				d.Fire();

				g_pMaterial->sndFire.Play(0);
				mt.Suspend(64.0*3/imax);
				//mt.Suspend(1+DIFF_LUNATIC-GetDifficult());
			}
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
			mt.Suspend(60 + 20 * GetDifficult());
		}
	}
	return 1;
}
//蛙符「１／３アタック」
int CBossChirnoMiddle::FrogRefrigerator() {
	SetHP(1400);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 550 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	while( 1 ) {
		deffence = 1;
		mt.Suspend(60);
		for( int k = 0;; k++ ) {
			//フリーズ弾
			const int jmax = 3;
			int jrand = rand(0,3);
			double angle = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			for( int j = 0; j < jmax; j++ ) {
				CTamaFreeze tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 2;
				tama.color = 5;
				tama.v = 1;
				tama.t = 120;
				tama.c = 240;
				tama.r = ( j == jrand );

				CDanmakuCircleLine d(&tama);
				d.aa = angle + 60*(j-(jmax-1)/2);
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
				if( k > 0 )
					g_pMaterial->sndMelt.Play(0);

				mt.Suspend(30);
			}
			mt.Suspend(30);
			for( int i = 0; i < 3; i++ ) {
				g_pMaterial->sndFreeze.Play(0);
				mt.Suspend(30);
			}
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
			mt.Suspend(120);
		}
	}
	return 1;
}

//----------------------------------------------------
CBossChirno::CBossChirno(){
	TSunFunctionCallback<CBossChirno> fn;
	fn.SetFunction(this,&CBossChirno::Thread);
	mt3.Start(&fn,0x8000);
}
int CBossChirno::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossChirnoBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossChirno::Draw(){
	CBossChirnoBase::Draw();
	return 1;
}
//全体動作（出現～会話～通常攻撃～スペルカード～撃破）
int CBossChirno::Thread(){
	TSunFunctionCallback<CBossChirno> fn;
	TSunFunctionCallback<CBossChirnoBase> fnb;

	flag = 0;
	deffence = 0;

	//会話
	g_pPlayer->Talk();
	pTalk = new CTalkChirno2();
	pTalk->Start();
	while( !pTalk->IsEnd() )
		mt3.Suspend();
	SAFE_DELETE(pTalk);
	g_pPlayer->Resume();
	
	flag = 1;
	fntBoss.CreateList("Chirno",12);

	//通常攻撃１
	fnb.SetFunction(this,&CBossChirno::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();


	//冷符「セ氏０℃」
	SpellCard();
	fn.SetFunction(this,&CBossChirno::Sesi);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();

	//通常攻撃１
	fnb.SetFunction(this,&CBossChirno::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();

	//氷符「アイスソード」
	SpellCard();
	fn.SetFunction(this,&CBossChirno::IceSword);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();

	//通常攻撃２
	fn.SetFunction(this,&CBossChirno::NormalAtack2);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();

	//霜符「フロストボックス」
	SpellCard();
	fn.SetFunction(this,&CBossChirno::Frost);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();

	//通常攻撃２
	fn.SetFunction(this,&CBossChirno::NormalAtack2);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();

	//花符「フローズンフラワー」
	SpellCard();
	fn.SetFunction(this,&CBossChirno::Flower);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();

	//幕間
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt2.Stop();
	mt3.Suspend(60);

	//雪符「コッホスノーフレーク」
	SpellCard();
	fn.SetFunction(this,&CBossChirno::Koch);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();

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

	//会話
	pTalk = new CTalkChirno3();
	pTalk->Start();
	while( !pTalk->IsEnd() )
	mt3.Suspend();
	SAFE_DELETE(pTalk);

	return 1;
}

//通常攻撃2
int CBossChirno::NormalAtack2() {
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
//冷符「セ氏０℃」
int CBossChirno::Sesi() {
	SetHP(1800);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 550 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	while( 1 ) {
		deffence = 1;
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			int imax = 40 + 120 * GetDifficult() * GetDifficult();
			CTamaFreeze tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 2;
			tama.color = 1;
			tama.v = 3;
			tama.r = 2;
			tama.t = 30;
			tama.c = 120;
			for( int i = 0; i < imax; i++ )
			{
				CTama *p = tama.Copy();
				p->color = rand(1,7);
				p->a = rand(0,360);
				g_lTama.Add( p );
		
				g_pMaterial->sndFire.Play(0);
				mt.Suspend(300.0/imax);
			}
			mt.Suspend(240);
			//全方位
			{
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 2;
				tama.color = 3;

				CDanmakuNonDirectionalLine d(&tama);
				if( GetDifficult() == DIFF_LUNATIC) {
					d.n = 94;
					d.m = 10.0;
					d.sv = 2.0;
					d.ev = 0.5;
				}else {
					d.n = 16 * GetDifficult() + 8;
					d.m = 1 + 2 * GetDifficult();
					d.sv = 1.5 + 0.25 * GetDifficult();
					d.ev = d.sv - 1;
				}
				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
			mt.Suspend(60);
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
			mt.Suspend(120);
		}
	}
	return 1;
}

//氷符「アイスソード」
int CBossChirno::IceSword() {
	SetHP(1600);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 550 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	deffence = 1;
	while( 1 ) {
		for( int k = 0; k < 200; k++ ) {
			//攻撃
			double angle;
			if( k == 0 )
				angle = 180;
			else
				angle = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			CTamaNormal tmp;
			tmp.x = GetX();
			tmp.y = GetY();
			tmp.type = 2;
			tmp.color = 3;
			tmp.v = 3;
			int imax = 50 * GetDifficult() / 3 + 10;
			double s = 120.0 / imax;
			for( int i = 0; i < imax; i++ ) {
				int jmax = GetDifficult()*2+1;
				double t = 56.0 / jmax;
				for( int j = 0; j < jmax; j++ ) {
					if( abs(t * (j-jmax/2)) <= (double)56*5/2*i/imax ) {
						CTama*pTama = tmp.Copy();
						pTama->a = angle;
						pTama->x += t * (j-jmax/2) * Cos(pTama->a + 90);
						pTama->y += t * (j-jmax/2) * Sin(pTama->a + 90);
						g_lTama.Add(pTama);
					}
				}
				for( int j = 0; j < jmax; j++ ) {
					if( (double) i / imax > 2.0/3.0 ) {
						{
							CTama*pTama = tmp.Copy();
							pTama->color = 3;
							pTama->a = angle - 180;
							pTama->x += t * (j-jmax/2) * Cos(pTama->a + 90);
							pTama->y += t * (j-jmax/2) * Sin(pTama->a + 90);
							g_lTama.Add(pTama);
						}
						{
							CTama*pTama = tmp.Copy();
							pTama->color = 3;
							pTama->a = angle + 90;
							pTama->x += t * (j-jmax/2) * Cos(pTama->a + 90);
							pTama->y += t * (j-jmax/2) * Sin(pTama->a + 90);
							g_lTama.Add(pTama);
						}
						{
							CTama*pTama = tmp.Copy();
							pTama->color = 3;
							pTama->a = angle - 90;
							pTama->x += t * (j-jmax/2) * Cos(pTama->a + 90);
							pTama->y += t * (j-jmax/2) * Sin(pTama->a + 90);
							g_lTama.Add(pTama);
						}
					}
				}
				g_pMaterial->sndFire.Play(0);
				mt.Suspend(s);
			}
			TamaFreeze();
			g_pMaterial->sndFreeze.Play(0);
			mt.Suspend(120);
			TamaRandom(1);
			g_pMaterial->sndMelt.Play(0);
			mt.Suspend(120);
			mt.Suspend(60);

			//移動
			const int move_count = 30;
			int xv,yv;
			xv = 0;
			if( GetY() < g_pPlayer->GetY() )
				yv = GAME_TOP + 120 - GetY();
			else
				yv = GAME_BOTTOM - 120 - GetY();

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

//霜符「フロストボックス」
int CBossChirno::Frost() {
	SetHP(2000);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 420 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	deffence = 1;
	while( 1 ) {
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			int imax = (GetDifficult() == DIFF_LUNATIC) ?
				220*4 :
				60+220*GetDifficult();
			CTamaFrost tmp;
			tmp.x = GetX();
			tmp.y = GetY();
			tmp.type = 2;
			for(int i = 0; i < imax; i++ ) {
				tmp.v = 1.0 + 2.0 * ( imax - i ) / imax;
				CTama *p = tmp.Copy();
				p->color = rand(1,6);
				p->a = (1-pow2(rand(0,1))) * 180 + pow2(rand(0,1)) * 180;
				g_lTama.Add( p );

				g_pMaterial->sndFire.Play(0);
				mt.Suspend(360.0/imax);
			}
			mt.Suspend(240);
			//自機狙い
			{
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 3;
				tama.color = 3;

				CDanmakuToPlayerLine d(&tama);
				d.w = 60;
				if( GetDifficult() == DIFF_LUNATIC) {
					d.n = 17;
					d.m = 8;
					d.sv = 4;
					d.ev = 2;
				}else {
					d.n = 2 * ( GetDifficult() + 1) + 1;
					d.m = 1 + GetDifficult();
					d.sv = 4;
					d.ev = 2;
				}
				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
			mt.Suspend(260);
			CTamaFrost::lTamaFrost.clear();
			TamaFreeze();
			TamaRandom(0.5);
			g_pMaterial->sndMelt.Play(0);
			mt.Suspend(360);
		}
	}
	return 1;
}

//花符「フローズンフラワー」
int CBossChirno::Flower() {
	SetHP(1500);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 420 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	deffence = 1;
	int i = 0;
	double of = 0;
	while( 1 ) {
		for( int k = 0; ; k++ ) {
			//攻撃
			CTamaFrost tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.v = 6.0;
			tama.type = 2;
			tama.color = k % 6 + 1;
			tama.mode = 2;
			tama.frost = 16 - (GetDifficult()+1) * 2;
			double t = 60;
			int imax;
			int n;
			switch(GetDifficult()) {
			case DIFF_LUNATIC:
				imax = 24;
				tama.frost = 14;
				n = 8;
				break;
			case DIFF_HARD:
				imax = 16;
				tama.frost = 18;
				n = 6;
				break;
			case DIFF_NORMAL:
				imax = 12;
				tama.frost = 64;
				n = 5;
				break;
			case DIFF_EASY:
				imax = 8;
				tama.frost = 96;
				n = 4;
				break;
			}
			for(int i = 0; i < imax; i++ ) {
				CDanmakuNonDirectional d(&tama);
				double offset = 3.0 * 360.0 * i / imax / 16;
				d.offset =  offset + k * 60;
				d.n = n;
				d.w = 360;
				d.Fire();

				d.offset =  -offset + k * 60;
				d.Fire();

				g_pMaterial->sndFire.Play(0);
				mt.Suspend(t/imax);
			}
			mt.Suspend(60);
			CTamaFrost::lTamaFrost.clear();
			TamaFreeze();
			TamaResume(1);
			g_pMaterial->sndMelt.Play(0);
			mt.Suspend(60);
		}
	}
	return 1;
}

//凍符「ピンポイントフリーズ
int CBossChirno::Freeze() {
	SetHP(1800);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 420 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	deffence = 1;
	while( 1 ) {
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			CTamaPinPoint tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 2;
			tama.color = 3;
			tama.v = 3;
			tama.mode = 0;

			CDanmakuToPlayerLine d(&tama);
			d.w = 360;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 32;
				d.m = 32;
				d.sv = 2;
				d.ev = 2;
			}else {
				d.n = 4 * ( GetDifficult() + 1);
				d.m = 1+GetDifficult();
				d.sv = 2;
				d.ev = 2;
			}
			d.Fire();
			g_pMaterial->sndFire.Play(0);
			mt.Suspend(240);
			TamaFreeze();
			TamaRandom(0.5);
			g_pMaterial->sndMelt.Play(0);
			mt.Suspend(360);
		}
	}
	return 1;
}

//クリスタル
int CBossChirno::Cristal() {
	return 1;
}
//雪符「コッホスノーフレーク」
int CBossChirno::Koch() {
	SetHP(2400);
	SetTime(40);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 420 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	deffence = 1;
	int n = GetDifficult() + (GetDifficult() <= DIFF_NORMAL);
	lKoch.clear();
	lKoch.push_back( 0 );
	KochRecursion( n );
	while( 1 ) {
		for( int k = 0; ; k++ ) {
			//攻撃
			double t = GetDifficult() == DIFF_LUNATIC ?
				56 + 8 * (k>=8) :
				6 * GetDifficult() + 6 + GetDifficult() * 2 * (k>=8);
			double angle = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			double offset = (k==0||k>=8) ? 30 : 30+angle;
			double v = 1.0 + 0.1 * GetDifficult();
			KochDanmaku( n, v, offset, offset+120, t );
			KochDanmaku( n, v, offset+120, offset+240, t );
			KochDanmaku( n, v, offset+240, offset+360, t );

			g_pMaterial->sndFire.Play(0);
			mt.Suspend(120-5*(k<8?k:8));
		}
	}
	return 1;
}
//コッホ曲線用再帰関数
int CBossChirno::KochRecursion( int n )
{
	if( n == 0 )
		return 0;

	list<double>::iterator itr;
	list<double>::iterator next;
	
	for( itr = lKoch.begin(); itr != lKoch.end(); itr = next ) {
		next = itr;
		next ++;
		lKoch.insert( itr, (*itr) );
		lKoch.insert( itr, (*itr) + 1 );
		lKoch.insert( itr, (*itr) - 1 );
		lKoch.insert( itr, (*itr) );
		lKoch.erase( itr );
	}
	KochRecursion( n - 1 );
	return 1;
}
//コッホ曲線のリスト構造から作成
int CBossChirno::KochDanmaku( int n, double v, double a1, double a2, double t )
{
	CTamaNormal tama;
	tama.x = GetX();
	tama.y = GetY();
	tama.v = v;
	tama.type = 2;
	tama.color = 0;
	double vx1 = Cos( a1 );
	double vy1 = Sin( a1 );
	double vx2 = Cos( a2 );
	double vy2 = Sin( a2 );
	double tx = vx2 - vx1;
	double ty = vy2 - vy1;
	double tt = sqrt( pow2( tx ) + pow2( ty ));

	double vx = vx1, vy = vy1;
	double aaa = Atan2( tx, ty );
	double vv = tt / pow( 3, n );

	CDanmakuSegment d( &tama);
	d.a1 = Atan2( vx, vy );
	d.v1 = sqrt(pow2(vx)+pow2(vy));
	list<double>::iterator itr;
	for( itr = lKoch.begin(); itr != lKoch.end(); itr++ ) {
		double aa = aaa -60.0 * (*itr);
		vx += vv * Cos(aa);
		vy += vv * Sin(aa);
		d.a2 = Atan2( vx, vy );
		d.v2 = sqrt(pow2(vx)+pow2(vy));
		d.Set( t );
		d.Fire();
		d.a1 = d.a2;
		d.v1 = d.v2;
	}
	return 1;
}
