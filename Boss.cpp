#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

CBossBase *g_pBoss = NULL;
//-------------------ボス共通---------------------------------

CBossBase::CBossBase()
{
	count = 0;
	x = GAME_RIGHT;
	y = GAME_CENTER_Y;
	SetHP(1000);
	deffence = 1;
	spell = 0;
	time = 99;
	dirty = 0;
	flag = 1;
	pBack = NULL;
	pTalk = NULL;
	g_pBoss = this;
}
CBossBase::~CBossBase(){
	SAFE_DELETE(pBack);
	SAFE_DELETE(pTalk);
	g_pBoss = NULL;
}
int CBossBase::Run(){
	count++;
	//ショットとの当たり判定
	RECT myRect = {x,y,x+GetW(),y+GetH()};
	list<CShot*>::iterator itr;
	for( itr = g_lShot.begin(); itr != g_lShot.end(); itr++ ) {
		if( *itr != NULL ) {
			RECT shotRect;
			(*itr)->GetHantei(&shotRect);
			if( GetHantei( &shotRect, &myRect )) {
				double damage = 0;
				if( deffence > 0 )
					damage = (*itr)->GetDamage();
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
		if( deffence > 0 ) {
			dirty = 1;
			if( !Damage( damage ) ) {
				return 0;
			}
		}
	}
	//自機との判定
	if( !Hantei() ) {
		return 0;
	}
	return 1;
}
//描画
int CBossBase::Draw() {
	if( spell ) {
		Sun3D.SetBlendingType(BLEND_ADD);
		g_pMaterial->imgEnjin.Draw(GetX()-256,GetY()-256,alpha(180),0.5*hp/hp_max+0.5,(double)count/40);
		Sun3D.SetBlendingType(BLEND_NORMAL);
	}
	return 1;
}
//システム描画
int CBossBase::Draw2() {
	//スペルカード名
	if( spell ) {
		fntSpell.DrawFormat(GAME_RIGHT-16-strlen(spell_name[g_pSystem->spell_num])/2*16,50);
	}
	//残り時間
	if( flag == 1 ) {
		SunEffectGauge gauge;
		int t = time-count/60;
		D3DCOLOR color;
		if( t < 5 ) {
			color = xrgb(255,0,0);
			if( count % 60 == 0 ) { //効果音
				g_pMaterial->sndCountDown.Play(0);
			}
		}
		else
			color = alpha(255);
		if( t >= 0 )
			g_pSystem->DrawNumber(602,45,t,2,color);
		//ゲージ
		gauge.m_rect.top += 18;
		gauge.m_point = hp / hp_max;
		gauge.m_downColor = xrgb(0,0,0);
		gauge.Draw();

		gauge.m_rect.left += 1;
		gauge.m_rect.top += 1;
		gauge.m_rect.right -= 1;
		gauge.m_rect.bottom -= 1;
		if( spell )
			gauge.m_downColor = xrgb(255,120,120);
		else
			gauge.m_downColor = xrgb(255,255,255);
		gauge.Draw();
	}
	//ボス名
	fntBoss.Draw(630,400,alpha(255),1,90);
	//ENEMY表示
	g_pMaterial->imgEnemyDisplay.Draw(2,GetY()-25);
	return 1;
}
//会話系
int CBossBase::Draw3(){
	if( pTalk != NULL )
		if( pTalk->IsSuspended() )
			pTalk->Resume();
	return 1;
}
//鳥目エフェクト
int CBossBase::Draw4(){
	return 1;
}
//自機との当たり判定
int CBossBase::Hantei()
{
	if( deffence == 0 )
		return 1;
	if( pow2( g_pPlayer->GetX() - GetX() ) + pow2( g_pPlayer->GetY() - GetY())
		< pow2 ( g_pPlayer->GetSize() + GetSize() )) {
		if( !g_pPlayer->GetMuteki() ) {
			g_pPlayer->Damage();
		}
	}
	return 1;
}
//アイテム
int CBossBase::GetItem(){
	return ITEM_NONE;
}
//ダメージ
int CBossBase::Damage( double damage )
{
	if( deffence == 0 )
		return 1;
	hp -= deffence * damage;
	if( hp <= 0 ) {
		if( !Death() )
			return 0;
	}
	else {
		//ダメージエフェクト・SEを入れる
		g_pMaterial->sndHit.Play(0);
	}
	return 1;
}
//死亡
int CBossBase::Death(){
	//死亡エフェクト・SEを入れる
	flag = 1;
	g_lEffect.Add( new CEffectDamage( GetX(), GetY() ));
	g_pMaterial->sndExplode.Play(0);
	if( GetItem() != ITEM_NONE )
		g_lItem.Add( new CItem( GetX(), GetY(), GetItem() ));
	return 0;
}
//スペルカード開始
int CBossBase::SpellCard() {
	g_pMaterial->sndSpellStart.Play(0);
	spell = 1;
	dirty = 0;
	fntSpell.CreateList(spell_name[g_pSystem->spell_num]);
	return 1;
}
//スペルカード終了
int CBossBase::SpellEnd() {
	g_pMaterial->sndSpellEnd.Play(0);
	if( pBack != NULL ) {
		SAFE_DELETE( g_pBack );
		g_pBack = pBack;
		pBack = NULL;
	}
	spell = 0;

	//スペルカード取得
	if( dirty == 0 ) {
		LONGLONG spellbonus = (time - count / 60) * SCORE_SPELLCARD;
		g_pSystem->AddScore( spellbonus );
		g_pSystem->spellbonus = spellbonus;
		g_pSystem->spellcard ++;
		//取得回数インクリメント
		if( g_pGame->mode != 3 )
			g_pRecord->data.spell[g_pSystem->spell_num][GetDifficult()].success ++;
	}
	else {
		//失敗
		g_pSystem->spellbonus = 0;
	}
	//挑戦回数インクリメント
	if( g_pGame->mode != 3 )
		g_pRecord->data.spell[g_pSystem->spell_num][GetDifficult()].challenge ++;
	g_pSystem->spell_num ++;
	g_pSystem->spellbonus_count = 120;

	//アイテム追加
	if( g_pGame->stage < 4 )
		AddItem(rand(-1,1)>0?ITEM_SCORE:ITEM_POWER, 5);
	else
		AddItem(ITEM_SCORE2, 5);
	return 1;
}
void CBossBase::SetHP(double hp){
	this->hp = hp;
	this->hp_max = hp;
	this->dirty = 0;
}
void CBossBase::SetTime(int time){
	this->time = time;
	this->count = 0;
}
int CBossBase::IsEnd(){
	if( hp <= 0 ){
		TamaToItem();
		return 1;
	}
	if( time-count/60 < 0 ){
		TamaToItem();
		dirty = 1;
		return 1;
	}
	return 0;
}
void CBossBase::Appear() {
	x -= 5;
}
void CBossBase::AddItem(int item,int num) {
	for( int i = 0; i < num; i++ ){
		double a = rand(0,360);
		double r = ( num>1 ? rand(0,50) : 0);
		int xx = GetX() + r * Cos(a);
		int yy = GetY() + r * Sin(a);
		g_lItem.Add( new CItem( xx, yy, item ));
	}
}
void CBossBase::Suspend(double m){mt.Suspend(m);}