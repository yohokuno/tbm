#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//-------------------ルーミア共通---------------------------------

CBossRumiaBase::CBossRumiaBase(){
	x = GAME_RIGHT + 80;
	y = GAME_CENTER_Y - GetH()/2;
}
int CBossRumiaBase::Draw(){
	int index = (count/7) % 2;
	RECT rect={index*GetW(),0,(index+1)*GetW(),GetH()};
	g_pStage1->imgRumia.DrawRect(x,y,rect);
	return CBossBase::Draw();
}
const double rumia_circle = 80;
//闇のエフェクト
int CBossRumiaBase::DrawEffect(){
	circle.m_x = GetX();
	circle.m_y = GetY();
	circle.m_inRadius = 0;
	circle.m_outRadius = rumia_circle;
	Sun3D.SetBlendingType(BLEND_MUL);
	circle.Draw();
	Sun3D.SetBlendingType(BLEND_NORMAL);
	return 1;
}
//スペルカード
int CBossRumiaBase::SpellCard() {
	flag = 1;
	pBack = g_pBack;
	g_pBack = new CBackRumia();
	g_lEffect2.Add( new CEffectCutin( &g_pStage1->imgCutin, NULL, GAME_CENTER_X - 256, 120 ));
	return CBossBase::SpellCard();
}
//近付き防止
int CBossRumiaBase::DontCome() {
	mt2.Suspend(120);
	while(1){
		if( pow2( g_pPlayer->GetX() - GetX() ) + pow2( g_pPlayer->GetY() - GetY() )
			< pow2( rumia_circle ) ){
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 5;
				tama.color = 0;

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
//-------------------ルーミア中ボス---------------------------------
CBossRumiaMiddle::CBossRumiaMiddle(){
	TSunFunctionCallback<CBossRumiaMiddle> fn;
	fn.SetFunction(this,&CBossRumiaMiddle::Thread);
	mt3.Start(&fn,0x8000);
}
int CBossRumiaMiddle::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossRumiaBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossRumiaMiddle::Draw(){
	double a = 60.0 * pow2( Sin((double)count*360/120) );
	circle.m_inColor = xrgb(a,a,a);
	circle.m_outColor = xrgb(a,a,a);
	CBossRumiaBase::Draw();
	DrawEffect();
	return 1;
}
//全体動作
int CBossRumiaMiddle::Thread(){
	TSunFunctionCallback<CBossRumiaMiddle> fn;
	TSunFunctionCallback<CBossRumiaBase> fnb;
	//出現
	flag = 0;
	deffence = 0;
	for( int i = 0; i < 46; i++ ){
		x -= 5;
		mt3.Suspend();
	}
	//通常攻撃
	flag = 1;
	deffence = 1;
	fn.SetFunction(this,&CBossRumiaMiddle::NormalAtack1);
	fnb.SetFunction(this,&CBossRumiaBase::DontCome);
	mt.Start(&fn,0x8000);
	mt2.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();
	mt2.Stop();

	//エフェクト
	flag = 0;
	deffence = 0;
	g_lEffect.Add( new CEffectBreak( GetX(), GetY() ));
	mt3.Suspend( 60 );

	//退却
	for( int i = 0; i < 46; i++ ){
		x += 5;
		mt3.Suspend();
	}
	return 1;
}
//通常攻撃1
int CBossRumiaMiddle::NormalAtack1() {
	SetHP(1000);
	SetTime(20);
	while( 1 ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			const int jmax = 2;
			double angle = 180 + Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			for( int j = 0; j < jmax; j++ ) {
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 5;
				tama.color = j ? 1 : 3;

				CDanmakuToPlayerLine d(&tama);
				d.w = 30;
				d.offset = 30 * ( j ? -1 : 1 ) + angle;
				if( GetDifficult() == DIFF_LUNATIC) {
					d.n = 10 + j;
					d.m = 9.0;
					d.sv = 3.0;
					d.ev = 0.5;
				}else {
					d.n = 3 * GetDifficult() + 3 + j;
					d.m = 1 + 3 * GetDifficult();
					d.sv = 1.5 + 0.25 * GetDifficult();
					d.ev = d.sv - 1;
				}

				d.Fire();

				//前後対称
				d.sv = -d.sv;
				d.ev = -d.ev;
				d.Fire();
				g_pMaterial->sndFire.Play(0);

				mt.Suspend(120 - 30 * GetDifficult());
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

//-------------------ルーミア---------------------------------
CBossRumia::CBossRumia(){
	TSunFunctionCallback<CBossRumia> fn;
	fn.SetFunction(this,&CBossRumia::Thread);
	mt3.Start(&fn,0x8000);
}
int CBossRumia::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossRumiaBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossRumia::Draw(){
	double a = 200.0 * pow2( Sin((double)count*360/120) );
	circle.m_inColor = xrgb(a,a,a);
	circle.m_outColor = xrgb(a,a,a);
	DrawEffect();
	CBossRumiaBase::Draw();
	return 1;
}
//全体動作（出現～会話～通常攻撃～スペルカード～撃破）
int CBossRumia::Thread(){
	TSunFunctionCallback<CBossRumia> fn;
	TSunFunctionCallback<CBossRumiaBase> fnb;
	fnb.SetFunction(this,&CBossRumiaBase::DontCome);
	//出現
	deffence = 0;
	flag = 0;
	for( int i = 0; i < 46; i++ ){
		x -= 5;
		mt3.Suspend();
	}

	//会話
	{
		g_pPlayer->Talk();
		pTalk = new CTalkRumia1();
		pTalk->Start();
		while( !pTalk->IsEnd() )
			mt3.Suspend();
		SAFE_DELETE(pTalk);
		g_pPlayer->Resume();
	}
	flag = 1;
	fntBoss.CreateList("Rumia",12);
	//通常攻撃
	fn.SetFunction(this,&CBossRumia::NormalAtack2);
	mt.Start(&fn,0x8000);
	mt2.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();

	//ナイトバード
	SpellCard();
	fn.SetFunction(this,&CBossRumia::NightBird);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();

	//通常攻撃
	fn.SetFunction(this,&CBossRumia::NormalAtack2);
	mt.Start(&fn,0x8000);
	mt2.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();

	//ディマーケイション
	SpellCard();
	fn.SetFunction(this,&CBossRumia::Demarcation);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();

	//幕間
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt2.Stop();
	mt3.Suspend(60);

	//ムーンライトレイ
	SpellCard();
	fn.SetFunction(this,&CBossRumia::MoonLightRay);
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
	{
		pTalk = new CTalkRumia2();
		pTalk->Start();
		while( !pTalk->IsEnd() )
			mt3.Suspend();
		SAFE_DELETE(pTalk);
	}
	return 1;
}
//通常攻撃2
int CBossRumia::NormalAtack2() {
	SetHP(1000);
	SetTime(20);
	deffence = 1;
	while( 1 ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			const int jmax = 2;
			double angle = 180 + Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			for( int j = 0; j < jmax; j++ ) {
				//攻撃
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 5;
				tama.color = j ? 1 : 3;

				CDanmakuToPlayerLine d(&tama);
				d.w = 30;
				d.offset = 30 * ( j ? -1 : 1 ) + angle;
				if( GetDifficult() == DIFF_LUNATIC) {
					d.n = 10 + j;
					d.m = 9.0;
					d.sv = 3.0;
					d.ev = 0.5;
				}else {
					d.n = 3 * GetDifficult() + 3 + j;
					d.m = 1 + 3 * GetDifficult();
					d.sv = 1.5 + 0.25 * GetDifficult();
					d.ev = d.sv - 1;
				}

				g_pMaterial->sndFire.Play(0);
				d.Fire();

				//前後対称
				d.sv = -d.sv;
				d.ev = -d.ev;
				d.Fire();

				mt.Suspend(120 - 30 * GetDifficult());
				//移動
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
				mt.Suspend(30);
			}
		}
	}
	return 1;
}
//夜符「ナイトバード」
int CBossRumia::NightBird(){
	SetHP(1500);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 500 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	while( 1 ) {
		mt.Suspend(120);
		deffence = 1;
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			const int jmax = 2;
			for( int j = 0; j < jmax; j++ ) {
				CTamaNormal tama;
				tama.type = 5;
				NormalAtackBase(&tama, j);
				mt.Suspend(120);
			}
			//移動
			const int move_count = 30;
			int xv,yv;
			if( k ) {
				xv = 550 - GetX();
				yv = GAME_TOP + 120 - GetY();
			}
			else {
				xv = 450 - GetX();
				yv = GAME_BOTTOM - 120 - GetY();
			}
			for( int i = 0; i < move_count; i++ ) {
				x += xv / move_count;
				y += yv / move_count;
				mt.Suspend(1);
			}
			mt.Suspend(30);
		}
	}
	return 1;
}

//片翼型弾幕
int CBossRumia::NormalAtackBase(CTamaNormal *pTama, int j) {
	int imax = 10 + 10 * GetDifficult();
	if( GetDifficult() == DIFF_LUNATIC )
		imax = 48;
	for( int i = 0; i < imax; i++ ) {
		pTama->x = GetX();
		pTama->y = GetY();
		pTama->color = 1;

		CDanmakuNonDirectionalLine d(pTama);
		d.offset= 180 * (i-GetDifficult()*imax/10) /imax;
		if( j%2 == 1 ) { //逆方向
			d.offset = -d.offset;
			pTama->color = 3;
		}
		d.n= 1;
		if( GetDifficult() == DIFF_LUNATIC) {
			d.m = 10;
			d.ev = 0.5;
			d.sv = d.ev + 4.0*i/imax;
		}else {
			d.m = 4 + 2 * GetDifficult();
			d.ev = 1.0 - 0.2 * GetDifficult();
			d.sv = d.ev + 2.0*i/imax*GetDifficult();
		}

		g_pMaterial->sndFire.Play(0);
		d.Fire();
		mt.Suspend(1);
	}
	return 1;
}
//ディマーケーション
int CBossRumia::Demarcation(){
	SetHP(2500);
	SetTime(35);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 550 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	while( 1 ) {
		mt.Suspend(120);
		deffence = 1;
		for( int k = 0; k < 2; k++ ) {
			//全方位攻撃
			for( int j = 0; j < 3; j++ ) {
				CTamaTurn tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.stop_count = 50;
				tama.stop_time = 30;
				CDanmakuNonDirectional danmaku(&tama);
				danmaku.offset = rand(0,360);
				if( GetDifficult() == DIFF_LUNATIC ){
					danmaku.n = 64;
				}
				else
					danmaku.n = 16 + 16 * GetDifficult();

				int imax = 1 + GetDifficult();
				if( GetDifficult() == DIFF_LUNATIC )
					imax = 7;
				for( int i = 0; i < imax; i++ ) {
					tama.turn = (i%2) ? -90 : 90;
					tama.type = 11;
					tama.color = 2 * j + 1;
					if( GetDifficult() == DIFF_LUNATIC )
						tama.v =  2.0 + 3.0 * (double)(i+1) / imax;
					else
						tama.v =  2.0 + ( 1.0 + 0.3*GetDifficult() ) * (double)(i+1) / (GetDifficult()+1);
					tama.v2 = 1;

					danmaku.Fire();
				}
				g_pMaterial->sndFire.Play(0);
				mt.Suspend(30);
				mt.Suspend(50);
				g_pMaterial->sndBell.Play(0);
			}
			mt.Suspend(60);
			//自機狙い
			for( int j = 0; j < GetDifficult(); j++ ) {
				CTamaTurnToPlayer tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.v2 = 2;
				tama.stop_count = 50;
				tama.stop_time = 30;
				tama.type = 5;
				tama.color = 0;

				int imax = 10 + 7 * GetDifficult();
				for( int i = 0; i < imax; i++ ) {
					CDanmakuNonDirectionalLine d(&tama);
					d.offset= 180 * (i-GetDifficult()*imax/10) /imax;
					if( j%2 == 1 ) { //逆方向
						d.offset = -d.offset;
					}
					d.n= 1;
					d.m = 1;
					d.ev = 1.0 - 0.2 * GetDifficult();
					d.sv = d.ev + 2.0*i/imax*GetDifficult()/2;

					d.Fire();

					g_pMaterial->sndFire.Play(0);
					mt.Suspend(2);
				}
				mt.Suspend(30);
			}

			//移動
			const int move_count = 30;
			int xv,yv;
			if( k ) {
				xv = 450 - GetX();
				yv = GAME_TOP + 120 - GetY();
			}
			else {
				xv = 550 - GetX();
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
//ムーンライトレイ
int CBossRumia::MoonLightRay(){
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
		mt.Suspend(120);
		deffence = 1;
		for( int k = 0; k < 2; k++ ) {
			//レーザー
			for( int j = 0; j < 2; j++ ) {
				CLaserSquare *pLaser = new CLaserSquare();
				pLaser->a = 60 * (2*j-1);
				pLaser->w = -0.4*(2*j-1);
				pLaser->start_count = 30;
				pLaser->end_count = 100;
				pLaser->vanish_count = 10+20*GetDifficult();
				pLaser->laser.m_sx = GetX();
				pLaser->laser.m_sy = GetY();
				pLaser->laser.m_inColor = argb(255,255,255,255);
				pLaser->laser.m_outColor = argb(127,0,255,255);
				g_lLaser.Add( pLaser );
				g_pMaterial->sndLaser.Play(0);
			}
			mt.Suspend(5);

			//自機狙い弾
			CTamaIncrease tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 5;
			tama.color = 1;
			tama.SetIncrease(3 * GetDifficult(),0,9 * GetDifficult());

			CDanmakuToPlayerRandom danmaku(&tama);
			danmaku.n = 8 + 16 * GetDifficult();
			danmaku.w = 15;
			danmaku.sv = 1 + 3 * GetDifficult();
			danmaku.ev = 3 + 3 * GetDifficult();
			if( GetDifficult() == DIFF_LUNATIC ) {
				danmaku.n = 64;
			}
			g_pMaterial->sndFire.Play(0);
			danmaku.Fire();


			mt.Suspend(240);
			//移動
			const int move_count = 30;
			int xv,yv;
			if( k ) {
				xv = 500 - GetX();
				yv = GAME_TOP + 120 - GetY();
			}
			else {
				xv = 550 - GetX();
				yv = GAME_BOTTOM - 120 - GetY();
			}
			for( int i = 0; i < move_count; i++ ) {
				x += xv / move_count;
				y += yv / move_count;
				mt.Suspend(1);
			}
			mt.Suspend(30);

			//全方位弾開始
			if( mt2.IsStop() ) {
				TSunFunctionCallback<CBossRumia> fn;
				fn.SetFunction(this,&CBossRumia::NonDirectional);
				mt2.Start(&fn,0x8000);
			}

			mt.Suspend(60);
		}
	}
	return 1;
}
//全方位弾
int CBossRumia::NonDirectional(){
	while(1){
		CTamaNormal tama;
		tama.x = GetX();
		tama.y = GetY();
		tama.type = 5;
		tama.color = 3;

		CDanmakuToPlayerLine d(&tama);
		if( GetDifficult() == DIFF_LUNATIC ) {
			d.n = 64+1;
			d.m = 2;
		}
		else {
			d.n = 4 + 16 * GetDifficult()+1;
			d.m = 1;
		}
		d.w = 180-180/d.n;
		d.sv = 1;
		d.ev = 2;

		d.Fire();
		g_pMaterial->sndFire.Play(0);

		mt2.Suspend(150 - 30 * GetDifficult());
	}
	return 1;
}