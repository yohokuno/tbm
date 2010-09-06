#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

SunList<CItem> g_lItem;

//全てのアイテムを吸い寄せる
int ItemSuiyose(){
	list<CItem*>::iterator itr;
	for( itr = g_lItem.begin(); itr != g_lItem.end(); itr++ ) {
		if( *itr != NULL ) {
			(*itr)->suiyose = 1;
		}
	}
	return 1;
}

//アイテムクラス（typeで型指定）
CItem::CItem(double x,double y,int type){
	this->x = x;
	this->y = y;
	this->type = type;
	this->count = 0;

	//フルパワーのときはパワーアップアイテムが全て得点アイテムに
	if( g_pSystem->power >= 1000 ) {
		if( type == ITEM_POWER )
			this->type = ITEM_SCORE;
		else if( type == ITEM_POWER2 || type == ITEM_FULLPOWER )
			this->type = ITEM_SCORE2;
	}

	vx = -1;
	ax = -0.1;

	//星は自動吸い寄せ
	if( type == ITEM_STAR )
		suiyose = 1;
	else
		suiyose = 0;
}
CItem::~CItem(){
}
int CItem::Run(){
	count++;
	double r = sqrt( pow2( g_pPlayer->GetX() - x ) + pow2( g_pPlayer->GetY() - y ) );
	if( suiyose == 0 || g_pPlayer->state == 3) {
		{
			//自然落下
			vx += ax - 0.02 * vx;
			x += vx;
			if( x < GAME_LEFT - 48 )
				return 0;
		}
	}
	else {
		//吸い寄せあり
		double v = 10.0 / sqrt( pow2( g_pPlayer->GetX() - x ) + pow2( g_pPlayer->GetY() - y ) );
		x += v * ( g_pPlayer->GetX() - x );
		y += v * ( g_pPlayer->GetY() - y );
	}
	if( r < g_pPlayer->GetSize() + GetSize() ) {
		g_pMaterial->sndItem.Play(0);
		g_pPlayer->GetItem( type );
		return 0;
	}
	return 1;
}
int CItem::Draw(){
	RECT rect;
	switch(type){
	case ITEM_STAR://星
		SetRect( &rect, 32, 0, 48, 16);
		break;
	case ITEM_POWER://パワーアップ
		SetRect( &rect, 0, 0, 16, 16);
		break;
	case ITEM_SCORE://点
		SetRect( &rect, 16, 0, 32, 16);
		break;
	case ITEM_POWER2://パワーアップ(大）
		SetRect( &rect, 0, 16, 24, 40);
		break;
	case ITEM_BOMB://ボム
		SetRect( &rect, 48, 16, 72, 40);
		break;
	case ITEM_FULLPOWER://フルパワー
		SetRect( &rect, 72, 16, 96, 40);
		break;
	case ITEM_1UP://１UP
		SetRect( &rect, 96, 0, 128, 32);
		break;
	case ITEM_SCORE2://スコア（大）
		SetRect( &rect, 24, 16, 48, 40);
		break;
	case ITEM_SCORE0://スコア（小）
		SetRect( &rect, 88, 0, 96, 8);
		break;
	}
	double X = x - (rect.right-rect.left)/2;
	double Y = y - (rect.bottom-rect.top)/2;
	g_pMaterial->imgItem.DrawRect(X,Y,rect);
	return 1;
}
double CItem::GetSize() {
	double result;
	switch(type){
	case ITEM_STAR://星
		result = 8;
		break;
	case ITEM_POWER://パワーアップ
		result = 8;
		break;
	case ITEM_SCORE://点
		result = 8;
		break;
	case ITEM_POWER2://パワーアップ(大）
		result = 12;
		break;
	case ITEM_BOMB://ボム
		result = 12;
		break;
	case ITEM_FULLPOWER://フルパワー
		result = 12;
		break;
	case ITEM_1UP://１UP
		result = 16;
		break;
	case ITEM_SCORE2://スコア（大）
		result = 12;
		break;
	case ITEM_SCORE0://スコア（小）
		result = 4;
		break;
	}

	if( suiyose == 0 )
		result += 32;
	return result;
}