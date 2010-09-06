#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//Ž©‹@‘_‚¢¬’e
int Kodama( double x, double y ){
	CTamaNormal tama;
	tama.x = x;
	tama.y = y;
	tama.v = 2;
	tama.type = 0;
	tama.color = 0;

	CDanmakuToPlayer d(&tama);
	d.w= 0;
	d.n= 1;

	return d.Fire();
}

//Ž©‹@‘_‚¢nWay’e
CDanmakuToPlayer::CDanmakuToPlayer(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 3;
	this->w = 10;
}
int CDanmakuToPlayer::Fire()
{
	double angle = Atan2( g_pPlayer->GetX() - pTama->x, g_pPlayer->GetY() - pTama->y);
	for( int i = 0; i < n; ++i ) {
		double m = (double)(n-1) / 2.0;
		double a;
		if( m != 0 )
			a = angle + w * ( (double)i -  m) / m;
		else
			a = angle;
		CTama *p = pTama->Copy();
		p->a = a;
		g_lTama.Add( p );
	}
	return 1;
}
//Ž©‹@‘_‚¢ƒ‰ƒ“ƒ_ƒ€’e
CDanmakuToPlayerRandom::CDanmakuToPlayerRandom(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 32;
	this->w = 10;
	this->offset = 0;
	this->sv = 1;
	this->ev = 5;
}
int CDanmakuToPlayerRandom::Fire()
{
	double angle = Atan2( g_pPlayer->GetX() - pTama->x, g_pPlayer->GetY() - pTama->y);
	for( int i = 0; i < n; ++i ) {
		double a = angle + rand( -w, w ) + offset;
		double v = rand( sv, ev );
		CTama *p = pTama->Copy();
		p->a = a;
		p->v = v;
		g_lTama.Add( p );
	}
	return 1;
}
//Ž©‹@‘_‚¢nWay’e
CDanmakuToPlayerLine::CDanmakuToPlayerLine(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 3;
	this->m = 5;
	this->offset = 0;
	this->w = 10;
	this->sv = 0.5;
	this->ev = 2;
}
int CDanmakuToPlayerLine::Fire()
{
	double angle = offset + Atan2( g_pPlayer->GetX() - pTama->x, g_pPlayer->GetY() - pTama->y);
	for( int i = 0; i < n; ++i ) {
		double nn = (double)(n-1) / 2.0;
		double a;
		if( nn != 0 )
			a = angle + w * ( (double)i -  nn) / nn;
		else
			a = angle;
		for( int j = 0; j < m; ++j ) {
			CTama *p = pTama->Copy();
			if( m != 1 )
				p->v = this->sv + j * ( this->ev - this->sv ) / ( this->m - 1 );
			else
				p->v = this->sv;
			p->a = a;
			g_lTama.Add( p );
		}
	}
	return 1;
}
//‘S•ûˆÊ’e
CDanmakuNonDirectional::CDanmakuNonDirectional(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 32;
	this->offset = 0;
	this->w  = 360;
}
int CDanmakuNonDirectional::Fire()
{
	for( int i = 0; i < n; ++i ) {
		double nn = (double)(n-1) / 2.0;
		double a;
		if( n != 0 )
			a = this->offset + w * (double)i/ n;
		else
			a = this->offset + 180;
		CTama *p = pTama->Copy();
		p->a = a;
		g_lTama.Add( p );
	}
	return 1;
}
//‘S•ûˆÊm—ñ’e
CDanmakuNonDirectionalLine::CDanmakuNonDirectionalLine(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 30;
	this->m = 5;
	this->offset = 0;
	this->sv = 0.5;
	this->ev = 2;
	this->w  = 180;
}
int CDanmakuNonDirectionalLine::Fire()
{
	for( int i = 0; i < n; ++i ) {
		double nn = (double)(n-1) / 2.0;
		double a;
		if( nn != 0 )
			a = this->offset + w * ( (double)i -  nn) / nn;
		else
			a = this->offset + 180;
		for ( int j = 0; j < m; ++j ) {
			CTama *p = pTama->Copy();
			if( m != 1 )
				p->v = this->sv + j * ( this->ev - this->sv ) / ( this->m - 1 );
			else
				p->v = this->sv;
			p->a = a;
			g_lTama.Add( p );
		}
	}
	return 1;
}
//‚Ä‚ñ‚Æ‚¤’Ž
CDanmakuTentou::CDanmakuTentou(CTamaNormal *pTama)
{
	this->pTama = pTama;
	this->n = 30;
	this->m = 5;
	this->sv = 0.5;
	this->ev = 2;
	this->offset  = 0;
}
int CDanmakuTentou::Fire()
{
	for( int i = 0; i < n; ++i ) {
		double nn = (double)(n-1) / 2.0;
		double a;
		if( nn != 0 )
			a = 360 * ( (double)i -  nn) / nn;
		else
			a = 180;
		for ( int j = 0; j < m; ++j ) {
			CTamaNormal *p = (CTamaNormal*)pTama->Copy();
			if( m != 1 )
				p->v = this->sv + j * ( this->ev - this->sv ) / ( this->m - 1 );
			else
				p->v = this->sv;
			p->a = a;
			if( pow2(p->v*Cos(a)+0.6*sv) + pow2(p->v*Sin(a)) < pow2(0.4) ||
				pow2(p->v*Cos(a)+0.2*sv) + pow2(p->v*Sin(a) - 0.8*sv) < pow2(0.4) ||
				pow2(p->v*Cos(a)+0.2*sv) + pow2(p->v*Sin(a) + 0.8*sv) < pow2(0.4) ||
				pow2(p->v*Cos(a)-0.2*sv) + pow2(p->v*Sin(a) - 0.3*sv) < pow2(0.4) ||
				pow2(p->v*Cos(a)-0.2*sv) + pow2(p->v*Sin(a) + 0.3*sv) < pow2(0.4) ||
				pow2(p->v*Cos(a)-0.8*sv) + pow2(p->v*Sin(a) - 0.4*sv) < pow2(0.4) ||
				pow2(p->v*Cos(a)-0.8*sv) + pow2(p->v*Sin(a) + 0.4*sv) < pow2(0.4) ) {
				p->type = 5;
				p->color = 0;
			}
			g_lTama.Add( p );
		}
	}
	return 1;
}
//ƒNƒ‚‚Ì‘ƒ
CDanmakuSpyder::CDanmakuSpyder(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 128;
	this->m = 8;
	this->offset = 0;
	this->sv = 1;
	this->ev = 0.8;
}
int CDanmakuSpyder::Fire()
{
	for( int i = 0; i < n; ++i ) {
		CTama *p = pTama->Copy();
		p->a = this->offset + 360.0 * (double)i / n;
		p->v = sv + (ev-sv) * fabs( Sin( 180.0*i/((double)n/m) ) );
		g_lTama.Add( p );
	}
	return 1;
}
//•ûŒü‚Â‚«‰~’e
CDanmakuCircle::CDanmakuCircle(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 3;
	this->offset = 0;
	this->w  = 360;
	this->aa = 180;
	this->vv = 0;
}
int CDanmakuCircle::Fire()
{
	for( int i = 0; i < n; ++i ) {
		double nn = (double)(n-1) / 2.0;
		double a;
		if( n != 0 )
			a = this->offset + w * (double)i/ n;
		else
			a = this->offset + 180;
		CTama *p = pTama->Copy();
		p->a = a;
		double xx = p->v * Cos( p->a ) + vv * Cos( aa );
		double yy = p->v * Sin( p->a ) + vv * Sin( aa );
		p->a = Atan2( xx, yy );
		p->v = sqrt( pow2( xx ) + pow2( yy ));
		g_lTama.Add( p );
	}
	return 1;
}
//Ž©‹@‘_‚¢‰~’e
CDanmakuCircleToPlayer::CDanmakuCircleToPlayer(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 3;
	this->offset = 0;
	this->w  = 360;
	this->aa = 0;
	this->vv = 0;
}
int CDanmakuCircleToPlayer::Fire()
{
	double angle = Atan2( g_pPlayer->GetX() - pTama->x, g_pPlayer->GetY() - pTama->y);
	for( int i = 0; i < n; ++i ) {
		double nn = (double)(n-1) / 2.0;
		double a;
		if( n != 0 )
			a = this->offset + w * (double)i/ n;
		else
			a = this->offset + 180;
		CTama *p = pTama->Copy();
		p->a = a;
		double xx = p->v * Cos( p->a ) + vv * Cos( aa + angle );
		double yy = p->v * Sin( p->a ) + vv * Sin( aa + angle );
		p->a = Atan2( xx, yy );
		p->v = sqrt( pow2( xx ) + pow2( yy ));
		g_lTama.Add( p );
	}
	return 1;
}
//‘S•ûˆÊ‰~’e
CDanmakuCircleNonDirectional::CDanmakuCircleNonDirectional(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 3;
	this->m = 4;
	this->offset = 0;
	this->w  = 360;
	this->aa = 180;
	this->vv = 0;
}
int CDanmakuCircleNonDirectional::Fire()
{
	CDanmakuCircle danmaku( pTama );
	danmaku.n = n;
	danmaku.vv = vv;
	for( int j = 0; j < m; j++ ) {
		double a;
		if( m != 0 )
			a = this->offset + w * (double)j / m;
		else
			a = this->offset + 180;
		danmaku.aa = a;
		danmaku.Fire();
	}
	return 1;
}
//nsm—ñ‰~’e
CDanmakuCircleLine::CDanmakuCircleLine(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 32;
	this->m = 5;
	this->offset = 0;
	this->w  = 360;
	this->aa = 180;
	this->vv = 0;
	this->sv = 0.5;
	this->ev = 2;
}
int CDanmakuCircleLine::Fire()
{
	for( int i = 0; i < n; ++i ) {
		double nn = (double)(n-1) / 2.0;
		double a;
		if( n != 0 )
			a = this->offset + w * (double)i/ n;
		else
			a = this->offset + 180;
		
		for ( int j = 0; j < m; ++j ) {
			CTama *p = pTama->Copy();
			if( m != 1 )
				p->v = this->sv + j * ( this->ev - this->sv ) / ( this->m - 1 );
			else
				p->v = this->sv;
			p->a = a;
			double xx = p->v * Cos( p->a ) + vv * Cos( aa );
			double yy = p->v * Sin( p->a ) + vv * Sin( aa );
			p->a = Atan2( xx, yy );
			p->v = sqrt( pow2( xx ) + pow2( yy ));
			g_lTama.Add( p );
		}
	}
	return 1;
}
//ü•ª
CDanmakuSegment::CDanmakuSegment(CTama *pTama)
{
	this->pTama = pTama;
}
int CDanmakuSegment::Fire()
{
	double vx1 = v1 * Cos( a1 );
	double vy1 = v1 * Sin( a1 );
	double vx2 = v2 * Cos( a2 );
	double vy2 = v2 * Sin( a2 );
	double nn = n - 1;
	double vx,vy;
	for( int i = 0; i < n; ++i ) {
		if( n != 1 ) {
			vx = vx1 * ( nn - i ) / nn + vx2 * i / nn;
			vy = vy1 * ( nn - i ) / nn + vy2 * i / nn;
		}
		else {
			vx = (vx1 + vx2)/2;
			vy = (vy1 + vy2)/2;
		}
		double vv = sqrt(pow2(vx) + pow2(vy));
		double aa = Atan2(vx,vy);
		CTama *p = pTama->Copy();
		p->v = pTama->v * vv;
		p->a = aa;
		g_lTama.Add( p );
	}
	return 1;
}
void CDanmakuSegment::Set( double t )
{
	double vx1 = v1 * Cos( a1 );
	double vy1 = v1 * Sin( a1 );
	double vx2 = v2 * Cos( a2 );
	double vy2 = v2 * Sin( a2 );
	n = sqrt(pow2( vx1-vx2 ) + pow2( vy1-vy2 )) * t;
}
//ü•ª—ñ
CDanmakuSegmentLine::CDanmakuSegmentLine(CTama *pTama)
{
	this->pTama = pTama;
}
int CDanmakuSegmentLine::Fire()
{
	double vx1 = v1 * Cos( a1 );
	double vy1 = v1 * Sin( a1 );
	double vx2 = v2 * Cos( a2 );
	double vy2 = v2 * Sin( a2 );
	double nn = n - 1;
	for( int i = 0; i < n; ++i ) {
		double vx = vx1 * i / nn + vx2 * ( nn - i ) / nn;
		double vy = vy1 * i / nn + vy2 * ( nn - i ) / nn;
		double vv = sqrt(pow2(vx) + pow2(vy));
		double aa = Atan2(vx,vy);
		for( int j = 0; j < m; ++j ) {
			CTama *p = pTama->Copy();
			if( m == 1 )
				p->v = vv;
			else
				p->v = vv * j / (m-1) + vv * ev * (m-1-j) / (m-1);
			p->a = aa;
			g_lTama.Add( p );
		}
	}
	return 1;
}
void CDanmakuSegmentLine::Set( double t )
{
	double vx1 = v1 * Cos( a1 );
	double vy1 = v1 * Sin( a1 );
	double vx2 = v2 * Cos( a2 );
	double vy2 = v2 * Sin( a2 );
	n = sqrt(pow2( vx1-vx2 ) + pow2( vy1-vy2 )) * t;
}

//‘½ŠpŒ`
CDanmakuPolygons::CDanmakuPolygons(CTama *pTama)
{
	this->pTama = pTama;
	offset = 0;
}
int CDanmakuPolygons::Fire()
{
	for(int i = 0; i < n; i++ ) {
		CDanmakuSegmentLine d(pTama);
		d.a1 = 360.0 * i / n + offset;
		d.a2 = 360.0 * (i+1) / n + offset;
		d.v1 = sv;
		d.v2 = sv;
		d.Set( t );
		d.m = m;
		d.ev = ev / sv;
		d.Fire();
	}
	return 1;
}
//‰¡Œ^—ñ’e
CDanmakuHorizon::CDanmakuHorizon(CTama *pTama)
{
	this->pTama = pTama;
	this->n = 32;
	this->m = 8;
	this->offset = 0;
	this->w  = 2;
}
int CDanmakuHorizon::Fire()
{
	CDanmakuNonDirectional d( pTama );
	for( int j = 0; j < m; ++j ) {
		d.n = n;
		d.w = 360 / m / w;
		d.offset = offset + 360 * j / m;
		d.Fire();
	}
	return 1;
}
