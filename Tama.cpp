#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

SunList<CTama> g_lTama;

#define TAMA_CLIP 64

//ëSÇƒÇÃíeÇÉAÉCÉeÉÄÇ…ïœÇ¶ÇÈ
int TamaToItem() {
	{
		list<CTama*>::iterator itr;
		for( itr = g_lTama.begin(); itr != g_lTama.end(); itr++ ) {
			if( *itr != NULL ) {
				if( !(*itr)->Item() ) {
					SAFE_DELETE( *itr );
				}
			}
		}
	}
	{
		list<CLaser*>::iterator itr;
		for( itr = g_lLaser.begin(); itr != g_lLaser.end(); itr++ ) {
			if( *itr != NULL ) {
				if( !(*itr)->Item() ) {
					SAFE_DELETE( *itr );
				}
			}
		}
	}
	return 1;
}
//ÉtÉäÅ[ÉY
int TamaFreeze() {
	list<CTama*>::iterator itr;
	for( itr = g_lTama.begin(); itr != g_lTama.end(); itr++ ) {
		if( *itr != NULL ) {
			if( !(*itr)->Freeze() ) {
				SAFE_DELETE( *itr );
			}
		}
	}
	return 0;
}
//ÉâÉìÉ_ÉÄÇ…ìÆÇ≠
int TamaRandom(double v) {
	list<CTama*>::iterator itr;
	for( itr = g_lTama.begin(); itr != g_lTama.end(); itr++ ) {
		if( *itr != NULL ) {
			(*itr)->v = v;
			(*itr)->a = rand(0,360);
		}
	}
	return 0;
}
//à⁄ìÆçƒäJ
int TamaResume(double v) {
	list<CTama*>::iterator itr;
	for( itr = g_lTama.begin(); itr != g_lTama.end(); itr++ ) {
		if( *itr != NULL ) {
			(*itr)->v = v;
		}
	}
	return 0;
}
//ìGíeâÊëúï`âÊ
void TamaRect( RECT &rect, int type, int color) {
	switch( type ) {
	case 0: //è¨íe
		SetRect(&rect,8*color,0,8*(color+1),8);
		break;
	case 1: //íÜíe
		SetRect(&rect,16*color,8,16*(color+1),24);
		break;
	case 2: //ìÒèdíe
		SetRect(&rect,16*color,24,16*(color+1),40);
		break;
	case 3: //ëÂíe
		SetRect(&rect,24*color,88,24*(color+1),112);
		break;
	case 4: //â¡éZíe
		SetRect(&rect,64*color,192,64*(color+1),256);
		break;
	case 5: //à≈íÜíe
		SetRect(&rect,16*color,56,16*(color+1),72);
		break;
	case 10: //ïƒíe
		SetRect(&rect,16*color,40,16*(color+1),48);
		break;
	case 11: //à≈íe
		SetRect(&rect,16*color,48,16*(color+1),56);
		break;
	case 12: //å‰éDíe
		SetRect(&rect,128+16*color,8,128+16*(color+1),24);
		break;
	case 13: //û∂íe
		SetRect(&rect,128+16*color,24,128+16*(color+1),40);
		break;
	case 14: //ÉNÉiÉCíe
		SetRect(&rect,128+16*color,40,128+16*(color+1),48);
		break;
	case 15: //Ç–Çµå`íe
		SetRect(&rect,128+16*color,48,128+16*(color+1),56);
		break;
	case 16: //ê·ÇÃåãèªíe
		SetRect(&rect,32*color,112,32*(color+1),144);
		break;
	case 17: //ÉiÉCÉtíe
		SetRect(&rect,32*color,144,32*(color+1),160);
		break;
	case 18: //í±íe
		SetRect(&rect,32*color,160,32*(color+1),192);
		break;
	}
}
//ìñÇΩÇËîªíËÇÃÉTÉCÉY
double TamaSize( int type ) {
	switch( type ) {
	case 0: //è¨íe
		return 1;
	case 1: //íÜíe
		return 2;
	case 2: //ìÒèdíe
		return 2;
	case 3: //ëÂíe
		return 3;
	case 4: //â¡éZíe
		return 4;
	case 5: //íÜà≈íe
		return 2;
	case 10: //ïƒíe
		return 1;
	case 11: //à≈íe
		return 1;
	case 12: //å‰éDíe
		return 2;
	case 13: //û∂íe
		return 2;
	case 14: //ÉNÉiÉCíe
		return 1;
	case 15: //Ç–Çµå`íe
		return 1;
	case 16: //ê·ÇÃåãèªíe
		return 4;
	case 17: //ÉiÉCÉtíe
		return 2;
	case 18: //í±íe
		return 2;
	}
	return 0;
}
int TamaW(int type)
{
	RECT rect;
	TamaRect(rect,type,0);
	return rect.right - rect.left;
}
int TamaH(int type)
{
	RECT rect;
	TamaRect(rect,type,0);
	return rect.bottom - rect.top;
}

int TamaDraw( double x,double y,int type,int color) {
	int result;
	RECT rect;
	TamaRect(rect,type,color);
	if( type == 4)
		Sun3D.SetBlendingType( BLEND_ADD );
	result = g_pMaterial->imgTama.DrawRect(x - TamaW(type)/2,y - TamaH(type)/2,rect);
	if( type == 4)
		Sun3D.SetBlendingType( BLEND_NORMAL );
	return result;
}
int TamaDrawRotate( double x,double y,double angle,int type,int color) {
	int result;
	RECT rect;
	TamaRect(rect,type,color);
	result = g_pMaterial->imgTama.DrawRect(x - TamaW(type)/2,y - TamaH(type)/2,rect,alpha(255),1,Deg2Rad(angle));
	return result;
}

//ìôë¨íºê¸â^ìÆ
CTamaNormal::CTamaNormal()
{
	this->x = GAME_RIGHT;
	this->y = GAME_TOP + GAME_HEIGHT/2;
	this->v = 1;
	this->a = 180;
	this->type = 13;
	this->color = 1;
	this->graze = 0;
	this->out = 1;
	this->count = 0;
}
int CTamaNormal::Run(){
	//âÊñ äO
	if( GetX() < GAME_LEFT - TAMA_CLIP ||
		GetX() > GAME_RIGHT + TAMA_CLIP ||
		GetY() < GAME_TOP - TAMA_CLIP ||
		GetY() > GAME_BOTTOM + TAMA_CLIP ) {
		if( out == 0 )
			return 0;
		out++;
		if( out > 60 )
			return 0;
	}
	else
		out = 0;
	return Run2();
}
int CTamaNormal::Run2(){
	count ++;

	double w,h;
	w = TamaW( type );
	h = TamaH( type );

	x += v * Cos(a);
	y += v * Sin(a);
	//ÉvÉåÉCÉÑÅ[Ç∆ÇÃÇ†ÇΩÇËîªíËÅiâ~å^ìØémÅj
	if( g_pPlayer->state != 3 ) {
		double r1 = TamaSize(type) + g_pPlayer->GetSize();
		double r2 = TamaSize(type) + g_pPlayer->GetGrazeSize();
		if( pow2(g_pPlayer->GetX()-GetX()) + pow2(g_pPlayer->GetY()-GetY()) < r1*r1 ) {
			if( !g_pPlayer->GetMuteki() )
				g_pPlayer->Damage();
			else 
				Vanish();
			return 0;
		}
		else if( !graze ) {//ÉOÉåÉCÉY
			if( g_pPlayer->state == 0 ){
				if( pow2(g_pPlayer->GetX()-GetX()) + pow2(g_pPlayer->GetY()-GetY()) < r2*r2 ) {
					g_pPlayer->Graze();
					g_lEffect.Add( new CEffectGraze(GetX(),GetY()));
					graze = 1;
				}
			}
		}
	}
	//É}ÉXÉ^Å[ÉXÉpÅ[ÉNÇ≈ÇÃíeè¡Çµ
	if( g_pPlayer->GetMasterHantei2( GetX(), GetY() ) ) {
		Item();
		Vanish();
		return 0;
	}
	return 1;
}
int CTamaNormal::Draw(){
	if( type < 10 ) {
		return TamaDraw(x,y,type,color);
	}
	else {
		return TamaDrawRotate(x,y,a,type,color);
	}
}
CTama *CTamaNormal::Copy() {
	CTamaNormal *p = new CTamaNormal();
	*p = *this;
	return p;
}
int CTamaNormal::Item() {
	g_lItem.Add( new CItem( GetX(),GetY(),0 ) );
	return 0;
}
int CTamaNormal::Freeze() {
	if( type != 4 )
		color = 0;
	v = 0;
	return 1;
}
double CTamaNormal::GetX() {
	return x;
}
double CTamaNormal::GetY() {
	return y;
}
double CTamaNormal::GetVX() {
	return v * Cos(a);
}
double CTamaNormal::GetVY() {
	return v * Sin(a);
}
int CTamaNormal::Vanish(){
	g_lEffect.Add( new CEffectRing( GetX(), GetY() ) );
	return 0;
}
void CTamaNormal::SetVelocityXY(double vx,double vy){
	this->v = sqrt( vx*vx + vy*vy );
	this->a = Atan2( vx, vy );
}
//ìôâ¡ë¨ìxâ^ìÆ
CTamaIncrease::CTamaIncrease(){
}
int CTamaIncrease::Run(){
	if(count < end )
		v += inc;
	return CTamaNormal::Run();
}
CTama *CTamaIncrease::Copy() {
	CTamaIncrease *p = new CTamaIncrease();
	*p = *this;
	return p;
}
void CTamaIncrease::SetIncrease( double sv,double ev,int end )
{
	v = sv;
	inc = (ev-sv)/end;
	this->end = end;
}
//ê‹ÇÍíe
CTamaTurn::CTamaTurn(){
}
int CTamaTurn::Run(){
	//í‚é~
	if(count == stop_count)
		v = 0;
	//ê‹ÇÍÇÈ
	else if( count == stop_count + stop_time){
		v = v2;
		a += turn;
	}

	return CTamaNormal::Run();
}
CTama *CTamaTurn::Copy() {
	CTamaTurn *p = new CTamaTurn();
	*p = *this;
	return p;
}
//é©ã@ë_Ç¢ê‹ÇÍíe
CTamaTurnToPlayer::CTamaTurnToPlayer(){
}
int CTamaTurnToPlayer::Run(){
	//í‚é~
	if(count == stop_count)
		v = 0;
	//ê‹ÇÍÇÈ
	if( count == stop_count + stop_time){
		v = v2;
		a = Atan2(g_pPlayer->GetX()-GetX(),g_pPlayer->GetY()-GetY());
	}

	return CTamaNormal::Run();
}
CTama *CTamaTurnToPlayer::Copy() {
	CTamaTurnToPlayer *p = new CTamaTurnToPlayer();
	*p = *this;
	return p;
}

//åuíe
CTamaHotaru::CTamaHotaru(){
}
int CTamaHotaru::Run(){
	a += sa * Sin(s*count);
	return CTamaNormal::Run();
}
CTama *CTamaHotaru::Copy() {
	CTamaHotaru *p = new CTamaHotaru();
	*p = *this;
	return p;
}

//ê‹ÇÍíe2
CTamaTurn2::CTamaTurn2(){
}
int CTamaTurn2::Run(){
	//ê‹ÇÍÇÈ
	if(count == turn_count){
		v = v2;
		a = turn;
	}

	return CTamaNormal::Run();
}
CTama *CTamaTurn2::Copy() {
	CTamaTurn2 *p = new CTamaTurn2();
	*p = *this;
	return p;
}
//ï™óÙíe
CTamaBurst::CTamaBurst(){
	n = 1;
}
int CTamaBurst::Run(){
	//ï™óÙ
	if(count == burst_count){
		count ++;
		CDanmakuNonDirectionalLine danmaku(this);
		danmaku.offset = a - 90 * t;
		danmaku.n = n;
		danmaku.m = m;
		danmaku.sv = sv;
		danmaku.ev = ev;
		danmaku.Fire();
	}

	if( GetX() < GAME_LEFT - TAMA_CLIP)
		return 0;
	return CTamaNormal::Run2();
}
CTama *CTamaBurst::Copy() {
	CTamaBurst *p = new CTamaBurst();
	*p = *this;
	return p;
}
//ÉÄÉJÉfíe
CTamaMukade::CTamaMukade(){
}
int CTamaMukade::Run(){
	int c = 120;
	if( count < c )
		a += (double) t * 90 / c;
	else if( count < c * 4 )
		a -= (double) t * 90 / c;
	else if( count < c * 5 )
		a += (double) t * 90 / c;
	//ï™óÙ
	if(count > burst_start && count < burst_end ) {
		if(count % burst_interval == burst_start % burst_interval){
			CTamaNormal tama;
			tama = (CTamaNormal)*this;
			tama.type = 2;
			CDanmakuNonDirectionalLine danmaku(&tama);
			danmaku.offset = a - 90 * t;
			danmaku.n = 1;
			danmaku.m = 2;
			danmaku.sv = -2;
			danmaku.ev = 2;
			danmaku.Fire();
		}
	}
	return  CTamaNormal::Run();
}
CTama *CTamaMukade::Copy() {
	CTamaMukade *p = new CTamaMukade();
	*p = *this;
	return p;
}
//ÉÄÉJÉfíe(ë´Åj
CTamaMukadeLeg::CTamaMukadeLeg(){
}
int CTamaMukadeLeg::Run(){
	int c = 120;
	if( count < c )
		a += (double) t * 90 / c;
	else if( count < c * 4 )
		a -= (double) t * 90 / c;
	else if( count < c * 5 )
		a += (double) t * 90 / c;
	xx += v * Cos(a);
	yy += v * Sin(a);
	x = xx + s * Cos(a+tt*90) + 4 * Cos(a) * Sin(count*16);
	y = yy + s * Sin(a+tt*90) + 4 * Sin(a) * Sin(count*16);
	return  CTamaNormal::Run();
}
CTama *CTamaMukadeLeg::Copy() {
	CTamaMukadeLeg *p = new CTamaMukadeLeg();
	*p = *this;
	return p;
}

//íuÇ´íe
CTamaSet::CTamaSet(){
}
int CTamaSet::Run(){
	a += (double) t * 360 / (c*(count+1));
	if( count % set_interval == 0 ) {
		CTamaNormal *pTama = new CTamaNormal;
		pTama->x = GetX();
		pTama->y = GetY();
		pTama->type = this->type;
		pTama->color = this->color;
		pTama->a = 0;
		pTama->v = 0;
		g_lTama.Add(pTama);
	}
	return CTamaNormal::Run();
}
CTama *CTamaSet::Copy() {
	CTamaSet *p = new CTamaSet();
	*p = *this;
	return p;
}
//ÉtÉäÅ[ÉYíe
int CTamaFreeze::Run(){
	if( count == t ) {
		color = 0;
		vv = v;
		v = 0;
	}
	else if( count == t + c ) {
		switch( r ) {
		case 0:
			color = 5;
			v = vv;
			break;
		case 1:
			v = 1;
			a = rand(0,360);
			break;
		case 2:
			v = 1;
			a = a+180;
			break;
		}
	}
	return  CTamaNormal::Run();
}
CTama *CTamaFreeze::Copy() {
	CTamaFreeze *p = new CTamaFreeze();
	*p = *this;
	return p;
}

//ï™óÙíe2
int CTamaBurst2::Run(){
	//ï™óÙ
	if(count > burst_start && count < burst_end && count % burst_interval == 0){
		count ++;
		CTamaNormal tama = (CTamaNormal)*this;
		CDanmakuNonDirectionalLine danmaku(&tama);
		danmaku.offset = a + offset;
		danmaku.w = w;
		danmaku.n = n;
		danmaku.m = m;
		danmaku.sv = sv;
		danmaku.ev = ev;
		danmaku.Fire();
	}

	if( GetX() < GAME_LEFT - TAMA_CLIP)
		return 0;
	return CTamaNormal::Run2();
}
CTama *CTamaBurst2::Copy() {
	CTamaBurst2 *p = new CTamaBurst2();
	*p = *this;
	return p;
}

//ëöíe
list<CTamaFrost*> CTamaFrost::lTamaFrost;
CTamaFrost::CTamaFrost(){
	mode = 0;		//ÉÇÅ[Éh
	frost = 8.0;	//ïtíÖãóó£
	freeze = 30;	//ì]ä∑éûä‘
}
CTamaFrost::~CTamaFrost(){
	lTamaFrost.remove( this );
}
int CTamaFrost::Run(){
	switch( mode ){
	case 0:
		//âÊñ í[Ç…ïtíÖ
		{
			list<CTamaFrost*>::iterator itr;
			if( x < GAME_LEFT + frost ||
				x > GAME_RIGHT - frost ||
				y < GAME_TOP + frost ||
				y > GAME_BOTTOM - frost ) {
				Stop();
			}
			else for( itr = lTamaFrost.begin(); itr != lTamaFrost.end(); itr++ ) {
				if( pow2(x-(*itr)->x) + pow2( y-(*itr)->y ) < pow2( frost ) ) {
					Stop();
					break;
				}
			}
			break;
		}

	case 1:
		//à⁄ìÆíÜ
		if( GetX() < GAME_LEFT - TAMA_CLIP ||
			GetX() > GAME_RIGHT + TAMA_CLIP ||
			GetY() < GAME_TOP - TAMA_CLIP ||
			GetY() > GAME_BOTTOM + TAMA_CLIP ) {
			return 0;
		}
		break;
	case 2:
		//ì]ä∑ëO
		if( count == freeze ) {
			a += 180;
			mode = 3;
		}
		break;
	case 3:
		//É{ÉXÇ…ïtíÖ
		{
			list<CTamaFrost*>::iterator itr;
			if( pow2(x-g_pBoss->GetX()) + pow2( y-g_pBoss->GetY() ) < pow2( frost ) ) {
				Stop();
			}
			else for( itr = lTamaFrost.begin(); itr != lTamaFrost.end(); itr++ ) {
				double xx = x-(*itr)->x;
				double yy = y-(*itr)->y;
				if( pow2(xx) + pow2( yy ) < pow2( frost ) ) {
					Stop();
					break;
				}
			}
			break;
		}
	case 4:
		//é©ã@Ç…ïtíÖ
		{
			list<CTamaFrost*>::iterator itr;
			if( pow2(x-g_pPlayer->GetX()) + pow2( y-g_pPlayer->GetY() ) < pow2( frost ) ) {
				Stop();
			}
			else for( itr = lTamaFrost.begin(); itr != lTamaFrost.end(); itr++ ) {
				double xx = x-(*itr)->x;
				double yy = y-(*itr)->y;
				if( pow2(xx) + pow2( yy ) < pow2( frost ) ) {
					Stop();
					break;
				}
			}
			break;
		}
	}
	return  CTamaNormal::Run2();
}
void CTamaFrost::Stop() {
	lTamaFrost.push_back( this );
	v = 0;
	color = 0;
	mode = 1;
}
CTama *CTamaFrost::Copy() {
	CTamaFrost *p = new CTamaFrost();
	*p = *this;
	return p;
}

//åãèªíe
list<CTamaCristal*> CTamaCristal::lTamaCristal;
CTamaCristal::CTamaCristal(){
	mode = 0;
}
CTamaCristal::~CTamaCristal(){
	lTamaCristal.remove( this );
}
int CTamaCristal::Run(){
	switch( mode ){
	case 1:
		//âÊñ äO
		if( GetX() < GAME_LEFT - TAMA_CLIP ||
			GetX() > GAME_RIGHT + TAMA_CLIP ||
			GetY() < GAME_TOP - TAMA_CLIP ||
			GetY() > GAME_BOTTOM + TAMA_CLIP ) {
			return 0;
		}
		break;
	case 2:
		if( count == 30 ) {
			a += 180;
			mode = 3;
		}
		break;
	case 3:
		{
			double frost = 8.0;
			double dist = 16.0;
			list<CTamaCristal*>::iterator itr;
			if( pow2(x-g_pBoss->GetX()) + pow2( y-g_pBoss->GetY() ) < pow2( frost ) ) {
				Stop();
			}
			else for( itr = lTamaCristal.begin(); itr != lTamaCristal.end(); itr++ ) {
				double xx = x-(*itr)->x;
				double yy = y-(*itr)->y;
				if( pow2(xx) + pow2( yy ) < pow2( frost ) ) {
/*
					double aa = Atan2( xx, yy );
					double rr = (int)( (*itr)->a + aa ) / 120 * 120;
					x = (*itr)->x + dist * Cos( rr );
					y = (*itr)->y + dist * Sin( rr );
					a = rr;
*/
					Stop();
					break;
				}
			}
			break;
		}
	case 4:
		{
			double frost = 16.0;
			list<CTamaCristal*>::iterator itr;
			if( x < GAME_LEFT + frost ||
				x > GAME_RIGHT - frost ||
				y < GAME_TOP + frost ||
				y > GAME_BOTTOM - frost ) {
				Stop();
			}
			else for( itr = lTamaCristal.begin(); itr != lTamaCristal.end(); itr++ ) {
				double xx = x-(*itr)->x;
				double yy = y-(*itr)->y;
				if( pow2(xx) + pow2( yy ) < pow2( frost ) ) {
					double aa = (*itr)->a + Atan2( xx, yy );
					double rr = (int)aa/120 * 120;
					x = (*itr)->x + frost * Cos( rr );
					y = (*itr)->y + frost * Sin( rr );
					a = (*itr)->a + 60;
					Stop();
					break;
				}
			}
			break;
		}
	}
	return  CTamaNormal::Run2();
}
void CTamaCristal::Stop() {
	lTamaCristal.push_back( this );
	v = 0;
	color = 0;
	mode = 1;
}
CTama *CTamaCristal::Copy() {
	CTamaCristal *p = new CTamaCristal();
	*p = *this;
	return p;
}

//ÉsÉìÉ|ÉCÉìÉgíe
int CTamaPinPoint::Run(){
	switch( mode ) {
	case 0:
		{
			double xx = g_pPlayer->GetX() - GetX();
			double yy = g_pPlayer->GetY() - GetY();
			double vx = v * Cos( a );
			double vy = v * Sin( a );
			double tt = xx * vx + yy * vy;
			if( v != 0 && tt < 0 ) {
				mode = 1;
				color = 0;
				vv = v;
				v = 0;
			}
			break;
		}
	case 1:
		break;
	}
	return  CTamaNormal::Run();
}
CTama *CTamaPinPoint::Copy() {
	CTamaPinPoint *p = new CTamaPinPoint();
	*p = *this;
	return p;
}

//É{ÉCÉhíe
list<CTamaBoid*> CTamaBoid::lTamaBoid;
double CTamaBoid::gx1=0, CTamaBoid::gy1=0;
double CTamaBoid::gx2=0, CTamaBoid::gy2=0;
double CTamaBoid::gx3=0, CTamaBoid::gy3=0;
CTamaBoid::CTamaBoid(){
	w1 = 0.0;
	w2 = 1.0;
	w3 = 0.0;
	loop = 0;
	lTamaBoid.push_back( this );
}
CTamaBoid::~CTamaBoid(){
	lTamaBoid.remove( this );
}
int CTamaBoid::Run(){
	double gx = (w1*gx1 + w2*gx2 + w3*gx3) / (w1+w2+w3);
	double gy = (w1*gy1 + w2*gy2 + w3*gy3) / (w1+w2+w3);
	double nvx = (gx - x) / (1000.0 + 0*count);
	double nvy = (gy - y) / (1000.0 + 0*count);
	double vx = v*Cos(a);
	double vy = v*Sin(a);
	
	list<CTamaBoid*>::iterator itr;
	for( itr = lTamaBoid.begin(); itr != lTamaBoid.end(); itr++ ) {
		if( (*itr) == this ) continue;
		double xx = x-(*itr)->x;
		double yy = y-(*itr)->y;
		double dd = pow2(xx) + pow2(yy);
		if( dd < pow2(20.0)) {
			const double para1 = 0.5;
			const double para2 = 20.0;
			if( para2 + x - (*itr)->x != 0) {
				if( x > (*itr)->x )
					nvx += para1 / (para2 + x - (*itr)->x);
				else
					nvx -= para1 / (para2 + x - (*itr)->x);
			}
			if( para2 + y - (*itr)->y != 0) {
				if( y > (*itr)->y )
					nvy += para1 / (para2 + y - (*itr)->y);
				else
					nvy -= para1 / (para2 + y - (*itr)->y);
			}
		}
		if( dd < pow2( 30.0 )) {
			nvx += (0.99 * vx + 0.01 * (*itr)->v*Cos((*itr)->a)) - vx;
			nvy += (0.99 * vy + 0.01 * (*itr)->v*Sin((*itr)->a)) - vy;
		}
	}
	vx += nvx;
	vy += nvy;
	//vx = 2.0 * exp(vx)/(1.0+exp(vx));
	//vy = 2.0 * exp(vy)/(1.0+exp(vy));
	a = Atan2(vx,vy);
	//v = sqrt(pow2(vx)+pow2(vy));
	v = vmax * exp(v)/(1.0+0.5*exp(v));

	if( loop ) { //âÊñ í[ÉãÅ[Év
		x = mod(x-GAME_LEFT,GAME_WIDTH+32)+GAME_LEFT;
		y = mod(y-GAME_TOP,GAME_HEIGHT+32)+GAME_TOP;
		return  CTamaNormal::Run2();
	}
	return  CTamaNormal::Run();
}
void CTamaBoid::UpdateGoal(){
	if( lTamaBoid.size() > 0 ) {
		gx1 = 0.0;
		gy1 = 0.0;
		list<CTamaBoid*>::iterator itr;
		for( itr = lTamaBoid.begin(); itr != lTamaBoid.end(); itr++ ) {
			if( *itr != NULL ) {
				gx1 += (*itr)->x;
				gy1 += (*itr)->y;
			}
		}
		gx1 /= lTamaBoid.size();
		gy1 /= lTamaBoid.size();
	}

	if( g_pPlayer != NULL ) {
		gx2 = g_pPlayer->GetX();
		gy2 = g_pPlayer->GetY();
	}

	if( g_pBoss != NULL ) {
		gx3 = g_pBoss->GetX();
		gy3 = g_pBoss->GetY();
	}
}
CTama *CTamaBoid::Copy() {
	CTamaBoid *p = new CTamaBoid();
	*p = *this;
	return p;
}
//ëæã…ê}
CTamaTaikyoku::CTamaTaikyoku()
{
	t1 = 60;
	t2 = 180;
	aa = 1.0;
}
int CTamaTaikyoku::Run() {
	if( count < t1 ) {
		a += 180.0/t1*aa;
	}
	else if( count < t2 ) {
		a += 180.0/(t2-t1)*aa;
	}
	else{
	}
	return CTamaNormal::Run();
}
CTama *CTamaTaikyoku::Copy() {
	CTamaTaikyoku *p = new CTamaTaikyoku();
	*p = *this;
	return p;
}

//É^ÉCÉtÅ[Éì
list<CTamaTyphoon*> CTamaTyphoon::lTamaTyphoon;
CTamaTyphoon::CTamaTyphoon(){
	lTamaTyphoon.push_back( this );
}
CTamaTyphoon::~CTamaTyphoon(){
	lTamaTyphoon.remove( this );
}
int CTamaTyphoon::Run(){
	list<CTamaTyphoon*>::iterator itr;
	for( itr = lTamaTyphoon.begin(); itr != lTamaTyphoon.end(); itr++ ) {
		if( *itr != this ) {
			double vx = v * Cos(a);
			double vy = v * Sin(a);

			double xx = x-(*itr)->x;
			double yy = y-(*itr)->y;
			double dist = sqrt( xx*xx + yy*yy );
			if( dist != 0.0 ) {
				//êÀóÕ
//				vx += xx / dist / dist * 0.01;
//				vy += yy / dist / dist * 0.01;

				//à¯óÕ
				vx += xx *(1.0/dist-0.01) * 0.0001;
				vy += yy *(1.0/dist-0.01) * 0.0001;

				v = sqrt( vx*vx + vy*vy );
				a = Atan2( vx, vy );
				if( v > 4 ) v = 4;
			}
		}
		//ÉRÉäÉIÉäÇÃóÕÅièŒÅj
		a -= 0.001 * v;
	}
	return  CTamaNormal::Run();
}
CTama *CTamaTyphoon::Copy() {
	CTamaTyphoon *p = new CTamaTyphoon();
	*p = *this;
	return p;
}
//ÉnÉäÉPÅ[Éì
CTamaHurricane::CTamaHurricane(){
	t = 1;
	c = 4.0;
}
int CTamaHurricane::Run(){
	a += (double) t * 360 / (c*(count+1));
	return CTamaNormal::Run();
}
CTama *CTamaHurricane::Copy() {
	CTamaHurricane *p = new CTamaHurricane();
	*p = *this;
	return p;
}
//ÉcÉiÉ~
CTamaTsunami::CTamaTsunami(){
	this->ax = -0.01;
	this->ay = 0.0;
	this->end = 30;
}
int CTamaTsunami::Run(){
	if( count < end ) {
		SetVelocityXY(GetVX()+ax,GetVY()+ay);
	}
	return CTamaNormal::Run();
}
CTama *CTamaTsunami::Copy() {
	CTamaTsunami *p = new CTamaTsunami();
	*p = *this;
	return p;
}

//îΩéÀíe
CTamaReflect::CTamaReflect(){
	num = 3;
}
int CTamaReflect::Run(){
	if( GetX() < GAME_LEFT || GetX() > GAME_RIGHT )
		SetVelocityXY( -GetVX(), GetVY() );
	if( GetY() < GAME_TOP || GetY() > GAME_BOTTOM )
		SetVelocityXY( GetVX(), -GetVY() );
	return CTamaNormal::Run();
}
CTama *CTamaReflect::Copy() {
	CTamaReflect *p = new CTamaReflect();
	*p = *this;
	return p;
}
