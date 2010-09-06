#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//-------------------リグル共通---------------------------------

CBossWriggleBase::CBossWriggleBase(){
	x = GAME_RIGHT + 80;
	y = GAME_CENTER_Y - GetH()/2;
}
int CBossWriggleBase::Draw(){
	int index = (count/7) % 2;
	RECT rect={index*GetW(),0,(index+1)*GetW(),GetH()};
	g_pStage2->imgWriggle.DrawRect(x,y,rect);
	return CBossBase::Draw();
}
//スペルカード
int CBossWriggleBase::SpellCard() {
	flag = 1;
	pBack = g_pBack;
	g_pBack = new CBackWriggle();
	g_lEffect2.Add( new CEffectCutin( &g_pStage2->imgCutin, NULL, GAME_CENTER_X, 120));
	return CBossBase::SpellCard();
}
//近付き防止
int CBossWriggleBase::DontCome() {
	mt2.Suspend(120);
	while(1){
		if( pow2( g_pPlayer->GetX() - GetX() ) + pow2( g_pPlayer->GetY() - GetY() )
			< pow2( 80 ) ){
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 1;
				tama.color = 5;

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
CBossWriggleMiddle::CBossWriggleMiddle(){
	TSunFunctionCallback<CBossWriggleMiddle> fn;
	fn.SetFunction(this,&CBossWriggleMiddle::Thread);
	mt3.Start(&fn,0x8000);
}
int CBossWriggleMiddle::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossWriggleBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossWriggleMiddle::Draw(){
	CBossWriggleBase::Draw();
	return 1;
}
//全体動作
int CBossWriggleMiddle::Thread(){
	TSunFunctionCallback<CBossWriggleMiddle> fn;
	TSunFunctionCallback<CBossWriggleBase> fnb;
	//出現
	flag = 0;
	deffence = 0;
	for( int i = 0; i < 46; i++ ){
		x -= 5;
		mt3.Suspend();
	}

	//通常攻撃
	fn.SetFunction(this,&CBossWriggleMiddle::NormalAtack1);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//ゲンジボタル
	SpellCard();
	fn.SetFunction(this,&CBossWriggleMiddle::GenjiBotaru);
	mt.Start(&fn,0x8000);
	fnb.SetFunction(this,&CBossWriggleBase::DontCome);
	mt2.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
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
int CBossWriggleMiddle::NormalAtack1() {
	SetHP(1000);
	SetTime(20);
	flag = 1;
	deffence = 1;
	while( 1 ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			int s = 1 + GetDifficult();
			int imax = s * (4 + 4 * (GetDifficult()) );
			for( int i = 0; i < imax; ++i ) {
				CTamaTurn tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 18;
				tama.color = 5;
				tama.stop_count = 60;
				tama.stop_time = 30;
				tama.v = 3;
				tama.v2 = 1;
				tama.turn = 0;

				CDanmakuNonDirectional d(&tama);
				int n = 2 + 2 * GetDifficult();
				d.offset = 360 * i / imax / n * s * (k*2-1);
				d.n = n;
				d.w = 360;
				d.Fire();

				g_pMaterial->sndFire.Play(0);
				mt.Suspend(64.0*3/imax);
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
//蛍符「ゲンジボタル」
int CBossWriggleMiddle::GenjiBotaru() {
	SetHP(1200);
	SetTime(20);
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
		for( int k = 0; k < 2; k++ ) {
			//置き弾
			const int jmax = 2;
			double angle = 180 + Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			for( int j = 0; j < jmax; j++ ) {
				CTamaSet tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 2;
				tama.color = 4;
				tama.v = 4;
				tama.c = 10 - 2 * GetDifficult();
				tama.t = j*2-1;
				tama.set_interval = 7 - 1 * GetDifficult();

				CDanmakuNonDirectional d(&tama);
				d.n = 4 + 4 * GetDifficult();

				d.Fire();

				g_pMaterial->sndFire.Play(0);

				mt.Suspend(120);
			}
			{//全方位
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 3;
				tama.color = 5;

				CDanmakuToPlayerLine d(&tama);
				d.w = 360;
				if( GetDifficult() == DIFF_LUNATIC ) {
					d.n = 128;
					d.m = 5;
				}
				else {
					d.n = 32 + 32 * GetDifficult();
					d.m = 1 + GetDifficult();
				}
				d.sv = 1;
				d.ev = 2;

				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
			mt.Suspend(120);
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

//----------------------------------------------------
CBossWriggle::CBossWriggle(){
	TSunFunctionCallback<CBossWriggle> fn;
	fn.SetFunction(this,&CBossWriggle::Thread);
	mt3.Start(&fn,0x8000);
}
int CBossWriggle::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossWriggleBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossWriggle::Draw(){
	CBossWriggleBase::Draw();
	return 1;
}
//全体動作（出現～会話～通常攻撃～スペルカード～撃破）
int CBossWriggle::Thread(){
	TSunFunctionCallback<CBossWriggle> fn;
	TSunFunctionCallback<CBossWriggleBase> fnb;
	fnb.SetFunction(this,&CBossWriggleBase::DontCome);

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
		pTalk = new CTalkWriggle1();
		pTalk->Start();
		while( !pTalk->IsEnd() )
			mt3.Suspend();
		SAFE_DELETE(pTalk);
		g_pPlayer->Resume();
	}
	flag = 1;
	fntBoss.CreateList("Wriggle",12);

	//通常攻撃
	fn.SetFunction(this,&CBossWriggle::NormalAtack2);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();

	//ナナホシテントウ
	SpellCard();
	fn.SetFunction(this,&CBossWriggle::Tentou);
	mt.Start(&fn,0x8000);
	mt2.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt2.Stop();

	//通常攻撃
	fn.SetFunction(this,&CBossWriggle::NormalAtack2);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//チョウセンカマキリ
	SpellCard();
	fn.SetFunction(this,&CBossWriggle::Kamakiri);
	mt.Start(&fn,0x8000);
	mt2.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt2.Stop();

	//通常攻撃
	fn.SetFunction(this,&CBossWriggle::NormalAtack2);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//トビズムカデ
	SpellCard();
	fn.SetFunction(this,&CBossWriggle::Mukade);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();

	//幕間
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt2.Stop();
	mt3.Suspend(60);

	//オニグモ
	SpellCard();
	fn.SetFunction(this,&CBossWriggle::Kumo);
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
		pTalk = new CTalkWriggle2();
		pTalk->Start();
		while( !pTalk->IsEnd() )
			mt3.Suspend();
		SAFE_DELETE(pTalk);
	}
	return 1;
}

//通常攻撃2
int CBossWriggle::NormalAtack2() {
	SetHP(1000);
	SetTime(20);
	flag = 1;
	deffence = 1;
	while( 1 ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			int s = 1 + GetDifficult();
			int imax = s * (4 + 4 * (GetDifficult()) );
			for( int i = 0; i < imax; ++i ) {
				CTamaTurn tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 18;
				tama.color = 5;
				tama.stop_count = 60;
				tama.stop_time = 30;
				tama.v = 3;
				tama.v2 = 1;
				tama.turn = 0;

				CDanmakuNonDirectional d(&tama);
				int n = 2 + 2 * GetDifficult();
				d.offset = 360 * i / imax / n * s * (k*2-1);
				d.n = n;
				d.w = 360;
				d.Fire();

				g_pMaterial->sndFire.Play(0);
				mt.Suspend(64.0*3/imax);
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
//虫符「ナナホシテントウ」
int CBossWriggle::Tentou() {
	SetHP(1200);
	SetTime(20);
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
			if( GetDifficult() > DIFF_NORMAL )
			{
				double angle = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
				CTamaTurn2 tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 2;
				tama.color = 1;
				tama.turn_count = 30;
				tama.v2 = 2+0.5*GetDifficult();

				CDanmakuTentou d(&tama);
				d.n = 16;
				d.m = 4;
				d.sv = 2.0;
				d.ev = 0.5;

				tama.turn = angle + 30;
				d.Fire();
				tama.turn = angle - 30;
				d.Fire();
				tama.turn = angle;
				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
			mt.Suspend(60);
			//全方位
			{
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 2;
				tama.color = 1;

				CDanmakuTentou d(&tama);
				if( GetDifficult() == DIFF_LUNATIC) {
					d.n = 94;
					d.m = 10.0;
					d.sv = 2.0;
					d.ev = 0.5;
				}else {
					d.n = 8 + 32 * GetDifficult();
					d.m = 1 + 3 * GetDifficult();
					d.sv = 1.5 + 0.25 * GetDifficult();
					d.ev = d.sv - 1;
				}
				d.Fire();
				g_pMaterial->sndFire.Play(0);
			}
			mt.Suspend(240);
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

//虫符「チョウセンカマキリ」
int CBossWriggle::Kamakiri() {
	SetHP(1200);
	SetTime(20);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 550 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	deffence = 1;
	while( 1 ) {
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			double angle = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			for( int j = 0; j < 2; j++ ) {
				int imax = 2+GetDifficult();
				for(int i = 0; i < imax; i++ ) {
					int lmax = (GetDifficult()>DIFF_NORMAL)?3:1;
					for( int l = 0; l < lmax; l++ ) {
						CTamaBurst *pTama = new CTamaBurst;
						pTama->x = GetX();
						pTama->y = GetY();
						pTama->type = 13;
						pTama->color = 5;
						pTama->burst_count = 50;
						pTama->t = 2*j-1;
						pTama->a = angle - (60-15*i/imax - l*8) * pTama->t;
						pTama->v = 1+8.0 * i / imax;
						pTama->sv = 0.5;
						pTama->ev = 1;
						pTama->m = 1 + GetDifficult();
						g_lTama.Add(pTama);
					}
				}
			}
			g_pMaterial->sndFire.Play(0);
			mt.Suspend(50);
			g_pMaterial->sndBell.Play(0);
			mt.Suspend(70);
		}
		//移動
		const int move_count = 30;
		int xv,yv;
		xv = 0;
		if( GetY() > g_pPlayer->GetY() )
			yv = GAME_TOP + 120 - GetY();
		else
			yv = GAME_BOTTOM - 120 - GetY();

		for( int i = 0; i < move_count; i++ ) {
			x += xv / move_count;
			y += yv / move_count;
			mt.Suspend(1);
		}
		mt.Suspend(120);
	}
	return 1;
}

//蟲符「トビズムカデ」
int CBossWriggle::Mukade() {
	SetHP(1200);
	SetTime(20);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 550 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	deffence = 1;
	while( 1 ) {
		for( int k = 0; k < 2; k++ ) {
			//攻撃
			for( int j = 0; j < 2; j++ ) {
				int imax = 32;
				for(int i = 0; i < imax; i++ ) {
					{//節
						CTamaMukade *pTama = new CTamaMukade;
						pTama->x = GAME_RIGHT;
						pTama->y = (j==0) ? GAME_TOP + 30 : GAME_BOTTOM - 30;
						pTama->type = 3;
						pTama->color = 1;
						pTama->t = 2*j-1;
						pTama->a = 180;
						pTama->v = 2.4;
						if( GetDifficult() == DIFF_LUNATIC)
							pTama->burst_interval = 10;
						else
							pTama->burst_interval = 120 - 10 * GetDifficult();
						if( i % (12-3*GetDifficult()) == 0 ) {
							pTama->burst_start = 100 - 4*i;
							pTama->burst_end = 480;
						}else{
							pTama->burst_start = 10000;
							pTama->burst_end = 10000;
						}
						g_lTama.Add(pTama);
					}
					{//左足
						CTamaMukadeLeg *pTama = new CTamaMukadeLeg;
						pTama->xx = GAME_RIGHT;
						pTama->yy = (j==0) ? GAME_TOP + 30 : GAME_BOTTOM - 30;
						pTama->type = 2;
						pTama->color = 1;
						pTama->t = 2*j-1;
						pTama->a = 180;
						pTama->v = 2.4;
						pTama->tt = 1;
						pTama->s = 16;
						g_lTama.Add(pTama);
					}
					{//足
						CTamaMukadeLeg *pTama = new CTamaMukadeLeg;
						pTama->xx = GAME_RIGHT;
						pTama->yy = (j==0) ? GAME_TOP + 30 : GAME_BOTTOM - 30;
						pTama->type = 2;
						pTama->color = 1;
						pTama->t = 2*j-1;
						pTama->a = 180;
						pTama->v = 2.4;
						pTama->tt = -1;
						pTama->s = 16;
						g_lTama.Add(pTama);
					}
					g_pMaterial->sndFire.Play(0);
					mt.Suspend(4);
				}
				mt.Suspend(120);
				{//全方位
					CTamaNormal tama;
					tama.x = GetX();
					tama.y = GetY();
					tama.type = 13;
					tama.color = 5;

					CDanmakuToPlayerLine d(&tama);
					d.w = 360;
					if( GetDifficult() == DIFF_LUNATIC ) {
						d.n = 64;
						d.m = 3;
					}
					else {
						d.n = 16 + 16 * GetDifficult();
						d.m = 1 + ( GetDifficult() >= 1 );
					}
					d.sv = 1;
					d.ev = 2;

					d.Fire();
					g_pMaterial->sndFire.Play(0);
				}
				mt.Suspend(120);
			}
		}
		mt.Suspend(120);
	}
	return 1;
}

//蟲符「オニグモ」
int CBossWriggle::Kumo() {
	SetHP(1500);
	SetTime(30);
	deffence = 0;
	for( int i = 0; i < 60; ++i ) {
		x += ( 450 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}
	mt.Suspend(60);
	deffence = 1;
	int i = 0;
	double of = 0;
	while( 1 ) {
		//ワインダー
		CTamaNormal tama;
		tama.x = GetX();
		tama.y = GetY();
		tama.type = 1;
		tama.color = 2;
		tama.v = 1 + 0.3 * GetDifficult();

		CDanmakuNonDirectional d(&tama);
		d.n = 8;
		d.w = 360;
		d.offset = 180.0/d.n + of;

		d.Fire();
		//横線
		if( i % 10 == 5 ){
			CDanmakuSpyder d(&tama);
			if( GetDifficult() == DIFF_LUNATIC )
				d.n = 150;
			else
				d.n = 32 + 32 * GetDifficult();
			d.m = 8;
			d.sv = tama.v;
			d.ev = tama.v * 0.8;
			d.offset = 180.0/d.m + of + rand(0,360.0/d.n);
			d.Fire();
			g_pMaterial->sndFire.Play(0);
		}
		mt.Suspend(16 - 4 * GetDifficult());
		i++;
		if( i > 30 )
			of += 2 * Sin( i * 180 / 10 );
	}
	return 1;
}
