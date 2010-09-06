#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

SunList<CShot> g_lShot;

const double v = 25.0;
const double a = 2;


CMainShot::CMainShot(double x,double y)
{
	this->x = x;
	this->y = y - GetH()/2;
	v = 0;
}
CMainShot::~CMainShot(){
}
int CMainShot::Run(){
	v += a;
	x += v;
	if( x > GAME_RIGHT + 48 )
		return 0;
	return 1;
}
int CMainShot::Draw(){
	g_pMaterial->imgShot1.Draw(x,y);
	return 1;
}
void CMainShot::GetHantei(RECT *pRect){
	pRect->left = x;
	pRect->top	= y;
	pRect->right = x + 38;
	pRect->bottom = y + 32;
}
double CMainShot::GetDamage(){
	g_lEffect2.Add( new CEffectStar( x+38, y+16 ) );
	g_pSystem->AddScore( SCORE_UTIKOMI );
	return 3.0;
}
double CMainShot::GetH(){
	return 48;
}


CSubShot::CSubShot(double x,double y,double s)
{
	this->x = x;
	this->y = y - GetH()/2;
	this->s = s;
}
CSubShot::~CSubShot(){
}
int CSubShot::Run(){
	x += v * Cos(s);
	if( x > GAME_RIGHT + 48 )
		return 0;
	y += v * Sin(s);
	return 1;
}
int CSubShot::Draw(){
	double a = s * PI / 180;
	g_pMaterial->imgShot2.Draw(x,y,alpha(120),1,a);
	return 1;
}

void CSubShot::GetHantei(RECT *pRect){
	pRect->left = x;
	pRect->top	= y;
	pRect->right = x + 48;
	pRect->bottom = y + 16;
}
double CSubShot::GetDamage(){
	g_lEffect.Add( new CEffectHit( x+48, y+8 ) );
	g_pSystem->AddScore( SCORE_UTIKOMI );
	return 1.0;
}
double CSubShot::GetH(){
	return 16;
}