#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//-------------------ミスティア共通---------------------------------

CBossMystiaBase::CBossMystiaBase(){
	x = GAME_RIGHT + 80;
	y = GAME_CENTER_Y - GetH()/2;
	yami = 0;
	yami_r = 60;
	yami_type = 0;
	CTamaBoid::lTamaBoid.clear();
	CTamaBoid::gx1=0, CTamaBoid::gy1=0;
	CTamaBoid::gx2=0, CTamaBoid::gy2=0;
	CTamaBoid::gx3=0, CTamaBoid::gy3=0;
}
int CBossMystiaBase::Run(){
	CTamaBoid::UpdateGoal();
	return CBossBase::Run();
}
int CBossMystiaBase::Draw(){
	int index = (count/7) % 2;
	RECT rect={index*GetW(),0,(index+1)*GetW(),GetH()};
	g_pStage4->imgMystia.DrawRect(x,y,rect);
	return CBossBase::Draw();
}
int CBossMystiaBase::Draw4(){
	if( yami ) {
		torime.m_inRadius = 1024 - count*5;
		if( torime.m_inRadius < yami_r ) torime.m_inRadius = yami_r;
		torime.m_outRadius = torime.m_inRadius + 60;
		switch( yami_type ) {
		case 0:
			torime.m_x = g_pPlayer->GetX();
			torime.m_y = g_pPlayer->GetY();
			break;
		}
		torime.Draw();
	}
	return CBossBase::Draw4();
}

//スペルカード
int CBossMystiaBase::SpellCard() {
	flag = 1;
	pBack = g_pBack;
	g_pBack = new CBackMystia();
	RECT rect = {0,0,296,448};
	g_lEffect2.Add( new CEffectCutin( &g_pStage4->imgTalk, &rect, GAME_CENTER_X, 120));
	return CBossBase::SpellCard();
}
//通常攻撃1
int CBossMystiaBase::NormalAtack1() {
	flag = 1;
	deffence = 1;
	CTamaBoid::lTamaBoid.clear();
	CTamaBoid::gx1=0, CTamaBoid::gy1=0;
	CTamaBoid::gx2=0, CTamaBoid::gy2=0;
	CTamaBoid::gx3=0, CTamaBoid::gy3=0;
	while( 1 ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			for( int j = 0; j < 3; j++ ){
				//攻撃
				int s = 1 + GetDifficult();
				int imax = s * s * 2;
				for( int i = 0; i < imax; ++i ) {
					CTamaBoid tama;
					tama.x = GetX() + rand(-20,20);
					tama.y = GetY() + rand(-20,20);
					tama.type = 17;
					switch(j) {
					case 0: tama.color = 1; break;
					case 1: tama.color = 2; break;
					case 2: tama.color = 3; break;
					}
					tama.v = 2;
					tama.vmax = 1.0+0.1*GetDifficult();
					tama.a = rand(0,360);
					tama.w1 = 1;//(j==1);
					tama.w2 = 1;//(j==0);
					tama.w3 = 0;//(j==2);
					tama.loop = (GetDifficult()>=DIFF_HARD);
					g_lTama.Add(tama.Copy());

					g_pMaterial->sndFire.Play(0);
					mt.Suspend(16.0*3/imax);
				}
				mt.Suspend(20);
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
			mt.Suspend(120);
		}
	}
	return 1;
}


//-------------------中ボス---------------------------------
CBossMystiaMiddle::CBossMystiaMiddle(){
	TSunFunctionCallback<CBossMystiaMiddle> fn;
	fn.SetFunction(this,&CBossMystiaMiddle::Thread);
	mt3.Start(&fn,0x8000);
}
int CBossMystiaMiddle::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossMystiaBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossMystiaMiddle::Draw(){
	CBossMystiaBase::Draw();
	return 1;
}
//全体動作
int CBossMystiaMiddle::Thread(){
	TSunFunctionCallback<CBossMystiaMiddle> fn;
	TSunFunctionCallback<CBossMystiaBase> fnb;

	//出現
	flag = 0;
	deffence = 0;
/*
	for( int i = 0; i < 46; i++ ){
		x -= 5;
		mt3.Suspend();
	}
	mt3.Suspend(30);
*/
	//会話
	g_pPlayer->Talk();
	pTalk = new CTalkMystia1();
	pTalk->Start();
	while( !pTalk->IsEnd() )
		mt3.Suspend();
	SAFE_DELETE(pTalk);
	g_pPlayer->Resume();

	//通常攻撃
	SetHP(1000);
	SetTime(20);
	fnb.SetFunction(this,&CBossMystiaBase::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	this->AddItem(ITEM_1UP,1);

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

//----------------------------------------------------
CBossMystia::CBossMystia(){
	TSunFunctionCallback<CBossMystia> fn;
	fn.SetFunction(this,&CBossMystia::Thread);
	mt3.Start(&fn,0x8000);
}
int CBossMystia::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossMystiaBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossMystia::Draw(){
	CBossMystiaBase::Draw();
	return 1;
}
//全体動作（出現～会話～通常攻撃～スペルカード～撃破）
int CBossMystia::Thread(){
	TSunFunctionCallback<CBossMystia> fn;
	TSunFunctionCallback<CBossMystiaBase> fnb;

	//出現
	flag = 0;
	deffence = 0;
	for( int i = 0; i < 46; i++ ){
		x -= 5;
		mt3.Suspend();
	}
	mt3.Suspend(30);

	//会話
	g_pPlayer->Talk();
	pTalk = new CTalkMystia2();
	pTalk->Start();
	while( !pTalk->IsEnd() )
		mt3.Suspend();
	SAFE_DELETE(pTalk);
	g_pPlayer->Resume();
	fntBoss.CreateList("Mystia",12);

	//通常攻撃
	SetHP(500);
	SetTime(20);
	fnb.SetFunction(this,&CBossMystiaBase::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//鳥居
	yami = 0;
	SpellCard();
	fn.SetFunction(this,&CBossMystia::Torii);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();
	SpellEnd();

	//通常攻撃
	SetHP(1000);
	SetTime(30);
	fnb.SetFunction(this,&CBossMystiaBase::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//横符
	SpellCard();
	fn.SetFunction(this,&CBossMystia::Yoko);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();
	SpellEnd();

	//通常攻撃
	SetHP(1500);
	SetTime(40);
	fnb.SetFunction(this,&CBossMystiaBase::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//縦符
	SpellCard();
	fn.SetFunction(this,&CBossMystia::Tate);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();
	SpellEnd();

	//通常攻撃
	SetHP(2000);
	SetTime(40);
	fnb.SetFunction(this,&CBossMystiaBase::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//闇符
	yami = 1;
	yami_r = 120;
	SpellCard();
	fn.SetFunction(this,&CBossMystia::Yoko);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();
	SpellEnd();

	//幕間
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt3.Suspend(60);

	//酉符
	yami = 1;
	yami_r = 60;
	SpellCard();
	fn.SetFunction(this,&CBossMystia::Tate);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();
	SpellEnd();

	//幕間
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt3.Suspend(60);

	//半円
	yami = 0;
	SpellCard();
	fn.SetFunction(this,&CBossMystia::HalfCircle);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();
	SpellEnd();

	//エフェクト
	mt.Stop();
	flag = 0;
	deffence = 0;
	g_lEffect.Add( new CEffectBreak( GetX(), GetY() ));
	g_pPlayer->Talk();
	mt3.Suspend( 60 );

	//画面外へ
	x=1000;

	//会話
	pTalk = new CTalkMystia3();
	pTalk->Start();
	while( !pTalk->IsEnd() )
	mt3.Suspend();
	SAFE_DELETE(pTalk);

	return 1;
}
//ヨコ
int CBossMystia::Yoko(){
	SetHP(1500);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 550 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	flag = 1;
	deffence = 1;
	for( int c = 0; ; c++ ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			int kk = k*2-1;
			//攻撃
			int imax = (GetDifficult()<=DIFF_HARD)?16*(1+GetDifficult()):96;
			for( int i = 0; i < imax; ++i ) {
				for( int j = 0; j < 2; j++ ){
					int jj = j*2-1;
					CTamaNormal tama;
					tama.x = GetX();
					tama.y = GetY();
					tama.type = 13;
					tama.color = k?6:7;
					tama.v = 2;

					CDanmakuNonDirectionalLine d(&tama);
					d.offset = 360.0*jj*i/imax+(k-c%2)*180;
					d.w = 60*jj;
					d.sv = 1.0;
					d.ev = 1.5;
					if( GetDifficult() == DIFF_LUNATIC) {
						d.n = 1;
						d.m = 3;
					}else {
						d.n = 1;
						d.m = 1;
					}
					g_pMaterial->sndFire.Play(0);
					d.Fire();
				}
				this->x += 4*kk*64/imax;
				mt.Suspend(64.0/imax);
			}
			mt.Suspend(60);
		}
		mt.Suspend(120);
	}
	return 1;
}
//タテ
int CBossMystia::Tate(){
	SetHP(1500);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 500 - GetX() ) / 60;
		y += ( 50 - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	flag = 1;
	deffence = 1;
	for( int c = 0; ; c++ ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			int kk = k*2-1;
			//攻撃
			int imax = (GetDifficult()<=DIFF_HARD)?16*(1+GetDifficult()):128;
			for( int i = 0; i < imax; ++i ) {
				for( int j = 0; j < 2; j++ ){
					int jj = j*2-1;
					CTamaNormal tama;
					tama.x = GetX();
					tama.y = GetY();
					tama.type = 13;
					tama.color = j?6:7;
					tama.v = 2;

					CDanmakuNonDirectionalLine d(&tama);
					d.offset = 360.0*((double)(k-c%2)-(double)i/imax) + 180*j;
					d.w = 60*jj;
					d.sv = 0.8;
					d.ev = 1.3;
					if( GetDifficult() == DIFF_LUNATIC) {
						d.n = 1;
						d.m = 3;
					}else {
						d.n = 1;
						d.m = 1+GetDifficult();
					}
					d.Fire();
				}
				g_pMaterial->sndFire.Play(0);
				this->y -= 4.0*kk*64/imax;
				mt.Suspend(64.0/imax);
			}
			mt.Suspend(60);
		}
		mt.Suspend(120);
	}
	return 1;
}
int CBossMystia::Yami(){
	return 1;
}
int CBossMystia::Torime(){
	return 1;
}
//ミステリーサークル
int CBossMystia::HalfCircle(){
	SetHP(2000);
	SetTime(30);
	deffence = 0;
	double tx = GetX();
	double ty = GetY();
	for( int i = 0; i < 60; ++i ) {
		x += ( 420 - tx ) / 60;
		y += ( GAME_CENTER_Y - ty ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	deffence = 1;
	while( 1 ) {
		for( int k = 0; ; k++ ) {
			//攻撃
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = (k%2)?1:3;
			tama.v = 1;

			CDanmakuNonDirectional d(&tama);
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 128;
			}else {
				d.n = 32*(1+GetDifficult());
			}
			d.offset = 180*(k%2)+180.0/d.n*rand(0,1);
			d.w = 180.0+180.0/d.n;
			d.Fire();
			g_pMaterial->sndFire.Play(0);
			mt.Suspend(120-20*GetDifficult()-5*min(k,5+GetDifficult()));
		}
	}
	return 1;
}
//鳥居
int CBossMystia::Torii(){
	SetHP(2000);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 420 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		Suspend(1);
	}
	deffence = 1;
	Suspend(30);
	while( 1 ) {
		for( int k = 0; k < 2; ++k){
			//攻撃
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 3;
			tama.color = 1;
			tama.v = 1;

			CDanmakuSegment d( &tama);
			d.v2 = 1.5;
			d.v1 = 1.5;

			d.a1 = -20;
			d.a2 = -180+20;
			d.Set( 4+4*GetDifficult() );
			d.Fire();
			g_pMaterial->sndFire.Play(0);
			Suspend(40);

			d.a1 = -40;
			d.a2 = +60;
			d.Set( 4+4*GetDifficult() );
			d.Fire();
			d.a1 = 180-60;
			d.a2 = 180+40;
			d.Set( 4+4*GetDifficult() );
			d.Fire();
			g_pMaterial->sndFire.Play(0);
			Suspend(40);

			d.a1 = -20;
			d.a2 = -180+20;
			d.Set( 4+4*GetDifficult() );
			d.Fire();
			g_pMaterial->sndFire.Play(0);
			Suspend(120);

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
			mt.Suspend(120);
		}
	}
	return 1;
}
