#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

SunList<CLaser> g_lLaser;

CLaserSquare::CLaserSquare(){
	count = 0;
	start_count = 30;
	end_count = 120;
	vanish_count = 30;
	graze_count = 0;
	iw = 5;
	ow = 15;
	a = 0;
	w = 0;
	laser.m_iw = 0;
	laser.m_ow = 0;
}
int CLaserSquare::Run(){
	count++;
	a += w;
//	a = fmod(a,360);
	double sx = laser.m_sx;
	double sy = laser.m_sy;
	laser.m_ex = sx - 800 * Cos(a);
	laser.m_ey = sy + 800 * Sin(a);
/*	if( a < Atan2(sx,sy ) || a >= 270.0 + Atan2(480-sy,sx) ){
		laser.m_ex = 0;
		laser.m_ey = sx*Tan(a);
	}
	else if( a < 90.0 + Atan2(sy,640-sx)){
		laser.m_ex = sx - sy/Tan(a);
		laser.m_ey = 0;
	}
	else if( a < 180.0 + Atan2(640-sx,4800-sy)){
		laser.m_ex = 640;
		laser.m_ey = sy + (640-sx)*Tan(a);
	}
	else if( a < 270.0 + Atan2(480-sy,sx)){
		laser.m_ex = sx - (480-sy)/Tan(a);
		laser.m_ey = 480;
	}
*/
	//太さの変動
	if( count < start_count ) {
		laser.m_iw += iw/start_count;
		laser.m_ow += ow/start_count;
	}
	else if( count < end_count ) {
		//プレイヤーとのあたり判定
		if( g_pPlayer->state != 3 ) {
			double x = g_pPlayer->GetX() - laser.m_sx;
			double y = g_pPlayer->GetY() - laser.m_sy;
			double ex = laser.m_ex - laser.m_sx;
			double ey = laser.m_ey - laser.m_sy;
			double r = abs( ey * x + ex * y ) / sqrt( ex*ex + ey*ey );
			if( r < iw) {
				if( !g_pPlayer->GetMuteki() )
					g_pPlayer->Damage();
				Item();
			}
			else if( graze_count == 0 ) {//グレイズ
				if( g_pPlayer->state == 0 ){
					if( r < iw + g_pPlayer->GetGrazeSize() ) {
						g_pPlayer->Graze();
						//g_lItem.Add( new CItem(GetX(),GetY(),ITEM_SCORE0));
						graze_count = 12;
					}
				}
			}
		}
	}
	else if( count < end_count + vanish_count ) {
		if( laser.m_iw > 0 && laser.m_ow > 0 ) {
			laser.m_iw -=	iw/vanish_count;
			laser.m_ow -=	ow/vanish_count;
		}
	}
	else if( count > end_count + vanish_count )
		return 0;
	
	if(graze_count > 0 )
		graze_count--;
	return 1;
}
int CLaserSquare::Draw(){
	laser.Draw();
	return 1;
}
int CLaserSquare::Item(){
	count = end_count;
	return 1;
}
