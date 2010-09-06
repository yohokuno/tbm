#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//-------------------ûÚã§í ---------------------------------

CBossChenBase::CBossChenBase(){
	x = GAME_RIGHT + 80;
	y = GAME_CENTER_Y - GetH()/2;
}
int CBossChenBase::Draw(){
	int index = (count/7) % 2;
	RECT rect={index*GetW(),0,(index+1)*GetW(),GetH()};
	g_pStage5->imgChen.DrawRect(x,y,rect);
	return CBossBase::Draw();
}
//ÉXÉyÉãÉJÅ[Éh
int CBossChenBase::SpellCard() {
	flag = 1;
	pBack = g_pBack;
	g_pBack = new CBackChen();
	RECT rect = {0,0,320,512};
	g_lEffect2.Add( new CEffectCutin( &g_pStage5->imgTalk, &rect, GAME_CENTER_X, 120));
	return CBossBase::SpellCard();
}
//í èÌçUåÇ1
int CBossChenBase::NormalAtack1() {
	flag = 1;
	deffence = 1;
	SetHP(1500);
	SetTime(30);
	while( 1 ) {
		mt.Suspend(60);
		for( int k = 0; k < 2; k++ ) {
			int jmax = 1+GetDifficult();
			for( int j = 0; j < jmax; j++ ){
				//çUåÇ
				CTamaNormal tama;
				tama.x = GetX();
				tama.y = GetY();
				tama.type = 12;
				tama.color = 1;

				CDanmakuPolygons d(&tama);
				d.n= 4;
				d.m = 5;
				d.sv = 2;
				d.ev = 2;
				if(GetDifficult()==DIFF_LUNATIC)
					d.t = 10;
				else
					d.t = 2*(1+GetDifficult());
				d.offset = 45+90*j/jmax;

				d.Fire();
				g_pMaterial->sndFire.Play(0);
				mt.Suspend(60);
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


//-------------------íÜÉ{ÉX---------------------------------
CBossChenMiddle::CBossChenMiddle(){
	TSunFunctionCallback<CBossChenMiddle> fn;
	fn.SetFunction(this,&CBossChenMiddle::Thread);
	mt3.Start(&fn,0x8000);
}
int CBossChenMiddle::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossChenBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossChenMiddle::Draw(){
	CBossChenBase::Draw();
	return 1;
}
//ëSëÃìÆçÏ
int CBossChenMiddle::Thread(){
	TSunFunctionCallback<CBossChenMiddle> fn;
	TSunFunctionCallback<CBossChenBase> fnb;

	//èoåª
	flag = 0;
	deffence = 0;

	//âÔòb
	g_pPlayer->Talk();
	pTalk = new CTalkChen1();
	pTalk->Start();
	while( !pTalk->IsEnd() )
		mt3.Suspend();
	SAFE_DELETE(pTalk);
	g_pPlayer->Resume();

	//í èÌçUåÇ
	fnb.SetFunction(this,&CBossChenBase::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//âAózÅuëæã…ê}Åv
	SpellCard();
	fn.SetFunction(this,&CBossChenMiddle::Taikyoku);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	this->AddItem(ITEM_BOMB,1);

	//ÉGÉtÉFÉNÉg
	flag = 0;
	deffence = 0;
	g_lEffect.Add( new CEffectBreak( GetX(), GetY() ));
	mt3.Suspend( 60 );

	//ëﬁãp
	for( int i = 0; i < 46; i++ ){
		x += 5;
		mt3.Suspend();
	}
	return 1;
}
//âAózÅuëæã…ê}Åv
int CBossChenMiddle::Taikyoku()
{
	flag = 1;
	deffence = 1;
	SetHP(2000);
	SetTime(40);
	Suspend(60);

	for( int i = 0; i < 60; ++i ) {
		x += ( 450 - GetX() ) / 60;
		y += ( GAME_CENTER_Y - GetY() ) / 60;
		mt.Suspend(1);
	}

	CTamaTaikyoku tama;
	tama.x = GetX();
	tama.y = GetY();
	tama.type = 12;
	tama.v = 3;
	int offset = 0.0;
	while(1){
		for( int i = 0;; i++){
			//çUåÇ
			tama.color = 1;
			tama.a = offset;
			g_lTama.Add( tama.Copy() );
			tama.color = 0;
			tama.a = offset + 180;
			g_lTama.Add( tama.Copy() );

			g_pMaterial->sndFire.Play(0);
			double m;
			if( GetDifficult() == DIFF_LUNATIC )
				m = 1.5*(4-GetDifficult());
			else
				m = 2.0*(4-GetDifficult());

			if( i > 120/m )
				offset += 3.0*m*Sin((double)i*2);
			this->Suspend(m);
		}
	}
	return 1;
}
//----------------------------------------------------
CBossChen::CBossChen(){
	TSunFunctionCallback<CBossChen> fn;
	fn.SetFunction(this,&CBossChen::Thread);
	mt3.Start(&fn,0x8000);
}
int CBossChen::Run(){
	mt.Resume();
	mt2.Resume();
	mt3.Resume();
	CBossChenBase::Run();
	if( mt3.IsEnd() )
		return 0;
	else
		return 1;
}
int CBossChen::Draw(){
	CBossChenBase::Draw();
	return 1;
}
//ëSëÃìÆçÏÅièoåªÅ`âÔòbÅ`í èÌçUåÇÅ`ÉXÉyÉãÉJÅ[ÉhÅ`åÇîjÅj
int CBossChen::Thread(){
	TSunFunctionCallback<CBossChen> fn;
	TSunFunctionCallback<CBossChenBase> fnb;

	//èoåª
	flag = 0;
	deffence = 0;
	for( int i = 0; i < 46; i++ ){
		x -= 5;
		mt3.Suspend();
	}
	mt3.Suspend(30);

	//âÔòb
	g_pPlayer->Talk();
	pTalk = new CTalkChen2();
	pTalk->Start();
	while( !pTalk->IsEnd() )
		mt3.Suspend();
	SAFE_DELETE(pTalk);
	g_pPlayer->Resume();
	fntBoss.CreateList("Chen",12);

	//í èÌçUåÇ
	fnb.SetFunction(this,&CBossChenBase::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//éOê∏
	SpellCard();
	fn.SetFunction(this,&CBossChen::Num3);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//í èÌçUåÇ
	fnb.SetFunction(this,&CBossChenBase::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//élãG
	SpellCard();
	fn.SetFunction(this,&CBossChen::Num4);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();
	mt2.Stop();

	//í èÌçUåÇ
	fnb.SetFunction(this,&CBossChenBase::NormalAtack1);
	mt.Start(&fnb,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	mt.Stop();

	//å‹çs
	SpellCard();
	fn.SetFunction(this,&CBossChen::Num5);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//ñãä‘
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt3.Suspend(60);

	//òZ‰ä
	SpellCard();
	fn.SetFunction(this,&CBossChen::Num6);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();
	mt2.Stop();

	//ñãä‘
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt3.Suspend(60);

	//éµê_
	SpellCard();
	fn.SetFunction(this,&CBossChen::Num7);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//ÉUÉRçÌèú
	list<CEnemy*>::iterator itr;
	for( itr = g_lEnemy.begin(); itr != g_lEnemy.end(); itr++ ) {
		if( (*itr) != this )
			SAFE_DELETE(*itr);
	}
	g_lEnemy.EraseNull();

	//ñãä‘
	flag = 0;
	deffence = 0;
	mt.Stop();
	mt3.Suspend(60);

	//î™åT
	SpellCard();
	fn.SetFunction(this,&CBossChen::Num8);
	mt.Start(&fn,0x8000);
	while( !IsEnd() ) mt3.Suspend();
	SpellEnd();
	mt.Stop();

	//ÉGÉtÉFÉNÉg
	mt.Stop();
	mt2.Stop();
	flag = 0;
	deffence = 0;
	g_lEffect.Add( new CEffectBreak( GetX(), GetY() ));
	g_pPlayer->Talk();
	mt3.Suspend( 60 );

	//âÊñ äOÇ÷
	x=1000;

	//âÔòb
	pTalk = new CTalkChen3();
	pTalk->Start();
	while( !pTalk->IsEnd() )
	mt3.Suspend();
	SAFE_DELETE(pTalk);

	return 1;
}
//éOê∏
int CBossChen::Num3()
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
			for( int j = 0; j < 3; j++ ) {
				//çUåÇ
				CTamaTurn t;
				t.x = GetX();
				t.y = GetY();
				t.type = 12;
				t.color = 1+j;
				if( j==1) t.color = 6;
				t.v = 3;
				t.stop_time = 60;
				t.stop_count = 30;
				t.turn = 90;
				t.v2 = 3;

				CDanmakuCircle d(&t);
				d.n = 16*(1+GetDifficult());
				d.w = 360;
				d.offset = rand(0,360);
				d.aa = 360.0 / 3.0 * j;
				d.vv = 2.0;
				d.Fire();
				g_pMaterial->sndFire.Play(0);
				this->Suspend(30);
			}
			this->Suspend(60);
			CTamaNormal t;
			t.x = GetX();
			t.y = GetY();
			t.type = 12;
			t.color = 2;

			CDanmakuPolygons d(&t);
			d.n= 3;
			d.m = 5;
			d.sv = 2;
			d.ev = 2;
			if(GetDifficult()==DIFF_LUNATIC)
				d.t = 12;
			else
				d.t = 2*(1+GetDifficult());
			d.offset = 30;
			d.Fire();
			
			CDanmakuNonDirectionalLine d2(&t);
			d2.n = 3;
			d2.m = 4*(1+GetDifficult());
			d2.w = 360/3*2;
			d2.offset = 30;
			d2.sv = 2;
			d2.ev = 3;
			d2.Fire();

			for( int j = 0; j < 3; j++ ) {
				CDanmakuCircle d(&t);
				d.n = 16*(1+GetDifficult());
				d.w = 360;
				d.offset = rand(0,360);
				d.aa = 30+360.0 / 3.0 * j;
				d.vv = 4.0;
				d.Fire();
			}
			g_pMaterial->sndFire.Play(0);
			this->Suspend(120);
		}
	}
	return 1;
}
//élãG
int CBossChen::Num4()
{
	deffence = 0;
	SetHP(1500);
	SetTime(40);
	Suspend(60);
	deffence = 1;

	for( int j = 0; ; j++ ) {
		double xx = GAME_RIGHT - rand(30,90);
		double yy = GAME_CENTER_Y + rand( -60,60);
		for( int i = 0; i < 60; ++i ) {
			x += ( xx - GetX() ) / 60;
			y += ( yy - GetY() ) / 60;
			mt.Suspend(1);
		}
		mt.Suspend(60);

		if( j %2 == 0 ) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 12;
			tama.color = 5;

			CDanmakuToPlayerLine d(&tama);
			d.w = 180;
			d.n = 5;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.m = 12.0;
				d.sv = 4.0;
				d.ev = -0.5;
			}else {
				d.m = 2 + 2 * GetDifficult();
				d.sv = 2.0 + 0.2 * GetDifficult();
				d.ev = 2.0 - 0.2 * GetDifficult();
			}
			g_pMaterial->sndFire.Play(0);
			d.Fire();
		}else{
			CTamaNormal t;
			t.x = GetX();
			t.y = GetY();
			t.type = 12;
			t.color = 0;
			t.v = 2.0;

			CDanmakuNonDirectional d(&t);
			d.n = 16*(1+GetDifficult());
			d.w = 360;
			d.offset = 0;
			d.Fire();
			g_pMaterial->sndFire.Play(0);
		}

		mt.Suspend(60);

		if( j == 0 ) {
			TSunFunctionCallback<CBossChen> fn;
			fn.SetFunction(this,&CBossChen::Magata);
			mt2.Start(&fn,0x8000);
		}
	}
	return 1;
}
int CBossChen::Magata(){
	while(1){
		int t = 64*(4-GetDifficult());
		for( int i = 0; i < (GAME_WIDTH+240)/t; i++ ){
			CTamaNormal tama;
			tama.x = GAME_LEFT + i * t;
			tama.type = 12;
			tama.v = 2;

			tama.color = 1;
			tama.y = GAME_TOP-16;
			tama.a = 135;
			g_lTama.Add( tama.Copy() );

			tama.y = GAME_BOTTOM+16;
			tama.color = 3;
			tama.a = -135;
			g_lTama.Add( tama.Copy() );
			g_pMaterial->sndFire.Play(0);
		}
		mt2.Suspend(16*(4-GetDifficult()));
	}
	return 1;
}
//å‹çs
int CBossChen::Num5()
{
	deffence = 0;
	SetHP(2000);
	SetTime(50);
	double xx = GetX();
	double yy = GetY();
	for( int i = 0; i < 60; ++i ) {
		x += ( GAME_CENTER_X+120 - xx ) / 60.0;
		y += ( GAME_CENTER_Y - yy ) / 60.0;
		mt.Suspend(1);
	}
	Suspend(60);
	deffence = 1;
	double offset = -90;
	for( int i = 0;; ++i ) {
		int color = 1+i%5;
		if( i != 0 ) {
			switch(i%5){case 3:color=5;break;case 4:color=7;break;}
			{
				CTamaNormal t;
				t.x = GetX();
				t.y = GetY();
				t.type = 4;
				t.color = i%5;
				if(i%5==4) t.color=0;
				t.v = 2.0;

				CDanmakuNonDirectionalLine d(&t);
				d.n = 5;
				d.m = GetDifficult();
				d.sv = 2.0-0.4*GetDifficult();
				d.ev = 2.0+0.4*GetDifficult();
				d.w = 360/5*4;
				d.offset = offset;
				d.Fire();
			}
			for(int j = 0; j < 1+(GetDifficult()==DIFF_LUNATIC); j++ ){
				CTamaNormal t;
				t.x = GetX();
				t.y = GetY();
				t.type = 12;
				t.color = color;

				CDanmakuPolygons d(&t);
				d.n= 5;
				d.m = 5;
				d.sv = 2-j;
				d.ev = 2-j;
				if(GetDifficult()==DIFF_LUNATIC)
					d.t = 8;
				else
					d.t = 1.5*(1+GetDifficult());
				d.offset = offset;
				d.Fire();
			}
			g_pMaterial->sndFire.Play(0);
			Suspend(30);
		}

		for( int j = 0; j < 120/(10-GetDifficult()*2); j++ ) {
			CTamaTurn t;
			t.x = GetX();
			t.y = GetY();
			t.type = 12;
			t.color = color;
			t.v = 16.0;
			t.turn = 180-90/5;
			t.v2 = 4.0;
			t.stop_count = 10;
			t.stop_time = 1;

			CDanmakuNonDirectional d(&t);
			d.n = 5;
			d.w = 360;
			d.offset = offset;
			d.Fire();
			g_pMaterial->sndFire.Play(0);
			Suspend(10-GetDifficult()*2);
		}
		if( GetDifficult() > DIFF_NORMAL )
			offset = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
		else
			offset = rand(0,360);
	}
	return 1;
}
//òZ‰ä
int CBossChen::Num6()
{
	deffence = 0;
	SetHP(1800);
	SetTime(50);
	double xx = GetX();
	double yy = GetY();
	for( int i = 0; i < 60; ++i ) {
		x += ( GAME_RIGHT-120 - xx ) / 60.0;
		y += ( GAME_CENTER_Y - yy ) / 60.0;
		mt.Suspend(1);
	}
	Suspend(60);
	deffence = 1;

	CTamaNormal t;
	t.x = GetX();
	t.y = GetY();
	t.type = 12;
	t.color = 1;

	CDanmakuPolygons d(&t);
	d.n= 3;
	d.m = 5;
	d.sv = 2;
	d.ev = 2;
	if(GetDifficult()==DIFF_LUNATIC)
		d.t = 12;
	else
		d.t = 2*(1+GetDifficult());
	d.offset = 30;
	d.Fire();
	d.offset = 180+30;
	d.Fire();
	g_pMaterial->sndFire.Play(0);
	mt.Suspend(120);

	TSunFunctionCallback<CBossChen> fn;
	fn.SetFunction(this,&CBossChen::Kagome);
	mt2.Start(&fn,0x8000);

	mt.Suspend(120);

	for( int j = 0; ; j++ ) {
		double xx = GAME_RIGHT - rand(120,220);
		double yy = GAME_CENTER_Y + rand( -160,160);
		for( int i = 0; i < 60; ++i ) {
			x += ( xx - GetX() ) / 60.0;
			y += ( yy - GetY() ) / 60.0;
			CDanmakuToPlayerLine d(&t);
			d.w = 60;
			d.n = 3;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.m = 12.0;
				d.sv = 4.0;
				d.ev = -0.5;
			}else {
				d.m = 2 + 2 * GetDifficult();
				d.sv = 2.0 + 0.2 * GetDifficult();
				d.ev = 2.0 - 0.2 * GetDifficult();
			}
			d.Fire();
			g_pMaterial->sndFire.Play(0);
			mt.Suspend(60);

			{
				CDanmakuPolygons d(&t);
				d.n= 6;
				d.m = 5;
				d.sv = 2;
				d.ev = 2;
				if(GetDifficult()==DIFF_LUNATIC)
					d.t = 12;
				else
					d.t = 2*(1+GetDifficult());
				d.offset = 30;
				d.Fire();
				g_pMaterial->sndFire.Play(0);
				mt.Suspend(120);
			}
		}

	}
	return 1;
}
int CBossChen::Kagome(){
	while(1){
		int t = 40*(5-GetDifficult());
		CTamaNormal tama;
		tama.type = 12;
		tama.v = 2;
		tama.color = 7;
		for( int i = 0; i < (GAME_WIDTH+240)/t+1; i++ ){

			tama.x = GAME_LEFT + i * t;
			tama.y = GAME_TOP-16;
			tama.a = 120;
			g_lTama.Add( tama.Copy() );

			tama.x = GAME_LEFT + i * t + t/2;
			tama.y = GAME_BOTTOM+16;
			tama.a = -120;
			g_lTama.Add( tama.Copy() );
		}
		for( int i = 0; i < (GAME_HEIGHT+16)/t+1; i++ ){
			tama.x = GAME_RIGHT + 16;
			tama.y = GAME_TOP + 40 + i * t;
			tama.a = 180;
			g_lTama.Add( tama.Copy() );
		}
		g_pMaterial->sndFire.Play(0);
		mt2.Suspend(4*(6-GetDifficult()));
	}
	return 1;
}
//éµê_
int CBossChen::Num7()
{
	deffence = 0;
	SetHP(2000);
	SetTime(50);
	double xx = GetX();
	double yy = GetY();
	for( int i = 0; i < 60; ++i ) {
		x += ( GAME_RIGHT-120 - xx ) / 60.0;
		y += ( GAME_CENTER_Y - yy ) / 60.0;
		mt.Suspend(1);
	}
	Suspend(60);
	deffence = 1;

	g_lEnemy.Add( new CEnemy7God(GAME_RIGHT-260,GAME_CENTER_Y+80, 1) );
	g_lEnemy.Add( new CEnemy7God(GAME_RIGHT-260,GAME_CENTER_Y-80, 2) );
	g_pMaterial->sndFire.Play(0);
	Suspend(180);

	g_lEnemy.Add( new CEnemy7God(GAME_RIGHT-120,GAME_CENTER_Y+200, 3) );
	g_lEnemy.Add( new CEnemy7God(GAME_RIGHT-120,GAME_CENTER_Y-200, 4) );
	g_pMaterial->sndFire.Play(0);
	Suspend(160);

	g_lEnemy.Add( new CEnemy7God(GAME_RIGHT-40,GAME_CENTER_Y+120, 5) );
	g_lEnemy.Add( new CEnemy7God(GAME_RIGHT-40,GAME_CENTER_Y-120, 6) );
	g_pMaterial->sndFire.Play(0);
	Suspend(180);
	g_lEnemy.Add( new CEnemy7God(GAME_RIGHT-200,GAME_CENTER_Y, 7) );
	g_pMaterial->sndFire.Play(0);

	Suspend(600);

	for( int i = 0;; i++ ){
		CTamaNormal t;
		t.x = GetX();
		t.y = GetY();
		t.type = 12;
		t.color = 3;

		CDanmakuPolygons d(&t);
		d.n= 7;
		d.m = 5;
		d.sv = 1;
		d.ev = 1;
		if(GetDifficult()==DIFF_LUNATIC)
			d.t = 14;
		else
			d.t = 2*(1+GetDifficult());
		d.offset = 30;
		d.Fire();
		g_pMaterial->sndFire.Play(0);
		mt.Suspend(120);
	}
	return 1;
}
//î™åT
int CBossChen::Num8()
{
	deffence = 0;
	SetHP(2000);
	SetTime(50);
	double xx = GetX();
	double yy = GetY();
	for( int i = 0; i < 60; ++i ) {
		x += ( GAME_RIGHT-120 - xx ) / 60.0;
		y += ( GAME_CENTER_Y - yy ) / 60.0;
		mt.Suspend(1);
	}
	Suspend(60);
	deffence = 1;
	for( int i = 0;; i++ ){
		CTamaNormal t;
		t.x = GetX();
		t.y = GetY();
		t.type = 12;
		t.color = 2;

		CDanmakuPolygons d(&t);
		d.n= 8;
		d.m = 5;
		d.sv = 2;
		d.ev = 2;
		if(GetDifficult()==DIFF_LUNATIC)
			d.t = 14;
		else
			d.t = 2*(1+GetDifficult());
		d.offset = -90-180/8;
		d.Fire();
		g_pMaterial->sndFire.Play(0);

		t.color = 0;
		CDanmakuSegment d2(&t);
		mt.Suspend(30);
		for( int k = 0; k < 3; k++ ){
			for( int j = 0; j < 8; j++ ){
				if( (((j+i)%8)>>k)%2 ) {
					d2.a1 = -90-180/8		+5+360/8*j;
					d2.a2 = -90-180/8+360/8	-5+360/8*j;
					d2.v1 = d2.v2 = 2;
					d2.Set(d.t);
					d2.Fire();
				}else{
					d2.a1 = -90-180/8		+5+360/8*j;
					d2.a2 = -90-180/8+180/8	-5+360/8*j;
					d2.v1 = 2;
					d2.v2 = 2*Cos(d2.a2-d2.a1);
					d2.Set(d.t);
					d2.Fire();
					d2.a1 = -90-180/8+180/8	+5+360/8*j;
					d2.a2 = -90-180/8+360/8	-5+360/8*j;
					d2.v1 = 2*Cos(d2.a2-d2.a1);
					d2.v2 = 2;
					d2.Set(d.t);
					d2.Fire();
				}
			}
			g_pMaterial->sndFire.Play(0);
			mt.Suspend(10);
		}
		
		mt.Suspend(30);
		t.color = 2;
		d.Fire();
		g_pMaterial->sndFire.Play(0);
		mt.Suspend(120);

		{
			CTamaNormal t;
			t.x = GetX();
			t.y = GetY();
			t.type = 4;
			t.color = 2;
			if(i%5==4) t.color=0;
			t.v = 2.0;

			CDanmakuNonDirectionalLine d(&t);
			d.n = 8;
			d.m = GetDifficult();
			d.sv = 2.0-0.4*GetDifficult();
			d.ev = 2.0+0.4*GetDifficult();
			d.w = 180/8*7;
			d.offset = 180/8+Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			d.Fire();
			g_pMaterial->sndFire.Play(0);
			mt.Suspend(120);
		}
	}
	return 1;
}
