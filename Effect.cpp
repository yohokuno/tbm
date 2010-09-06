#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

SunList<CEffect> g_lEffect;
SunList<CEffect> g_lEffect2;
SunList<CEffect> g_lEffect3;

//エフェクト基礎クラス
CEffectBase::CEffectBase(){
	count = 0;
}
int CEffectBase::Run(){
	count ++;
	return 1;
}

//リングエフェクト
CEffectRing::CEffectRing(double x,double y)
{
	this->effect.m_x = x;
	this->effect.m_y = y;
	this->effect.m_inRadius = 0;
	this->effect.m_outRadius = 10;
	this->effect.m_inColor = mono(0);
	this->effect.m_outColor = mono(255);
	this->effect.m_num = 16;
}
int CEffectRing::Run(){
	if(!CEffectBase::Run())
		return 0;
	if( count > 10 )
		return 0;
	this->effect.m_outRadius += 3;
	return 1;
}
int CEffectRing::Draw(){
	Sun3D.SetBlendingType( BLEND_BRILLIANT );
	this->effect.Draw();
	return 1;
}
//ダメージエフェクト
CEffectDamage::CEffectDamage(double x,double y)
{
	this->effect.m_x = x;
	this->effect.m_y = y;
	this->effect.m_inRadius = 0;
	this->effect.m_outRadius = 10;
	this->effect.m_inColor = mono(0);
	this->effect.m_outColor = mono(255);
	this->effect.m_num = 32;
}
int CEffectDamage::Run(){
	if(!CEffectBase::Run())
		return 0;
	if( count > 40 )
		return 0;
	this->effect.m_outColor = mono(255*(40-count)/40);
	this->effect.m_outRadius += 10;
	return 1;
}
int CEffectDamage::Draw(){
	Sun3D.SetBlendingType( BLEND_ADD );
	this->effect.Draw();
	return 1;
}
//カットインエフェクト
CEffectCutin::CEffectCutin(SunTexture *p,RECT *pRect,double x,double y) {
	count = 0;
	this->x = x;
	this->y = y;
	pTex = p;
	if( pRect != NULL )
		rect = *pRect;
	else
		SetRect(&rect,0,0,p->m_d3dii.Width,p->m_d3dii.Height);
}
const int max_cutin = 120;
int CEffectCutin::Run() {
	if(!CEffectBase::Run())
		return 0;
	if( count == max_cutin )
		return 0;
	y -= 1;
	return 1;
}
int CEffectCutin::Draw() {
	if( g_pTitle->otaku )
		return pTex->DrawRect(x,y,rect,alpha(255*sin(PI*(count-1)/max_cutin)));
	else return 1;
}

//フォントエフェクト
CEffectFont::CEffectFont(char *text,D3DCOLOR color,double x,double y,double vx,double vy,int c1,int c2,int c3){
	this->font.CreateList(text);
	this->color = color;
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
	this->c1 = c1;
	this->c2 = c2;
	this->c3 = c3;
	this->count = 0;
}
int CEffectFont::Run() {
	if(!CEffectBase::Run())
		return 0;
	if( count < c1 ) {
		x += vx;
		y += vy;
		a = count*255/c1 ;
	}
	else if( count < c1 + c2 ) {
		a = 255;
	}
	else {
		x += vx;
		y += vy;
		a = (c1+c2-count)*255/c3;
		if( count == c1 + c2 + c3 )
			return 0;
	}
	return 1;
}
int CEffectFont::Draw() {
	if( a > 255 ) a = 255;
	font.DrawFormat(x,y,color,a);
	return 1;
}
//グレイズエフェクト
CEffectGraze::CEffectGraze(double x,double y){
	count = 0;
	a = rand2( 0, 360 );
	v = 5;
	this->x = x;
	this->y = y;
}
int CEffectGraze::Run(){
	if(!CEffectBase::Run())
		return 0;
	if( count > 10 )
		return 0;
	x += v * Sin( a );
	y += v * Cos( a );
	return 1;
}
int CEffectGraze::Draw(){
	g_pMaterial->imgGraze.Draw(x,y,alpha(255),1,Deg2Rad(a));
	return 1;
}

//スターエフェクト
CEffectStar::CEffectStar(double x,double y){
	count = 0;
	a = rand2( 90-15, 90+15 );
	v = rand2( -2, 0 );
	this->x = x;
	this->y = y;
}
int CEffectStar::Run(){
	if(!CEffectBase::Run())
		return 0;
	if( count > 30 )
		return 0;
	x += v * Sin( a );
	y += v * Cos( a );
	return 1;
}
int CEffectStar::Draw(){
	g_pMaterial->imgStar.Draw(x,y,alpha(255*Sin((double)count*180/30)),1,Deg2Rad(a));
	return 1;
}

//ヒットエフェクト
CEffectHit::CEffectHit(double x,double y){
	count = 0;
	a = rand2( 90-15, 90+15 );
	v = rand2( 5, 1 );
	this->x = x;
	this->y = y;
}
int CEffectHit::Run(){
	if(!CEffectBase::Run())
		return 0;
	if( count > 10 )
		return 0;
	x += v * Sin( a );
	y += v * Cos( a );
	return 1;
}
int CEffectHit::Draw(){
	g_pMaterial->imgHit.Draw(x,y);
	return 1;
}

//ホリックエフェクト
CEffectHolic::CEffectHolic(double x,double y){
	this->x = x;
	this->y = y;
}
int CEffectHolic::Run(){
	if(!CEffectBase::Run())
		return 0;
	if( count > 40 )
		return 0;
	return 1;
}
int CEffectHolic::Draw(){
	g_pMaterial->imgHolic.Draw(x-256,y-256,alpha(255*Sin((double)count*180/40)),(double)count/40.0);
	return 1;
}
//ボス破壊エフェクト
CEffectBreak::CEffectBreak(double x,double y){
	this->x = x;
	this->y = y;
}
int CEffectBreak::Run(){
	if(!CEffectBase::Run())
		return 0;
	if( count % 1 == 0)
		g_lEffect.Add( new CEffectGraze( x, y ) );
	if( count % 5 == 0) {
		g_lEffect.Add( new CEffectRing( x + rand2(-80,80), y + rand2(-80,80)) );
		g_pMaterial->sndBreak2.Play(0);
	}
	if( count == 60 ) {
		g_lEffect.Add( new CEffectDamage( x, y ) );
		g_pMaterial->sndBreak.Play(0);
		return 0;
	}
	return 1;
}
int CEffectBreak::Draw(){
	return 1;
}
//鳥目エフェクト
CEffectTorime::CEffectTorime(){
}
int CEffectTorime::Run(){
	if(!CEffectBase::Run())
		return 0;
	return 1;
}
int CEffectTorime::Draw(){
	return 1;
}