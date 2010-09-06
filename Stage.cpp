#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//ステージ
CStage *g_pStage = NULL;

CStage::CStage(){
	count = 0;
}
CStage::~CStage(){
	StopBGM();
	SAFE_DELETE( g_pBack );
}
int CStage::Run(){
	if( !mt.IsEnd() ) {
		mt.Resume();
		count ++;
		if( mt.IsEnd() ){
			g_pSystem->state = SYSTEM_RESULT;
			g_pGame->stage ++;
			return 1;
		}
	}
	return 1;
}
int CStage::Draw(){
	return 1;
}
void CStage::Stop(){
	PauseBGM();
}
void CStage::Resume(){
	ResumeBGM();
}

SunList<CEnemy> g_lEnemy;

//-------------------ザコ共通,ザコはいいなあ---------------------------------

CEnemyBase::CEnemyBase()
{
	count = 0;
	x = y = 0;
}
CEnemyBase::~CEnemyBase(){
}
//実行
int CEnemyBase::Run(){
	//移動
	if( x < GAME_LEFT - GetW() ||
		x > GAME_RIGHT ||
		y < GAME_TOP - GetH() ||
		y > GAME_BOTTOM ) {
		return 0;
	}
	return Run2();
}
//実行（画面外判定なし）
int CEnemyBase::Run2(){
	count++;
	//ショットとの当たり判定
	RECT myRect = {x,y,x+GetW(),y+GetH()};
	list<CShot*>::iterator itr;
	for( itr = g_lShot.begin(); itr != g_lShot.end(); itr++ ) {
		if( *itr != NULL ) {
			RECT shotRect;
			(*itr)->GetHantei(&shotRect);
			if( GetHantei( &shotRect, &myRect )) {
				double damage = (*itr)->GetDamage();
				SAFE_DELETE( *itr );
				if( !Damage( damage ) ) {
					return 0;
				}
			}
		}
	}
	//レーザーとの当たり判定
	RECT laserRect;
	if( g_pPlayer->GetLaserHantei( &laserRect ) ) {
		if( GetHantei( &laserRect, &myRect )) {
			if( !Damage( g_pPlayer->GetLaserDamage()) ) {
				return 0;
			}
		}
	}
	//マスタースパークとの当たり判定
	double damage = g_pPlayer->GetMasterHantei(x+GetW()/2,y+GetH()/2);
	if( damage > 0 ) {
		if( !Damage( damage ) ) {
			return 0;
		}
	}
	//自機との判定
	if( !Hantei() ) {
		return 0;
	}
	return 1;
}
//ダメージ
int CEnemyBase::Damage( double damage )
{
	hp -= damage;
	//ダメージエフェクト・SEを入れる
	if( hp <= 0 ) {
		//死亡エフェクト・SEを入れる
		return Death();
	}
	else {
		g_pMaterial->sndHit.Play(0);
	}
	return 1;
}
//しぼんぬ
int CEnemyBase::Death() {
	g_lEffect.Add( new CEffectRing( GetX(), GetY() ));
	if( GetItem() != ITEM_NONE )
		g_lItem.Add( new CItem( GetX(), GetY(), GetItem() ));
	g_pMaterial->sndExplode.Play(0);
	return 0;
}
//自機との当たり判定
int CEnemyBase::Hantei()
{
	if( pow2( g_pPlayer->GetX() - GetX() ) + pow2( g_pPlayer->GetY() - GetY())
		< pow2 ( g_pPlayer->GetSize() + GetSize() )) {
		if( !g_pPlayer->GetMuteki() ) {
			g_pPlayer->Damage();
			return Damage(10);
		}
		return Damage(1);
	}
	return 1;
}
//-------------------妖精共通---------------------------------
//描画
int CEnemyFairyBase::Draw()
{
	int index = (count/7) % 4;
	int c = color % 5;
	RECT rect={index*GetW(),c*GetH(),(index+1)*GetW(),(c+1)*GetH()};
	if( color <= 4 )
		g_pMaterial->imgFairy.DrawRect(x,y,rect);
	else if( color <= 8 )
		g_pMaterial->imgFairy2.DrawRect(x,y,rect);
	return 1;
}
//アイテム
int CEnemyFairyBase::GetItem(){
	int type = irand(1,2);
	return type;
}


//-------------------毛玉共通---------------------------------
//描画
int CEnemyKedamaBase::Draw()
{
	RECT rect={color*32,0,(color+1)*32,32};
	double angle = (double)count / 5;
	g_pMaterial->imgKedama.DrawRect(x,y,rect,alpha(255),1,angle);
	return 1;
}
//アイテム
int CEnemyKedamaBase::GetItem(){
	return irand(1,2);
}
//-------------------フード共通---------------------------------
//描画
int CEnemyHoodBase::Draw()
{
	int index = (count/7) % 2;
	int c = color % 5;
	RECT rect={index*GetW(),c*GetH(),(index+1)*GetW(),(c+1)*GetH()};
	g_pMaterial->imgHood.DrawRect(x,y,rect);
	//g_pMaterial->imgHood.Draw(x,y);
	return 1;
}
//アイテム
int CEnemyHoodBase::GetItem(){
	return irand(1,2);
}

